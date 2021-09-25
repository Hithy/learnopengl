#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexture;

uniform mat4 view;
uniform mat4 model;
uniform mat4 projection;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoods;

void main() {
  vec4 view_pos = view * model * vec4(aPos, 1.0f);
  TexCoods = aTexture;
  FragPos = view_pos.xyz;
  Normal = transpose(inverse(mat3(view * model))) * aNormal;
  gl_Position = projection * view_pos;
}
