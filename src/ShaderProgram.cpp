#include "ShaderProgram.h"
#include <GL/glew.h>
#include <iostream>

ShaderProgram::ShaderProgram() : ID(0) {}

ShaderProgram::~ShaderProgram() {
  if (ID != 0) {
    glDeleteProgram(ID);
  }
}

bool ShaderProgram::create() {
  ID = glCreateProgram();
  if (ID == 0) {
    std::cerr << "ERROR::SHADER_PROGRAM::CREATION_FAILED" << std::endl;
    return false;
  }
  return true;
}

bool ShaderProgram::attachShader(const Shader &shader) {
  if (ID == 0) {
    std::cerr << "ERROR::SHADER_PROGRAM::NOT_CREATED" << std::endl;
    return false;
  }

  if (shader.getID() == 0) {
    std::cerr << "ERROR::SHADER_PROGRAM::SHADER_NOT_LOADED" << std::endl;
    return false;
  }

  glAttachShader(ID, shader.getID());
  return true;
}

bool ShaderProgram::link() {
  if (ID == 0) {
    std::cerr << "ERROR::SHADER_PROGRAM::NOT_CREATED" << std::endl;
    return false;
  }

  glLinkProgram(ID);

  GLint success;
  GLchar infoLog[512];
  glGetProgramiv(ID, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(ID, 512, NULL, infoLog);
    std::cerr << "ERROR::SHADER_PROGRAM::LINKING_FAILED\n"
              << infoLog << std::endl;
    return false;
  }

  deleteShaders();
  return true;
}

bool ShaderProgram::deleteShaders() {
  for (const Shader &shader : shaders) {
    if (shader.getID() != 0) {
      glDeleteShader(shader.getID());
    }
  }
  shaders.clear();
  return true;
}

bool ShaderProgram::associateShaders(std::vector<Shader> shaders) {
  this->shaders = std::move(shaders);
  bool result;
  bool loadResult;
  for (Shader &shader : this->shaders) {
    loadResult = shader.load();
    if (!loadResult) {
      std::cerr << "Failed to load shader from file: " << shader.getID()
                << std::endl;
      return false;
    }

    result = attachShader(shader);
    if (!result) {
      std::cerr << "Failed to attach shader." << std::endl;
      return false;
    }
  }
  return true;
}
