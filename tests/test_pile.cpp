#include <gtest/gtest.h>
#include <stdexcept>
#include <vector>
#include "core/Pile.h"

using namespace core;

TEST(PileTest, InitializationIsEmpty)
{
    Pile pile;
    EXPECT_TRUE(pile.isEmpty());
    EXPECT_EQ(pile.size(), 0);
}

TEST(PileTest, AddAndRemoveSingleCard)
{
    Pile pile;
    Card c(Rank::Eight, Suit::Hearts, true);

    pile.addCard(c);
    EXPECT_FALSE(pile.isEmpty());
    EXPECT_EQ(pile.size(), 1);

    // Top card should be readable without removal
    EXPECT_EQ(pile.getTopCard().getRank(), Rank::Eight);

    Card removed =  pile.removeCard();
    EXPECT_EQ(removed.getRank(), Rank::Eight);
    EXPECT_TRUE(pile.isEmpty());
}

TEST(PileTest, MultiCardOperationsPreserveOrder)
{
    Pile pile;
    std::vector<Card> stack = {
        Card(Rank::Seven, Suit::Spades, true),
        Card(Rank::Six, Suit::Diamonds, true),
        Card(Rank::Five, Suit::Clubs, true)
    };

    pile.addCards(stack);
    EXPECT_EQ(pile.size(), 3);
    // The Five is at the bottom of the sub-stack, so it becomes the new top
    EXPECT_EQ(pile.getTopCard().getRank(), Rank::Five);

    std::vector<Card> removedStack = pile.removeCards(3);
    EXPECT_EQ(removedStack.size(), 3);
    EXPECT_TRUE(pile.isEmpty());

    // Validates the removed stack maintains its original structural order
    EXPECT_EQ(removedStack[0].getRank(), Rank::Seven);
    EXPECT_EQ(removedStack[1].getRank(), Rank::Six);
    EXPECT_EQ(removedStack[2].getRank(), Rank::Five);
}

TEST(PileTest, BoundaryExceptions){
    Pile pile;

    EXPECT_THROW(pile.getTopCard(), std::out_of_range);
    EXPECT_THROW(pile.removeCard(), std::out_of_range);
    EXPECT_THROW(pile.removeCards(1), std::out_of_range);

    pile.addCard(Card(Rank::Jack, Suit::Hearts));
    // Cannot remove 2 from a pile of 1
    EXPECT_THROW(pile.removeCards(2), std::out_of_range);
}