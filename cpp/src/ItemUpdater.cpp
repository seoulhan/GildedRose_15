#include "GildedRose.h"

class AgedBrieUpdater {
public:
  void update(Item &item) {
    item.quality++;
    item.sellIn--;
    if (item.sellIn < 0)
      item.quality++;
    item.quality = std::min(50, item.quality);
  }
};

class BackstagePassUpdater {
public:
  void update(Item &item) {
    item.quality++;
    if (item.sellIn < 11)
      item.quality++;
    if (item.sellIn < 6)
      item.quality++;
    item.sellIn--;
    if (item.sellIn < 0)
      item.quality = 0;
    item.quality = std::min(50, item.quality);
  }
};

class SulfurasUpdater {
public:
  void update(Item &item) { return; }
};

class NormalItemUpdater {
public:
  void update(Item &item) {
    item.quality--;
    item.sellIn--;
    if (item.sellIn < 0)
      item.quality--;
    item.quality = std::max(0, item.quality);
  }
};