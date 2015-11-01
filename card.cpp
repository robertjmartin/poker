
#include "card.h"

suit Card::GetSuit()
{
	return _suit;
}

string Card::GetSuitString()
{
	switch(_suit)
	{
	case Spades:
		return "Spades";
	case Clubs:
		return "Clubs";
	case Hearts:
		return "Hearts";
	case Diamonds:
		return "Diamonds";
	default:
		return "Error";
	}
}

int Card::GetValue()
{
	return _value;
}