#version 330 core

out vec4 FragColor;
in vec3 ourColor;
in vec2 TexCoord;

// uniform float Elapse;
uniform sampler2D texture1;

void main() {
  // float depth = LinearizeDepth(gl_FragCoord.z) / far; // divide by far for demonstration
  // FragColor = vec4(vec3(depth), 1.0);
  FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}
