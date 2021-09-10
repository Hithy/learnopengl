#version 330 core

out vec4 FragColor;
in VS_OUT {
  vec2 TexCoords;
} fs_in;

// uniform float Elapse;
uniform sampler2D texture1;

void main() {
  FragColor = texture(texture1, fs_in.TexCoords);
}
