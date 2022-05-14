#include <iostream>
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <time.h>       /* time */
#include "Card.h"

using std::cout;

int main()
{

    srand(time(NULL));


    cout << "Hello World!\n";

    for (int i = 0; i < 4; i++) {

        for (int j = 2; j < 15; j++) {
            Card newCard = Card(i, j);
            newCard.ToString();
        }
    }

    cout << "\n\n";

    Deck newDeck = Deck();
    newDeck.ToString();

    cout << "\n\n";

    newDeck.Shuffle();

    newDeck.ToString();

    return 0;
}
