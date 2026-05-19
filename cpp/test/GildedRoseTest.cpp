#include <gtest/gtest.h>
#include "GildedRose.h"

class GildedRoseTest : public ::testing::Test {
protected:
    static Item updateSingleItem(const std::string& name, int sellIn, int quality) {
        std::vector<Item> items = { Item(name, sellIn, quality) };
        GildedRose app(items);

        app.updateQuality();

        return app.items[0];
    }
};

TEST_F(GildedRoseTest, NormalItemDecreasesQualityAndSellInBeforeSellDate) {
    // Given
    const std::string name = "Elixir of the Mongoose";

    // When
    const Item updated = updateSingleItem(name, 10, 20);

    // Then
    ASSERT_EQ(9, updated.sellIn);
    EXPECT_EQ(19, updated.quality);
}

TEST_F(GildedRoseTest, NormalItemDegradesTwiceAsFastOnSellDate) {
    // Given
    const std::string name = "Elixir of the Mongoose";

    // When
    const Item updated = updateSingleItem(name, 0, 20);

    // Then
    ASSERT_EQ(-1, updated.sellIn);
    EXPECT_EQ(18, updated.quality);
}

TEST_F(GildedRoseTest, NormalItemDegradesTwiceAsFastAfterSellDate) {
    // Given
    const std::string name = "Elixir of the Mongoose";

    // When
    const Item updated = updateSingleItem(name, -1, 20);

    // Then
    ASSERT_EQ(-2, updated.sellIn);
    EXPECT_EQ(18, updated.quality);
}

TEST_F(GildedRoseTest, NormalItemQualityNeverDropsBelowZero) {
    // Given
    const std::string name = "Elixir of the Mongoose";

    // When
    const Item updated = updateSingleItem(name, 5, 0);

    // Then
    ASSERT_EQ(4, updated.sellIn);
    EXPECT_EQ(0, updated.quality);
}

TEST_F(GildedRoseTest, NormalItemQualityCanStartAtUpperBoundaryAndDecrease) {
    // Given
    const std::string name = "Elixir of the Mongoose";

    // When
    const Item updated = updateSingleItem(name, 5, 50);

    // Then
    ASSERT_EQ(4, updated.sellIn);
    EXPECT_EQ(49, updated.quality);
}

TEST_F(GildedRoseTest, AgedBrieIncreasesQualityAndDecreasesSellInBeforeSellDate) {
    // Given
    const std::string name = "Aged Brie";

    // When
    const Item updated = updateSingleItem(name, 10, 20);

    // Then
    ASSERT_EQ(9, updated.sellIn);
    EXPECT_EQ(21, updated.quality);
}

TEST_F(GildedRoseTest, AgedBrieIncreasesTwiceAsFastOnSellDate) {
    // Given
    const std::string name = "Aged Brie";

    // When
    const Item updated = updateSingleItem(name, 0, 20);

    // Then
    ASSERT_EQ(-1, updated.sellIn);
    EXPECT_EQ(22, updated.quality);
}

TEST_F(GildedRoseTest, AgedBrieIncreasesTwiceAsFastAfterSellDate) {
    // Given
    const std::string name = "Aged Brie";

    // When
    const Item updated = updateSingleItem(name, -1, 20);

    // Then
    ASSERT_EQ(-2, updated.sellIn);
    EXPECT_EQ(22, updated.quality);
}

TEST_F(GildedRoseTest, AgedBrieQualityNeverExceedsFifty) {
    // Given
    const std::string name = "Aged Brie";

    // When
    const Item updated = updateSingleItem(name, 5, 50);

    // Then
    ASSERT_EQ(4, updated.sellIn);
    EXPECT_EQ(50, updated.quality);
}

TEST_F(GildedRoseTest, AgedBrieCanIncreaseFromZeroQuality) {
    // Given
    const std::string name = "Aged Brie";

    // When
    const Item updated = updateSingleItem(name, 5, 0);

    // Then
    ASSERT_EQ(4, updated.sellIn);
    EXPECT_EQ(1, updated.quality);
}

TEST_F(GildedRoseTest, BackstagePassIncreasesQualityByOneWhenConcertIsFarAway) {
    // Given
    const std::string name = "Backstage passes to a TAFKAL80ETC concert";

    // When
    const Item updated = updateSingleItem(name, 11, 20);

    // Then
    ASSERT_EQ(10, updated.sellIn);
    EXPECT_EQ(21, updated.quality);
}

TEST_F(GildedRoseTest, BackstagePassIncreasesQualityByTwoTenDaysBeforeConcert) {
    // Given
    const std::string name = "Backstage passes to a TAFKAL80ETC concert";

    // When
    const Item updated = updateSingleItem(name, 10, 20);

    // Then
    ASSERT_EQ(9, updated.sellIn);
    EXPECT_EQ(22, updated.quality);
}

TEST_F(GildedRoseTest, BackstagePassIncreasesQualityByThreeFiveDaysBeforeConcert) {
    // Given
    const std::string name = "Backstage passes to a TAFKAL80ETC concert";

    // When
    const Item updated = updateSingleItem(name, 5, 20);

    // Then
    ASSERT_EQ(4, updated.sellIn);
    EXPECT_EQ(23, updated.quality);
}

TEST_F(GildedRoseTest, BackstagePassQualityDropsToZeroOnSellDate) {
    // Given
    const std::string name = "Backstage passes to a TAFKAL80ETC concert";

    // When
    const Item updated = updateSingleItem(name, 0, 20);

    // Then
    ASSERT_EQ(-1, updated.sellIn);
    EXPECT_EQ(0, updated.quality);
}

TEST_F(GildedRoseTest, BackstagePassQualityDropsToZeroAfterSellDate) {
    // Given
    const std::string name = "Backstage passes to a TAFKAL80ETC concert";

    // When
    const Item updated = updateSingleItem(name, -1, 20);

    // Then
    ASSERT_EQ(-2, updated.sellIn);
    EXPECT_EQ(0, updated.quality);
}

TEST_F(GildedRoseTest, BackstagePassQualityNeverExceedsFifty) {
    // Given
    const std::string name = "Backstage passes to a TAFKAL80ETC concert";

    // When
    const Item updated = updateSingleItem(name, 5, 50);

    // Then
    ASSERT_EQ(4, updated.sellIn);
    EXPECT_EQ(50, updated.quality);
}

TEST_F(GildedRoseTest, SulfurasNeverChangesSellInOrQuality) {
    // Given
    const std::string name = "Sulfuras, Hand of Ragnaros";

    // When
    const Item updated = updateSingleItem(name, 10, 80);

    // Then
    ASSERT_EQ(10, updated.sellIn);
    EXPECT_EQ(80, updated.quality);
}

TEST_F(GildedRoseTest, SulfurasDoesNotChangeOnSellDate) {
    // Given
    const std::string name = "Sulfuras, Hand of Ragnaros";

    // When
    const Item updated = updateSingleItem(name, 0, 80);

    // Then
    ASSERT_EQ(0, updated.sellIn);
    EXPECT_EQ(80, updated.quality);
}

TEST_F(GildedRoseTest, SulfurasDoesNotChangeAfterSellDate) {
    // Given
    const std::string name = "Sulfuras, Hand of Ragnaros";

    // When
    const Item updated = updateSingleItem(name, -1, 80);

    // Then
    ASSERT_EQ(-1, updated.sellIn);
    EXPECT_EQ(80, updated.quality);
}

TEST_F(GildedRoseTest, SulfurasKeepsQualityAboveNormalUpperBoundary) {
    // Given
    const std::string name = "Sulfuras, Hand of Ragnaros";

    // When
    const Item updated = updateSingleItem(name, 5, 80);

    // Then
    ASSERT_EQ(5, updated.sellIn);
    EXPECT_EQ(80, updated.quality);
}

TEST_F(GildedRoseTest, SulfurasKeepsZeroQualityIfProvided) {
    // Given
    const std::string name = "Sulfuras, Hand of Ragnaros";

    // When
    const Item updated = updateSingleItem(name, 5, 0);

    // Then
    ASSERT_EQ(5, updated.sellIn);
    EXPECT_EQ(0, updated.quality);
}

TEST_F(GildedRoseTest, ConjuredItemDecreasesQualityTwiceAsFastBeforeSellDate) {
    // Given
    const std::string name = "Conjured Mana Cake";

    // When
    const Item updated = updateSingleItem(name, 10, 20);

    // Then
    ASSERT_EQ(9, updated.sellIn);
    EXPECT_EQ(18, updated.quality);
}

TEST_F(GildedRoseTest, ConjuredItemDecreasesQualityFourPointsOnSellDate) {
    // Given
    const std::string name = "Conjured Mana Cake";

    // When
    const Item updated = updateSingleItem(name, 0, 20);

    // Then
    ASSERT_EQ(-1, updated.sellIn);
    EXPECT_EQ(16, updated.quality);
}

TEST_F(GildedRoseTest, ConjuredItemDecreasesQualityFourPointsAfterSellDate) {
    // Given
    const std::string name = "Conjured Mana Cake";

    // When
    const Item updated = updateSingleItem(name, -1, 20);

    // Then
    ASSERT_EQ(-2, updated.sellIn);
    EXPECT_EQ(16, updated.quality);
}

TEST_F(GildedRoseTest, ConjuredItemQualityNeverDropsBelowZero) {
    // Given
    const std::string name = "Conjured Mana Cake";

    // When
    const Item updated = updateSingleItem(name, 10, 0);

    // Then
    ASSERT_EQ(9, updated.sellIn);
    EXPECT_EQ(0, updated.quality);
}

TEST_F(GildedRoseTest, ConjuredItemQualityCanStartAtUpperBoundaryAndDecrease) {
    // Given
    const std::string name = "Conjured Mana Cake";

    // When
    const Item updated = updateSingleItem(name, 10, 50);

    // Then
    ASSERT_EQ(9, updated.sellIn);
    EXPECT_EQ(48, updated.quality);
}

TEST_F(GildedRoseTest, ConjuredItemNameCanContainConjuredKeyword) {
    // Given
    const std::string name = "Highly Conjured Artifact";

    // When
    const Item updated = updateSingleItem(name, 10, 20);

    // Then
    ASSERT_EQ(9, updated.sellIn);
    EXPECT_EQ(18, updated.quality);
}
