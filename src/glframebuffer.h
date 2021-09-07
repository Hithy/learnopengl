#include "glbase.h"

class Shader;

namespace LearnGL {
class GLFrameBuffer : public GLBase {
public:
  GLFrameBuffer();
  void Init() override;

protected:
  void Draw() override;

private:
  void GenCubeVAO();
  void GenFloorVAO();

private:
  unsigned int _VAO;
  unsigned int _VBO;

  unsigned int _floor_vao;
  unsigned int _floor_vbo;

  unsigned int _cube_vao;
  unsigned int _cube_vbo;

  unsigned int _cube_texture;
  unsigned int _floor_texture;

  unsigned int fbo;

  Shader *_shader;
  Shader *_simple_shader;
};
}; // namespace LearnGL
