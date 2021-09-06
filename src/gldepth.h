#include "glbase.h"

class Shader;

namespace LearnGL {
class GLDepth : public GLBase {
public:
  GLDepth();
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

  Shader *_shader;
};
}; // namespace LearnGL
