#pragma once
#include <iostream>

/// Blocks until the user inputs something of type T in the console
template<typename T>
T get_input_from_user()
{
    T input; // NOLINT
    while (!(std::cin >> input)) {
        std::cin.clear(); // The user did not input a valid T so we need
        std::cin.sync();  // to clear cin and try again
        std::cout << "Invalid input, try again!\n";
    }
    return input;
}