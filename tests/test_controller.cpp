/**
 * @file test_controller.cpp
 * @brief Unit tests for the KlondikeController facade.
 *
 * @copyright Copyright (c) 2026. All rights reserved.
 *            This project is released under the MIT License.
 */

#include <string>                     // for basic_string
#include "core/Card.h"                // for Card
#include "core/Deck.h"                // for Deck
#include "core/KlondikeBoard.h"       // for KlondikeBoard
#include "core/KlondikeController.h"  // for KlondikeController
#include "core/Pile.h"                // for Pile
#include "gtest/gtest.h"              // for AssertionResult, Message, Test


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