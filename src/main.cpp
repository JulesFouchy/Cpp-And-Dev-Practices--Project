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

int main()
{
    std::cout << player_is_alive(5) << '\n';
    std::cout << player_is_alive(1) << '\n';
    std::cout << player_is_alive(0) << '\n';
    std::cout << player_is_alive(-1) << '\n';
    std::cout << player_is_alive(-5) << '\n';
}