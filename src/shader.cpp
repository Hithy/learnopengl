#include "shader.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <fstream>
#include <string>
#include <cstring>
#include <iostream>

static std::string get_file_content(const char *file_path) {
  std::string res;

  const int BUFF_SIZE = 1024;
  

  auto file = std::fstream(file_path);
  if (file.is_open()) {
    char buffer[BUFF_SIZE + 1] = { 0 };
    
    while (!file.eof()) {
      ::memset(buffer, 0, BUFF_SIZE);
      file.read(buffer, BUFF_SIZE);

      res += buffer;
    }
  }

  return res;
}

Shader::Shader(const char *vert_path, const char *frag_path) {
  auto vert_src_str = get_file_content(vert_path);
  auto frag_src_str = get_file_content(frag_path);

  const auto* vert_src = vert_src_str.c_str();
  const auto* frag_src = frag_src_str.c_str();

  // std::cout << vert_src << std::endl;
  // std::cout << frag_src << std::endl;

  unsigned int vert_shader, frag_shader;

  vert_shader = glCreateShader(GL_VERTEX_SHADER);
  frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
  
  glShaderSource(vert_shader, 1, &vert_src, NULL);
  glShaderSource(frag_shader, 1, &frag_src, NULL);

  glCompileShader(vert_shader);
  glCompileShader(frag_shader);
  

  id = glCreateProgram();
  glAttachShader(id, vert_shader);
  glAttachShader(id, frag_shader);
  glLinkProgram(id);

  glDeleteShader(vert_shader);
  glDeleteShader(frag_shader);
}

void Shader::use() {
  glUseProgram(id);
}

void Shader::SetUniform1f(const char *param, float value) {
  int param_pos = glGetUniformLocation(id, param);
  glUniform1f(param_pos, value);
}

void Shader::SetUniform1i(const char *param, int value) {
  int param_pos = glGetUniformLocation(id, param);
  glUniform1i(param_pos, value);
}

void Shader::SetUniformMat4fv(const char *param, float *ptr) {
  int param_pos = glGetUniformLocation(id, param);
  glUniformMatrix4fv(param_pos, 1, GL_FALSE, ptr);
}
