/**
 * @file ConsoleView.cpp
 * @brief Implementation of the text-based CLI for Klondike Solitaire.
 *
 * @copyright Copyright (c) 2026. All rights reserved.
 *            This project is released under the MIT License.
 */

#include "ConsoleView.h"
#include <stddef.h>             // for size_t
#include <array>                // for array
#include <iostream>             // for basic_ostream, operator<<, basic_ist...
#include <sstream>              // for basic_stringstream
#include <vector>               // for vector
#include "core/Card.h"          // for Card, Rank, Suit
#include "core/Deck.h"          // for Deck
#include "core/KlondikeBoard.h" // for KlondikeBoard
#include "core/Pile.h"          // for Pile

namespace cli
{

    ConsoleView::ConsoleView(core::KlondikeController &controller) : controller_(controller) {}

    std::string ConsoleView::formatCard(const core::Card &card) const
    {
        if (!card.isFaceUp())
        {
            return "[???]";
        }

        std::string rankStr;
        switch (card.getRank())
        {
        case core::Rank::Ace:
            rankStr = "A";
            break;
        case core::Rank::Jack:
            rankStr = "J";
            break;
        case core::Rank::Queen:
            rankStr = "Q";
            break;
        case core::Rank::King:
            rankStr = "K";
            break;
        default:
            rankStr = std::to_string(static_cast<int>(card.getRank()));
            break;
        }

        std::string suitStr;
        std::string colorCode = "\033[0m"; // Default (Black)
        switch (card.getSuit())
        {
        case core::Suit::Hearts:
            suitStr = "\u2665";
            colorCode = "\033[31m";
            break; // Red
        case core::Suit::Diamonds:
            suitStr = "\u2666";
            colorCode = "\033[31m";
            break; // Red
        case core::Suit::Clubs:
            suitStr = "\u2663";
            break;
        case core::Suit::Spades:
            suitStr = "\u2660";
            break;
        }

        return colorCode + "[" + rankStr + suitStr + "]\033[0m";
    }

    void ConsoleView::renderBoard() const
    {
        const auto &board = controller_.getBoard();

        std::cout << "\n========================================\n";

        // Draw Stock and Waste
        std::cout << "Stock: " << (board.getStock().isEmpty() ? "[   ]" : "[XXX]")
                  << " (" << board.getStock().remainingCards() << ")   ";

        std::cout << "Waste: ";
        if (board.getWaste().isEmpty())
        {
            std::cout << "[   ]";
        }
        else
        {
            std::cout << formatCard(board.getWaste().getTopCard());
        }
        std::cout << "\n\n";

        // Draw Foundations
        std::cout << "Foundations:\n";
        const auto &foundations = board.getFoundations();
        for (size_t i = 0; i < foundations.size(); ++i)
        {
            std::cout << "F" << i << ": ";
            if (foundations[i].isEmpty())
                std::cout << "[   ]";
            else
                std::cout << formatCard(foundations[i].getTopCard());
            std::cout << "  ";
        }
        std::cout << "\n\n";

        // Draw Tableaus (Horizontal layout for CLI simplicity)
        std::cout << "Tableaus:\n";
        const auto &tableaus = board.getTableaus();
        for (size_t i = 0; i < tableaus.size(); ++i)
        {
            std::cout << "T" << i << ": ";
            if (tableaus[i].isEmpty())
            {
                std::cout << "[   ]";
            }
            else
            {
                // Read-only copy of the cards to print them in order
                auto cards = tableaus[i].getCards();
                for (const auto &card : cards)
                {
                    std::cout << formatCard(card) << " ";
                }
            }
            std::cout << "\n";
        }
        std::cout << "========================================\n";
    }

    void ConsoleView::printMenu() const
    {
        std::cout << "Commands:\n"
                  << "  d  --------------- Draw from Stock to Waste\n"
                  << "  w t <T>  --------- Move Waste to Tableau (e.g., w t 0)\n"
                  << "  w f <F>  --------- Move Waste to Foundation (e.g, w f 0)\n"
                  << "  t f <T> <F>  ----- Move Tableau to Foundation (e.g., t f 0 1)\n"
                  << "  t t <S> <D> <N>  - Move <N> cards from Tableau <S> to <D> (e.g., t t 0 1 3)\n"
                  << "  q  --------------- Quit\n";
    }

    void ConsoleView::processCommand(const std::string &command)
    {
        std::stringstream ss(command);
        std::string action;

        ss >> action;

        bool success = false;

        if (action == "d")
        {
            success = controller_.draw();
        }
        else if (action == "w")
        {
            std::string destType;
            size_t dest;
            if (ss >> destType >> dest)
            {
                if (destType == "t")
                    success = controller_.moveWasteToTableau(dest);
                else if (destType == "f")
                    success = controller_.moveWasteToFoundation(dest);
            }
        }
        else if (action == "t")
        {
            std::string destType;
            if (ss >> destType)
            {
                if (destType == "f")
                {
                    size_t src, dest;
                    if (ss >> src >> dest)
                        success = controller_.moveTableauToFoundation(src, dest);
                }
                else if (destType == "t")
                {
                    size_t src, dest, count;
                    if (ss >> src >> dest >> count)
                        success = controller_.moveTableauToTableau(src, dest, count);
                }
            }
        }

        if (!success && !action.empty())
        {
            std::cout << "\033[31mInvalid move or command.\033[0m\n";
        }
    }

    void ConsoleView::run()
    {
        std::string command;

        while (true)
        {
            renderBoard();

            // Check for win immediately after rendering the board
            if (controller_.isGameWon())
            {
                std::cout << "\n\033[32mCongratulations mate! You finished the game!\033[0m\n";
                std::cout << "Wanna play another one? (y/n): ";

                std::string choice;
                std::getline(std::cin, choice);

                if (choice == "y" || choice == "Y")
                {
                    controller_.startNewGame(); // Resets the deck, shuffles, and redeals
                    continue;                   // Skips the rest of the loop and starts fresh
                }
                else
                {
                    break; // Exits the game loop
                }
            }

            // Only print the menu and ask for a command if the game is NOT won
            printMenu();
            std::cout << "\n> ";
            std::getline(std::cin, command);

            if (command == "q" || command == "quit")
            {
                std::cout << "Thanks for playing!\n";
                break;
            }

            processCommand(command);
        }
    }
} // namespace cli