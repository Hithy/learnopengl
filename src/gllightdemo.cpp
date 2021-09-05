#include "gllightdemo.h"

#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace LearnGL {

glm::vec3 pointLightPositions[] = {
    glm::vec3(0.7f,  0.2f,  2.0f),
    glm::vec3(2.3f, -3.3f, -4.0f),
    glm::vec3(-4.0f,  2.0f, -12.0f),
    glm::vec3(0.0f,  0.0f, -3.0f)
};

float vertices[] = {
    // positions          // normals           // texture coords
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
};

static glm::vec3 cubePositions[] = {
    glm::vec3( 0.0f,  0.0f,  0.0f), 
    glm::vec3( 2.0f,  5.0f, -15.0f), 
    glm::vec3(-1.5f, -2.2f, -2.5f),  
    glm::vec3(-3.8f, -2.0f, -12.3f),  
    glm::vec3( 2.4f, -0.4f, -3.5f),  
    glm::vec3(-1.7f,  3.0f, -7.5f),  
    glm::vec3( 1.3f, -2.0f, -2.5f),  
    glm::vec3( 1.5f,  2.0f, -2.5f), 
    glm::vec3( 1.5f,  0.2f, -1.5f), 
    glm::vec3(-1.3f,  1.0f, -1.5f)  
};

static void load_texture() {
  int width, height, nrChannels;
  const char *image_path_wall = "asserts/images/container2.png";
  const char *image_path_face = "asserts/images/container2_specular.png";
  unsigned char *data = NULL;

  unsigned int texture1, texture2;
  glGenTextures(1, &texture1);
  glGenTextures(1, &texture2);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture1);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  data = stbi_load(image_path_wall, &width, &height, &nrChannels, 0);
  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
  }

  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, texture2);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  stbi_set_flip_vertically_on_load(true);
  data = stbi_load(image_path_face, &width, &height, &nrChannels, 0);
  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
  }
}


GLLightDemo::GLLightDemo() : GLBase("hello_gl", 1920, 1080) {}

void GLLightDemo::Init() {
  _shader = new Shader("shader/light.vert", "shader/light.frag");
  _light_shader = new Shader("shader/light.vert", "shader/light_base.frag");

  LoadTexture();
  InitVAO();

  glEnable(GL_DEPTH_TEST);
}

void GLLightDemo::Draw() {
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindVertexArray(_VAO);

    auto view = _camera.GetView();
    auto projection = glm::perspective(glm::radians(_camera.fov), 1920.0f / 1080.0f, 0.1f, 100.0f);
    auto base_model = glm::mat4(1.0f);
    auto light_model = glm::translate(base_model, glm::vec3(1.2f, 1.0f, 2.0f));
    

    _shader->use();
    _shader->SetUniform3fv("viewPos", _camera.pos.x, _camera.pos.y, _camera.pos.z);

    // directional light
    _shader->SetUniform3fv("dirLight.direction", -0.2f, -1.0f, -0.3f);
    _shader->SetUniform3fv("dirLight.ambient", 0.05f, 0.05f, 0.05f);
    _shader->SetUniform3fv("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
    _shader->SetUniform3fv("dirLight.specular", 0.5f, 0.5f, 0.5f);
    // point light 1
    _shader->SetUniform3fv("pointLights[0].position", pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
    _shader->SetUniform3fv("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
    _shader->SetUniform3fv("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
    _shader->SetUniform3fv("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
    _shader->SetUniform1f("pointLights[0].constant", 1.0f);
    _shader->SetUniform1f("pointLights[0].linear", 0.09);
    _shader->SetUniform1f("pointLights[0].quadratic", 0.032);
    // point light 2
    _shader->SetUniform3fv("pointLights[1].position", pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
    _shader->SetUniform3fv("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
    _shader->SetUniform3fv("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
    _shader->SetUniform3fv("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
    _shader->SetUniform1f("pointLights[1].constant", 1.0f);
    _shader->SetUniform1f("pointLights[1].linear", 0.09);
    _shader->SetUniform1f("pointLights[1].quadratic", 0.032);
    // point light 3
    _shader->SetUniform3fv("pointLights[2].position", pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
    _shader->SetUniform3fv("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
    _shader->SetUniform3fv("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
    _shader->SetUniform3fv("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
    _shader->SetUniform1f("pointLights[2].constant", 1.0f);
    _shader->SetUniform1f("pointLights[2].linear", 0.09);
    _shader->SetUniform1f("pointLights[2].quadratic", 0.032);
    // point light 4
    _shader->SetUniform3fv("pointLights[3].position", pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
    _shader->SetUniform3fv("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
    _shader->SetUniform3fv("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
    _shader->SetUniform3fv("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
    _shader->SetUniform1f("pointLights[3].constant", 1.0f);
    _shader->SetUniform1f("pointLights[3].linear", 0.09);
    _shader->SetUniform1f("pointLights[3].quadratic", 0.032);

    // _shader->SetUniform3fv("material.ambient", 1.0f, 0.5f, 0.31f);
    // _shader->SetUniform3fv("material.diffuse", 1.0f, 0.5f, 0.31f);
    // _shader->SetUniform3fv("material.specular", 0.5f, 0.5f, 0.5f);
    _shader->SetUniform1i("material.diffuse", 0);
    _shader->SetUniform1i("material.specular", 1);
    _shader->SetUniform1f("material.shininess", 32.0f);

    glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
    // lightColor.x = sin(glfwGetTime() * 2.0f);
    // lightColor.y = sin(glfwGetTime() * 0.7f);
    // lightColor.z = sin(glfwGetTime() * 1.3f);

    glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
    glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f);

    _shader->SetUniform3fv("light.ambient", ambientColor.x, ambientColor.y, ambientColor.z);
    _shader->SetUniform3fv("light.diffuse", diffuseColor.x, diffuseColor.y, diffuseColor.z);
    _shader->SetUniform3fv("light.specular", 1.0f, 1.0f, 1.0f);
    // _shader->SetUniform3fv("light.position", 1.2f, 1.0f, 2.0f);
    _shader->SetUniform3fv("light.position", _camera.pos.x, _camera.pos.y, _camera.pos.z);
    auto view_mat = _camera.GetView();
    auto view_forward = glm::inverse(view_mat) * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);
    _shader->SetUniform3fv("light.direction", view_forward.x, view_forward.y, view_forward.z);
    _shader->SetUniform1f("light.cutOff", glm::cos(glm::radians(12.5f)));
    _shader->SetUniform1f("light.outCutOff", glm::cos(glm::radians(17.5f)));

    _shader->SetUniform1f("light.constant", 1.0f);
    _shader->SetUniform1f("light.linear", 0.09f);
    _shader->SetUniform1f("light.quadratic", 0.032f);

    _shader->SetUniformMat4fv("view", glm::value_ptr(view));
    _shader->SetUniformMat4fv("projection", glm::value_ptr(projection));

    for (unsigned int i = 0;
         i < sizeof(cubePositions) / sizeof(cubePositions[0]); i++) {
      auto box_model = glm::translate(base_model, cubePositions[i]);
      float angle = 20.0f * i;
      box_model = glm::rotate(box_model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
      _shader->SetUniformMat4fv("model", glm::value_ptr(box_model));
      glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    _light_shader->use();
    _light_shader->SetUniformMat4fv("view", glm::value_ptr(view));
    _light_shader->SetUniformMat4fv("projection", glm::value_ptr(projection));
    _light_shader->SetUniformMat4fv("model", glm::value_ptr(light_model));
    _light_shader->SetUniform3fv("lightColor", 1.0f, 1.0f, 1.0f);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void GLLightDemo::InitVAO() {
  glGenBuffers(1, &_VBO);
  glGenVertexArrays(1, &_VAO);

  glBindVertexArray(_VAO);

  glBindBuffer(GL_ARRAY_BUFFER, _VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glEnable(GL_DEPTH_TEST);
  glBindVertexArray(0);
}

void GLLightDemo::LoadTexture() {
    int width, height, nrChannels;
  const char *image_path_wall = "asserts/images/container2.png";
  const char *image_path_face = "asserts/images/container2_specular.png";
  unsigned char *data = NULL;

  unsigned int texture1, texture2;
  glGenTextures(1, &texture1);
  glGenTextures(1, &texture2);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture1);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  data = stbi_load(image_path_wall, &width, &height, &nrChannels, 0);
  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
  }

  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, texture2);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  stbi_set_flip_vertically_on_load(true);
  data = stbi_load(image_path_face, &width, &height, &nrChannels, 0);
  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
  }
}
}; // namespace LearnGL
