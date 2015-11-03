
#include "HandRank.h"
#include <stdarg.h>

/*

-------------------------------------------------------------------------
|8 bits |4 bits    |4 bits     | 4 bits   | 4 bits  | 4 bits  | 4 bits  |
|pad    |Hand Rank |High Value |Low Value | Kick1   | Kick2   | Kick3   |
-------------------------------------------------------------------------
uint HandRank


Hand Rank                     High Value              Low Value            Kick1                      Kick2                Kick3
0000 0 - High Card              Value of high card      Next highest card    Next highest card          Next highest card    Next highest card
0001 1 - Pair                   Value of pair           0                    High card not in pair      Next highest card    Next highest card
0010 2 - Two Pair               Value of highest pair   Value of low pair    Card not in pair           0                    0
0011 3 - Three of a Kind        Value of three          0                    High card not in three     Next highest card    0
0100 4 - Straight               Value of high card      0                    0                          0                    0 
0101 5 - Flush
0110 6 - Full House
0111 7 - Four of a Kind
1000 8 - Striaght Flush

Card Values
0010 2 2
0011 3 3
0100 4 4
0101 5 5
0110 6 6
0111 7 7
1000 8 8
1001 9 9
1010 A 10
1011 B J
1100 C Q
1101 D K
1110 E A

*/

const unsigned int RankMask =  0xFF0FFFFF;
const unsigned int HighVMask = 0xFFF0FFFF;
const unsigned int LowVMask =  0xFFFF0FFF;
const unsigned int Kick1Mask=  0xFFFFF0FF;
const unsigned int Kick2Mask = 0xFFFFFF0F;
const unsigned int Kick3Mask = 0xFFFFFFF0;

#define PAIR			0x0001
#define TWOPAIR			0x0002
#define THREEOFAKIND	0x0003
#define STRAIGHT		0x0004
#define FLUSH			0x0005
#define FULLHOUSE		0x0006
#define FOUROFAKIND		0x0007
#define STRAIGHTFLUSH	0x0008

void HandRank::SetRank( unsigned char rank, unsigned int& HandRank )
{
	rank &= 0x0F;
	HandRank &= RankMask;
	HandRank += (unsigned int)rank << 20;
}

void HandRank::SetHighV( unsigned char value, unsigned int& HandRank )
{
	value &= 0x0F;
	HandRank &= HighVMask;
	HandRank += (unsigned int)value << 16;
}

void HandRank::SetLowV( unsigned char value, unsigned int& HandRank )
{
	value &= 0x0F;
	HandRank &= LowVMask;
	HandRank += (unsigned int)value << 12;
}


void HandRank::SetKick1( unsigned char value, unsigned int& HandRank )
{
	value &= 0x0F;
	HandRank &= Kick1Mask;
	HandRank += (unsigned int)value << 8;
}

void HandRank::SetKick2( unsigned char value, unsigned int& HandRank )
{
	value &= 0x0F;
	HandRank &= Kick2Mask;
	HandRank += (unsigned int)value << 4;
}

void HandRank::SetKick3( unsigned char value, unsigned int& HandRank )
{
	value &= 0x0F;
	HandRank &= Kick3Mask;
	HandRank += (unsigned int)value;
}

bool HandRank::CheckStraightFlush(list<Card> cards, unsigned int& HandRank )
{
	return false;
}

bool HandRank::CheckFourOfAKind(list<Card> cards, unsigned int& HandRank )
{
	return false;
}

bool HandRank::CheckFullHouse(list<Card> cards, unsigned int& HandRank )
{
	return false;
}

bool HandRank::CheckFlush(list<Card> cards, unsigned int& HandRank )
{
	list<Card>::iterator it = cards.begin();
	int count[4] = {0,0,0,0};
	int flushsuit = -1;	

	while( it != cards.end())
	{
		suit s = it->GetSuit();
		count[(int)s]++;
		it++;
	}

	for( int i = 0; i < 4; i++ )
	{
		if( count[i] >= 5)
		{
			flushsuit = i;
			break;
		}
	}

	if( flushsuit == -1 )
		return false;
	else
	{
		SetRank(FLUSH, HandRank);

		list<Card> FlushCards;
		it = cards.begin();
		while( it != cards.end())
		{
			if( (int)it->GetSuit() == flushsuit )
			{
				FlushCards.push_back(Card(it->GetValue(), it->GetSuit()));
			}
			it++;
		}

		Card F = FlushCards.front();
		FlushCards.pop_front();
		SetHighV(F.GetValue(), HandRank);
		
		F = FlushCards.front();
		SetLowV(F.GetValue(), HandRank);

		SetKickers(FlushCards, HandRank);
		return true;
	}
}

bool HandRank::CheckStraight(list<Card> cards, unsigned int& HandRank )
{
	list<Card>::iterator it = cards.begin();

	int lastvalue = it->GetValue();
	int highcard = it->GetValue();
	int count = 1;
	bool Ace = false;

	if( it->GetValue() == 0x0E )
		Ace = true;
	
	it++;

	while( it != cards.end())
	{
		if( it->GetValue() == --lastvalue )
		{
			if( ++count == 5)
			{
				SetRank(STRAIGHT, HandRank);
				SetHighV(highcard, HandRank);
				return true;
			}			
		}
		else
		{
			lastvalue = it->GetValue();
			highcard = it->GetValue();
			count = 1;
		}
		it++;
	}

	if( count == 4 && lastvalue == 2 && Ace )
	{
		SetRank(STRAIGHT, HandRank);
		SetHighV(highcard, HandRank);
		return true;
	}

	return false;
}

bool HandRank::CheckThreeOfAKind(list<Card> cards, unsigned int& HandRank )
{
	list<Card>::iterator it = cards.begin();
	list<Card>::iterator next = cards.begin();
	list<Card>::iterator next2 = cards.begin();
	next++;
	next2++;next2++;
	
	while( next2 != cards.end() )
	{
		if( it->GetValue() == next->GetValue() && it->GetValue() == next2->GetValue() )
		{
			SetRank(THREEOFAKIND, HandRank);
			SetHighV(it->GetValue(), HandRank);
			cards.erase(it);
			cards.erase(next);
			cards.erase(next2);
			SetKickers(cards, HandRank);
			return true;
		}

		it++;
		next++;
		next2++;
	}

	return false;

}

bool HandRank::CheckTwoPair(list<Card> cards, unsigned int& HandRank )
{
	list<Card>::iterator it = cards.begin();
	list<Card>::iterator next = cards.begin();
	int pairsfound = 0;
	int values[2];

	next++;
	
	while( next != cards.end() )
	{
		if( it->GetValue() == next->GetValue() )
		{
			values[pairsfound] = it->GetValue();

			if(++pairsfound == 2)
			{
				SetRank(TWOPAIR, HandRank);
				SetHighV(max(values[0], values[1]), HandRank);
				SetLowV(min(values[0], values[1]), HandRank);
				cards.erase(it);
				cards.erase(next);
				SetKickers(cards, HandRank);
				return true;
			}
			it++;
			next++;

			if( next == cards.end() )
				return false;
		}

		it++;
		next++;
	}

	return false;
}

bool HandRank::CheckPair(list<Card> cards, unsigned int& HandRank )
{
	list<Card>::iterator it = cards.begin();
	list<Card>::iterator next = cards.begin();
	next++;
	
	while( next != cards.end() )
	{
		if( it->GetValue() == next->GetValue() )
		{
			SetRank(PAIR, HandRank);
			SetHighV(it->GetValue(), HandRank);
			cards.erase(it);
			cards.erase(next);
			SetKickers(cards, HandRank);
			return true;
		}

		it++;
		next++;
	}

	return false;
}

void HandRank::SetHighCard(list<Card> cards, unsigned int& HandRank )
{
	if( cards.size() < 5 )
		throw new exception();

	Card c = cards.front();
	cards.pop_front();
	SetHighV(c.GetValue(), HandRank);
	
	c = cards.front();
	cards.pop_front();
	SetLowV(c.GetValue(), HandRank);
	
	SetKickers(cards, HandRank);
}

void HandRank::SetKickers(list<Card> cards, unsigned int& HandRank )
{
	if( cards.empty() )
		return;

	Card c = cards.front();
	cards.pop_front();
	SetKick1(c.GetValue(), HandRank);

	if( cards.empty() )
		return;

	c = cards.front();
	cards.pop_front();
	SetKick2(c.GetValue(), HandRank);

	if( cards.empty() )
		return;

	c = cards.front();
	cards.pop_front();
	SetKick3(c.GetValue(), HandRank);
}

unsigned int HandRank::GetHandRank(int handsize, Card c1, Card c2, Card c3, Card c4, Card c5)
{
	list<Card> CardList;
	CardList.push_back(c1);
	CardList.push_back(c2);
	CardList.push_back(c3);
	CardList.push_back(c4);
	CardList.push_back(c5);
	return GetHandRank(CardList);
}


unsigned int HandRank::GetHandRank(int handsize, Card c1, Card c2, Card c3, Card c4, Card c5, Card c6, Card c7)
{
	list<Card> CardList;
	CardList.push_back(c1);
	CardList.push_back(c2);
	CardList.push_back(c3);
	CardList.push_back(c4);
	CardList.push_back(c5);
	CardList.push_back(c6);
	CardList.push_back(c7);
	return GetHandRank(CardList);
}

unsigned int HandRank::GetHandRank(list<Card> cards)
{
	unsigned int HandRank = 0;
	cards.sort();
	cards.reverse();

	if( CheckFlush(cards, HandRank)) return HandRank;
	if( CheckStraight(cards, HandRank)) return HandRank;
	if( CheckThreeOfAKind(cards, HandRank)) return HandRank;
	if( CheckTwoPair(cards, HandRank)) return HandRank;
	if( CheckPair(cards, HandRank) ) return HandRank;

	SetHighCard(cards, HandRank);
	return HandRank;
}

