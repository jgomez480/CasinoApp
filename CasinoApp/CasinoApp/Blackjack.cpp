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

    // The highest possible number of cards that can be held
    const int max_hand_cards = 12;

    // Object declaration
    Deck main_deck = Deck();
    Hand dealer_hand = Hand(max_hand_cards);
    Hand player_hand = Hand(max_hand_cards);
    Hand split_hand = Hand(max_hand_cards);

    // Game variables struct
    BlackjackVars game_vars = {
        &main_deck,
        &dealer_hand,
        &player_hand,
        &split_hand,
    };

    // Variables
    int play_counter = 0;
    int shuffle_mark = 4;
    int player_hand_value, dealer_hand_value;
    bool playing = true;
    bool invalid;
    char action;

    int player_balance = 1000;
    int bet_amount = 0;

    while (playing) {
        game_vars.game_in_progress = true;
        game_vars.can_double_down = false;
        game_vars.can_split = false;
        game_vars.has_split = false;
        game_vars.first_hand = true;

        cout << endl;

        // If multiple games have been played, will shuffle deck.
        if (play_counter % shuffle_mark == 0) {
            main_deck.Shuffle();
            std::cout << "Shuffling..." << std::endl;
        }

        // Getting player's bet amount
        cout << "Betting Balance: " << player_balance << endl;
        bool invalid_bet = true;
        while (invalid_bet) {
            cout << "Enter bet: ";
            cin >> bet_amount;

            if (bet_amount > player_balance) {
                cout << "Not enough balance. Try again" << endl;
            }
            else if (bet_amount <= 0) {
                cout << "Invalid bet amount. Try again" << endl;
            }
            else {
                invalid_bet = false;
            }
        }
        
        // Draw two cards for dealer
        Hit(main_deck, dealer_hand);
        Hit(main_deck, dealer_hand);
        // Show dealer's up card
        cout << "\nDealer:\n";
        cout << dealer_hand.cards[0].ToString() << ", XXXXXXXX" << endl;

        // Draw two cards for player
        Hit(main_deck, player_hand);
        Hit(main_deck, player_hand);

        // Calculates whether player can split (same value cards)
        cout << endl;
        if (player_balance >= bet_amount) {
            game_vars.can_double_down = true;
            if (player_hand.cards[0].GetValue() == player_hand.cards[1].GetValue()) {
                game_vars.can_split = true;
            }
        }


        while (game_vars.game_in_progress) {
            invalid = false;

            // Displays player's current hand and total
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

            
            // Display showing possible action inputs
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

            // Execute one of four actions (Hit, Stand, Double down, Split) based on keyboard input char
            switch (action) {
            case 'h': // Hit
                cout << "Hit!" << endl;
                PlayerHit(game_vars);   
                break;
            case 's': // Stand
                cout << "Stand" << endl;
                Stand(game_vars);
                break;
            case 'd': // Double Down
                if (game_vars.can_double_down) {
                    cout << "Double Down!" << endl;
                    DoubleDown(game_vars);
                }
                else {
                    invalid = true;
                }
                break;
            case 'x': // Split
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

            // If action does not exist or can not be executed
            if (invalid) {
                cout << "Invalid Action, Try Again." << endl;
                continue;
            }
        }

        play_counter++;

        // User keyboard input whether to play again or end game
        char again;
        cout << "Play Again? (y/n): ";
        cin >> again;
        if (again == 'N' || again == 'n') {
            playing = false;
        }

        // Resetting hands
        player_hand.ClearHand();
        split_hand.ClearHand();
        dealer_hand.ClearHand();
    }
}


/// <summary>
/// Compares dealer hand values and player hand value to see who won/lost.
/// Prints result
/// </summary>
/// <param name="dealer_value">int - Current value of dealer's hand</param>
/// <param name="player_value">int - Current value of players's hand</param>
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

/// <summary>
/// Logic for dealer hitting. Continues to hit until dealer's hand reaches 17 or above
/// </summary>
/// <param name="game_vars"></param>
void DealerHit(BlackjackVars& game_vars) {
    Deck& deck = *game_vars.deck;
    Hand& dealer = *game_vars.dealer;
    int dealer_value = GetHandValue(dealer);

    // While dealers card are not 17 or higher will keep drawing cards
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

/// <summary>
/// Logic for double down action. Works similar to PlayerHit except executes a stand at end
/// </summary>
/// <param name="game_vars">BlackjackVars - All game variables required in struct</param>
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

    Stand(game_vars);
}

/// <summary>
/// Ends the current round of blackjack. Dealers finished by hitting until they must stop. Cards are compared to see which hands won.
/// </summary>
/// <param name="game_vars">BlackjackVars - All blackjack variables required in struct</param>
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

/// <summary>
/// Gets total integer value of all cards in hand.
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
void Hit(Deck& deck, Hand& hand) {
    Card new_card = deck.GetNextCard();
    hand.AddCard(new_card);
}

/// <summary>
/// Is this hand a blackjack (21)
/// </summary>
/// <param name="hand">Hand - hand to check</param>
/// <returns>Boolean result of check</returns>
bool IsBlackjack(Hand& hand) {
    return GetHandValue(hand) == 21;
}

/// <summary>
/// Is this hand a bust (over 21)
/// </summary>
/// <param name="hand">Hand - hand to check</param>
/// <returns>Boolean result of check</returns>
bool IsBust(Hand& hand) {
    return GetHandValue(hand) > 21;
}

/// <summary>
/// Compares two hands to see if they are a push (Same Value)
/// </summary>
/// <param name="first_hand">Hand - First hand to check</param>
/// <param name="second_hand">Hand - Second hand to check against</param>
/// <returns>Boolean result of check</returns>
bool IsPush(Hand& first_hand, Hand& second_hand) {
    return GetHandValue(first_hand) == GetHandValue(second_hand);
}

/// <summary>
/// Logic for hit. Draws new card for players current hand. Ends game or switches to split pair if bust or blackjack.
/// </summary>
/// <param name="game_vars">BlackjackVars - All game variables required in struct</param>
void PlayerHit(BlackjackVars& game_vars) {
    Deck& deck = *game_vars.deck;
    Hand& player = *game_vars.player;
    Hand& split = *game_vars.split;
    int player_hand_value;

    game_vars.can_double_down = false;
    game_vars.can_split = false;

    // Player using first hand (not split hand)
    if (game_vars.first_hand) {
        Hit(deck, player);
        DisplayHand(player);
        // If bust or blackjack
        if (IsBlackjack(player) || IsBust(player)) {
            cout << "Total: " << GetHandValue(player) << endl;
            // End round
            if (!game_vars.has_split) {
                EndRound(game_vars);
            }
            // Switch hand to split 
            else {
                game_vars.first_hand = false;
            }
        }
    }
    else {
        Hit(deck, split);
        // End the round if player bust or blackjack
        if (IsBlackjack(split) || IsBust(split)) {
            cout << "Total: " << GetHandValue(split) << endl;
            EndRound(game_vars);
            game_vars.game_in_progress = false;
        }
    }
}

/// <summary>
/// Logic for split action. 
/// </summary>
/// <param name="game_vars">BlackjackVars - All game variables required in struct</param>
void Split(BlackjackVars& game_vars) {
    Deck& deck = *game_vars.deck;
    Hand& player = *game_vars.player;
    Hand& split = *game_vars.split;

    // Set vars for split
    game_vars.can_split = false;
    game_vars.has_split = true;

    // Separate hands
    split.AddCard(player.cards[1]);
    player.Remove(player.cards[1]);

    // Draw new card for each hand
    Hit(deck, player);
    Hit(deck, split);

    // Print new hands
    cout << "Current Cards Status" << endl;
    cout << "First Hand: ";
    DisplayHand(player);
    cout << "Second Hand: ";
    DisplayHand(split);
}

/// <summary>
/// Logic to stand. Ends the round or switches to other hand if split
/// </summary>
/// <param name="game_vars">BlackjackVars - All game variables required in struct</param>
void Stand(BlackjackVars& game_vars) {
    if (game_vars.has_split && game_vars.first_hand) {
        game_vars.first_hand = false;
    }
    else {
        EndRound(game_vars);
    }
}