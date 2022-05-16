#include <iostream>
#include <Windows.h>
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

    // The highest possible number of that can be held
    const int max_hand_cards = 12;

    Deck main_deck = Deck(true);
    Hand dealer_hand = Hand(max_hand_cards);
    Hand player_hand = Hand(max_hand_cards);
    Hand split_hand = Hand(max_hand_cards);

    BlackjackVars game_vars = {
        &main_deck,
        &dealer_hand,
        &player_hand,
        &split_hand,
    };

    int play_counter = 0;
    int shuffle_mark = 4;
    int player_hand_value, dealer_hand_value;
    bool playing = true;
    bool invalid;
    char action;

    while (playing) {
        game_vars.game_in_progress = true;
        game_vars.can_double_down = true;
        game_vars.can_split = false;
        game_vars.has_split = false;
        game_vars.first_hand = true;
        
        

        cout << endl;
        if (play_counter % shuffle_mark == 0) {
            main_deck.Shuffle();
            std::cout << "Shuffling..." << std::endl;
        }
        
        Hit(main_deck, dealer_hand);
        Hit(main_deck, dealer_hand);
        cout << "\nDealer:\n";
        cout << dealer_hand.cards[0].ToString() << ", XXXXXXXX" << endl;

        Hit(main_deck, player_hand);
        Hit(main_deck, player_hand);

        cout << endl;
        if (player_hand.cards[0].GetValue() == player_hand.cards[1].GetValue()) {
            game_vars.can_split = true;
        }

        while (game_vars.game_in_progress) {
            invalid = false;

            cout << "\nPlayer:\n";
            if (game_vars.first_hand) {
                DisplayHand(player_hand);
                player_hand_value = GetHandValue(player_hand);
            }
            else {
                DisplayHand(split_hand);
                player_hand_value = GetHandValue(split_hand);
            }

            cout << "Total: " << player_hand_value << endl;
            if (player_hand_value > 21) {
                cout << "Bust!" << endl;
                if (game_vars.has_split) {
                    game_vars.has_split = false;
                    game_vars.can_double_down = true;
                }
                else {
                    game_vars.game_in_progress = false;
                }
                break;
            }
            
            cout << "Enter action (h = hit, s = stand";
            if (game_vars.can_double_down) {
                cout << ", d = double down";
            }
            if (game_vars.can_split) {
                cout << ", x = split";
            }
            cout << "): ";
            cin >> action;
            action = tolower(action);

            switch (action) {
            case 'h':
                cout << "Hit!" << endl;
                PlayerHit(game_vars);   
                break;
            case 's':
                cout << "Stand" << endl;
                Stand(game_vars);
                break;
            case 'd':
                if (game_vars.can_double_down) {
                    cout << "Double Down!" << endl;
                    DoubleDown(game_vars);
                }
                else {
                    invalid = true;
                }
                break;
            case 'x':
                if (game_vars.can_split) {
                    cout << "Split!" << endl;
                    Split(game_vars);
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

        play_counter++;

        char again;
        cout << "Play Again? (y/n): ";
        cin >> again;

        if (again == 'N' || again == 'n') {
            playing = false;
        }

        player_hand.ClearHand();
        split_hand.ClearHand();
        dealer_hand.ClearHand();
    }
}

void DoubleDown(BlackjackVars& game_vars) {
    Deck& deck = *game_vars.deck;
    Hand& player = *game_vars.player;
    Hand& split = *game_vars.split;
    int player_hand_value;
    game_vars.can_double_down = false;
    if (game_vars.first_hand) {
        Hit(deck, player);
        DisplayHand(player);
        player_hand_value = GetHandValue(player);
    }
    else {
        Hit(deck, split);
        DisplayHand(split);
        player_hand_value = GetHandValue(split);
    }
    cout << "Total: " << player_hand_value << endl;

    if (game_vars.has_split && game_vars.first_hand) {
        game_vars.first_hand = false;
    }
    else {
        EndRound(game_vars);
    }
}

void Split(BlackjackVars& game_vars) {
    Deck& deck = *game_vars.deck;
    Hand& player = *game_vars.player;
    Hand& split = *game_vars.split;
    game_vars.can_split = false;
    game_vars.has_split = true;
    split.AddCard(player.cards[1]);
    player.Remove(player.cards[1]);
    Hit(deck, player);
    Hit(deck, split);
    cout << "Current Cards Status" << endl;
    cout << "First Hand: ";
    DisplayHand(player);
    cout << "Second Hand: ";
    DisplayHand(split);
}

void Stand(BlackjackVars& game_vars) {
    if (game_vars.has_split && game_vars.first_hand) {
        game_vars.first_hand = false;
    }
    else {
        EndRound(game_vars);
    }
}

void EndRound(BlackjackVars& game_vars) {

    Deck& deck = *game_vars.deck;
    Hand& dealer = *game_vars.dealer;
    Hand& player = *game_vars.player;
    Hand& split = *game_vars.split;

    cout << endl;

    DisplayHand(dealer);
    int dealer_value, player_value;

    dealer_value = GetHandValue(dealer);
    cout << "Dealer Total: " << dealer_value << endl;

    while (dealer_value < 17 || (dealer_value == 17 && dealer.Contains(ace))) {
        cout << "Dealer Hitting..." << endl;
        Sleep(1000);
        Hit(deck, dealer);
        DisplayHand(dealer);
        dealer_value = GetHandValue(dealer);
        cout << "Dealer Total: " << dealer_value << endl;
    }

    player_value = GetHandValue(player);
    if (game_vars.has_split) {
        cout << "First Hand: ";
        CheckHands(dealer_value, player_value);
        cout << "Second Hand: "; 
        player_value = GetHandValue(split);
        CheckHands(dealer_value, player_value);
    }
    else {
        CheckHands(dealer_value, player_value);
    }

    if (game_vars.has_split) {
        game_vars.has_split = false;
        game_vars.can_double_down = true;
    }

    game_vars.game_in_progress = false;
}

void CheckHands(int dealer_value, int player_value) {
    if (player_value > 21) {
        cout << "Bust!" << endl;
    }
    else if (dealer_value > 21) {
        cout << "Dealer Busts! You Win!" << endl;
    }
    else if (dealer_value > player_value) {
        cout << "House Wins!" << endl;
    }
    else if (dealer_value < player_value) {
        if (player_value == 21) {
            cout << "Blackjack!" << endl;
        }
        else {
            cout << "You Win!" << endl;
        }
    }
    else {
        cout << "Push!" << endl;
    }
}

bool IsBlackjack(Hand& hand) {
    return GetHandValue(hand) == 21;
}

bool IsBust(Hand& hand) {
    return GetHandValue(hand) > 21;
}

bool IsPush(Hand& first_hand, Hand& second_hand) {
    return GetHandValue(first_hand) == GetHandValue(second_hand);
}

/// <summary>
/// Gets integer value of all cards in hand.
/// Note: Blackjack rules, Face cards = 10, Ace = 11
/// </summary>
/// <param name="hand"> - Hand object to count values from</param>
/// <returns>Total value of cards</returns>
int GetHandValue(Hand& hand) {
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

void PlayerHit(BlackjackVars& game_vars) {
    Deck& deck = *game_vars.deck;
    Hand& player = *game_vars.player;
    Hand& split = *game_vars.split;
    int player_hand_value;

    game_vars.can_double_down = false;
    game_vars.can_split = false;

    if (game_vars.first_hand) {
        Hit(deck, player);
        DisplayHand(player);
        if (IsBlackjack(player) || IsBust(player)) {
            cout << "Total: " << GetHandValue(player) << endl;
            if (!game_vars.has_split) {
                EndRound(game_vars);
            }
            else {
                game_vars.first_hand = false;
            }
        }
    }
    else {
        Hit(deck, split);
        if (IsBlackjack(split) || IsBust(split)) {
            cout << "Total: " << GetHandValue(split) << endl;
            EndRound(game_vars);
            game_vars.game_in_progress = false;
        }
    }
}

void DealerHit(BlackjackVars& game_vars) {
    Deck& deck = *game_vars.deck;
    Hand& dealer = *game_vars.dealer;
    int dealer_value = GetHandValue(dealer);
    while (dealer_value < 17 || (dealer_value == 17 && dealer.Contains(ace))) {
        cout << "Dealer Hitting..." << endl;
        Sleep(1000);
        Hit(deck, dealer);
        DisplayHand(dealer);
        dealer_value = GetHandValue(dealer);
        cout << "Dealer Total: " << dealer_value << endl;
    }
}

/// <summary>
/// Prints out all cards in a given hand separated by commas
/// </summary>
/// <param name="hand"> - Hand to print</param>
void DisplayHand(Hand& hand) {
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