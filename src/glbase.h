#include "camera.h"

struct GLFWwindow;

namespace LearnGL {

unsigned int GenTexture(const char* path);

class GLBase {
public:
  GLBase(const char *title, int width, int height);

  virtual void Init();
  void Run();

  void ToggleMouse();
  bool IsCacheMouse();

  Camera &GetCamera() { return _camera; }

  virtual ~GLBase();

protected:
  virtual void Draw();

protected:
  GLFWwindow *_window;
  Camera _camera;

  int _window_width;
  int _window_height;

private:
  bool _gui_catch_mouse;
};

}; // namespace LearnGL
