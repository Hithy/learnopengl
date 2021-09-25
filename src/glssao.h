#include "glbase.h"

class Shader;

namespace LearnGL {
class GLSSAO : public GLBase {
public:
  GLSSAO();
  void Init() override;

protected:
  void Draw() override;

private:
  void DrawGBuffer();
  void DrawSSAO();
  void DrawLightColor();
  
  void GenGbuffer();
  void GenSSAObuffer();

  void GenNoiseMap();

private:
  unsigned int _VAO;
  unsigned int _VBO;

  unsigned int _floor_vao;
  unsigned int _floor_vbo;

  unsigned int _cube_texture;
  unsigned int _cube_specular;
  unsigned int _floor_texture;

  unsigned int _gbuffer_fbo;
  unsigned int _gbuffer_position;
  unsigned int _gbuffer_normal;
  unsigned int _gbuffer_ablebo;

  unsigned int _noise_map;

  unsigned int _ssao_fbo;
  unsigned int _ssao_texture;

  Shader *_light_shader;
  Shader *_simple_shader;

  Shader *_ssao_gbuffer_shader;
  Shader *_ssao_calc_shader;
  Shader *_ssao_light_shader;
};
}; // namespace LearnGL
