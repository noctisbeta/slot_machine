#pragma once

#include "Color.h"
#include <array>

class SlotMachine {
public:
  SlotMachine();
  ~SlotMachine();

  void spin();
  std::array<Color, 3> getReelColors() const { return reelColors; }

  static const std::array<Color, 8> masterAvailableColors;

private:
  std::array<Color, 3> reelColors;
  void changeReelColors();
};
