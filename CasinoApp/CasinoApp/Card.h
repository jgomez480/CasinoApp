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

public:
	int suit;
	int value;

	Card() {
		suit = 0;
		value = 0;
	}

	Card(int suit, int value) {
		this->suit = suit;
		this->value = value;
	}



	void ToString() {

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
			std::stringstream temp_str;
			temp_str << value;
			temp_str >> value_str;
			break;
		}

		std::cout << value_str + " of " + suit_str << std::endl;
	}
};



class Deck {
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

	Card GetNextCard() {
		Card result = cards[next_card_index];
		next_card_index++;
		if (next_card_index > MAX_CARDS) {
			next_card_index = 0;
		}

		return result;
	}

	void Shuffle() {
		std::cout << "Shuffling..." << std::endl;

		int first_index, second_index;
		Card temp_card;

		for (int count = 0; count < 100; count++) {
			first_index = rand() % MAX_CARDS;
			second_index = rand() % MAX_CARDS;

			std::cout << "First " << first_index << " Second " << second_index << std::endl;

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