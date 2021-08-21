#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <chrono>
#include <complex>

#include "shader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using std::cout;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

const char* vert_path = "../shader/simple.vert";
const char* frag_path = "../shader/simple.frag";

float vertices[] = {
 -0.5f, 0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   0.0f, 1.0f,
 0.5f, 0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f,
 0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f,
 -0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f,

 -0.5f, 0.5f, 1.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
 0.5f, 0.5f, 1.0f,   0.0f, 1.0f, 0.0f,   0.0f, 1.0f,
 0.5f, -0.5f, 1.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
 -0.5f, -0.5f, 1.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
};

unsigned int indices[] = {
  0, 1, 2,
  0, 2, 3,

  1, 5, 6,
  1, 6, 2,

  5, 4, 7,
  5, 7, 6,

  4, 0, 3,
  4, 3, 7,

  4, 5, 1,
  4, 1, 0,

  3, 2, 6,
  3, 6, 7
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

  auto model = glm::mat4(1.0f);
  model = glm::rotate(model, glm::radians(-60.0f), glm::vec3(1.0f, 0.0f, 0.0f));

  auto view = glm::mat4(1.0f);
  view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

  auto projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

  Shader my_shader(vert_path, frag_path);

  {
    int width, height, nrChannels;
    const char *image_path_wall = "../asserts/images/wall.jpg";
    const char *image_path_face = "../asserts/images/awesomeface.png";
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
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                   GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);
      stbi_image_free(data);
    } else {
      cout << "fail to open image: " << image_path_wall << std::endl;
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
    } else {
      cout << "fail to open image: " << image_path_face << std::endl;
    }
  }

  unsigned int VBO, VAO, EBO;

  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);
  glGenVertexArrays(1, &VAO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)(6 * sizeof(float)));
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  
  // trans = glm::rotate(trans, glm::radians(30.0f), glm::vec3(0.0f, 0.0f, 1.0f));
  // trans = glm::scale(trans, glm::vec3(0.5f, 0.5f, 0.5f));

  uint64_t currt_frame = 0;
  glEnable(GL_DEPTH_TEST);
  while (!glfwWindowShouldClose(window)) {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    my_shader.use();
    my_shader.SetUniform1i("texture1", 0);
    my_shader.SetUniform1i("texture2", 1);

    model = glm::rotate(model, glm::radians(1.0f), glm::vec3(0.5f, 1.0f, 0.0f));
    
    my_shader.SetUniformMat4fv("model", glm::value_ptr(model));
    my_shader.SetUniformMat4fv("projection", glm::value_ptr(projection));
    my_shader.SetUniformMat4fv("view", glm::value_ptr(view));
    auto now_time = std::chrono::steady_clock::now();

    // auto delta = std::chrono::duration_cast<
    //     std::chrono::duration<float, std::ratio<1, 10> > >(
    //     now_time.time_since_epoch());
    // my_shader.SetUniform1f("Elapse", (std::sin(delta.count()) + 1) / 2.0f);
    // auto trans = glm::mat4(1.0f);
    // trans = glm::translate(trans, glm::vec3(0.5f, 0.5f, 0.5f));
    // trans = glm::rotate(trans, glm::radians(0.0f + currt_frame), glm::vec3(0.0f, 0.0f, 1.0f));
    // my_shader.SetUniformMat4fv("transform", glm::value_ptr(trans));

    glBindVertexArray(VAO);
    // glDrawArrays(GL_TRIANGLES, 0, 3);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

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
