/**
 * @file test_rules.cpp
 * @brief Unit tests enforcing the stateless Klondike rules validator.
 *
 * @copyright Copyright (c) 2026. All rights reserved.
 *            This project is released under the MIT License.
 */

#include <string>                         // for basic_string
#include "core/Card.h"                    // for Card, Rank, Suit
#include "games/klondike/KlondikeRules.h" // for KlondikeRules
#include "core/Pile.h"                    // for Pile
#include "gtest/gtest.h"                  // for AssertionResult, Message, TestPartRe...

using namespace core;

TEST(KlondikeRulesTest, AlternateColorsLogic)
{
    Card hearts(Rank::Five, Suit::Hearts, true);     // Red
    Card diamonds(Rank::Nine, Suit::Diamonds, true); // Red
    Card spades(Rank::Ace, Suit::Spades, true);      // Black
    Card clubs(Rank::Queen, Suit::Clubs, true);      // Black

    // Valid alternate colors
    EXPECT_TRUE(KlondikeRules::areAlternateColors(diamonds, spades));
    EXPECT_TRUE(KlondikeRules::areAlternateColors(clubs, hearts));

    // Invalid same colors
    EXPECT_FALSE(KlondikeRules::areAlternateColors(diamonds, hearts));
    EXPECT_FALSE(KlondikeRules::areAlternateColors(spades, clubs));
}

TEST(KlondikeRulesTest, MoveToTableau)
{
    Pile tableau;
    Card king(Rank::King, Suit::Clubs, true);
    Card queen(Rank::Queen, Suit::Hearts, true);
    Card wrongQueen(Rank::Queen, Suit::Spades, true); // Black on Black
    Card wrongRank(Rank::Jack, Suit::Diamonds, true); // Jack on King (skips Queen)

    // Rule 1: Empty tableau only accepts Kings
    EXPECT_FALSE(KlondikeRules::canMoveToTableau(queen, tableau));
    EXPECT_TRUE(KlondikeRules::canMoveToTableau(king, tableau));

    // Setup tableau for stack tests
    tableau.addCard(king);

    // Rule 2: Must be exactly one rank lower and alternate color
    // Case 1. Red Queen on a Black King
    EXPECT_TRUE(KlondikeRules::canMoveToTableau(queen, tableau));
    // Case 2. Black Queen on a Black King
    EXPECT_FALSE(KlondikeRules::canMoveToTableau(wrongQueen, tableau));
    // Case 3.  Red Jack on a Black King (not 1 rank lower)
    EXPECT_FALSE(KlondikeRules::canMoveToTableau(wrongRank, tableau));
}

TEST(KlondikeRulesTest, MoveToFoundation)
{
    Pile foundation;
    Card aceClubs(Rank::Ace, Suit::Clubs, true);
    Card twoDiamonds(Rank::Two, Suit::Diamonds, true);
    Card twoClubs(Rank::Two, Suit::Clubs, true);
    Card threeClubs(Rank::Three, Suit::Clubs, true);

    // Rule 1: Empty foundation only accepts Aces
    EXPECT_FALSE(KlondikeRules::canMoveToFoundation(twoDiamonds, foundation));
    EXPECT_TRUE(KlondikeRules::canMoveToFoundation(aceClubs, foundation));

    // Setup foundation for stack tests
    foundation.addCard(aceClubs);

    // Rule 2: Must be exactly one rank higher and identical suit
    // Case 1. Two Clubs card on Ace Clubs card
    EXPECT_TRUE(KlondikeRules::canMoveToFoundation(twoClubs, foundation));
    // Case 2. Two Diamonds card on Ace Clubs card (wrong suit)
    EXPECT_FALSE(KlondikeRules::canMoveToFoundation(twoDiamonds, foundation));
    // Case 3.  Three Clubs card on Ace Clubs card (skips Two Clubs card)
    EXPECT_FALSE(KlondikeRules::canMoveToFoundation(threeClubs, foundation));
}