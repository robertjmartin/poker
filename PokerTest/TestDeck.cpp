
#include "TestDeck.h"

TestDeck::TestDeck()
{
	
}

void TestDeck::Shuffle(int count)
{
	while( !_cards.empty() )
	{
		Card* C = _cards.back();
		_cards.pop_back();
		delete C;
	}

	Card* BurnCard = new Card(1, Spades);

	_cards.push_back(_communitycard[4]);
	_cards.push_back(BurnCard);
	_cards.push_back(_communitycard[3]);
	_cards.push_back(BurnCard);	
	_cards.push_back(_communitycard[2]);
	_cards.push_back(_communitycard[1]);
	_cards.push_back(_communitycard[0]);
	_cards.push_back(BurnCard);

	int i = _dealerposition;

	do
	{
		_cards.push_back(_playercard2[i]);
		i--;
		if(i < 0) i = (_numberofplayers-1);
	} while( i != _dealerposition );

	do
	{
		_cards.push_back(_playercard1[i]);
		i--;
		if(i < 0) i = (_numberofplayers-1);
	} while( i != _dealerposition );

}

void TestDeck::SetNumberOfPlayers(int count)
{
	_numberofplayers = count;
}

void TestDeck::SetDealerPosition(int position)
{
	_dealerposition = position;
}

void TestDeck::SetPlayersCards(int position, Card* card1, Card* card2 )
{
	_playercard1[position] = card1;
	_playercard2[position] = card2;
}

void TestDeck::SetCommunityCard(int position, Card* card)
{
	_communitycard[position] = card;
}