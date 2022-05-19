#pragma once

#include "Card.h";

enum GameResults {
	kBust,
	kLose,
	kWin,
	kBlackjack,
	kPush
};

/*
// Datatypes
struct BlackjackVars {
	Deck* deck;
	Hand* dealer;
	Hand* player;
	Hand* split;
	int player_balance;
	int bet_amount;
	int split_bet_amount;
	bool game_in_progress;
	bool can_double_down;
	bool can_split;
	bool has_split;
	bool first_hand;
};*/

// Datatypes
struct Player {
	Hand* main_hand;
	Hand* split_hand;
	int betting_balance;
	int bet_amount;
	int split_bet_amount;
	bool game_in_progress;
	bool turn_in_progress;
	bool can_double_down;
	bool can_split;
	bool has_split;
	bool first_hand;
};


struct BlackjackVars {
	Deck* deck;
	Hand* dealer;
	Player* players;
	int player_count;
};


struct BlackjackTotal {
	int high_value;
	int low_value;
};


// Function Prototypes
void PlayBlackjack();

int CheckHands(int dealer_value, int player_value);

void DisplayHand(Hand& hand);

void DoubleDown(Player* current_player);

void DealerHit();

void EndRound();

int GetBestValue(BlackjackTotal& total);

BlackjackTotal GetHandValue(Hand& hand);

void HandleBetResult(Player* current_player, int game_result, int bet);

void Hit(Deck& deck, Hand& hand);

bool IsBlackjack(Hand& hand);

bool IsBust(Hand& hand);

bool IsPush(Hand& first_hand, Hand& second_hand);

void PlayerHit(Player* current_player);

void Split(Player* current_player);

void Stand(Player* current_player);