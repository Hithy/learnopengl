#version 330 core

out float FragColor;

in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gNoise;

uniform mat4 projection;

#define SAMPLE_COUNT 64
uniform vec3 ssao_sample[SAMPLE_COUNT];

int kernelSize = 64;
float radius = 0.5;
float bias = 0.025;

// tile noise texture over screen based on screen dimensions divided by noise size
const vec2 noiseScale = vec2(1920.0/4.0, 1080.0/4.0);

void main() {
  vec3 FragPos = texture(gPosition, TexCoords).rgb;
  vec3 Normal = texture(gNormal, TexCoords).rgb;
  vec3 RandomVec = normalize(texture(gNoise, TexCoords * noiseScale).rgb);

  vec3 T = normalize(RandomVec - dot(RandomVec, Normal) * Normal);
  vec3 B = cross(Normal, T);
  mat3 TBN = mat3(T, B, Normal);

  float ssao_res = 0.0f;
  for (int i=0; i<kernelSize; i++) {
    vec3 sample_pos = FragPos + TBN * ssao_sample[i] * radius;

    vec4 offset = projection * vec4(sample_pos, 1.0f);
    offset /= offset.w;
    offset.xyz = offset.xyz * 0.5 + 0.5;

    float sample_depth = texture(gPosition, offset.xy).z;
    float rangeCheck = smoothstep(0.0, 1.0, radius / abs(sample_pos.z - sample_depth));
    if (sample_pos.z + bias < sample_depth) {
      ssao_res += 1.0f * rangeCheck;
    }
  }
  FragColor = 1.0f - ssao_res / kernelSize;
  // FragColor = 1.0f;
}

 

void main1()
{
    // get input for SSAO algorithm
    vec3 fragPos = texture(gPosition, TexCoords).xyz;
    vec3 normal = normalize(texture(gNormal, TexCoords).rgb);
    vec3 randomVec = normalize(texture(gNoise, TexCoords * noiseScale).xyz);
    // create TBN change-of-basis matrix: from tangent-space to view-space
    vec3 tangent = normalize(randomVec - normal * dot(randomVec, normal));
    vec3 bitangent = cross(normal, tangent);
    mat3 TBN = mat3(tangent, bitangent, normal);
    // iterate over the sample kernel and calculate occlusion factor
    float occlusion = 0.0;
    for(int i = 0; i < kernelSize; ++i)
    {
        // get sample position
        vec3 samplePos = TBN * ssao_sample[i]; // from tangent to view-space
        samplePos = fragPos + samplePos * radius; 
        
        // project sample position (to sample texture) (to get position on screen/texture)
        vec4 offset = vec4(samplePos, 1.0);
        offset = projection * offset; // from view to clip-space
        offset.xyz /= offset.w; // perspective divide
        offset.xyz = offset.xyz * 0.5 + 0.5; // transform to range 0.0 - 1.0
        
        // get sample depth
        float sampleDepth = texture(gPosition, offset.xy).z; // get depth value of kernel sample
        
        // range check & accumulate
        float rangeCheck = smoothstep(0.0, 1.0, radius / abs(fragPos.z - sampleDepth));
        occlusion += (sampleDepth >= samplePos.z + bias ? 1.0 : 0.0) * rangeCheck;           
    }
    occlusion = 1.0 - (occlusion / kernelSize);
    
    FragColor = occlusion;
}