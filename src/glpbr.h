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
};
}; // namespace LearnGL
