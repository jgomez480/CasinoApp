#include <iostream>
#include <Windows.h>
#include "Blackjack.h"

using std::cout;
using std::cin;
using std::endl;

BlackjackVars game_vars;

/// <summary>
/// Initates blackjack game. Conducted via I/O operations with user.
/// </summary>
void PlayBlackjack()
{ 
    cout << "Welcome To Blackjack" << endl;
    cout << "==========================" << endl;

    // Constants
    const int max_hand_cards = 12; // The highest possible number of cards that can be held
    int shuffle_mark = 4;          // After how many games should deck shuffle

    // Object declaration
    Deck main_deck = Deck();
    Hand dealer_hand = Hand(max_hand_cards);


    // Game variables struct
    game_vars = {
        &main_deck,
        &dealer_hand,
    };

    // Get how many players from user
    cout << "Enter how many players: ";
    cin >> game_vars.player_count;
    game_vars.players = new Player[game_vars.player_count];

    // Initalize players
    Player new_player = Player();
    for (int i = 0; i < game_vars.player_count; i++) {
        new_player = Player();
        new_player.betting_balance = (i + 1) * 100;
        new_player.main_hand = new Hand(max_hand_cards);
        new_player.split_hand = new Hand(max_hand_cards);
        new_player.first_hand = false;
        game_vars.players[i] = new_player;
    }


    // Variables
    Player* current_player;           // Current player taking turn
    Hand* player_hand = nullptr;      // Player's first main hand
    Hand* split_hand = nullptr;       // Player's second hand for splits
    BlackjackTotal player_hand_value; // Player's hand values
    BlackjackTotal dealer_hand_value; // Dealer's hand values
    int play_counter = 0;             // How many games have been played
    bool playing = true;              // Is the game still going (ends program)
    bool invalid;                     // Invalid action flag (hit, stand, ...)
    char action;                      // User input action



    while (playing) {
        // If multiple games have been played, will shuffle deck.
        if (play_counter % shuffle_mark == 0) {
            main_deck.Shuffle();
            cout << "Shuffling..." << endl;
        }

        // Loop through all players
        for (int i = 0; i < game_vars.player_count; i++) {
            current_player = &game_vars.players[i];

            // Clear previous hands
            current_player->main_hand->ClearHand();
            current_player->split_hand->ClearHand();


            // Getting player's bet amount
            if (current_player->betting_balance <= 0) {
                cout << "No balance left." << endl;
                return;
            }

            cout << "Betting Balance: " << current_player->betting_balance << endl;
            bool invalid_bet = true;
            while (invalid_bet) {
                cout << "Enter bet: ";
                cin >> current_player->bet_amount;

                if (current_player->bet_amount > current_player->betting_balance) {
                    cout << "Not enough balance. Try again" << endl;
                }
                else if (current_player->bet_amount <= 0) {
                    cout << "Invalid bet amount. Try again" << endl;
                }
                else {
                    invalid_bet = false;
                }
            }
            cout << "Player bet balance " << current_player->betting_balance;
            current_player->betting_balance -= current_player->bet_amount;
            cout << "Player bet balance " << current_player->betting_balance << endl;
        }

        // Draw two cards for dealer
        Hit(main_deck, dealer_hand);
        Hit(main_deck, dealer_hand);

        // Loop through all players
        for (int player_index = 0; player_index < game_vars.player_count; player_index++) {

            // Set the current player
            current_player = &game_vars.players[player_index];

            // Reset player variables
            current_player->game_in_progress = true;
            current_player->can_double_down = false;
            current_player->can_split = false;
            current_player->has_split = false;
            current_player->first_hand = true;

            // Set pointer to player's hands
            player_hand = current_player->main_hand;
            split_hand = current_player->split_hand;

            // Show dealer's up card
            cout << "\n\n";
            cout << "Dealer:" << endl;
            cout << dealer_hand.cards[0].ToString() << ", XXXXXXXX" << endl;

            // Draw two cards for player
            Hit(main_deck, *current_player->main_hand);
            Hit(main_deck, *current_player->main_hand);

            // Calculates whether player can split (same value cards)
            cout << endl;
            if (current_player->betting_balance >= current_player->bet_amount) {
                current_player->can_double_down = true;
                if (player_hand->cards[0].GetValue() == player_hand->cards[1].GetValue()) {
                    current_player->can_split = true;
                }
            }

            while (current_player->game_in_progress) {
                invalid = false;

                // Displays player's current hand and total
                cout << "\nPlayer:\n";
                if (current_player->first_hand) {
                    DisplayHand(*player_hand);
                    player_hand_value = GetHandValue(*player_hand);
                }
                else {
                    DisplayHand(*split_hand);
                    player_hand_value = GetHandValue(*split_hand);
                }


                // Display showing possible action inputs
                cout << "Enter action (h = hit, s = stand";
                if (current_player->can_double_down) {
                    cout << ", d = double down";
                }
                if (current_player->can_split) {
                    cout << ", x = split";
                }
                cout << "): ";
                cin >> action;
                action = tolower(action);

                // Execute one of four actions (Hit, Stand, Double down, Split) based on keyboard input char
                switch (action) {
                case 'h': // Hit
                    cout << "Hit!" << endl;
                    PlayerHit(current_player);
                    break;
                case 's': // Stand
                    cout << "Stand" << endl;
                    Stand(current_player);
                    break;
                case 'd': // Double Down
                    if (current_player->can_double_down) {
                        cout << "Double Down!" << endl;
                        DoubleDown(current_player);
                    }
                    else {
                        invalid = true;
                    }
                    break;
                case 'x': // Split
                    if (current_player->can_split) {
                        cout << "Split!" << endl;
                        Split(current_player);
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
        }

        EndRound();

        play_counter++;

        // User keyboard input whether to play again or end game
        char again;
        cout << "Play Again? (y/n): ";
        cin >> again;
        if (again == 'N' || again == 'n') {
            playing = false;
        }

        // Resetting hands
        dealer_hand.ClearHand();
    }

    for (int i = 0; i < game_vars.player_count; i++) {
        delete &game_vars.players[i];
    }
    delete &game_vars;
}


/// <summary>
/// Compares dealer hand values and player hand value to see who won/lost.
/// Prints result
/// </summary>
/// <param name="dealer_value">int - Current value of dealer's hand</param>
/// <param name="player_value">int - Current value of players's hand</param>
int CheckHands(BlackjackTotal dealer_value, BlackjackTotal player_value) {

    int game_result;
    int player = GetBestValue(player_value);
    int dealer = GetBestValue(dealer_value);

    cout << "Player: " << player << " Dealer: " << dealer << endl;

    // Game win/loss logic
    if (player > 21) {
        cout << "Bust!" << endl;
        game_result = kBust;
    }
    else if (dealer > 21) {
        cout << "Dealer Busts! You Win!" << endl;
        game_result = kWin;
    }
    else if (dealer > player) {
        cout << "House Wins!" << endl;
        game_result = kLose;
    }
    else if (dealer < player) {
        if (player == 21) {
            cout << "Blackjack!" << endl;
            game_result = kBlackjack;
        }
        else {
            cout << "You Win!" << endl;
            game_result = kWin;
        }
    }
    else {
        cout << "Push!" << endl;
        game_result = kPush;
    }

    // Return the game result symbolic code
    return game_result;
}

/// <summary>
/// Logic for dealer hitting. Continues to hit until dealer's hand reaches 17 or above
/// </summary>
/// <param name="game_vars"></param>
void DealerHit() {
    Deck& deck = *game_vars.deck;
    Hand& dealer = *game_vars.dealer;
    BlackjackTotal dealer_value = GetHandValue(dealer);

    // While dealers card are not 17 or higher will keep drawing cards
    while (dealer_value.high_value < 17 
       || (dealer_value.high_value == 17 && dealer.Contains(ace)) 
       || (dealer_value.high_value > 21 && dealer_value.low_value < 17)) {
        cout << "Dealer Hitting..." << endl;
        Sleep(1000);
        Hit(deck, dealer);
        DisplayHand(dealer);
        dealer_value = GetHandValue(dealer);
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

    BlackjackTotal value = GetHandValue(hand);

    if (value.high_value > 21 || value.high_value == value.low_value) {
        cout << "Total: " << value.low_value << endl;
    }
    else {
        cout << "Total: " << value.low_value << " / " << value.high_value << endl;
    }
}

/// <summary>
/// Logic for double down action. Works similar to PlayerHit except executes a stand at end
/// </summary>
/// <param name="game_vars">BlackjackVars - All game variables required in struct</param>
void DoubleDown(Player* current_player) {
    Deck& deck = *game_vars.deck;
    Hand& player = *current_player->main_hand;
    Hand& split = *current_player->split_hand;
    BlackjackTotal player_hand_value;
    current_player->can_double_down = false;
    if (current_player->first_hand) {
        current_player->betting_balance -= current_player->bet_amount;
        current_player->bet_amount *= 2;
        Hit(deck, player);
        DisplayHand(player);
        player_hand_value = GetHandValue(player);
    }
    else { 
        current_player->betting_balance -= current_player->split_bet_amount;
        current_player->split_bet_amount *= 2;
        Hit(deck, split);
        DisplayHand(split);
        player_hand_value = GetHandValue(split);
    }
    cout << "Total: " << player_hand_value.low_value << " / " << player_hand_value.high_value << endl;

    Stand(current_player);
}

/// <summary>
/// Ends the current round of blackjack. Dealers finished by hitting until they must stop. Cards are compared to see which hands won.
/// </summary>
/// <param name="game_vars">BlackjackVars - All blackjack variables required in struct</param>
void EndRound() {
    Deck& deck = *game_vars.deck;
    Hand& dealer = *game_vars.dealer;
    Player* current_player;

    for (int player_index = 0; player_index < game_vars.player_count; player_index++) {
        current_player = &game_vars.players[player_index];
        Hand& player_main_hand = *current_player->main_hand;
        Hand& player_split_hand = *current_player->split_hand;

        cout << "\nPlayer " << player_index + 1 << "\n----------------------------------" << endl;

        if (current_player->has_split) {
            cout << "First hand: ";
            DisplayHand(player_main_hand);
            cout << "Second hand: ";
            DisplayHand(player_split_hand);
        }
        else {
            cout << "Hand: ";
            DisplayHand(player_main_hand);

        }
    }

    Sleep(1000);

    BlackjackTotal dealer_value, player_value;

    cout << endl;
    DisplayHand(dealer);

    Sleep(500);

    DealerHit();
    dealer_value = GetHandValue(dealer);

    for (int player_index = 0; player_index < game_vars.player_count; player_index++) {

        current_player = &game_vars.players[player_index];
        Hand& player_main_hand = *current_player->main_hand;
        Hand& player_split_hand = *current_player->split_hand;
        int game_result;


        cout << "Player " << player_index + 1 << ": ";

        player_value = GetHandValue(player_main_hand);
        if (current_player->has_split) {
            cout << "First Hand: ";
            game_result = CheckHands(dealer_value, player_value);
            HandleBetResult(current_player, game_result, current_player->bet_amount);
            cout << "Second Hand: ";
            player_value = GetHandValue(player_split_hand);
            game_result = CheckHands(dealer_value, player_value);
            HandleBetResult(current_player, game_result, current_player->split_bet_amount);
        }
        else {
            game_result = CheckHands(dealer_value, player_value);
            HandleBetResult(current_player, game_result, current_player->bet_amount);
        }

        if (current_player->has_split) {
            current_player->has_split = false;
            current_player->can_double_down = true;
        }
    }
}

/// <summary>
/// Gives the better of the two possible blackjack values
/// </summary>
/// <param name="">BlackjackTotal - The total struct with both values</param>
/// <returns>Int - Resulting best value</returns>
int GetBestValue(BlackjackTotal& total) {
    // If not a bust, then the high value will always be the best
    int result;
    if (total.high_value > 21) {
        result = total.low_value;
    }
    else {
        result = total.high_value;
    }
    return result;
}


/// <summary>
/// Gets total integer value of all cards in hand.
/// Note: Blackjack rules, Face cards = 10, Ace = 11
/// </summary>
/// <param name="hand"> - Hand object to count values from</param>
/// <returns>Total value of cards</returns>
BlackjackTotal GetHandValue(Hand& hand) {
    Card current;
    int current_val;
    int high_total = 0;
    int low_total = 0;
    bool ace_counted = false;
    for (int i = 0; i < hand.size; i++) {
        current = hand.cards[i];
        current_val = current.GetValue();
        if (current_val == ace) { // Aces
            if (!ace_counted) {
                high_total += 11;
                ace_counted = true;
            }
            else {
                high_total += 1;
            }
            low_total += 1;
        }
        else if (current.GetValue() >= 10) { // Face Cards
            high_total += 10;
            low_total += 10;
        }
        else {
            high_total += current_val;
            low_total += current_val;
        }
    }

    BlackjackTotal total = {
        high_total,
        low_total
    };

    return total;
}

void HandleBetResult(Player* current_player, int game_result, int bet) {
    if (game_result == kWin) {
        current_player->betting_balance += bet * 2;
    }
    else if (game_result == kBlackjack) {
        current_player->betting_balance += bet * 2.5;
    }
    else if (game_result == kPush) {
        current_player->betting_balance += bet;
    }
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
    BlackjackTotal total = GetHandValue(hand);
    return total.low_value == 21 || total.high_value == 21;
}

/// <summary>
/// Is this hand a bust (over 21)
/// </summary>
/// <param name="hand">Hand - hand to check</param>
/// <returns>Boolean result of check</returns>
bool IsBust(Hand& hand) {
    return GetHandValue(hand).low_value > 21;
}

/// <summary>
/// Compares two hands to see if they are a push (Same Value)
/// </summary>
/// <param name="first_hand">Hand - First hand to check</param>
/// <param name="second_hand">Hand - Second hand to check against</param>
/// <returns>Boolean result of check</returns>
bool IsPush(Hand& first_hand, Hand& second_hand) {
    BlackjackTotal first_total = GetHandValue(first_hand);
    BlackjackTotal second_total = GetHandValue(second_hand);
    
    int first_val = GetBestValue(first_total);
    int second_val = GetBestValue(second_total);;
   
    return first_val == second_val;
}

/// <summary>
/// Logic for hit. Draws new card for players current hand. Ends game or switches to split pair if bust or blackjack.
/// </summary>
/// <param name="game_vars">BlackjackVars - All game variables required in struct</param>
void PlayerHit(Player* current_player) {


    Deck& deck = *game_vars.deck;
    Hand& player = *current_player->main_hand;
    Hand& split = *current_player->split_hand;
    int player_hand_value;

    cout << endl;

    current_player->can_double_down = false;
    current_player->can_split = false;

    // Player using first hand (not split hand)
    if (current_player->first_hand) {
        Hit(deck, player);
        DisplayHand(player);
        // If bust or blackjack
        if (IsBlackjack(player) || IsBust(player)) {
            // End round
            if (!current_player->has_split) {
                current_player->game_in_progress = false;
            }
            // Switch hand to split 
            else {
                current_player->first_hand = false;
            }
        }
    }
    else {
        Hit(deck, split);
        // End the round if player bust or blackjack
        if (IsBlackjack(split) || IsBust(split)) {
            current_player->game_in_progress = false;
        }
    }
}

/// <summary>
/// Logic for split action. 
/// </summary>
/// <param name="game_vars">BlackjackVars - All game variables required in struct</param>
void Split(Player* current_player) {
    Deck& deck = *game_vars.deck;
    Hand& player = *current_player->main_hand;
    Hand& split = *current_player->split_hand;
    current_player->split_bet_amount = current_player->bet_amount;
    current_player->betting_balance -= current_player->split_bet_amount;

    // Set vars for split
    current_player->can_split = false;
    current_player->has_split = true;

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
void Stand(Player* current_player) {
    if (current_player->has_split && current_player->first_hand) {
        current_player->first_hand = false;
    }
    else {
        current_player->game_in_progress = false;
    }
}