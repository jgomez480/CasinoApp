#include <iostream>

using std::cout;
using std::cin;
using std::endl;

void PlayRoulette() {
    cout << "Welcome To Roulette\n";
    cout << "==========================\n";

    // Variables
    bool playing = true;

    while (playing) {



        // User keyboard input whether to play again or end game
        char again;
        cout << "Play Again? (y/n): ";
        cin >> again;
        if (again == 'N' || again == 'n') {
            playing = false;
        }
    }
}