#version 330 core

out vec4 FragColor;
in vec3 ourColor;
in vec2 TexCoord;

// uniform float Elapse;
uniform sampler2D texture1;

float near = 0.1;
float far  = 100.0;

float LinearizeDepth(float depth)
{
  float z = depth * 2.0 - 1.0; // back to NDC
  return (2.0 * near * far) / (far + near - z * (far - near));
}

void main() {
  // float depth = LinearizeDepth(gl_FragCoord.z) / far; // divide by far for demonstration
  // FragColor = vec4(vec3(depth), 1.0);
  vec4 texColor = texture(texture1, TexCoord);
  if (texColor.a < 0.1)
    discard;
  FragColor = texColor;
}
