#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTex;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 WorldPos;
out vec3 Normal;
out vec2 TexCoords;

void main () {
  WorldPos = (model * vec4(aPos, 1.0f)).xyz;
  TexCoords = aTex;
  Normal = transpose(inverse(mat3(model))) * aNormal;

  gl_Position = projection * view * vec4(WorldPos, 1.0);
}
