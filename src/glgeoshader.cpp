#include "glgeoshader.h"

#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"

#include "stb_image.h"

namespace LearnGL {

static float cubeVertices[] = {
  // positions          // texture Coords
  -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
  0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
  0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
  0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
  -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
  -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

  -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
  0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
  0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
  0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
  -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
  -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

  -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
  -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
  -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
  -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
  -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
  -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

  0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
  0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
  0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
  0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
  0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
  0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

  -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
  0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
  0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
  0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
  -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
  -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

  -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
  0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
  0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
  0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
  -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
  -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

static float geoPoints[] = {
  0.0f, 0.0f, 0.0f,
};

GLGeoShader::GLGeoShader()
    : GLBase("hello_gl", 1920, 1080) {}

unsigned int GLGeoShader::GenGeoVAO() {
  unsigned int vao;
  unsigned int vbo;
  
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(geoPoints), geoPoints, GL_STATIC_DRAW);

  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  return vao;
}

void GLGeoShader::Init() {
  _rgb_shader_r = new Shader("shader/glsl_adv.vert", "shader/geoshader.gs", "shader/glsl_r.frag");
  _rgb_shader_g = new Shader("shader/glsl_adv.vert", "shader/geoshader.gs", "shader/glsl_g.frag");
  _rgb_shader_b = new Shader("shader/glsl_adv.vert", "shader/geoshader.gs", "shader/glsl_b.frag");
  _rgb_shader_y = new Shader("shader/glsl_adv.vert", "shader/geoshader.gs", "shader/glsl_y.frag");

  _geo_shader = new Shader("shader/glsl_adv.vert", "shader/geoshader.gs", "shader/glsl_y.frag");

  // gen texture
  _cube_texture = GenTexture("asserts/images/marble.jpg");

  glGenBuffers(1, &_camera_pos_buffer);
  glBindBuffer(GL_UNIFORM_BUFFER, _camera_pos_buffer);
  glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
  glBindBuffer(GL_UNIFORM_BUFFER, 0);

  glGenBuffers(1, &_cube_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, _cube_vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

  glGenVertexArrays(1, &_cube_vao);
  glBindVertexArray(_cube_vao);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  glBindVertexArray(0);

  _geo_vao = GenGeoVAO();

  unsigned int rgb_shader_mat_idx_r = glGetUniformBlockIndex(_rgb_shader_r->GetID(), "Matrics");
  unsigned int rgb_shader_mat_idx_g = glGetUniformBlockIndex(_rgb_shader_g->GetID(), "Matrics");
  unsigned int rgb_shader_mat_idx_b = glGetUniformBlockIndex(_rgb_shader_b->GetID(), "Matrics");
  unsigned int rgb_shader_mat_idx_y = glGetUniformBlockIndex(_rgb_shader_y->GetID(), "Matrics");
  unsigned int geo_shader_mat_idx = glGetUniformBlockIndex(_geo_shader->GetID(), "Matrics");

  glUniformBlockBinding(_rgb_shader_r->GetID(), rgb_shader_mat_idx_r, 0);
  glUniformBlockBinding(_rgb_shader_g->GetID(), rgb_shader_mat_idx_g, 0);
  glUniformBlockBinding(_rgb_shader_b->GetID(), rgb_shader_mat_idx_b, 0);
  glUniformBlockBinding(_rgb_shader_y->GetID(), rgb_shader_mat_idx_y, 0);
  glUniformBlockBinding(_geo_shader->GetID(), geo_shader_mat_idx, 0);

  glBindBufferBase(GL_UNIFORM_BUFFER, 0, _camera_pos_buffer);

  // glEnable(GL_PROGRAM_POINT_SIZE);
  glEnable(GL_DEPTH_TEST);
}

void GLGeoShader::UpdateCameraPos() {
  auto view = _camera.GetView();
  auto projection = glm::perspective(glm::radians(_camera.fov), (float) _window_width / _window_height,
                                     0.1f, 100.0f);

  glBindBuffer(GL_UNIFORM_BUFFER, _camera_pos_buffer);
  glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(view), glm::value_ptr(view));
  glBufferSubData(GL_UNIFORM_BUFFER, sizeof(view), sizeof(projection), glm::value_ptr(projection));
  glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void GLGeoShader::Draw() {
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  UpdateCameraPos();

  auto base_model = glm::mat4(1.0f);
  auto model = glm::mat4(1.0f);

  // glBindVertexArray(_cube_vao);
  glBindVertexArray(_geo_vao);

  _rgb_shader_r->use();
  model = glm::translate(base_model, glm::vec3(-0.6f, 0.6f, -3.0f));
  _rgb_shader_r->SetUniformMat4fv("model", glm::value_ptr(model));
  glDrawArrays(GL_POINTS, 0, 1);

  _rgb_shader_g->use();
  model = glm::translate(base_model, glm::vec3(0.6f, 0.6f, -3.0f));
  _rgb_shader_g->SetUniformMat4fv("model", glm::value_ptr(model));
  glDrawArrays(GL_POINTS, 0, 1);

  _rgb_shader_b->use();
  model = glm::translate(base_model, glm::vec3(-0.6f, -0.6f, -3.0f));  
  _rgb_shader_b->SetUniformMat4fv("model", glm::value_ptr(model));
  glDrawArrays(GL_POINTS, 0, 1);

  _rgb_shader_y->use();
  model = glm::translate(base_model, glm::vec3(0.6f, -0.6f, -3.0f));
  _rgb_shader_y->SetUniformMat4fv("model", glm::value_ptr(model));
  glDrawArrays(GL_POINTS, 0, 1);
}
}; // namespace LearnGL
