#include "glbase.h"

class Shader;

namespace LearnGL {
class GLGLSL : public GLBase {
public:
  GLGLSL();
  void Init() override;

protected:
  void Draw() override;

private:
  void UpdateCameraPos();

private:
  unsigned int _VAO;
  unsigned int _VBO;

  unsigned int _floor_vao;
  unsigned int _floor_vbo;

  unsigned int _cube_vao;
  unsigned int _cube_vbo;

  unsigned int _cube_texture;
  unsigned int _floor_texture;

  unsigned int _camera_pos_buffer;

  Shader *_shader;

  Shader *_rgb_shader_r;
  Shader *_rgb_shader_g;
  Shader *_rgb_shader_b;
  Shader *_rgb_shader_y;
};
}; // namespace LearnGL
