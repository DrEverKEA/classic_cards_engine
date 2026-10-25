#pragma once

// Maps to the MVC 'core' directory. Isolates domain entities to prevent
// naming collisions with future external UI or graphics libraries.
namespace core
{
    // Strongly typed enum to enforce domain constraints and prevent
    // implicit conversions to integers or other enums (e.g., Rank).
    enum class Suit
    {
        Clubs,
        Diamonds,
        Hearts,
        Spades
    };

    // Ace initialized to 1 to establish a mathematical baseline.
    // Subsequent ranks auto-increment (King = 13) to allow simple arithmetic comparisons
    // in game rule logic (e.g., checking if rank A == rank B + 1).
    enum class Rank
    {
        Ace = 1,
        Two,
        Three,
        Four,
        Five,
        Six,
        Seven,
        Eight,
        Nine,
        Ten,
        Jack,
        Queen,
        King
    };
    class Card
    {
        // Cards default to face-down, matching standard initialization when generating a new deck.
    public:
        Card(Rank rank, Suit suit, bool isFaceUp = false);

        // Read-only accessors. Marked const to ensure thread safety and
        // allow safe iteration over const pile containers.
        Rank getRank() const;
        Suit getSuit() const;
        bool isFaceUp() const;

        // State mutator. Uses a domain-specific verb rather than a generic setter
        // to keep engine logic readable (e.g., card.flip()).
        void flip();

    private:
        // Private state enforces encapsulation. The ONLY way to alter a card is via public methods.
        // The trailing underscore (rank_) is a standard C++ convention to instantly distinguish
        // internal member variables from external constructor arguments.
        Rank rank_;
        Suit suit_;
        bool isFaceUp_;
    };
} // namespace core