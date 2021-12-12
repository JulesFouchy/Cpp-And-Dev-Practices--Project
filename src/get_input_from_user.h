#pragma once
#include <iostream>

/// Blocks until the user inputs an int in the console
inline int get_int_from_user()
{
    int n; // NOLINT
    while (!(std::cin >> n)) {
        std::cin.clear(); // The user did not input a number so we need
        std::cin.sync();  // to clear cin and try again
        std::cout << "This is not a number, try again!\n";
    }
    return n;
}