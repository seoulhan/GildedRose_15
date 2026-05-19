#include "GildedRose.h"

GildedRose::GildedRose(std::vector<Item>& items) : items(items) {}

namespace {
const char* kAgedBrie = "Aged Brie";
const char* kBackstagePass = "Backstage passes to a TAFKAL80ETC concert";
const char* kSulfuras = "Sulfuras, Hand of Ragnaros";

bool isConjured(const Item& item) {
    return item.name.find("Conjured") != std::string::npos;
}

void increaseQuality(Item& item, int amount = 1) {
    item.quality += amount;
    if (item.quality > 50) {
        item.quality = 50;
    }
}

void decreaseQuality(Item& item, int amount = 1) {
    item.quality -= amount;
    if (item.quality < 0) {
        item.quality = 0;
    }
}
}

void GildedRose::updateQuality() {
    for (Item& item : items) {
        if (item.name == kSulfuras) {
            continue;
        }

        if (item.name == kAgedBrie) {
            increaseQuality(item, item.sellIn <= 0 ? 2 : 1);
        } else if (item.name == kBackstagePass) {
            if (item.sellIn <= 0) {
                item.quality = 0;
            } else if (item.sellIn <= 5) {
                increaseQuality(item, 3);
            } else if (item.sellIn <= 10) {
                increaseQuality(item, 2);
            } else {
                increaseQuality(item);
            }
        } else if (isConjured(item)) {
            decreaseQuality(item, item.sellIn <= 0 ? 4 : 2);
        } else {
            decreaseQuality(item, item.sellIn <= 0 ? 2 : 1);
        }

        item.sellIn -= 1;
    }
}
