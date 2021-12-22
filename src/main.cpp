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

bool word_contains(char letter, std::string_view word)
{
    return word.find(letter) != std::string_view::npos;
}

void mark_as_guessed(char guessed_letter, std::vector<bool>& letters_guessed, std::string_view word_to_guess)
{
    assert(word_to_guess.size() == letters_guessed.size()); // Its important to assert to make sure that your assumptions are actually checked in code
    std::transform(letters_guessed.begin(), letters_guessed.end(), word_to_guess.begin(), letters_guessed.begin(), [&](bool b, char letter) {
        if (guessed_letter == letter) {
            return true;
        }
        else {
            return b;
        }
    });
}

void remove_one_life(int& lives_count)
{
    lives_count--;
}

int main()
{
    std::string       word = "hello";
    std::vector<bool> letters_guessed(word.size(), false);
    show_word_to_guess_with_missing_letters(word, letters_guessed);
    mark_as_guessed('c', letters_guessed, word);
    show_word_to_guess_with_missing_letters(word, letters_guessed);
    mark_as_guessed('e', letters_guessed, word);
    show_word_to_guess_with_missing_letters(word, letters_guessed);
    mark_as_guessed('h', letters_guessed, word);
    show_word_to_guess_with_missing_letters(word, letters_guessed);
    mark_as_guessed('d', letters_guessed, word);
    show_word_to_guess_with_missing_letters(word, letters_guessed);
    mark_as_guessed('l', letters_guessed, word);
    show_word_to_guess_with_missing_letters(word, letters_guessed);
    mark_as_guessed('o', letters_guessed, word);
    show_word_to_guess_with_missing_letters(word, letters_guessed);
}