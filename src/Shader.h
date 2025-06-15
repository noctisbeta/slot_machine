#pragma once

#include <string>

class Shader {
public:
  enum ShaderType { VERTEX_SHADER, FRAGMENT_SHADER };

public:
  Shader(const std::string &filePath, ShaderType type);
  ~Shader();

  bool load();

  unsigned int getID() const { return ID; }

private:
  std::string filePath;
  ShaderType type;
  unsigned int ID;

  std::string parseShaderSource(const std::string &filePath);
};