#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexture;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoods;

void main() {
  vec4 world_pos = model * vec4(aPos, 1.0f);
  TexCoods = aTexture;
  FragPos = world_pos.xyz;
  Normal = transpose(inverse(mat3(model))) * aNormal;
  gl_Position = projection * view * world_pos;
}
