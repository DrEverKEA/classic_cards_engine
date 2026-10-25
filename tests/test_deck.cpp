#include <gtest/gtest.h>
#include <stdexcept>
#include "core/Deck.h"

using namespace core;

// Validates the factory initialization guarantees a standard 52-card supply.
TEST(DeckTest, InitializationCreatesFiftyTwoCards)
{
    Deck deck;
    EXPECT_EQ(deck.remainingCards(), 52);
    EXPECT_FALSE(deck.isEmpty());
}

// Validates that dealing mutates the internal container and returns a card.
TEST(DeckTest, DealCardReducesDeckSize)
{
    Deck deck;
    Card dealtCard = deck.dealCard();

    EXPECT_EQ(deck.remainingCards(), 51);
}

// Validates the URBG shuffle operation preserves the exact container size.
TEST(DeckTest, SuffleMaintainsDeckSize)
{
    Deck deck;
    deck.suffle();

    EXPECT_EQ(deck.remainingCards(), 52);
}

// Validates boundary safety by ensuring an exception is thrown when dealing from an empty deck.
TEST(DeckTest, DealingFromEmptyDeckThrowsException)
{
    Deck deck;

    for (int i = 0; i < 52; ++i)
    {
        deck.dealCard();
    }

    EXPECT_TRUE(deck.isEmpty());
    EXPECT_THROW(deck.dealCard(), std::out_of_range);
}
