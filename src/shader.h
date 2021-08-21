

class Shader {
public:
  Shader(const char* vert_path, const char* frag_path);

  void use();
  void SetUniform1f(const char *param, float value);
  void SetUniform1i(const char *param, int value);
  void SetUniformMat4fv(const char *param, float *ptr);

private:
  unsigned int id;
};
