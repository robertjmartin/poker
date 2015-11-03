

#include "PokerBot.h"

PokerBot::PokerBot(unsigned int chipcount) : PokerPlayer(chipcount)
{
}

int PokerBot::Action(unsigned int mypot, unsigned int bet)
{
//	if( bet < 100 )
		return bet-mypot;
//	else
	//	return -1;
}

void PokerBot::NotifyHandStart(unsigned int dealerposition)
{
}

void PokerBot::NotifyPlayerInfo(unsigned int position, bool you, string name, unsigned int chipStackSize)
{
}

void PokerBot::NotifyAction(unsigned int position, int action)
{	
}

void PokerBot::NotifyCard(cardtype type, unsigned int position, Card* c)
{
}

void PokerBot::NotifyWinner(unsigned int position, unsigned int potsize)
{
}

int PokerBot::StillInGame()
{
	return true;
}