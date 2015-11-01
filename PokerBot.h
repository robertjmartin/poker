
#ifndef _POKERBOT_H_
#define _POKERBOT_H_

#include "PokerPlayer.h"
#include "card.h"

class PokerBot : public PokerPlayer
{
public:
	PokerBot(unsigned int chipcount);
	int Action(unsigned int mypot, unsigned int bet);
	void NotifyHandStart(unsigned int dealerposition);
	void NotifyPlayerInfo(unsigned int position, bool you, string name, unsigned int chipStackSize);
	void NotifyAction(unsigned int position, int action);
	void NotifyCard(cardtype type, unsigned int position, Card* c);
	void NotifyWinner(unsigned int position, unsigned int potsize);
	int StillInGame();

private:

};



#endif
