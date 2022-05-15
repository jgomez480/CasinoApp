#include <iostream>
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif
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
    int dealer_hand_value;

    // The highest possible number of that can be held
    const int max_hand_cards = 12;

    Hand dealer_hand = Hand(max_hand_cards);
    Hand player_hand = Hand(max_hand_cards);
    Hand split_hand = Hand(max_hand_cards);
    bool split = false;

    while (playing) {
        can_double_down = true;
        can_split = false;
        split = false;

        main_deck.ToString();
        cout << endl;
        main_deck.Shuffle();
        std::cout << "Shuffling..." << std::endl;
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
            if (split) {
                DisplayHand(split_hand);
                player_hand_value = GetHandValue(split_hand);
            }
            else {
                DisplayHand(player_hand);
                player_hand_value = GetHandValue(player_hand);
            }

            cout << "Total: " << player_hand_value << endl;
            if (player_hand_value > 21) {
                cout << "Bust!" << endl;
                if (split) {
                    split = false;
                    can_double_down = true;
                }
                else {
                    game_in_progress = false;
                }
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

                if (split) {
                    Hit(main_deck, split_hand);
                }
                else {
                    Hit(main_deck, player_hand);
                }
                break;
            case 's':
                cout << "Stand\n" << endl;

                DisplayHand(dealer_hand);
                dealer_hand_value = GetHandValue(dealer_hand);
                cout << "Dealer Total: " << dealer_hand_value << endl;

                while (dealer_hand_value < 17 || (dealer_hand_value == 17 && dealer_hand.Contains(ace))) {
                    cout << "Dealer Hitting..." << endl;
                    Sleep(1000);
                    Hit(main_deck, dealer_hand);
                    DisplayHand(dealer_hand);
                    dealer_hand_value = GetHandValue(dealer_hand);
                    cout << "Dealer Total: " << dealer_hand_value << endl;
                }
                
                if (dealer_hand_value > 21) {
                    cout << "Dealer Busts! You Win!" << endl;
                }
                else if (dealer_hand_value > player_hand_value) {
                    cout << "House Wins!" << endl;
                }
                else if (dealer_hand_value < player_hand_value) {
                    cout << "You Win!" << endl;
                }
                else {
                    cout << "Push!" << endl;
                }

                if (split) {
                    split = false;
                    can_double_down = true;
                }
                else {
                    game_in_progress = false;
                }
                break;
            case 'd':
                if (can_double_down) {
                    cout << "Double Down!" << endl;
                    can_double_down = false;
                    if (split) {
                        Hit(main_deck, split_hand);
                    }
                    else {
                        Hit(main_deck, player_hand);
                    }
                }
                else {
                    invalid = true;
                }
                break;
            case 'x':
                if (can_split) {
                    cout << "Split!" << endl;
                    split = true;
                    split_hand.AddCard(player_hand.cards[1]);
                    player_hand.Remove(player_hand.cards[1]);
                    Hit(main_deck, player_hand);
                    Hit(main_deck, split_hand);
                    cout << "Current Cards Status" << endl;
                    cout << "First Hand: ";
                    DisplayHand(player_hand);
                    cout << "Second Hand: ";
                    DisplayHand(split_hand);
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

        player_hand.ClearHand();
        dealer_hand.ClearHand();
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
    for (int i = 0; i < hand.size; i++) {
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
    if (hand.size == 0) {
        cout << "No Hand";
        return;
    }

    int i;
    for (i = 0; i < hand.size - 1; i++) {
        cout << hand.cards[i].ToString() << ", ";
    }
    cout << hand.cards[i].ToString() << endl;
}