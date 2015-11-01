
#ifndef _HANDRANK_H_
#define _HANDRANK_H_

#include "card.h"
#include <list>

using namespace std;

class HandRank
{
public:
	static unsigned int GetHandRank( int handsize, Card card1, ...);		

private:
	static void HandRank::SetRank( unsigned char rank, unsigned int& HandRank );
	static void HandRank::SetHighV( unsigned char value, unsigned int& HandRank );
	static void HandRank::SetLowV( unsigned char value, unsigned int& HandRank );
	static void HandRank::SetKickers(list<Card> cards, unsigned int& HandRank );
	static void HandRank::SetKick1( unsigned char value, unsigned int& HandRank );
	static void HandRank::SetKick2( unsigned char value, unsigned int& HandRank );
	static void HandRank::SetKick3( unsigned char value, unsigned int& HandRank );

	static bool HandRank::CheckStraightFlush(list<Card> cards, unsigned int& HandRank );
	static bool HandRank::CheckFourOfAKind(list<Card> cards, unsigned int& HandRank );
	static bool HandRank::CheckFullHouse(list<Card> cards, unsigned int& HandRank );
	static bool HandRank::CheckFlush(list<Card> cards, unsigned int& HandRank );
	static bool HandRank::CheckStraight(list<Card> cards, unsigned int& HandRank );
	static bool HandRank::CheckThreeOfAKind(list<Card> cards, unsigned int& HandRank );
	static bool HandRank::CheckTwoPair(list<Card> cards, unsigned int& HandRank );
	static bool HandRank::CheckPair(list<Card>, unsigned int& HandRank);
	static void HandRank::SetHighCard(list<Card> cards, unsigned int& HandRank );

};

#endif