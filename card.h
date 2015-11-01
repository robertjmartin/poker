
#ifndef _CARD_H_
#define _CARD_H_

#include <string>

enum suit:unsigned int{ Spades, Clubs, Hearts, Diamonds };
enum cardtype: unsigned int{ CommunityCard, HiddenHandCard, VisibleHandCard };
using namespace std;

class Card
{
public:
	Card(unsigned int v, suit s)
	{
	if( v > 14 || s > Diamonds )
		throw new exception();
	
	_value = v;
	_suit = s;
	}
	
	Card(const Card& c)
	{
		_value = c._value;
		_suit = c._suit;
	}

    int GetValue();
	string GetValueString();
    suit GetSuit();
	string GetSuitString();
	
	bool operator< (const Card& rhs) const
	{
		return _value < rhs._value;
	}

	bool operator> (const Card& rhs) const
	{
		return _value > rhs._value;
	}

	private:
	suit _suit;
	unsigned int _value;
};



#endif