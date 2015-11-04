
#ifndef _HOLDEMTABLE_H_
#define _HOLDEMTABLE_H_

#include "PokerPlayer.h"
#include "deck.h"
#include "HandRank.h"
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

#define MAX_SEATS 10u

class HoldEmTable
{
public:
	HoldEmTable(int TableID);
	HoldEmTable(int TableID, Deck* deck);
	void Init(int TableID);
	unsigned int AddPlayer(PokerPlayer* newPlayer);
	void playHand();
	int PotSize();
	unsigned int GetNumberOfPlayers();
	unsigned int GetChipCount(unsigned int seat);

	const static unsigned int MaxSeats = MAX_SEATS;

private:
	void NotifyAllHandStart();
	void NotifyAllChipStack();
	void NotifyAllCommunityCard(Card* card);
	void NotifyAllAction(unsigned int position, int action);
	void NotifyAllWinner(unsigned int position, unsigned int size);
	
	PokerPlayer* _seat[MAX_SEATS];
	int _pot[MAX_SEATS];
	int _option;
	Deck* _deck;
	vector<Card*> _communitycards;
	vector<Card*> _discard;
	unsigned int _dealerposition;
	unsigned int _actionto;
	unsigned int _currentblind;

	unsigned int GetNextPosition(unsigned int x);
	unsigned int GetNextActivePosition(unsigned int x);
	unsigned int GetNextEmptyPosition(unsigned int x);
	unsigned int GetLastToAct(unsigned int firstpos);
	void BootPlayer(int s);
	bool BettingComplete();

	void CollectBlindsAndDeal();
	void CollectBets();
	void DealCommunityCards(int x);
	void DistributePot();

public:
	ofstream tablelog;
};

#endif
