#include "glstencil.h"

#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"

#include "stb_image.h"

namespace LearnGL {

float cubeVertices[] = {
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

float planeVertices[] = {
  // positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
  5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
  -5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
  -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
  
  5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
  -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
  5.0f, -0.5f, -5.0f,  2.0f, 2.0f								
};

GLStencil::GLStencil() : GLBase("hello_gl", 1920, 1080) {}

void GLStencil::Init() {
  _shader = new Shader("shader/depth.vert", "shader/depth.frag");
  _simple_shader = new Shader("shader/depth.vert", "shader/stencil.frag");

  // gen texture
  _cube_texture = GenTexture("asserts/images/marble.jpg");
  _floor_texture = GenTexture("asserts/images/metal.png");
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, _cube_texture);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, _floor_texture);

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

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_STENCIL_TEST);
}

void GLStencil::Draw() {
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

  auto view = _camera.GetView();
  auto projection =
      glm::perspective(glm::radians(_camera.fov),
                       (float)_window_width / _window_height, 0.1f, 100.0f);
  _shader->use();
  _shader->SetUniformMat4fv("view", glm::value_ptr(view));
  _shader->SetUniformMat4fv("projection", glm::value_ptr(projection));

  glStencilMask(0xFF);
  glStencilFunc(GL_ALWAYS, 1, 0xff);
  glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE0, _cube_texture);
  glBindVertexArray(_cube_vao);
  auto model = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, 0.0f, -1.0f));
  _shader->SetUniformMat4fv("model", glm::value_ptr(model));
  glDrawArrays(GL_TRIANGLES, 0, 36);
  model = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 0.0f, 0.0f));
  _shader->SetUniformMat4fv("model", glm::value_ptr(model));
  glDrawArrays(GL_TRIANGLES, 0, 36);
  glBindTexture(GL_TEXTURE0, _floor_texture);

  glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
  glBindVertexArray(_floor_vao);
  model = glm::mat4(1.0f);
  _shader->SetUniformMat4fv("model", glm::value_ptr(model));
  glDrawArrays(GL_TRIANGLES, 0, 6);

  _simple_shader->use();
  _simple_shader->SetUniformMat4fv("view", glm::value_ptr(view));
  _simple_shader->SetUniformMat4fv("projection", glm::value_ptr(projection));

  glStencilMask(0xFF);
  glStencilFunc(GL_NOTEQUAL, 1, 0xff);
  glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

  glBindVertexArray(_cube_vao);
  auto scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.02f, 1.02f, 1.02f));
  model = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, 0.0f, -1.0f)) * scale;
  _simple_shader->SetUniformMat4fv("model", glm::value_ptr(model));
  glDrawArrays(GL_TRIANGLES, 0, 36);
  model = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 0.0f, 0.0f)) * scale;
  _simple_shader->SetUniformMat4fv("model", glm::value_ptr(model));
  glDrawArrays(GL_TRIANGLES, 0, 36);
}
}; // namespace LearnGL
