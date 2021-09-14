#include "glbase.h"

class Shader;

namespace LearnGL {
class GLGeoShader : public GLBase {
public:
  GLGeoShader();
  void Init() override;

protected:
  void Draw() override;

private:
  void UpdateCameraPos();
  unsigned int GenGeoVAO();

private:
  unsigned int _geo_vao;
  
  unsigned int _cube_vao;
  unsigned int _cube_vbo;

  unsigned int _cube_texture;

  unsigned int _camera_pos_buffer;

  Shader *_rgb_shader_r;
  Shader *_rgb_shader_g;
  Shader *_rgb_shader_b;
  Shader *_rgb_shader_y;

  Shader *_geo_shader;
};
}; // namespace LearnGL
