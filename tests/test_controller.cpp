/**
 * @file test_controller.cpp
 * @brief Unit tests for the KlondikeController facade.
 *
 * @copyright Copyright (c) 2026. All rights reserved.
 *            This project is released under the MIT License.
 */

#include <string>                              // for basic_string
#include "../src/core/Card.h"                  // for Card
#include "../src/core/Deck.h"                  // for Deck
#include "games/klondike/KlondikeBoard.h"      // for KlondikeBoard
#include "games/klondike/KlondikeController.h" // for KlondikeController
#include "games/klondike/KlondikeRules.h"      // for KlondikeRules
#include "core/Pile.h"                         // for Pile
#include "gtest/gtest.h"                       // for AssertionResult, Message, Test

using namespace core;

TEST(KlondikeControllerTest, DrawAndRecycleMechanics)
{
    KlondikeController controller;
    const KlondikeBoard &board = controller.getBoard();

    // Initial state: Stock has 24, Waste is empty
    EXPECT_EQ(board.getStock().remainingCards(), 24);
    EXPECT_TRUE(board.getWaste().isEmpty());

    // Draw one card
    bool drawSuccess = controller.draw();

    EXPECT_TRUE(drawSuccess);
    EXPECT_EQ(board.getStock().remainingCards(), 23);
    EXPECT_EQ(board.getWaste().size(), 1);
    // Drawn cards must be face-up
    EXPECT_TRUE(board.getWaste().getTopCard().isFaceUp());

    // Draw the remaining 23 cards
    for (int i = 0; i < 23; ++i)
    {
        controller.draw();
    }
    EXPECT_TRUE(board.getStock().isEmpty());
    EXPECT_EQ(board.getWaste().size(), 24);

    // Recycle: Drawing from an empty stock should move the waste back to the stock
    bool recycleSuccess = controller.draw();
    EXPECT_TRUE(recycleSuccess);
    EXPECT_EQ(board.getStock().remainingCards(), 24);
    EXPECT_TRUE(board.getWaste().isEmpty());
}

TEST(KlondikeControllerTest, SafelyRejectEmptyMoves)
{
    KlondikeController controller;

    // Waste is initially empty, so moving from it must fail gracefully
    EXPECT_FALSE(controller.moveWasteToTableau(0));
    EXPECT_FALSE(controller.moveWasteToFoundation(0));
}

TEST(KlondikeControllerTest, BoundsChecking)
{
    KlondikeController controller;

    // Attempting to move to tableaus/foundations outside the array bounds should safely return false,
    // not crash the engine with an out_of_bounds exception.
    EXPECT_FALSE(controller.moveWasteToTableau(99));
    EXPECT_FALSE(controller.moveWasteToFoundation(99));
    EXPECT_FALSE(controller.moveTableauToTableau(99, 0, 1));
}

TEST(KlondikeControllerTest, MoveKingToEmptyTableau)
{
    Pile emptyTableau;
    Card kingSpades(Rank::King, Suit::Spades, true);
    Card queenHearts(Rank::Queen, Suit::Hearts, true);

    // An empty tableau must accept a King
    EXPECT_TRUE(KlondikeRules::canMoveToTableau(kingSpades, emptyTableau));

    // An empty tableau must reject any non-King card
    EXPECT_FALSE(KlondikeRules::canMoveToTableau(queenHearts, emptyTableau));
}

TEST(KlondikeControllerTest, TableauAlternatingColorAndDescendingRank)
{
    Pile tableau;
    tableau.addCard(Card(Rank::Nine, Suit::Clubs, true)); // Top card is 9 of Clubs (Black)

    Card eightHearts(Rank::Eight, Suit::Hearts, true);  // 8 of Hearts (Red): rank 8 on 9, alternating color
    Card eightSpades(Rank::Eight, Suit::Spades, true);  // 8 of Spades (Black): same color
    Card sevenHearts(Rank::Seven, Suit::Hearts, true);  // 7 of Hearts (Red): non-sequential rank

    // Legal move: 8 Red on 9 Black
    EXPECT_TRUE(KlondikeRules::canMoveToTableau(eightHearts, tableau));

    // Reject same color
    EXPECT_FALSE(KlondikeRules::canMoveToTableau(eightSpades, tableau));

    // Reject non-consecutive rank
    EXPECT_FALSE(KlondikeRules::canMoveToTableau(sevenHearts, tableau));
}

TEST(KlondikeControllerTest, FoundationSuitAndOrderIntegrity)
{
    Pile emptyFoundation;
    Card aceHearts(Rank::Ace, Suit::Hearts, true);
    Card twoHearts(Rank::Two, Suit::Hearts, true);

    // An empty foundation must accept an Ace
    EXPECT_TRUE(KlondikeRules::canMoveToFoundation(aceHearts, emptyFoundation));

    // An empty foundation must reject non-Ace cards
    EXPECT_FALSE(KlondikeRules::canMoveToFoundation(twoHearts, emptyFoundation));

    // Build the foundation with the Ace of Hearts
    emptyFoundation.addCard(aceHearts);

    Card twoSpades(Rank::Two, Suit::Spades, true);    // Same rank (2), wrong suit (Spades)
    Card threeHearts(Rank::Three, Suit::Hearts, true); // Same suit (Hearts), skipped rank (3)

    // Legal: 2 of Hearts on Ace of Hearts
    EXPECT_TRUE(KlondikeRules::canMoveToFoundation(twoHearts, emptyFoundation));

    // Reject incorrect suit
    EXPECT_FALSE(KlondikeRules::canMoveToFoundation(twoSpades, emptyFoundation));

    // Reject skipped rank
    EXPECT_FALSE(KlondikeRules::canMoveToFoundation(threeHearts, emptyFoundation));
}

TEST(KlondikeControllerTest, WinConditionTrigger)
{
    KlondikeController controller;

    // A freshly dealt game must not be won
    EXPECT_FALSE(controller.isGameWon());
}