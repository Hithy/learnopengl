#include "gldeferred.h"

#include <string>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"

#include "stb_image.h"
#include "utils.h"

namespace LearnGL {

glm::vec3 objectPositions[] = {
    glm::vec3(-5.0, -3.0, -5.0),
    glm::vec3(0.0, -3.0, -5.0),
    glm::vec3(5.0, -3.0, -5.0),
    glm::vec3(-5.0, -3.0, 0.0),
    glm::vec3(0.0, -3.0, 0.0),
    glm::vec3(5.0, -3.0, 0.0),
    glm::vec3(-5.0, -3.0, 5.0),
    glm::vec3(0.0, -3.0, 5.0),
    glm::vec3(5.0, -3.0, 5.0)
};

std::vector<glm::vec3> lightPositions = {
  glm::vec3(-5.0, -1.0, -5.0),
  glm::vec3(0.0, -1.0, -5.0),
  glm::vec3(5.0, -1.0, -5.0),
  glm::vec3(-5.0, -1.0, 0.0),
  glm::vec3(0.0, -1.0, 0.0),
  glm::vec3(5.0, -1.0, 0.0),
  glm::vec3(-5.0, -1.0, 5.0),
  glm::vec3(0.0, -1.0, 5.0),
  glm::vec3(5.0, -1.0, 5.0),
};

GLDeferred::GLDeferred() : GLBase("hello_gl", 1920, 1080) {}

void GLDeferred::Init() {
  _g_buffer_shader = new Shader("shader/gbuffer_vs.glsl", "shader/gbuffer_fs.glsl");
  _light_shader = new Shader("shader/deferred_light_vs.glsl", "shader/deferred_light_fs.glsl");
  _simple_shader = new Shader("shader/deferred_simple_vs.glsl", "shader/deferred_simple_fs.glsl");

  GenGbuffer();

  // gen texture
  _cube_texture = GenTexture("asserts/images/container2.png");
  _cube_specular = GenTexture("asserts/images/container2_specular.png");
  // _floor_texture = GenTexture("asserts/images/metal.png");

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
}

void GLDeferred::GenGbuffer() {
  glGenFramebuffers(1, &_gbuffer_fbo);
  glGenTextures(1, &_gbuffer_position);
  glGenTextures(1, &_gbuffer_normal);
  glGenTextures(1, &_gbuffer_ablebo);

  glBindFramebuffer(GL_FRAMEBUFFER, _gbuffer_fbo);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, _gbuffer_position);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, _window_width, _window_height, 0, GL_RGBA, GL_FLOAT, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _gbuffer_position, 0);

  glBindTexture(GL_TEXTURE_2D, _gbuffer_normal);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, _window_width, _window_height, 0, GL_RGBA, GL_FLOAT, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, _gbuffer_normal, 0);

  glBindTexture(GL_TEXTURE_2D, _gbuffer_ablebo);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, _window_width, _window_height, 0, GL_RGBA, GL_FLOAT, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, _gbuffer_ablebo, 0);

  unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
  glDrawBuffers(3, attachments);

  unsigned int depth_buffer;
  glGenRenderbuffers(1, &depth_buffer);
  glBindRenderbuffer(GL_RENDERBUFFER, depth_buffer);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, _window_width, _window_height);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_buffer);

  glBindTexture(GL_TEXTURE_2D, 0);
  glBindRenderbuffer(GL_RENDERBUFFER, 0);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GLDeferred::Draw() {
  glBindFramebuffer(GL_FRAMEBUFFER, _gbuffer_fbo);
  
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  _g_buffer_shader->use();
  auto view = _camera.GetView();
  auto projection = glm::perspective(glm::radians(_camera.fov), 1920.0f / 1080.0f,
                                     0.1f, 100.0f);
  auto model = glm::mat4(1.0f);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, _cube_texture);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, _cube_specular);
  _g_buffer_shader->SetUniform1i("diffuse", 0);
  _g_buffer_shader->SetUniform1i("specular", 1);

  _g_buffer_shader->SetUniformMat4fv("view", glm::value_ptr(view));
  _g_buffer_shader->SetUniformMat4fv("projection", glm::value_ptr(projection));

  for (auto const &obj_pos : objectPositions) {
    _g_buffer_shader->SetUniformMat4fv(
        "model", glm::value_ptr(glm::translate(model, obj_pos)));
    renderBox();
  }

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_DEPTH_BUFFER_BIT);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, _gbuffer_position);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, _gbuffer_normal);
  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_2D, _gbuffer_ablebo);

  _light_shader->use();
  _light_shader->SetUniform1i("gFragPos", 0);
  _light_shader->SetUniform1i("gNormal", 1);
  _light_shader->SetUniform1i("gAbleboSpec", 2);
  _light_shader->SetUniform3fv("view_pos", _camera.pos.x, _camera.pos.y, _camera.pos.z);

  _light_shader->SetUniform1i("light_count", lightPositions.size());

  for (int i = 0; i < lightPositions.size(); i++) {
    auto const& light_pos = lightPositions[i];
    std::string light_prefix = "light_list[" + std::to_string(i) + "]";

    _light_shader->SetUniform3fv((light_prefix + ".position").c_str(), light_pos.x, light_pos.y, light_pos.z);

    _light_shader->SetUniform3fv((light_prefix + ".diffuse").c_str(), 1.0f, 1.0f, 1.0f);
    _light_shader->SetUniform3fv((light_prefix + ".specular").c_str(), 1.0f, 1.0f, 1.0f);
    _light_shader->SetUniform1f((light_prefix + ".constant").c_str(), 1.0f);
    _light_shader->SetUniform1f((light_prefix + ".linear").c_str(), 0.0f);
    _light_shader->SetUniform1f((light_prefix + ".quadratic").c_str(), 0.0f);
    _light_shader->SetUniform1f((light_prefix + ".radius").c_str(), 2.0f);
  }

  renderQuad();

  glBindFramebuffer(GL_READ_FRAMEBUFFER, _gbuffer_fbo);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
  glBlitFramebuffer(0, 0, _window_width, _window_height, 0, 0, _window_width, _window_height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);

  _simple_shader->use();
  _simple_shader->SetUniformMat4fv("view", glm::value_ptr(view));
  _simple_shader->SetUniformMat4fv("projection", glm::value_ptr(projection));
  glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 0.1f, 0.1f));
  for (int i = 0; i < lightPositions.size(); i++) {
    _simple_shader->SetUniformMat4fv(
        "model", glm::value_ptr(glm::translate(glm::mat4(1.0f), lightPositions[i]) * scale));
    renderBox();
  }
}
}; // namespace LearnGL
