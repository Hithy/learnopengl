#include "glglsl.h"

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

static float planeVertices[] = {
  // positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
  5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
  -5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
  -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
  
  5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
  -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
  5.0f, -0.5f, -5.0f,  2.0f, 2.0f								
};

GLGLSL::GLGLSL() : GLBase("hello_gl", 1920, 1080) {}

void GLGLSL::Init() {
  _shader = new Shader("shader/glsl_adv.vert", "shader/glsl_adv.frag");
  _rgb_shader_r = new Shader("shader/glsl_adv.vert", "shader/glsl_r.frag");
  _rgb_shader_g = new Shader("shader/glsl_adv.vert", "shader/glsl_g.frag");
  _rgb_shader_b = new Shader("shader/glsl_adv.vert", "shader/glsl_b.frag");
  _rgb_shader_y = new Shader("shader/glsl_adv.vert", "shader/glsl_y.frag");

  // gen texture
  _cube_texture = GenTexture("asserts/images/marble.jpg");
  _floor_texture = GenTexture("asserts/images/metal.png");

  glGenBuffers(1, &_camera_pos_buffer);
  glBindBuffer(GL_UNIFORM_BUFFER, _camera_pos_buffer);
  glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
  glBindBuffer(GL_UNIFORM_BUFFER, 0);

  glGenBuffers(1, &_cube_vao);
  glGenBuffers(1, &_cube_vbo);
  glGenVertexArrays(1, &_cube_vao);
  glBindBuffer(GL_ARRAY_BUFFER, _cube_vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

  glBindVertexArray(_cube_vao);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  glGenBuffers(1, &_floor_vao);
  glGenBuffers(1, &_floor_vbo);
  glGenVertexArrays(1, &_floor_vao);
  glBindBuffer(GL_ARRAY_BUFFER, _floor_vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);

  glBindVertexArray(_floor_vao);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  glBindVertexArray(0);

  unsigned int camera_idx = glGetUniformBlockIndex(_shader->GetID(), "Matrics");
  unsigned int rgb_shader_mat_idx_r = glGetUniformBlockIndex(_rgb_shader_r->GetID(), "Matrics");
  unsigned int rgb_shader_mat_idx_g = glGetUniformBlockIndex(_rgb_shader_g->GetID(), "Matrics");
  unsigned int rgb_shader_mat_idx_b = glGetUniformBlockIndex(_rgb_shader_b->GetID(), "Matrics");
  unsigned int rgb_shader_mat_idx_y = glGetUniformBlockIndex(_rgb_shader_y->GetID(), "Matrics");

  glUniformBlockBinding(_shader->GetID(), camera_idx, 0);
  glUniformBlockBinding(_rgb_shader_r->GetID(), rgb_shader_mat_idx_r, 0);
  glUniformBlockBinding(_rgb_shader_g->GetID(), rgb_shader_mat_idx_g, 0);
  glUniformBlockBinding(_rgb_shader_b->GetID(), rgb_shader_mat_idx_b, 0);
  glUniformBlockBinding(_rgb_shader_y->GetID(), rgb_shader_mat_idx_y, 0);

  glBindBufferBase(GL_UNIFORM_BUFFER, 0, _camera_pos_buffer);

  // glEnable(GL_PROGRAM_POINT_SIZE);
  glEnable(GL_DEPTH_TEST);
}

void GLGLSL::UpdateCameraPos() {
  auto view = _camera.GetView();
  auto projection = glm::perspective(glm::radians(_camera.fov), 800.0f / 600.0f,
                                     0.1f, 100.0f);

  glBindBuffer(GL_UNIFORM_BUFFER, _camera_pos_buffer);
  glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(view), glm::value_ptr(view));
  glBufferSubData(GL_UNIFORM_BUFFER, sizeof(view), sizeof(projection), glm::value_ptr(projection));
  glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void GLGLSL::Draw() {
  glClearColor(0.0f, 0.0f, 0.6f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  UpdateCameraPos();
  
  _shader->use();
  auto base_model = glm::mat4(1.0f);
  auto model = glm::mat4(1.0f);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, _cube_texture);
  glBindVertexArray(_cube_vao);
  model = glm::translate(base_model, glm::vec3(0.0f, 0.0f, -3.0f));
  _shader->SetUniformMat4fv("model", glm::value_ptr(model));
  // glDrawArrays(GL_TRIANGLES, 0, 36);

  _rgb_shader_r->use();
  model = glm::translate(base_model, glm::vec3(-0.6f, 0.6f, -3.0f));
  _rgb_shader_r->SetUniformMat4fv("model", glm::value_ptr(model));
  glDrawArrays(GL_TRIANGLES, 0, 36);

  _rgb_shader_g->use();
  model = glm::translate(base_model, glm::vec3(0.6f, 0.6f, -3.0f));
  _rgb_shader_g->SetUniformMat4fv("model", glm::value_ptr(model));
  glDrawArrays(GL_TRIANGLES, 0, 36);

  _rgb_shader_b->use();
  model = glm::translate(base_model, glm::vec3(-0.6f, -0.6f, -3.0f));  
  _rgb_shader_b->SetUniformMat4fv("model", glm::value_ptr(model));
  glDrawArrays(GL_TRIANGLES, 0, 36);

  _rgb_shader_y->use();
  model = glm::translate(base_model, glm::vec3(0.6f, -0.6f, -3.0f));
  _rgb_shader_y->SetUniformMat4fv("model", glm::value_ptr(model));
  glDrawArrays(GL_TRIANGLES, 0, 36);
}
}; // namespace LearnGL
