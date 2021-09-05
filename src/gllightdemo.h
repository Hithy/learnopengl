#include "glbase.h"

class Shader;

namespace LearnGL {
class GLLightDemo : public GLBase {
public:
  GLLightDemo();
  void Init() override;

protected:
  void Draw() override;

private:
  void InitVAO();
  void LoadTexture();

private:
  unsigned int _VAO;
  unsigned int _VBO;
  Shader *_shader;
  Shader *_light_shader;
};
}; // namespace LearnGL
