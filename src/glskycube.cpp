#include "glskycube.h"

#include <string>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"

#include "stb_image.h"

namespace LearnGL {

static float skyboxVertices[] = {
    // positions
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
};

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

GLSkyCube::GLSkyCube() : GLBase("hello_gl", 1920, 1080) {}

void GLSkyCube::GenSkyBoxVAO() {
  glGenBuffers(1, &_skybox_vbo);
  glGenVertexArrays(1, &_skybox_vao);

  glBindVertexArray(_skybox_vao);
  glBindBuffer(GL_ARRAY_BUFFER, _skybox_vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glBindVertexArray(0);
}

unsigned int GLSkyCube::GenSkyBoxTextures() {
  unsigned int skybox_texture;
  glGenTextures(1, &skybox_texture);
  glBindTexture(GL_TEXTURE_CUBE_MAP, skybox_texture);

  std::vector<std::string> faces{"right.jpg",  "left.jpg",  "top.jpg",
                                 "bottom.jpg", "front.jpg", "back.jpg"};

  for (int i = 0; i < faces.size(); i++) {
    const auto file_path = "asserts/images/skybox/" + faces[i];
    int width, height, nrChannels;
    unsigned char *data =
        stbi_load(file_path.c_str(), &width, &height, &nrChannels, 0);

    if (!data) {
      throw std::string("fail to open file: ") + file_path;
    }

    GLenum format;
    if (nrChannels == 1)
      format = GL_RED;
    else if (nrChannels == 3)
      format = GL_RGB;
    else if (nrChannels == 4)
      format = GL_RGBA;

    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format,
                 GL_UNSIGNED_BYTE, data);
    stbi_image_free(data);
  }

  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

  return skybox_texture;
}

void GLSkyCube::Init() {
  _shader = new Shader("shader/depth.vert", "shader/depth.frag");
  _skybox_shader = new Shader("shader/skybox.vert", "shader/skybox.frag");

  // gen texture
  _cube_texture = GenTexture("asserts/images/marble.jpg");
  _floor_texture = GenTexture("asserts/images/metal.png");
  _skybox_texture = GenSkyBoxTextures();

  glGenBuffers(1, &_cube_vbo);
  glGenVertexArrays(1, &_cube_vao);
  glBindBuffer(GL_ARRAY_BUFFER, _cube_vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

  glBindVertexArray(_cube_vao);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  glGenBuffers(1, &_floor_vbo);
  glGenVertexArrays(1, &_floor_vao);
  glBindBuffer(GL_ARRAY_BUFFER, _floor_vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);

  glBindVertexArray(_floor_vao);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  GenSkyBoxVAO();

  glBindVertexArray(0);

  glEnable(GL_DEPTH_TEST);
}

void GLSkyCube::DrawSkybox() {
  auto skybox_view = glm::mat4(glm::mat3(_camera.GetView()));
  auto projection = glm::perspective(glm::radians(_camera.fov), 800.0f / 600.0f,
                                     0.1f, 100.0f);
  glDepthMask(GL_FALSE);
  _skybox_shader->use();
  _skybox_shader->SetUniformMat4fv("view", glm::value_ptr(skybox_view));
  _skybox_shader->SetUniformMat4fv("projection", glm::value_ptr(projection));
  glBindVertexArray(_skybox_vao);
  glBindTexture(GL_TEXTURE_CUBE_MAP, _skybox_texture);
  glDrawArrays(GL_TRIANGLES, 0, 36);
  glDepthMask(GL_TRUE);
}

void GLSkyCube::Draw() {
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  DrawSkybox();

  auto view = _camera.GetView();
  auto projection = glm::perspective(glm::radians(_camera.fov), 800.0f / 600.0f,
                                     0.1f, 100.0f);
  auto model = glm::mat4(1.0f);

  _shader->use();
  _shader->SetUniformMat4fv("view", glm::value_ptr(view));
  _shader->SetUniformMat4fv("projection", glm::value_ptr(projection));

  glActiveTexture(GL_TEXTURE0);

  glBindVertexArray(_cube_vao);
  glBindTexture(GL_TEXTURE_2D, _cube_texture);
  model = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, 0.0f, -1.0f));
  _shader->SetUniformMat4fv("model", glm::value_ptr(model));
  glDrawArrays(GL_TRIANGLES, 0, 36);
  model = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 0.0f, 0.0f));
  _shader->SetUniformMat4fv("model", glm::value_ptr(model));
  glDrawArrays(GL_TRIANGLES, 0, 36);

  glBindVertexArray(_floor_vao);
  glBindTexture(GL_TEXTURE_2D, _floor_texture);
  model = glm::mat4(1.0f);
  _shader->SetUniformMat4fv("model", glm::value_ptr(model));
  glDrawArrays(GL_TRIANGLES, 0, 6);
  
}
}; // namespace LearnGL
