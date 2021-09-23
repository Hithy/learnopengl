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

unsigned int Shader::CreateShader(const char *path, unsigned int type) {
  unsigned int shader;

  auto src_str = get_file_content(path);
  const auto* src = src_str.c_str();

  shader = glCreateShader(type);
  glShaderSource(shader, 1, &src, NULL);
  glCompileShader(shader);

  return shader;
}

Shader::Shader(const char *vert_path, const char *frag_path) {
  auto vert_shader = CreateShader(vert_path, GL_VERTEX_SHADER);
  auto frag_shader = CreateShader(frag_path, GL_FRAGMENT_SHADER); 

  id = glCreateProgram();
  glAttachShader(id, vert_shader);
  glAttachShader(id, frag_shader);
  glLinkProgram(id);

  glDeleteShader(vert_shader);
  glDeleteShader(frag_shader);
}

Shader::Shader(const char *vert_path, const char *gs_path, const char *frag_path) {
  auto vert_shader = CreateShader(vert_path, GL_VERTEX_SHADER);
  auto geo_shader = CreateShader(gs_path, GL_GEOMETRY_SHADER);
  auto frag_shader = CreateShader(frag_path, GL_FRAGMENT_SHADER); 

  id = glCreateProgram();
  glAttachShader(id, vert_shader);
  glAttachShader(id, geo_shader);
  glAttachShader(id, frag_shader);
  glLinkProgram(id);

  glDeleteShader(vert_shader);
  glDeleteShader(geo_shader);
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

void Shader::SetUniformMat4fv(const char *param, const float *ptr) {
  int param_pos = glGetUniformLocation(id, param);
  glUniformMatrix4fv(param_pos, 1, GL_FALSE, ptr);
}

void Shader::SetUniform3fv(const char *param, float x, float y, float z) {
  int param_pos = glGetUniformLocation(id, param);
  glUniform3f(param_pos, x, y, z);
}


