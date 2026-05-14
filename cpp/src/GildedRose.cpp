#include "GildedRose.h"

const std::string AGED_BRIE = "Aged Brie";
const std::string BACKSTAGE_PASSES =
    "Backstage passes to a TAFKAL80ETC concert";
const std::string SULFURAS = "Sulfuras, Hand of Ragnaros";

class ItemUpdater {
public:
  virtual ~ItemUpdater() = default;
  virtual void update(Item &item) = 0;
};

class AgedBrieUpdater : public ItemUpdater {
public:
  void update(Item &item) {
    item.quality++;
    item.sellIn--;
    if (item.sellIn < 0)
      item.quality++;
    item.quality = std::min(50, item.quality);
  }
};

class BackstagePassUpdater : public ItemUpdater {
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

class SulfurasUpdater : public ItemUpdater {
public:
  void update(Item &item) { return; }
};

class NormalItemUpdater : public ItemUpdater {
public:
  void update(Item &item) {
    item.quality--;
    item.sellIn--;
    if (item.sellIn < 0)
      item.quality--;
    item.quality = std::max(0, item.quality);
  }
};

GildedRose::GildedRose(std::vector<Item> &items) : items(items) {}

void GildedRose::updateQuality() {
  for (Item &item : items) {
    if (item.name == SULFURAS) {
      continue;
    } else if (item.name == AGED_BRIE) {
      AgedBrieUpdater().update(item);
    } else if (item.name == BACKSTAGE_PASSES) {
      BackstagePassUpdater().update(item);
    } else {
      NormalItemUpdater().update(item);
    }
  }
}

void GildedRose::updateNormalItem(Item &item) {
  item.quality--;
  item.sellIn--;
  if (item.sellIn < 0)
    item.quality--;
  item.quality = std::max(0, item.quality);
}

void GildedRose::updateBackstagePasses(Item &item) {
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

void GildedRose::updateAgedBrie(Item &item) {
  item.quality++;
  item.sellIn--;
  if (item.sellIn < 0)
    item.quality++;
  item.quality = std::min(50, item.quality);
}
