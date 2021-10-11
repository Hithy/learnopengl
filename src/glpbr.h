#include "glbase.h"

#include "glm/glm.hpp"

class Shader;

namespace LearnGL {
class GLPbr : public GLBase {
public:
  GLPbr();
  void Init() override;

protected:
  void Draw() override;

private:
  Shader *_pbr_shader;

  glm::vec3 _ball_color;
  glm::vec3 _light_color;
};
}; // namespace LearnGL
