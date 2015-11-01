
#include "deck.h"
#include <algorithm>

Deck::Deck()
{
	for(int s=0; s<=Diamonds; s++ )
	{
		for(int v=2; v<=14; v++ )
		{
			suit S = suit(s);
			
			try
			{
				Card* C = new Card(v,S);
				_cards.push_back(C);
			}
			catch(exception e)
			{
				_cards.empty();
				throw new exception();
			}			
		}
	}
}

void Deck::Shuffle(int count)
{
	for( int i = 0; i < count; i++)
		random_shuffle(_cards.begin(), _cards.end());
}

int Deck::GetSize()
{
	return _cards.size();
}

Card* Deck::DrawCard()
{
	if( _cards.empty() )
	{
		return NULL;
	}
	else
	{
		Card* c = _cards.back();
		_cards.pop_back();
		return c;
	}
}

void Deck::AddCards(vector<Card*>& Cards)
{
	while( !Cards.empty() )
	{
		Card* C = Cards.back(); 
		Cards.pop_back();
		_cards.push_back(C);
	}
}
