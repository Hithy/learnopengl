

class Shader {
public:
  Shader(const char* vert_path, const char* frag_path);

  void use();
  void SetUniform1f(const char *param, float value);
  void SetUniform1i(const char *param, int value);
  void SetUniformMat4fv(const char *param, float *ptr);
  void SetUniform3fv(const char *param, float x, float y, float z);

  unsigned int GetID() const { return id; }

private:
  unsigned int id;
};
