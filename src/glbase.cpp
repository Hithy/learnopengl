#include "glbase.h"

#include <iostream>
#include <unordered_map>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace LearnGL {

static std::unordered_map<GLFWwindow *, GLBase *> window2gl;

static void framebuffer_size_callback(GLFWwindow *window, int width,
                                      int height) {
  glViewport(0, 0, width, height);
}

void process_input(GLFWwindow *window) {
  auto& camera = window2gl[window]->GetCamera();

  const float camera_speed = 0.05f;

  if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }

  auto view = camera.GetView();
  view = glm::inverse(view);

  auto forward = view * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);
  auto right = view * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    camera.pos += glm::vec3(forward * camera_speed);
  } else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    camera.pos -= glm::vec3(forward * camera_speed);
  } else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    camera.pos -= glm::vec3(right * camera_speed);
  } else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    camera.pos += glm::vec3(right * camera_speed);
  }
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
  static float lastX = xpos;
  static float lastY = ypos;

  auto& camera = window2gl[window]->GetCamera();

  float xoffset = xpos - lastX;
  float yoffset = ypos - lastY;

  lastX = xpos;
  lastY = ypos;

  const float sensitivity = 0.1f;
  xoffset *= sensitivity;
  yoffset *= sensitivity;

  camera.yaw += xoffset;
  camera.pitch -= yoffset;

  if (camera.pitch > 89.0f) {
    camera.pitch = 89.0f;
  }

  if (camera.pitch < -89.0f) {
    camera.pitch = -89.0f;
  }
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
  auto& camera = window2gl[window]->GetCamera();

  camera.fov -= (float)yoffset;
  if (camera.fov < 1.0f) {
    camera.fov = 1.0f;
  }

  if (camera.fov > 60.0f) {
    camera.fov = 60.0f;
  }
}

GLBase::GLBase(const char *title, int width, int height)
    : _window_width(width), _window_height(height) {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#if __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  _window = glfwCreateWindow(width, height, title, NULL, NULL);
  if (!_window) {
    throw "fail to create window";
  }
  window2gl[_window] = this;

  glfwMakeContextCurrent(_window);
  glfwSetFramebufferSizeCallback(_window, framebuffer_size_callback);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    throw "fail to glad load gl loader\n";
  }

  glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetCursorPosCallback(_window, mouse_callback);
  glfwSetScrollCallback(_window, scroll_callback);
}

GLBase::~GLBase() {
  if (_window) {
    glfwTerminate();
  }
}

void GLBase::Init() {
}

void GLBase::Run() {
  while (!glfwWindowShouldClose(_window)) {
    process_input(_window);

    Draw();

    glfwSwapBuffers(_window);
    glfwPollEvents();
  }
}

void GLBase::Draw() {}

unsigned int GenTexture(const char *path) {
  unsigned int texture = 0;
  int width, height, nrChannels;

  unsigned char *data =
      stbi_load(path, &width, &height, &nrChannels, 0);
  if (data) {
    GLenum format;
    if (nrChannels == 1)
      format = GL_RED;
    else if (nrChannels == 3)
      format = GL_RGB;
    else if (nrChannels == 4)
      format = GL_RGBA;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
  }

  return texture;
}

} // namespace LearnGL
