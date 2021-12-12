#include <iostream>
#include <random>

/// Returns a random int between min (included) and max (included)
int rand(int min, int max)
{
    static std::default_random_engine  generator{std::random_device{}()};
    std::uniform_int_distribution<int> distribution{min, max};
    return distribution(generator);
}

int main()
{
    std::cout << rand(3, 11) << '\n';
    std::cout << rand(3, 11) << '\n';
    std::cout << rand(3, 11) << '\n';
    std::cout << rand(3, 11) << '\n';
    std::cout << rand(3, 11) << '\n';
}