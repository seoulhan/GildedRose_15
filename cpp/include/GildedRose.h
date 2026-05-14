#pragma once
#include "Item.h"
#include <vector>

class GildedRose {
public:
  std::vector<Item> &items;

  GildedRose(std::vector<Item> &items);
  void updateQuality();

private:
  void updateAgedBrie(Item &item);
  void updateBackstagePasses(Item &item);
  void updateNormalItem(Item &item);
};
