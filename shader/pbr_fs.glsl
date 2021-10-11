#version 330 core

struct Light {
  vec3 position;
  vec3 direction;

  vec3 diffuse;
  vec3 specular;

  float constant;
  float linear;
  float quadratic;

  float radius;
};

// light
uniform Light point_lights[4]; // only point light

// per obj
uniform float roughness;
uniform float metallic;
uniform vec3 albedo;

// viewer
uniform vec3 cam_pos;

const float PI = 3.14159265359;

in vec3 WorldPos;
in vec2 TexCoords;
in vec3 Normal;

out vec4 FragColor;

float DistributionGGX(vec3 N, vec3 H, float roughness);
float GeometrySchlickGGX(float NdotV, float roughness);
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness);
vec3 fresnelSchlick(float cosTheta, vec3 F0);

void main() {
  vec3 N = normalize(Normal);
  vec3 V = normalize(cam_pos - WorldPos);

  vec3 sum_color = vec3(0.0);

  for (int i=0; i<4; i++) {
    Light light = point_lights[i];
    vec3 L = normalize(light.position - WorldPos);
    vec3 H = normalize(L + V);

    vec3 light_color = light.diffuse;
    float dist = length(light.position - WorldPos);
    float attenuation = 1.0 / (dist * dist);
    vec3 radiance = light_color * attenuation;

    float NDF = DistributionGGX(N, H, roughness);
    float G = GeometrySmith(N, V, L, roughness);

    vec3 F0 = vec3(0.4);
    F0 = mix(F0, albedo, metallic);
    vec3 F = fresnelSchlick(max(0.0, dot(V, H)), F0);

    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - metallic;

    float VdotN = max(0.0, dot(V, N));
    float LdotN = max(0.0, dot(L, N));

    vec3 numerator = NDF * G * F;
    float denominator = 4 * VdotN * LdotN + 0.0001;

    vec3 inner = (kD * albedo / PI) + numerator / denominator;
    sum_color += inner * radiance * LdotN;
  }

  vec3 ambient = vec3(0.3) * albedo;
  vec3 color = sum_color + ambient;

  // hdr
  color = color / (color + vec3(1.0));
  color = pow(color, vec3(1.0 / 2.2));

  FragColor = vec4(color, 1.0);
}

vec3 fresnelSchlick(float cosTheta, vec3 F0) {
  return F0 + (1.0 + F0) * pow(1.0 - cosTheta, 5.0);
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
  float a = roughness * roughness;
  float k = (a + 1.0) * (a + 1.0) / 8.0;

  return NdotV / (NdotV * (1.0 - k) + k);
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {
  float NdotV = max(0.0, dot(N, V));
  float NdotL = max(0.0, dot(N, L));

  return GeometrySchlickGGX(NdotV, roughness) * GeometrySchlickGGX(NdotL, roughness);
}

float DistributionGGX(vec3 N, vec3 H, float roughness) {
  float a = roughness * roughness;
  float a2 = a * a;

  float NdotH = max(dot(N, H), 0.0);
  float NdotH2 = NdotH * NdotH;

  float tmp = (NdotH2 * (a2 - 1.0) + 1.0);

  return a2 / (PI * tmp * tmp);
}
