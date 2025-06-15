#include "SlotMachine.h"
#include <array>

const std::array<Color, 8> SlotMachine::masterAvailableColors = {
    Color{1.0f, 0.4f, 0.4f}, // Red
    Color{0.4f, 1.0f, 0.4f}, // Green
    Color{0.4f, 0.4f, 1.0f}, // Blue
    Color{1.0f, 1.0f, 0.4f}, // Yellow
    Color{1.0f, 0.4f, 1.0f}, // Magenta
    Color{0.4f, 1.0f, 1.0f}, // Cyan
    Color{0.8f, 0.5f, 0.2f}, // Orange
    Color{0.5f, 0.0f, 0.5f}  // Purple
};

SlotMachine::SlotMachine() { changeReelColors(); }

SlotMachine::~SlotMachine() {}

void SlotMachine::spin() { changeReelColors(); }

void SlotMachine::changeReelColors() {
  for (int i = 0; i < 3; ++i) {
    int colorIndex = rand() % (8);
    reelColors[i] = masterAvailableColors[colorIndex];
  }
}