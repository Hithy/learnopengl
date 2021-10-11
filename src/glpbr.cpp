#include "glpbr.h"

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
  glm::vec3(0.0, -1.0, -5.0),
  glm::vec3(5.0, -1.0, -5.0),
  glm::vec3(-5.0, -1.0, 0.0),
  // glm::vec3(0.0, -1.0, 0.0),
  // glm::vec3(5.0, -1.0, 0.0),
  // glm::vec3(-5.0, -1.0, 5.0),
  // glm::vec3(0.0, -1.0, 5.0),
  // glm::vec3(5.0, -1.0, 5.0),
};

GLPbr::GLPbr() : GLBase("hello_gl", 1920, 1080) {
}

void GLPbr::Init() {
  _pbr_shader = new Shader("shader/pbr_vs.glsl", "shader/pbr_fs.glsl");

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
}

void GLPbr::Draw() {
  auto view = _camera.GetView();
  auto projection = glm::perspective(glm::radians(_camera.fov), 1920.0f / 1080.0f,
                                     0.1f, 100.0f);

  _pbr_shader->use();
  _pbr_shader->SetUniformMat4fv("view", glm::value_ptr(view));
  _pbr_shader->SetUniformMat4fv("projection", glm::value_ptr(projection));

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  for (int i = 0; i < lightPositions.size(); i++) {
    auto const& light_pos = lightPositions[i];
    std::string light_prefix = "point_light[" + std::to_string(i) + "]";

    _pbr_shader->SetUniform3fv((light_prefix + ".position").c_str(), light_pos.x, light_pos.y, light_pos.z);

    _pbr_shader->SetUniform3fv((light_prefix + ".diffuse").c_str(), 1.0f, 1.0f, 1.0f);
    _pbr_shader->SetUniform3fv((light_prefix + ".specular").c_str(), 1.0f, 1.0f, 1.0f);
    _pbr_shader->SetUniform1f((light_prefix + ".constant").c_str(), 1.0f);
    _pbr_shader->SetUniform1f((light_prefix + ".linear").c_str(), 0.09f);
    _pbr_shader->SetUniform1f((light_prefix + ".quadratic").c_str(), 0.032f);
    _pbr_shader->SetUniform1f((light_prefix + ".radius").c_str(), 10.0f);
  }

  for (auto const &obj_pos : objectPositions) {
    auto trans = glm::translate(glm::mat4(1.0f), obj_pos);
    _pbr_shader->SetUniformMat4fv(
        "model", glm::value_ptr(trans));
    renderSphere();
  }

  // renderQuad();

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
  // ImGui::ColorEdit3("diffuse color", (float*)&_light_color_diffuse); // Edit 3 floats representing a color
  // ImGui::ColorEdit3("specular color", (float*)&_light_color_specular); // Edit 3 floats representing a color
}
}; // namespace LearnGL
