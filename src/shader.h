

class Shader {
public:
  Shader(const char* vert_path, const char* frag_path);

  void use();
  void SetUniform1f(const char* param, float value);

private:
  unsigned int id;
};
