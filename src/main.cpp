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

    return words[rand(0, words.size() - 1)];
}

int main()
{
    std::cout << pick_a_random_word() << '\n';
}