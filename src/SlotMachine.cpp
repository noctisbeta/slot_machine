#include "SlotMachine.h"
#include <array>

const std::array<float, 24> SlotMachine::masterAvailableColors = {
    1.0f, 0.4f, 0.4f, // Red
    0.4f, 1.0f, 0.4f, // Green
    0.4f, 0.4f, 1.0f, // Blue
    1.0f, 1.0f, 0.4f, // Yellow
    1.0f, 0.4f, 1.0f, // Magenta
    0.4f, 1.0f, 1.0f, // Cyan
    0.8f, 0.5f, 0.2f, // Orange
    0.5f, 0.0f, 0.5f  // Purple
};

SlotMachine::SlotMachine() { changeReelColors(); }

SlotMachine::~SlotMachine() {}

void SlotMachine::spin() { changeReelColors(); }

void SlotMachine::changeReelColors() {
  for (int i = 0; i < 9; ++i) {
    int colorIndex = rand() % (24);
    reelColors[i] = masterAvailableColors[colorIndex];
  }
}