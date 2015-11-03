#ifndef _POKERCONSOLEPLAYER_H_
#define _POKERCONSOLEPLAYER_H_

#include "PokerPlayer.h"
#include "card.h"

class PokerConsolePlayer : public PokerPlayer
{
public:
	PokerConsolePlayer(unsigned int chipcount);
	int Action(unsigned int mypot, unsigned int bet);
	void NotifyHandStart(unsigned int dealerposition);
	void NotifyPlayerInfo(unsigned int position, bool you, string name, unsigned int chipStackSize);
	void NotifyAction(unsigned int position, int action);
	void NotifyCard(cardtype type, unsigned int position, Card* c);
	void NotifyWinner(unsigned int position, unsigned int potsize);
	int StillInGame();
};

#endif
