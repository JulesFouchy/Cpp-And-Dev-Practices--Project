#include "play_guess_the_number.h"
#include <iostream>
#include "rand.h"

/// Blocks until the user inputs an int in the console
int get_int_from_user()
{
    int n; // NOLINT
    while (!(std::cin >> n)) {
        std::cin.clear(); // The user did not input a number so we need
        std::cin.sync();  // to clear cin and try again
        std::cout << "This is not a number, try again!\n";
    }
    return n;
}

void play_guess_the_number()
{
    // Pick a random number
    static constexpr int MIN             = 0;   // `static constexpr` is the "proper" way of declaring constants known at compile time
    static constexpr int MAX             = 100; // It is as efficient as `#define` but has the benefit of working like a normal C++ variable: it has a type, etc.
    const int            number_to_guess = rand(MIN, MAX);
    std::cout << "I picked a number between " << MIN << " and " << MAX << '\n';
    // Ask the user for a guess
    bool finished = false;
    while (!finished) {
        const int user_guess = get_int_from_user();
        if (user_guess < number_to_guess) {
            std::cout << "Greater\n";
        }
        else if (user_guess > number_to_guess) {
            std::cout << "Smaller\n";
        }
        else {
            std::cout << "Congrats, you won!\n";
            finished = true;
        }
    }
}