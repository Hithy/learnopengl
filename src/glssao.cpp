#include "glssao.h"

#include <string>
#include <vector>
#include <random>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"
#include "model.h"

#include "stb_image.h"
#include "utils.h"
#include "imgui.h"

namespace LearnGL {

static glm::vec3 objectPositions[] = {
    glm::vec3(-5.0, -3.0, -5.0),
    // glm::vec3(0.0, -3.0, -5.0),
    // glm::vec3(5.0, -3.0, -5.0),
    // glm::vec3(-5.0, -3.0, 0.0),
    // glm::vec3(0.0, -3.0, 0.0),
    // glm::vec3(5.0, -3.0, 0.0),
    // glm::vec3(-5.0, -3.0, 5.0),
    // glm::vec3(0.0, -3.0, 5.0),
    // glm::vec3(5.0, -3.0, 5.0)
};

static std::vector<glm::vec3> lightPositions = {
  glm::vec3(-5.0, -3.0, 0.0),
  // glm::vec3(0.0, -1.0, -5.0),
  // glm::vec3(5.0, -1.0, -5.0),
  // glm::vec3(-5.0, -1.0, 0.0),
  // glm::vec3(0.0, -1.0, 0.0),
  // glm::vec3(5.0, -1.0, 0.0),
  // glm::vec3(-5.0, -1.0, 5.0),
  // glm::vec3(0.0, -1.0, 5.0),
  // glm::vec3(5.0, -1.0, 5.0),
};

GLSSAO::GLSSAO() : GLBase("hello_gl", 1920, 1080)
  , _light_color_diffuse(0.2f, 0.2f, 0.7f)
  , _light_color_specular(0.2f, 0.2f, 0.7f) {
}

void GLSSAO::Init() {
  this->_ssao_gbuffer_shader =
      new Shader("shader/gbuffer_ssao_vs.glsl", "shader/gbuffer_ssao_fs.glsl");
  this->_ssao_calc_shader =
      new Shader("shader/ssao_calc_vs.glsl", "shader/ssao_calc_fs.glsl");
  this->_ssao_light_shader =
      new Shader("shader/ssao_calc_vs.glsl", "shader/ssao_light_fs.glsl");

  GenGbuffer();
  GenSSAObuffer();
  GenNoiseMap();

  // gen texture
  _cube_texture = GenTexture("asserts/images/container2.png");
  _cube_specular = GenTexture("asserts/images/container2_specular.png");
  // _floor_texture = GenTexture("asserts/images/metal.png");

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
}

static float lerp(float a, float b, float f)
{
    return a + f * (b - a);
}

static std::vector<glm::vec3> GenSSAOSamples() {
  std::vector<glm::vec3> res;
  std::uniform_real_distribution<float> randomFloats(
      0.0, 1.0); // random floats between [0.0, 1.0]
  std::default_random_engine generator;

  for (int i = 0; i < 16; i++) {
    for (int j = 0; j < 16; j++) {
      float x = randomFloats(generator) * 2.0f - 1.0f;
      float y = randomFloats(generator) * 2.0f - 1.0f;
      float z = randomFloats(generator);
      glm::vec3 sample(x, y, z);
      sample = glm::normalize(sample);
      sample *= randomFloats(generator);

      float scale = float(i) / 64.0;
      scale = lerp(0.1f, 1.0f, scale * scale);
      sample *= scale;
      
      res.push_back(sample);
    }
  }

  return res;
}

void GLSSAO::GenNoiseMap() {
  std::uniform_real_distribution<float> randomFloats(
      0.0, 1.0); // random floats between [0.0, 1.0]
  std::default_random_engine generator;

  std::vector<glm::vec3> noise_vec_list;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      float x = randomFloats(generator) * 2.0f - 1.0f;
      float y = randomFloats(generator) * 2.0f - 1.0f;
      noise_vec_list.push_back(glm::vec3(x, y, 0.0f));
    }
  }

  glGenTextures(1, &_noise_map);
  glBindTexture(GL_TEXTURE_2D, _noise_map);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, 4, 4, 0, GL_RGB, GL_FLOAT,
               &noise_vec_list[0]);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void GLSSAO::GenSSAObuffer() {
  glGenFramebuffers(1, &_ssao_fbo);
  glGenTextures(1, &_ssao_texture);

  glBindFramebuffer(GL_FRAMEBUFFER, _ssao_fbo);

  glBindTexture(GL_TEXTURE_2D, _ssao_texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, _window_width, _window_height, 0,
               GL_RED, GL_FLOAT, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         _ssao_texture, 0);

  glBindTexture(GL_TEXTURE_2D, 0);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GLSSAO::GenGbuffer() {
  glGenFramebuffers(1, &_gbuffer_fbo);
  glGenTextures(1, &_gbuffer_position);
  glGenTextures(1, &_gbuffer_normal);
  glGenTextures(1, &_gbuffer_ablebo);

  glBindFramebuffer(GL_FRAMEBUFFER, _gbuffer_fbo);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, _gbuffer_position);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, _window_width, _window_height, 0,
               GL_RGB, GL_FLOAT, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         _gbuffer_position, 0);

  glBindTexture(GL_TEXTURE_2D, _gbuffer_normal);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, _window_width, _window_height, 0,
               GL_RGB, GL_FLOAT, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D,
                         _gbuffer_normal, 0);

  glBindTexture(GL_TEXTURE_2D, _gbuffer_ablebo);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, _window_width, _window_height, 0,
               GL_RGBA, GL_FLOAT, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D,
                         _gbuffer_ablebo, 0);

  unsigned int attachments[3] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1,
                                 GL_COLOR_ATTACHMENT2};
  glDrawBuffers(3, attachments);

  unsigned int depth_buffer;
  glGenRenderbuffers(1, &depth_buffer);
  glBindRenderbuffer(GL_RENDERBUFFER, depth_buffer);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, _window_width,
                        _window_height);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                            GL_RENDERBUFFER, depth_buffer);

  glBindTexture(GL_TEXTURE_2D, 0);
  glBindRenderbuffer(GL_RENDERBUFFER, 0);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GLSSAO::DrawGBuffer() {
  static Model* backpack = new Model("asserts/models/backpack/backpack.obj");
  glBindFramebuffer(GL_FRAMEBUFFER, _gbuffer_fbo);

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  _ssao_gbuffer_shader->use();

  auto scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.01f, 0.01f, 0.01f));
  for (auto const &obj_pos : objectPositions) {
    auto trans = glm::translate(glm::mat4(1.0f), obj_pos);
    _ssao_gbuffer_shader->SetUniformMat4fv(
        "model", glm::value_ptr(trans * scale));
    backpack->Draw(*_ssao_gbuffer_shader);
  }
}

void GLSSAO::DrawSSAO() {
  static auto samples = GenSSAOSamples();

  glBindFramebuffer(GL_FRAMEBUFFER, _ssao_fbo);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  _ssao_calc_shader->use();

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, _gbuffer_position);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, _gbuffer_normal);
  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_2D, _noise_map);
  _ssao_calc_shader->SetUniform1i("gPosition", 0);
  _ssao_calc_shader->SetUniform1i("gNormal", 1);
  _ssao_calc_shader->SetUniform1i("gNoise", 2);

  for (int i = 0; i < samples.size(); i++) {
    auto sample_str = "ssao_sample[" + std::to_string(i) + "]";
    _ssao_calc_shader->SetUniform3fv(sample_str.c_str(), samples[i].x, samples[i].y, samples[i].z);
  }

  renderQuad();
}

void GLSSAO::Draw() {
  auto view = _camera.GetView();
  auto projection = glm::perspective(glm::radians(_camera.fov), 1920.0f / 1080.0f,
                                     0.1f, 100.0f);

  _ssao_gbuffer_shader->use();
  _ssao_gbuffer_shader->SetUniformMat4fv("view", glm::value_ptr(view));
  _ssao_gbuffer_shader->SetUniformMat4fv("projection", glm::value_ptr(projection));

  _ssao_calc_shader->use();
  _ssao_calc_shader->SetUniformMat4fv("projection", glm::value_ptr(projection));

  DrawGBuffer();
  DrawSSAO();

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  _ssao_light_shader->use();

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, _gbuffer_position);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, _gbuffer_normal);
  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_2D, _gbuffer_ablebo);
  glActiveTexture(GL_TEXTURE3);
  glBindTexture(GL_TEXTURE_2D, _ssao_texture);

  _ssao_light_shader->SetUniform1i("gFragPos", 0);
  _ssao_light_shader->SetUniform1i("gNormal", 1);
  _ssao_light_shader->SetUniform1i("gAbleboSpec", 2);
  _ssao_light_shader->SetUniform1i("gSSAO", 3);

  _ssao_light_shader->SetUniform1i("light_count", lightPositions.size());
  _ssao_light_shader->SetUniformMat4fv("view", glm::value_ptr(view));

  for (int i = 0; i < lightPositions.size(); i++) {
    auto const& light_pos = lightPositions[i];
    std::string light_prefix = "light_list[" + std::to_string(i) + "]";

    _ssao_light_shader->SetUniform3fv((light_prefix + ".position").c_str(), light_pos.x, light_pos.y, light_pos.z);

    _ssao_light_shader->SetUniform3fv((light_prefix + ".diffuse").c_str(), _light_color_diffuse.r, _light_color_diffuse.g, _light_color_diffuse.b);
    _ssao_light_shader->SetUniform3fv((light_prefix + ".specular").c_str(), _light_color_specular.r, _light_color_specular.g, _light_color_specular.b);
    _ssao_light_shader->SetUniform1f((light_prefix + ".constant").c_str(), 1.0f);
    _ssao_light_shader->SetUniform1f((light_prefix + ".linear").c_str(), 0.09f);
    _ssao_light_shader->SetUniform1f((light_prefix + ".quadratic").c_str(), 0.032f);
    _ssao_light_shader->SetUniform1f((light_prefix + ".radius").c_str(), 10.0f);
  }

  renderQuad();

  // glBindFramebuffer(GL_READ_FRAMEBUFFER, _gbuffer_fbo);
  // glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
  // glBlitFramebuffer(0, 0, _window_width, _window_height, 0, 0, _window_width, _window_height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);

  // _simple_shader->use();
  // _simple_shader->SetUniformMat4fv("view", glm::value_ptr(view));
  // _simple_shader->SetUniformMat4fv("projection", glm::value_ptr(projection));
  // glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 0.1f, 0.1f));
  // for (int i = 0; i < lightPositions.size(); i++) {
  //   _simple_shader->SetUniformMat4fv(
  //       "model", glm::value_ptr(glm::translate(glm::mat4(1.0f), lightPositions[i]) * scale));
  //   renderBox();
  // }
  ImGui::ColorEdit3("diffuse color", (float*)&_light_color_diffuse); // Edit 3 floats representing a color
  ImGui::ColorEdit3("specular color", (float*)&_light_color_specular); // Edit 3 floats representing a color
}
}; // namespace LearnGL
