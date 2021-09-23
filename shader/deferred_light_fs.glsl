#version 330 core

struct Light {
  vec3 position;

  vec3 diffuse;
  vec3 specular;

  float constant;
  float linear;
  float quadratic;

  float radius;
};

#define MaxLightCount 36
uniform Light light_list[MaxLightCount];
uniform int light_count;
uniform vec3 view_pos;

uniform sampler2D gFragPos;
uniform sampler2D gNormal;
uniform sampler2D gAbleboSpec;

in vec2 TexCoords;
out vec4 FragColor;

void main() {
  vec3 frag_pos = texture(gFragPos, TexCoords).rgb;
  vec3 normal = texture(gNormal, TexCoords).rgb;
  vec3 g_diffuse = texture(gAbleboSpec, TexCoords).rgb;
  float g_specular = texture(gAbleboSpec, TexCoords).a;

  vec3 result = g_diffuse * 0.1;
  vec3 view_dir = normalize(view_pos - frag_pos);

  for (int i=0; i<light_count; i++) {
    float light_distance = length(light_list[i].position - frag_pos);

    if (light_distance < light_list[i].radius) {
      vec3 light_dir = normalize(light_list[i].position - frag_pos);
      vec3 halfway_dir = normalize(light_dir + view_dir);

      vec3 diffuse = light_list[i].diffuse * max(dot(light_dir, normal), 0.0) * g_diffuse;

      float spec = pow(max(0.0, dot(halfway_dir, normal)), 16.0f);
      vec3 specular = light_list[i].specular * spec * g_specular;

      float attenuation = 1.0 / (light_list[i].constant + light_list[i].linear * light_distance + light_list[i].quadratic * light_distance * light_distance);

      result += (diffuse + specular) * attenuation;
    }
  }

  FragColor = vec4(result, 1.0);
}
