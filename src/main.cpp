#include <array>
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

int main()
{
    std::cout << player_has_won({true, false}) << '\n';
    std::cout << player_has_won({true, true, true}) << '\n';
    std::cout << player_has_won({false, true, true}) << '\n';
}