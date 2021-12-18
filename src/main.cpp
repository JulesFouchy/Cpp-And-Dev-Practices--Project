#include <array>
#include <cassert>
#include <iostream>
#include "play_guess_the_number.h"
#include "rand.h"

const char* pick_a_random_word()
{
    static constexpr std::array words = {
        "code",
        "crous",
        "imac",
        "opengl",
    };

    return words[rand<size_t>(0, words.size() - 1)];
}

void show_number_of_lives(int number_of_lives)
{
    std::cout << "You have " << number_of_lives << " lives\n";
}

bool player_is_alive(int number_of_lives)
{
    return number_of_lives > 0;
}

bool player_has_won(const std::vector<bool>& letters_guessed)
{
    return std::all_of(letters_guessed.begin(), letters_guessed.end(), [](bool letter_guessed) {
        return letter_guessed;
    });
}

void show_word_to_guess_with_missing_letters(const std::string& word, const std::vector<bool>& letters_guessed)
{
    assert(word.size() == letters_guessed.size()); // Its important to assert to make sure that your assumptions are actually checked in code
    for (size_t i = 0; i < word.size(); ++i) {     // Unfortunately we have to use a raw loop to index into both word and letters_guessed. In C++23 we will be able to use zip instead which is amazing! The loop would then look like `for (const auto& [letter, has_been_guessed] : zip(word, letters_guessed))`
        if (letters_guessed[i]) {
            std::cout << word[i];
        }
        else {
            std::cout << '_';
        }
        std::cout << ' ';
    }
    std::cout << '\n';
}

int main()
{
    show_word_to_guess_with_missing_letters("hello", {true, false, false, true, false});
}