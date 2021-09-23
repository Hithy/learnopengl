#include "glbase.h"

class Shader;

namespace LearnGL {
class GLDeferred : public GLBase {
public:
  GLDeferred();
  void Init() override;

protected:
  void Draw() override;

private:
  void GenGbuffer();

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

  Shader *_g_buffer_shader;
  Shader *_light_shader;
  Shader *_simple_shader;
};
}; // namespace LearnGL
