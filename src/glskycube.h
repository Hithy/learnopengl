#include "glbase.h"

class Shader;

namespace LearnGL {
class GLSkyCube : public GLBase {
public:
  GLSkyCube();
  void Init() override;

protected:
  void Draw() override;

private:
  void GenSkyBoxVAO();
  unsigned int GenSkyBoxTextures();

  void DrawSkybox();

private:
  unsigned int _skybox_vao;
  unsigned int _skybox_vbo;
  unsigned int _skybox_texture;

  unsigned int _floor_vao;
  unsigned int _floor_vbo;
  unsigned int _floor_texture;

  unsigned int _cube_vao;
  unsigned int _cube_vbo;
  unsigned int _cube_texture;
  

  Shader *_shader;
  Shader *_skybox_shader;
};
}; // namespace LearnGL
