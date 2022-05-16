#pragma once

#include "Card.h";

struct BlackjackVars {
	Deck* deck;
	Hand* dealer;
	Hand* player;
	Hand* split;
	bool game_in_progress;
	bool can_double_down;
	bool can_split;
	bool has_split;
	bool first_hand;
};

// Function Prototypes
void PlayBlackjack();

void DoubleDown(BlackjackVars& game_vars);

void DisplayHand(Hand& hand);

bool IsBlackjack(Hand& hand);

bool IsBust(Hand& hand);

bool IsPush(Hand& first_hand, Hand& second_hand);

void DealerHit(BlackjackVars& game_vars);

void PlayerHit(BlackjackVars& game_vars);

void Hit(Deck& deck, Hand& hand);

void EndRound(BlackjackVars& game_vars);

void Stand(BlackjackVars& game_vars);

void CheckHands(int dealer_value, int player_value);

int GetHandValue(Hand& hand);

void Split(BlackjackVars& game_vars);