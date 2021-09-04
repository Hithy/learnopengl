#include "glbase.h"

class Shader;

namespace LearnGL {
class GLDefault : public GLBase {
public:
  GLDefault();
  void Init() override;

protected:
  void Draw() override;

private:
  unsigned int _VAO;
  unsigned int _VBO;
  Shader *_shader;
};
}; // namespace LearnGL
