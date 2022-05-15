#include <iostream>
#include "Blackjack.h"

using std::cout;
using std::cin;
using std::endl;

/// <summary>
/// Initates blackjack game. Conducted via I/O operations with user.
/// </summary>
void PlayBlackjack()
{
    cout << "Welcome To Blackjack\n";
    cout << "==========================\n";

    Deck main_deck = Deck();
    int play_counter = 0;
    int shuffle_mark = 4;
    bool playing = true;
    bool can_split, can_double_down;


    // The highest possible number of that can be held
    const int max_hand_cards = 12;

    Hand dealer_hand = Hand(max_hand_cards);
    Hand player_hand = Hand(max_hand_cards);

    while (playing) {
        can_double_down = true;
        can_split = false;

        main_deck.ToString();
        cout << endl;
        main_deck.Shuffle();
        main_deck.ToString();

        
        Hit(main_deck, dealer_hand);
        Hit(main_deck, dealer_hand);
        cout << "\nDealer:\n";
        cout << dealer_hand.cards[0].ToString() << ", XXXXXXXX" << endl;

        Hit(main_deck, player_hand);
        Hit(main_deck, player_hand);

        cout << endl;
        if (player_hand.cards[0].GetValue() == player_hand.cards[1].GetValue()) {
            can_split = true;
        }

        bool game_in_progress = true;
        bool invalid;
        int player_hand_value;
        while (game_in_progress) {
            invalid = false;
            char action;

            cout << "\nPlayer:\n";
            DisplayHand(player_hand);
            player_hand_value = GetHandValue(player_hand);
            cout << "Total: " << player_hand_value << endl;
            if (player_hand_value > 21) {
                cout << "Bust!" << endl;
                game_in_progress = false;
                break;
            }
            
            cout << "Enter action (h = hit, s = stand";

            if (can_double_down) {
                cout << ", d = double down";
            }
            if (can_split) {
                cout << ", x = split";
            }
            cout << "): ";

            cin >> action;
            action = tolower(action);

            switch (action) {
            case 'h':
                cout << "Hit!" << endl;
                can_double_down = false;
                Hit(main_deck, player_hand);
                break;
            case 's':
                cout << "Stand" << endl;
                game_in_progress = false;
                break;
            case 'd':
                if (can_double_down) {
                    cout << "Double Down!" << endl;
                    can_double_down = false;
                    Hit(main_deck, player_hand);
                }
                else {
                    invalid = true;
                }
                break;
            case 'x':
                if (can_split) {
                    cout << "Split!" << endl;
                }
                else {
                    invalid = true;
                }
                break;
            default:
                invalid = true;
            }

            if (invalid) {
                cout << "Invalid Action, Try Again." << endl;
                continue;
            }
        }



        char again;
        cout << "Play Again? (y/n): ";
        cin >> again;

        if (again == 'N' || again == 'n') {
            playing = false;
        }
    }
}

/// <summary>
/// Gets integer value of all cards in hand.
/// Note: Blackjack rules, Face cards = 10, Ace = 11
/// </summary>
/// <param name="hand"> - Hand object to count values from</param>
/// <returns>Total value of cards</returns>
int GetHandValue(Hand hand) {
    Card current;
    int current_val;
    int total = 0;
    for (int i = 0; i < hand.cards_size; i++) {
        current = hand.cards[i];
        current_val = current.GetValue();
        if (current_val >= 14) { // Aces
            total += 11;
        }
        else if (current.GetValue() >= 10) { // Face Cards
            total += 10;
        }
        else {
            total += current_val;
        }
    }
    return total;
}

/// <summary>
/// Grabs card from deck and places into hand
/// </summary>
/// <param name="deck"> - Deck to pull cards from</param>
/// <param name="hand"> - Hand to place new card into</param>
void Hit(Deck &deck, Hand &hand) {
    Card new_card = deck.GetNextCard();
    hand.AddCard(new_card);
}

/// <summary>
/// Prints out all cards in a given hand separated by commas
/// </summary>
/// <param name="hand"> - Hand to print</param>
void DisplayHand(Hand hand) {
    if (hand.cards_size == 0) {
        cout << "No Hand";
        return;
    }

    int i;
    for (i = 0; i < hand.cards_size - 1; i++) {
        cout << hand.cards[i].ToString() << ", ";
    }
    cout << hand.cards[i].ToString() << endl;
}
