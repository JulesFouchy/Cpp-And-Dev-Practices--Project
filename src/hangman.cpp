#include "hangman.h"
#include <array>
#include <cassert>
#include <iostream>
#include "get_input_from_user.h"
#include "rand.h"
#include <algorithm>

class WordWithMissingLetters {
public:
    WordWithMissingLetters(std::string_view word)
        : _word{word}
        , _letters_revealed(word.size(), false)
    {
    }

    void mark_as_guessed(char guessed_letter) // mark_as_guessed is the only function in our hangman program that needs mutable access to the private variables.
    {                                         // And actually it only needs to index into _letters_revealed so we could even have mark_as_guessed() as a free function if we added methods to get _letters_revealed.begin() and _letters_revealed.end()
        std::transform(_letters_revealed.begin(), _letters_revealed.end(), _word.begin(), _letters_revealed.begin(), [&](bool b, char letter) {
            if (guessed_letter == letter) {
                return true;
            }
            else {
                return b;
            }
        });
    }

    const std::string&       word() const { return _word; }                        // We add getters but no setters: people can see the values but not modify them
    const std::vector<bool>& letters_guessed() const { return _letters_revealed; } // because we don't want anybody to be able to mess up our invariant

private:
    std::string       _word;
    std::vector<bool> _letters_revealed;
};

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

void show_word_to_guess_with_missing_letters(const WordWithMissingLetters& word)
{
    for (size_t i = 0; i < word.word().size(); ++i) { // Unfortunately we have to use a raw loop to index into both word and letters_guessed. In C++23 we will be able to use zip instead which is amazing! The loop would then look like `for (const auto& [letter, has_been_guessed] : zip(word, letters_guessed))`
        if (word.letters_guessed()[i]) {
            std::cout << word.word()[i];
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

void remove_one_life(int& lives_count)
{
    lives_count--;
}

void show_congrats_message(std::string_view word_to_guess)
{
    std::cout << "Congrats, you won!\nThe word was \"" << word_to_guess << "\"\n";
}

void show_defeat_message(std::string_view word_to_guess)
{
    std::cout << "Sorry, you lost!\nThe word was \"" << word_to_guess << "\"\n";
}

void play_hangman()
{
    WordWithMissingLetters word{pick_a_random_word()};
    int                    number_of_lives = 8;
    while (player_is_alive(number_of_lives) && !player_has_won(word.letters_guessed())) {
        show_number_of_lives(number_of_lives);
        show_word_to_guess_with_missing_letters(word);
        const auto guess = get_input_from_user<char>();
        if (word_contains(guess, word.word())) {
            word.mark_as_guessed(guess);
        }
        else {
            remove_one_life(number_of_lives);
        }
    }
    if (player_has_won(word.letters_guessed())) {
        show_congrats_message(word.word());
    }
    else {
        show_defeat_message(word.word());
    }
}