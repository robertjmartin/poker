
#ifndef _HANDRANK_H_
#define _HANDRANK_H_

#include "card.h"
#include <list>
#include <algorithm>

using namespace std;

class HandRank
{
public:
	static unsigned int GetHandRank(int handsize, Card c1, Card c2, Card c3, Card c4, Card c5);
	static unsigned int GetHandRank(int handsize, Card c1, Card c2, Card c3, Card c4, Card c5, Card c6, Card c7);
	static unsigned int GetHandRank(list<Card> cards);

private:
	static void SetRank( unsigned char rank, unsigned int& HandRank );
	static void SetHighV( unsigned char value, unsigned int& HandRank );
	static void SetLowV( unsigned char value, unsigned int& HandRank );
	static void SetKickers(list<Card> cards, unsigned int& HandRank );
	static void SetKick1( unsigned char value, unsigned int& HandRank );
	static void SetKick2( unsigned char value, unsigned int& HandRank );
	static void SetKick3( unsigned char value, unsigned int& HandRank );

	static bool CheckStraightFlush(list<Card> cards, unsigned int& HandRank );
	static bool CheckFourOfAKind(list<Card> cards, unsigned int& HandRank );
	static bool CheckFullHouse(list<Card> cards, unsigned int& HandRank );
	static bool CheckFlush(list<Card> cards, unsigned int& HandRank );
	static bool CheckStraight(list<Card> cards, unsigned int& HandRank );
	static bool CheckThreeOfAKind(list<Card> cards, unsigned int& HandRank );
	static bool CheckTwoPair(list<Card> cards, unsigned int& HandRank );
	static bool CheckPair(list<Card>, unsigned int& HandRank);
	static void SetHighCard(list<Card> cards, unsigned int& HandRank );

};

#endif
