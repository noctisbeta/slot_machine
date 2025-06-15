#include "Shader.h"
#include <GL/glew.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

Shader::Shader(const std::string &filePath, ShaderType type)
    : filePath(filePath), type(type), ID(0) {}

Shader::~Shader() {
  if (ID != 0) {
    glDeleteShader(ID);
  }
}

bool Shader::load() {
  if (ID != 0) {
    glDeleteShader(ID);
    ID = 0;
  }

  int glShaderType =
      (type == VERTEX_SHADER) ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER;

  ID = glCreateShader(glShaderType);
  if (ID == 0) {
    std::cerr << "ERROR::SHADER::CREATION_FAILED: " << filePath << std::endl;
    return false;
  }

  std::string shaderSource = parseShaderSource(filePath);

  if (shaderSource.empty()) {
    std::cerr << "ERROR::SHADER::SOURCE_EMPTY: " << filePath << std::endl;
    glDeleteShader(ID);
    ID = 0;
    return false;
  }

  const char *sourceCStr = shaderSource.c_str();

  glShaderSource(ID, 1, &sourceCStr, NULL);
  glCompileShader(ID);

  GLint success;
  GLchar infoLog[1024];
  glGetShaderiv(ID, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(ID, 1024, NULL, infoLog);
    std::cerr << "ERROR::SHADER::COMPILATION_FAILED: " << filePath << "\n"
              << infoLog << std::endl;
    glDeleteShader(ID);
    ID = 0;
    return false;
  }

  return true;
}

std::string Shader::parseShaderSource(const std::string &filePath) {
  std::ifstream shaderFile(filePath);

  if (!shaderFile.is_open()) {
    std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_OPENED: " << filePath
              << std::endl;
    return "";
  }

  std::stringstream shaderStream;

  shaderStream << shaderFile.rdbuf();

  if (shaderFile.fail() && !shaderFile.eof()) {
    std::cerr << "ERROR::SHADER::FILE_READ_ERROR: " << filePath << std::endl;
    shaderFile.close();
    return "";
  }

  shaderFile.close();

  return shaderStream.str();
}