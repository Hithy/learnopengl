#include "camera.h"

class GLFWwindow;

namespace LearnGL {

unsigned int GenTexture(const char* path);

class GLBase {
public:
  GLBase(const char *title, int width, int height);

  virtual void Init();
  void Run();

  Camera &GetCamera() { return _camera; }

  virtual ~GLBase();

protected:
  virtual void Draw();

protected:
  GLFWwindow *_window;
  Camera _camera;

  int _window_width;
  int _window_height;
};

}; // namespace LearnGL
