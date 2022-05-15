#include <iostream>
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <time.h>       /* time */
#include "Card.h"
#include "Blackjack.h"

using std::cout;
using std::cin;

int main()
{
    // Randomize the seed
    srand(time(NULL));

    PlayBlackjack();
    return 0;
}
