#version 330 core

layout (location = 0) out vec3 gFragPos;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAbleboSpec;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoods;

void main() {
  gFragPos = FragPos;
  gNormal = normalize(Normal);
  gAbleboSpec.rgb = vec3(0.95f);
  // gAbleboSpec.rgb = texture(diffuse, TexCoods).rgb;
  // gAbleboSpec.a = 1.0;
  // gAbleboSpec.a = texture(specular, TexCoods).r;
}
