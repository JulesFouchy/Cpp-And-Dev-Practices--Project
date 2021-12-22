#include "get_input_from_user.h"
#include "hangman.h"
#include "play_guess_the_number.h"

void show_the_list_of_commands()
{
    std::cout << "What do you want to do?\n"
                 "1: Play \"Guess the Number\"\n"
                 "2: Play \"Hangman\"\n"
                 "q: Quit\n";
}

int main()
{
    bool quit = false;
    while (!quit) {
        show_the_list_of_commands();
        const auto command = get_input_from_user<char>();
        switch (command) {
        case '1':
            play_guess_the_number();
            break;
        case '2':
            play_hangman();
            break;
        case 'q':
            quit = true;
            break;
        default:
            std::cout << "Sorry I don't know that command!\n";
            break;
        }
    }
}