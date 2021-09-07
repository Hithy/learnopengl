#include "glframebuffer.h"

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

static float screenVertices[] = {
  -1.0f, -1.0f, 0.0f, 0.0f,
  1.0f, -1.0f, 1.0f, 0.0f,
  1.0f,  1.0f, 1.0f, 1.0f,
  1.0f,  1.0f, 1.0f, 1.0f,
  -1.0f,  1.0f, 0.0f, 1.0f,
  -1.0f, -1.0f, 0.0f, 0.0f,

  0.5f, 0.5f, 0.0f, 0.0f,
  1.0f, 0.5f, 1.0f, 0.0f,
  1.0f,  1.0f, 1.0f, 1.0f,
  1.0f,  1.0f, 1.0f, 1.0f,
  0.5f,  1.0f, 0.0f, 1.0f,
  0.5f, 0.5f, 0.0f, 0.0f,
};

GLFrameBuffer::GLFrameBuffer()
    : GLBase("hello_gl", 1920, 1080) {}

void GLFrameBuffer::GenCubeVAO() {
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

  glBindVertexArray(0);
}

void GLFrameBuffer::GenFloorVAO() {
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
}

void GLFrameBuffer::GenScreenVAO() {
  glGenBuffers(1, &_screen_vao);
  glGenBuffers(1, &_screen_vbo);
  glGenVertexArrays(1, &_screen_vao);
  glBindBuffer(GL_ARRAY_BUFFER, _screen_vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(screenVertices), screenVertices, GL_STATIC_DRAW);

  glBindVertexArray(_screen_vao);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  glBindVertexArray(0);
}

void GLFrameBuffer::GenFrameBuffer() {
  glGenFramebuffers(1, &_fbo);
  glBindFramebuffer(GL_FRAMEBUFFER, _fbo);

  glGenTextures(1, &_fbo_texture);
  glBindTexture(GL_TEXTURE_2D, _fbo_texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _window_width, _window_height, 0,
               GL_RGB, GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glBindTexture(GL_TEXTURE_2D, 0);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _fbo_texture, 0);

  glGenRenderbuffers(1, &_fbo_stencil_depth);
  glBindRenderbuffer(GL_RENDERBUFFER, _fbo_stencil_depth);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, _window_width, _window_height);
  glBindRenderbuffer(GL_RENDERBUFFER, 0);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _fbo_stencil_depth);

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    throw "Framebuffer not complete";
  }

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GLFrameBuffer::Init() {
  _shader = new Shader("shader/depth.vert", "shader/depth.frag");
  _quad_shader = new Shader("shader/fbo.vert", "shader/fbo.frag");

  _cube_texture = GenTexture("asserts/images/container.jpg");
  _floor_texture = GenTexture("asserts/images/metal.png");

  GenFrameBuffer();

  GenCubeVAO();
  GenFloorVAO();
  GenScreenVAO();
}

void GLFrameBuffer::DrawFBO() {
  glEnable(GL_DEPTH_TEST);

  glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  auto view = _camera.GetView();
  auto projection =
      glm::perspective(glm::radians(_camera.fov),
                       (float)_window_width / _window_height, 0.1f, 100.0f);
  _shader->use();
  _shader->SetUniformMat4fv("view", glm::value_ptr(view));
  _shader->SetUniformMat4fv("projection", glm::value_ptr(projection));

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, _cube_texture);
  glBindVertexArray(_cube_vao);
  auto model = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, 0.0f, -1.0f));
  //_shader->SetUniformMat4fv("model", glm::value_ptr(model));
  glDrawArrays(GL_TRIANGLES, 0, 36);
  model = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 0.0f, 0.0f));
  _shader->SetUniformMat4fv("model", glm::value_ptr(model));
  glDrawArrays(GL_TRIANGLES, 0, 36);

  glBindTexture(GL_TEXTURE_2D, _floor_texture);
  glBindVertexArray(_floor_vao);
  model = glm::mat4(1.0f);
  _shader->SetUniformMat4fv("model", glm::value_ptr(model));
  glDrawArrays(GL_TRIANGLES, 0, 6);
}

void GLFrameBuffer::DrawNormal() {
  glDisable(GL_DEPTH_TEST);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  _quad_shader->use();
  glBindTexture(GL_TEXTURE_2D, _fbo_texture);
  glBindVertexArray(_screen_vao);
  glDrawArrays(GL_TRIANGLES, 0, 12);
}

void GLFrameBuffer::Draw() {
  DrawFBO();

  DrawNormal();
}
}; // namespace LearnGL
