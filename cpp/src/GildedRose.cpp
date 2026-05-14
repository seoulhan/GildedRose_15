#include "GildedRose.h"

const std::string AGED_BRIE = "Aged Brie";
const std::string BACKSTAGE_PASSES =
    "Backstage passes to a TAFKAL80ETC concert";
const std::string SULFURAS = "Sulfuras, Hand of Ragnaros";

GildedRose::GildedRose(std::vector<Item> &items) : items(items) {}

void GildedRose::updateQuality() {
  for (Item &item : items) {
    if (item.name == SULFURAS) {
      continue;
    } else if (item.name == AGED_BRIE) {
      updateAgedBrie(item);
    } else if (item.name == BACKSTAGE_PASSES) {
      updateBackstagePasses(item);
    } else {
      updateNormalItem(item);
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
