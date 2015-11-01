
#ifndef _POKERTESTPLAYER_H_
#define _POKERTESTPLAYER_H_

#include "PokerPlayer.h"
#include "TestLogger.h"
#include <queue>

#define MAX_SEATS 10 // BUGBUG does this need to be passed?

class PokerTestPlayer : public PokerPlayer
{
public:
	PokerTestPlayer(unsigned int chipcount);
	int Action(unsigned int mypot, unsigned int bet);
	void NotifyHandStart(unsigned int dealerposition);
	void NotifyPlayerInfo(unsigned int position, bool you, string name, unsigned int chipStackSize);
	void NotifyAction(unsigned int position, int action);
	void NotifyCard(cardtype type, unsigned int position, Card* c);
	void NotifyWinner(unsigned int position, unsigned int potsize);
	int StillInGame();

	void QueueActions(int action, ...);
	bool ActionListEmpty();
private:
	queue<int> _actionlist;

	unsigned int _currentdealer;
	unsigned int _lastaction;
	bool _positioninplay[10];
	TestLogger* Logger;
	
};

#endif
