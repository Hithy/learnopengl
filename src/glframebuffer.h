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
  void GenScreenVAO();

  void GenFrameBuffer();

  void DrawNormal();
  void DrawFBO();

private:
  unsigned int _floor_vao;
  unsigned int _floor_vbo;
  unsigned int _floor_texture;

  unsigned int _cube_vao;
  unsigned int _cube_vbo;
  unsigned int _cube_texture;

  unsigned int _screen_vao;
  unsigned int _screen_vbo;

  unsigned int _fbo;
  unsigned int _fbo_texture;
  unsigned int _fbo_stencil_depth;

  Shader *_shader;
  Shader *_quad_shader;
};
}; // namespace LearnGL
