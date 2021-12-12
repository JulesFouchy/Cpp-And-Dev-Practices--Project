#include <iostream>
#include <random>

/// Returns a random int between min (included) and max (included)
int rand(int min, int max)
{
    static std::default_random_engine  generator{std::random_device{}()};
    std::uniform_int_distribution<int> distribution{min, max};
    return distribution(generator);
}

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

int main()
{
    const int n = get_int_from_user();
    std::cout << "Got " << n << '\n';
}