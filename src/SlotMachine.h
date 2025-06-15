#pragma once

#include <array>

class SlotMachine {
public:
  SlotMachine();
  ~SlotMachine();

  void spin();
  std::array<float, 9> getReelColors() const { return reelColors; }

  static const std::array<float, 24> masterAvailableColors;

private:
  std::array<float, 9> reelColors;
  void changeReelColors();
};
