#include "Shader.h"
#include "ShaderProgram.h"
#include "SlotMachine.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <ctime>
#include <functional>
#include <iostream>
#include <vector>

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

void renderTriangle(const float leftCorner, const float sideLength, float r,
                    float g, float b) {
  Shader vertexShader("assets/vertex_shader.glsl", Shader::VERTEX_SHADER);
  Shader fragmentShader("assets/fragment_shader.glsl", Shader::FRAGMENT_SHADER);

  ShaderProgram shaderProgram;

  if (!shaderProgram.create()) {
    std::cerr << "Failed to create shader program." << std::endl;
    return;
  }

  shaderProgram.associateShaders({vertexShader, fragmentShader});

  if (!shaderProgram.link()) {
    std::cerr << "Failed to link shader program." << std::endl;
    return;
  }

  // 5. Vertex Buffer Object (VBO) and Vertex Array Object (VAO)
  float vertices[] = {
      leftCorner,
      -0.5f,
      0.0f, //
      leftCorner + sideLength,
      -0.5f,
      0.0f, //
      leftCorner + sideLength / 2.0f,
      0.5f,
      0.0f //
  };

  unsigned int VBO, VAO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO); // Bind VAO

  glBindBuffer(GL_ARRAY_BUFFER, VBO); // Bind VBO
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,
               GL_STATIC_DRAW); // Copy vertex data to VBO

  // 6. Configure Vertex Attributes
  // Tell OpenGL how to interpret the vertex data (for layout location 0)
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0); // Enable vertex attribute at location 0

  // Unbind VBO (VAO still has it registered)
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  // Unbind VAO (optional here, but good practice if other objects were to be
  // bound) glBindVertexArray(0);

  // 7. Draw the triangle
  glUseProgram(shaderProgram.getID()); // Use our shader program

  // Set the color uniform
  int triangleColorLocation =
      glGetUniformLocation(shaderProgram.getID(), "triangleColor");
  if (triangleColorLocation == -1) {
    std::cerr << "Warning: Could not find uniform 'triangleColor'" << std::endl;
  } else {
    glUniform3f(triangleColorLocation, r, g, b);
  }

  glBindVertexArray(
      VAO); // Bind the VAO (which has our VBO and attribute configuration)
  glDrawArrays(GL_TRIANGLES, 0, 3); // Draw the triangle (3 vertices)

  // 8. Cleanup (IMPORTANT: because we create these every frame in this
  // function) This is inefficient. Ideally, create these once and delete them
  // at program exit.
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteProgram(shaderProgram.getID());
}

int main(void) {
  srand(static_cast<unsigned int>(time(0)));

  GLFWwindow *window;

  if (!glfwInit())
    return -1;

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = glfwCreateWindow(800, 600, "Slot Machine Colors", NULL, NULL);
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

  glViewport(0, 0, 800, 600);
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
      float r = color.r;
      float g = color.g;
      float b = color.b;
      renderTriangle(-0.9f + i * 0.6f, 0.5f, r, g, b);
    }

    glfwPollEvents();
    glfwSwapBuffers(window);
  }

  glfwTerminate();
  return 0;
}
