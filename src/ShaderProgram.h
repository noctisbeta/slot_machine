#pragma once

#include "Shader.h"
#include <vector>

class ShaderProgram {
public:
  ShaderProgram();
  ~ShaderProgram();

  bool create();
  bool associateShaders(std::vector<Shader> shaders);
  bool link();

  unsigned int getID() const { return ID; }

private:
  unsigned int ID;
  std::vector<Shader> shaders;

  bool attachShader(const Shader &shader);
  bool deleteShaders();
};