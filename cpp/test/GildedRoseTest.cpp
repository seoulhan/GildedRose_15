#include "GildedRose.h"
#include <gtest/gtest.h>

TEST(GildedRoseTest, NormalItem_Quality_LowerBound) {
  std::vector<Item> items = {Item("noname", 0, 0)};
  GildedRose app(items);
  app.updateQuality();

  EXPECT_EQ(-1, app.items[0].sellIn);
  EXPECT_EQ(0, app.items[0].quality);
}

TEST(GildedRoseTest, NormalItem_Expired) {
  std::vector<Item> items = {Item("noname", 0, 5)};
  GildedRose app(items);
  app.updateQuality();

  EXPECT_EQ(-1, app.items[0].sellIn);
  EXPECT_EQ(3, app.items[0].quality);
}

TEST(GildedRoseTest, LegendaryItem_InvariantInSellin) {
  std::vector<Item> items = {Item("Sulfuras, Hand of Ragnaros", 0, 5)};
  GildedRose app(items);
  app.updateQuality();

  EXPECT_EQ(0, app.items[0].sellIn);
  EXPECT_EQ(5, app.items[0].quality);
}

TEST(GildedRoseTest, LegendaryItem_InvariantAfterSellin) {
  std::vector<Item> items = {Item("Sulfuras, Hand of Ragnaros", -1, 5)};
  GildedRose app(items);
  app.updateQuality();

  EXPECT_EQ(-1, app.items[0].sellIn);
  EXPECT_EQ(5, app.items[0].quality);
}

TEST(GildedRoseTest, AgedBrie_AgedAfterSellin) {
  std::vector<Item> items = {Item("Aged Brie", 0, 0)};
  GildedRose app(items);
  app.updateQuality();

  EXPECT_EQ(-1, app.items[0].sellIn);
  EXPECT_EQ(2, app.items[0].quality);
}

TEST(GildedRoseTest, AgedBrie_Quality_UpperBound) {
  std::vector<Item> items = {Item("Aged Brie", 0, 50)};
  GildedRose app(items);
  app.updateQuality();

  EXPECT_EQ(-1, app.items[0].sellIn);
  EXPECT_EQ(50, app.items[0].quality);
}

TEST(GildedRoseTest, TAFKAL80ETC_Value_GT_D10) {
  std::vector<Item> items = {
      Item("Backstage passes to a TAFKAL80ETC concert", 15, 0)};
  GildedRose app(items);
  app.updateQuality();

  EXPECT_EQ(14, app.items[0].sellIn);
  EXPECT_EQ(1, app.items[0].quality);
}

TEST(GildedRoseTest, TAFKAL80ETC_Value_LT_D0) {
  std::vector<Item> items = {
      Item("Backstage passes to a TAFKAL80ETC concert", 0, 0)};
  GildedRose app(items);
  app.updateQuality();

  EXPECT_EQ(-1, app.items[0].sellIn);
  EXPECT_EQ(0, app.items[0].quality);
}

TEST(GildedRoseTest, TAFKAL80ETC_Value_LT_D0_Overflow) {
  std::vector<Item> items = {
      Item("Backstage passes to a TAFKAL80ETC concert", 0, 51)};
  GildedRose app(items);
  app.updateQuality();

  EXPECT_EQ(-1, app.items[0].sellIn);
  EXPECT_EQ(0, app.items[0].quality);
}

TEST(GildedRoseTest, Item_Empty) {
  std::vector<Item> items = {};
  GildedRose app(items);
  app.updateQuality();

  EXPECT_EQ(0, app.items.size());
}