
#ifndef _POKERPLAYER_H_
#define _POKERPLAYER_H_

#include <string>
#include "deck.h"
using namespace std;

class PokerPlayer
{
public:	
	PokerPlayer(unsigned int chipcount);
	PokerPlayer(unsigned int chipcount, string name);
	virtual int Action(unsigned int mypot, unsigned int bet) = 0;
	virtual int StillInGame() = 0;
	virtual void NotifyHandStart(unsigned int dealerposition) = 0;
	virtual void NotifyPlayerInfo(unsigned int position, bool you, string name, unsigned int chipStackSize) = 0;
	virtual void NotifyAction(unsigned int seat, int action) = 0;
	virtual void NotifyCard(cardtype type, unsigned int position, Card* c) = 0;
	virtual void NotifyWinner(unsigned int position, unsigned int potsize) = 0;
	string GetName();
	unsigned int GetChipCount();	

	static unsigned int _nextPlayerNameId;

protected:
	unsigned int _myposition;

private:
	string _name;

	unsigned int _chipcount;
    Card* _hand[2];
	
	void DealCard(Card* C);
	unsigned int CollectBlind(unsigned int x);
	Card* CollectCard();
	int GetAction(unsigned int mypot, unsigned int bet);

	friend class HoldEmTable;

};



#endif
