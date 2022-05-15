#pragma once

#include "Card.h";

// Function Prototypes
void PlayBlackjack();

void DisplayHand(Hand hand);

void Hit(Deck &deck, Hand &hand);

int GetHandValue(Hand hand);