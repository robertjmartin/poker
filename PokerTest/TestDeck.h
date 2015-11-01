
#ifndef _TESTDECK_H_
#define _TESTDECK_H_

#include "deck.h"

class TestDeck : public Deck
{
public:
	TestDeck();
	void Shuffle(int count);

	void SetNumberOfPlayers(int count);
	void SetDealerPosition(int position);
	void SetPlayersCards(int position, Card* card1, Card* card2);
	void SetCommunityCard(int position, Card* card);

private:
	int _numberofplayers;
	int _dealerposition;
	Card* _playercard1[10];
	Card* _playercard2[10];
	Card* _communitycard[5];
};


#endif