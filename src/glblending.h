#include "glbase.h"

class Shader;

namespace LearnGL {
class GLBlending : public GLBase {
public:
  GLBlending();
  void Init() override;

protected:
  void Draw() override;

private:
  unsigned int _VAO;
  unsigned int _VBO;

  unsigned int _floor_vao;
  unsigned int _floor_vbo;

  unsigned int _cube_vao;
  unsigned int _cube_vbo;

  unsigned int _cube_texture;
  unsigned int _floor_texture;
  unsigned int _grass_texture;
  unsigned int _window_texture;

  Shader *_shader;
  Shader *_grass_shader;
};
}; // namespace LearnGL
