#include "GildedRose.h"

const std::string AGED_BRIE = "Aged Brie";
const std::string BACKSTAGE_PASSES =
    "Backstage passes to a TAFKAL80ETC concert";
const std::string SULFURAS = "Sulfuras, Hand of Ragnaros";

GildedRose::GildedRose(std::vector<Item> &items) : items(items) {}

void GildedRose::updateQuality() {
  for (Item &item : items) {
    if (item.name == SULFURAS) {
    } else if (item.name == AGED_BRIE) {
      item.quality++;
      item.sellIn--;
      if (item.sellIn < 0)
        item.quality++;
      item.quality = std::min(50, item.quality);

    } else if (item.name == BACKSTAGE_PASSES) {
      item.quality++;
      if (item.sellIn < 11)
        item.quality++;
      if (item.sellIn < 6)
        item.quality++;
      item.sellIn--;
      if (item.sellIn < 0)
        item.quality = 0;
      item.quality = std::min(50, item.quality);

    } else {
      item.quality--;
      item.sellIn--;
      if (item.sellIn < 0)
        item.quality--;
      item.quality = std::max(0, item.quality);
    }
  }
}
