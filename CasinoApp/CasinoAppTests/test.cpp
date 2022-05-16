#include "pch.h"
#include "../CasinoApp/Card.h"

TEST(CardTest, CardInitalization) {
	Card new_card = Card(hearts, 2);
	EXPECT_EQ(new_card.GetValue(), 2);
	EXPECT_EQ(new_card.GetSuit(), 0);
	EXPECT_NE(new_card.ToString().find("2"), std::string::npos);
	EXPECT_NE(new_card.ToString().find("Hearts"), std::string::npos);

	new_card = Card(spades, 5);
	EXPECT_EQ(new_card.GetValue(), 5);
	EXPECT_EQ(new_card.GetSuit(), 3);
	EXPECT_NE(new_card.ToString().find("5"), std::string::npos);
	EXPECT_NE(new_card.ToString().find("Spades"), std::string::npos);

	new_card = Card(diamonds, ace);
	EXPECT_EQ(new_card.GetValue(), ace);
	EXPECT_EQ(new_card.GetSuit(), 1);
	EXPECT_NE(new_card.ToString().find("Ace"), std::string::npos);
	EXPECT_NE(new_card.ToString().find("Diamonds"), std::string::npos);

	new_card = Card(hearts, 8);
	EXPECT_EQ(new_card.GetValue(), 8);
	EXPECT_EQ(new_card.GetSuit(), 0);
	EXPECT_NE(new_card.ToString().find("8"), std::string::npos);
	EXPECT_NE(new_card.ToString().find("Hearts"), std::string::npos);

	new_card = Card(spades, 3);
	EXPECT_EQ(new_card.GetValue(), 3);
	EXPECT_EQ(new_card.GetSuit(), 3);
	EXPECT_NE(new_card.ToString().find("3"), std::string::npos);
	EXPECT_NE(new_card.ToString().find("Spades"), std::string::npos);

	new_card = Card(spades, 10);
	EXPECT_EQ(new_card.GetValue(), 10);
	EXPECT_EQ(new_card.GetSuit(), 3);
	EXPECT_NE(new_card.ToString().find("10"), std::string::npos);
	EXPECT_NE(new_card.ToString().find("Spades"), std::string::npos);

	new_card = Card(clubs, queen);
	EXPECT_EQ(new_card.GetValue(), queen);
	EXPECT_EQ(new_card.GetSuit(), 2);
	EXPECT_NE(new_card.ToString().find("Queen"), std::string::npos);
	EXPECT_NE(new_card.ToString().find("Clubs"), std::string::npos);

	new_card = Card(clubs, queen);
	EXPECT_NE(new_card.GetValue(), jack);
	EXPECT_NE(new_card.GetValue(), king);
	EXPECT_NE(new_card.GetValue(), ace);
	EXPECT_NE(new_card.GetValue(), 10);
	EXPECT_NE(new_card.GetSuit(), hearts);
	EXPECT_NE(new_card.GetSuit(), diamonds);
	EXPECT_NE(new_card.GetSuit(), spades);
	EXPECT_EQ(new_card.ToString().find("Jack"), std::string::npos);
	EXPECT_EQ(new_card.ToString().find("Hearts"), std::string::npos);

};

TEST(DeckTest, DeckInitalization) {
	Deck new_deck = Deck();
	Card temp_card;
	for (int suit = 0; suit < 4; suit++) {
		for (int value = 2; value < 15; value++) {
			temp_card = new_deck.GetNextCard();
			EXPECT_EQ(temp_card.GetValue(), value);
			EXPECT_EQ(temp_card.GetSuit(), suit);
		}
	}
};


TEST(DeckTest, DeckShuffle) {
	Deck new_deck = Deck();
	Card card_array[52];
	Card first_card, pulled_card;
	int index_count = 0;
	bool contained;

	new_deck.Shuffle();

	for (int suit = 0; suit < 4; suit++) {
		for (int value = 2; value < 15; value++) {
			card_array[index_count] = Card(suit, value);
			index_count++;
		}
	}

	first_card = new_deck.GetNextCard();
	pulled_card = new_deck.GetNextCard();

	while (!first_card.Equals(pulled_card)) {
		contained = false;
		std::cout << pulled_card.ToString() << std::endl;
		for (int i = 0; i < 52; i++) {
			if (card_array[i].Equals(pulled_card)) {
				contained = true;
			}
		}

		EXPECT_EQ(contained, true);
		pulled_card = new_deck.GetNextCard();
	}
};

TEST(HandTest, HandInitalization) {
	Hand new_hand = Hand(10);
	EXPECT_EQ(new_hand.size, 0);
	EXPECT_EQ(new_hand.max, 10);
	for (int i = 0; i < 10; i++) {
		EXPECT_EQ(new_hand.cards[i].GetSuit(), 0);
		EXPECT_EQ(new_hand.cards[i].GetValue(), 0);
	}

	new_hand = Hand(25);
	EXPECT_EQ(new_hand.size, 0);
	EXPECT_EQ(new_hand.max, 25);
	for (int i = 0; i < 25; i++) {
		EXPECT_EQ(new_hand.cards[i].GetSuit(), 0);
		EXPECT_EQ(new_hand.cards[i].GetValue(), 0);
	}

	new_hand = Hand(100);
	EXPECT_EQ(new_hand.size, 0);
	EXPECT_EQ(new_hand.max, 100);
	for (int i = 0; i < 100; i++) {
		EXPECT_EQ(new_hand.cards[i].GetSuit(), 0);
		EXPECT_EQ(new_hand.cards[i].GetValue(), 0);
	}
};
