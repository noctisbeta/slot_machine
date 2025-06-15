#include "Shader.h"
#include "ShaderProgram.h"
#include "SlotMachine.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <functional>
#include <iostream>
#include <vector>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window,
                  std::function<void()> onSpacePressCallback) {
  static bool space_key_processed_this_press = false;

  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
    if (onSpacePressCallback && !space_key_processed_this_press) {
      onSpacePressCallback();
      space_key_processed_this_press = true;
    }
  } else {
    space_key_processed_this_press = false;
  }
}

void renderPrimitive(const std::vector<float> &vertices, GLenum drawMode,
                     Color color, const char *vertexShaderPath,
                     const char *fragmentShaderPath) {
  if (vertices.empty() || (vertices.size() % 3 != 0)) {
    std::cerr
        << "Error: Vertex data for primitive is empty or not a multiple of 3."
        << std::endl;
    return;
  }

  Shader vertexShader(vertexShaderPath, Shader::VERTEX_SHADER);
  Shader fragmentShader(fragmentShaderPath, Shader::FRAGMENT_SHADER);
  ShaderProgram shaderProgram;

  if (!shaderProgram.create()) {
    std::cerr << "Failed to create shader program for primitive." << std::endl;
    return;
  }

  shaderProgram.associateShaders({vertexShader, fragmentShader});

  if (!shaderProgram.link()) {
    std::cerr << "Failed to link shader program for primitive." << std::endl;
    return;
  }

  unsigned int VBO, VAO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float),
               vertices.data(), GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glUseProgram(shaderProgram.getID());

  int colorLocation =
      glGetUniformLocation(shaderProgram.getID(),
                           "triangleColor"); // Assuming "triangleColor" uniform
  if (colorLocation == -1) {
    std::cerr << "Warning: Could not find uniform 'triangleColor' for primitive"
              << std::endl;
  } else {
    glUniform3f(colorLocation, color.r, color.g, color.b);
  }

  glBindVertexArray(VAO);
  glDrawArrays(drawMode, 0, static_cast<GLsizei>(vertices.size() / 3));

  // Cleanup
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteProgram(shaderProgram.getID());
}

void renderTriangle(const float leftCorner, const float sideLength,
                    Color color) {
  std::vector<float> vertices = {leftCorner,
                                 -0.8f,
                                 0.0f,
                                 leftCorner + sideLength,
                                 -0.8f,
                                 0.0f,
                                 leftCorner + sideLength / 2.0f,
                                 0.0f,
                                 0.0f};

  renderPrimitive(vertices, GL_TRIANGLES, color, "assets/vertex_shader.glsl",
                  "assets/fragment_shader.glsl");
}

void renderCircle(float centerX, float centerY, float radius, int numSegments,
                  Color color) {
  std::vector<float> vertices;

  // Center vertex
  vertices.push_back(centerX);
  vertices.push_back(centerY);
  vertices.push_back(0.0f);

  // Circumference vertices
  for (int i = 0; i <= numSegments; ++i) {
    float angle = 2.0f * static_cast<float>(M_PI) * static_cast<float>(i) /
                  static_cast<float>(numSegments);
    vertices.push_back(centerX + radius * std::cos(angle));
    vertices.push_back(centerY + radius * std::sin(angle));
    vertices.push_back(0.0f);
  }

  renderPrimitive(vertices, GL_TRIANGLE_FAN, color, "assets/vertex_shader.glsl",
                  "assets/fragment_shader.glsl");
}

int main(void) {
  srand(static_cast<unsigned int>(time(0)));

  GLFWwindow *window;

  if (!glfwInit())
    return -1;

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = glfwCreateWindow(800, 800, "Slot Machine Colors", NULL, NULL);
  if (!window) {
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  GLenum result = glewInit();
  if (GLEW_OK != result) {
    std::cerr << "Error: " << glewGetErrorString(result) << std::endl;
    glfwTerminate();
    return -1;
  }

  glViewport(0, 0, 800, 800);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  bool spaceActionRequested = false;

  SlotMachine slotMachine;

  auto spaceKeyPressCallback = [&]() { spaceActionRequested = true; };

  while (!glfwWindowShouldClose(window)) {
    processInput(window, spaceKeyPressCallback);

    if (spaceActionRequested) {
      slotMachine.spin();
      spaceActionRequested = false;
    }

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    const auto &reelColors = slotMachine.getReelColors();
    Color color;
    for (int i = 0; i < 3; ++i) {
      color = reelColors[i];
      renderTriangle(-0.9f + i * 0.6f, 0.5f, color);
    }

    for (int i = 0; i < 10; ++i) {
      float x = -0.8f + i * 0.16f;
      float y = 0.6f;
      renderCircle(x, y, 0.05f, 36, Color{1.0f, 1.0f, 0.0f});
    }

    glfwPollEvents();
    glfwSwapBuffers(window);
  }

  glfwTerminate();
  return 0;
}
