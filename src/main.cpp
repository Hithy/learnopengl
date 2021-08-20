#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <chrono>
#include <complex>

#include "shader.h"

using std::cout;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

const char* vert_path = "../shader/simple.vert";
const char* frag_path = "../shader/simple.frag";

float vertices[] = {
  -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
  0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
  0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f
};

void test_glm() {
    glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
    glm::mat4 trans = glm::mat4(1.0f);

    trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));
    vec = trans * vec;

    std::cout << vec.x << vec.y << vec.z << std::endl;
}


int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#if __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  auto *window = glfwCreateWindow(800, 600, "hello_gl", NULL, NULL);
  if (!window) {
    cout << "fail to create window\n";
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    cout << "fail to glad load gl loader\n";
    return -1;
  }

  Shader my_shader(vert_path, frag_path);

  unsigned int VBO, VAO;

  glGenBuffers(1, &VBO);
  glGenVertexArrays(1, &VAO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  
  // trans = glm::rotate(trans, glm::radians(30.0f), glm::vec3(0.0f, 0.0f, 1.0f));
  // trans = glm::scale(trans, glm::vec3(0.5f, 0.5f, 0.5f));
  

  uint64_t currt_frame = 0;
  while (!glfwWindowShouldClose(window)) {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    my_shader.use();
    auto now_time = std::chrono::steady_clock::now();

    // auto delta = std::chrono::duration_cast<
    //     std::chrono::duration<float, std::ratio<1, 10> > >(
    //     now_time.time_since_epoch());
    // my_shader.SetUniform1f("Elapse", (std::sin(delta.count()) + 1) / 2.0f);
    auto trans = glm::mat4(1.0f);
    trans = glm::rotate(trans, glm::radians(0.0f + currt_frame), glm::vec3(0.0f, 0.0f, 1.0f));
    trans = glm::translate(trans, glm::vec3(0.5f, 0.5f, 0.5f));
    my_shader.SetUniformMat4fv("transform", glm::value_ptr(trans));

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(window);
    glfwPollEvents();

    currt_frame++;
  }

  glfwTerminate();

  return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
