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

int main()
{
    std::cout << pick_a_random_word() << '\n';
    show_number_of_lives(10);
}