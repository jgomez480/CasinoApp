#pragma once

#include <stdlib.h>     /* srand, rand */
#include <iostream>
#include <sstream>

enum Suit {
	hearts,
	diamonds,
	clubs,
	spades
};

enum Value {
	jack = 11,
	queen,
	king,
	ace
};

class Card {

	int suit;
	int value;

public:

	Card() {
		suit = 0;
		value = 0;
	}

	Card(int suit, int value) {
		this->suit = suit;
		this->value = value;
	}

	int GetValue() {
		return value;
	}

	int GetSuit() {
		return suit;
	}

	bool Equals(Card card) {
		return suit == card.suit && value == card.value;
	}

	bool Equals(int inputSuit, int inputValue) {
		return suit == inputSuit && value == inputValue;
	}

	std::string ToString() {

		std::string suit_str;
		switch (suit) {
		case hearts:
			suit_str = "Hearts";
			break;
		case diamonds:
			suit_str = "Diamonds";
			break;
		case clubs:
			suit_str = "Clubs";
			break;
		case spades:
			suit_str = "Spades";
			break;
		default:
			suit_str = "Error, no suit value";
		}

		std::string value_str;

		switch (value) {
		case jack:
			value_str = "Jack";
			break;
		case queen:
			value_str = "Queen";
			break;
		case king:
			value_str = "King";
			break;
		case ace:
			value_str = "Ace";
			break;
		default:
			// Convert int valule to string
			std::stringstream temp_str;
			temp_str << value;
			temp_str >> value_str;
			break;
		}

		return (value_str + " of " + suit_str);
	}
};

class Hand {
public:
	Card* cards;
	int max;
	int size = 0;

	Hand(int max_cards) {
		max = max_cards;
		cards = new Card[max];
	}

	void AddCard(Card card) {
		cards[size] = card;
		size++;
	}

	bool Contains(int suit, int value) {
		for (int i = 0; i < size; i++) {
			if (cards[size].GetSuit() == suit
				&& cards[size].GetValue() == value) {
				return true;
			}
		}
		return false;
	}

	bool Contains(int value) {
		for (int i = 0; i < size; i++) {
			if (cards[size].GetValue() == value) {
				return true;
			}
		}
		return false;
	}

	void ClearHand() {
		delete[] cards;
		cards = new Card[max];
		size = 0;
	}

	bool Remove(int suit, int value) {
		bool removed = false;
		for (int i = 0; i < size; i++) {
			if (cards[i].GetSuit() == suit
				&& cards[i].GetValue() == value) {
				removed = true;
				size--;
			}

			if (removed) {
				cards[i] = cards[i + 1];
			}
		}
		return removed;
	}

	bool Remove(Card card) {
		bool removed = false;
		for (int i = 0; i < size; i++) {
			if (card.Equals(cards[i])) {
				removed = true;
				size--;
			}

			if (removed) {
				cards[i] = cards[i + 1];
			}
		}
		return removed;
	}
};



class Deck {
	// Constants
	static const int MAX_CARDS = 52;
	static const int MAX_SUITS = 4;
	static const int MIN_VALUES = 2;
	static const int MAX_VALUES = 14;

	int next_card_index = 0;
	Card cards[MAX_CARDS];

public:
	Deck() {
		for (int i = 0; i < MAX_SUITS; i++) {
			for (int j = MIN_VALUES; j <= MAX_VALUES; j++) {
				Card newCard = Card(i, j);
				cards[i * (MAX_VALUES - MIN_VALUES) + (j - MIN_VALUES + i)] = newCard;
			}
		}
	}

	Deck(bool yes) {
		for (int i = 0; i < MAX_SUITS; i++) {
			for (int j = MIN_VALUES; j <= MAX_VALUES; j++) {
				Card newCard = Card(0, 8);
				cards[i * (MAX_VALUES - MIN_VALUES) + (j - MIN_VALUES + i)] = newCard;
			}
		}
	}

	Card GetNextCard() {
		Card result = cards[next_card_index];
		next_card_index++;
		if (next_card_index >= MAX_CARDS) {
			next_card_index = 0;
		}

		return result;
	}

	void Shuffle() {
		int first_index, second_index;
		Card temp_card;

		for (int count = 0; count < 100; count++) {
			first_index = rand() % MAX_CARDS;
			second_index = rand() % MAX_CARDS;

			temp_card = cards[first_index];
			cards[first_index] = cards[second_index];
			cards[second_index] = temp_card;
		}
	}

	void ToString() {
		for (int i = 0; i < MAX_CARDS; i++) {
			cards[i].ToString();
		}
	}
};