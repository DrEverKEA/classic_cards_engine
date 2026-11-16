/**
 * @file ConsoleView.h
 * @brief Defines the Command Line Interface (CLI) view for the Klondike game.
 *
 * @copyright Copyright (c) 2026. All rights reserved.
 *            This project is released under the MIT License.
 */

#pragma once

#include <string>
#include "core/IGameController.h"
namespace core
{
    class Card;
}

namespace cli
{

    // The text-based user interface for the terminal. Renders the game board using ANSI escape codes
    // for color, user text commands, and sends them to the core Controller.
    class ConsoleView
    {
    public:
        // Constructs the CLI view attached to a game controller.
        explicit ConsoleView(core::IGameController &controller);

        // Starts the main text - based game loop.
        void run();

    private:
        core::IGameController &controller_;

        void renderBoard() const;
        std::string formatCard(const core::Card &card) const;
        void printMenu() const;

        void processCommand(const std::string &command);
    };
} // namespace cli