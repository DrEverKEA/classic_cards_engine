/**
 * @file KlondikeController.cpp
 * @brief Implementation of the MVC Controller and move execution logic.
 *
 * @copyright Copyright (c) 2026. All rights reserved.
 *            This project is released under the MIT License.
 */

#include "core/KlondikeController.h"

namespace core
{
    bool KlondikeController::draw()
    {
        //TODO
    }

    bool KlondikeController::moveWasteToTableau(size_t tableauIndex)
    {
        //TODO
    }

    bool KlondikeController::moveWasteToFoundation(size_t foundationIndex)
    {
        //TODO
    }

    bool KlondikeController::moveTableauToFoundation(size_t tableauIndex, size_t foundationIndex)
    {
        //TODO
    }

    bool KlondikeController::moveTableauToTableau(size_t sourceIndex, size_t destIndex, size_t count)
    {
        //TODO
    }

    const KlondikeBoard &KlondikeController::getBoard() const
    {
        return board_;
    }
} // namespace core