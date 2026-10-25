#include <gtest/gtest.h>
#include "core/Card.h"

using namespace core;

// Validates that a card defaults to face-down when created normally
TEST(CardTest, DefaultInitializationIsFaceDown)
{
    Card card(Rank::Ace, Suit::Hearts);

    EXPECT_EQ(card.getRank(), Rank::Ace);
    EXPECT_EQ(card.getSuit(), Suit::Hearts);
    EXPECT_FALSE(card.isFaceUp());
}

// Validates that the constructor accepts an explicit face-up state override
TEST(CardTest, ExplicitInitializationCanBeFaceUp)
{
    Card card(Rank::Queen, Suit::Diamonds, true);

    EXPECT_TRUE(card.isFaceUp());
}

// Validates the state mutator successfully toggles the boolean both ways
TEST(CardTest, FlipTogglesFaceUpState)
{
    Card card(Rank::Nine, Suit::Spades);

    EXPECT_FALSE(card.isFaceUp());
    card.flip();
    EXPECT_TRUE(card.isFaceUp());
    card.flip();
    EXPECT_FALSE(card.isFaceUp());
}