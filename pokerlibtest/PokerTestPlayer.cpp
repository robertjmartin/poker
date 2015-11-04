#include "stdafx.h"
#include "PokerTestPlayer.h"
#include "HoldEmTable.h"
#include <cstdarg>

PokerTestPlayer::PokerTestPlayer(unsigned int chipcount) : PokerPlayer(chipcount)
{
	_currentdealer=MAX_SEATS;
	_lastaction=MAX_SEATS;
	for( int i=0; i< 10; i++)
	{
		_positioninplay[i] = false;
	}
}

int PokerTestPlayer::Action(unsigned int mypot, unsigned int bet)
{
	// verify we are actually in hand
	if( _currentdealer == MAX_SEATS )
		throw new exception();

	do
	{   if(++_lastaction == MAX_SEATS)
			_lastaction = 0;
	} while(_positioninplay[_lastaction] == false);

	// check if i really should be next action
	if( _lastaction != _myposition )
		throw new exception();

	if( _actionlist.size() == 0 )
		throw new exception();

	int x = _actionlist.front();
	_actionlist.pop();
	return x;
}

void PokerTestPlayer::NotifyHandStart(unsigned int dealerposition)
{
	_currentdealer=dealerposition;
}

void PokerTestPlayer::NotifyPlayerInfo(unsigned int position, bool you, string name, unsigned int chipStackSize)
{
	_positioninplay[position] = true;
}

void PokerTestPlayer::NotifyAction(unsigned int position, int action)
{
	_lastaction = position;

	if( action == -1 )
		_positioninplay[position] = false; // BUGBUG also if all in
}

void PokerTestPlayer::NotifyCard(cardtype type, unsigned int position, Card* c)
{	
}

void PokerTestPlayer::NotifyWinner(unsigned int position, unsigned int potsize)
{
	_currentdealer = MAX_SEATS;
	
	for( int i=0; i< 10; i++)
	{
		_positioninplay[i] = false;
	}
}

void PokerTestPlayer::QueueActions( int action, ... )
{
	va_list arguments;
	va_start(arguments, action); 

	_actionlist.push(action);

	while(1)
	{
		int a = va_arg(arguments, int);
		if( a == -2 )
			return;
		_actionlist.push(a);
	}
}

bool PokerTestPlayer::ActionListEmpty()
{
	return( _actionlist.empty());
}

int PokerTestPlayer::StillInGame()
{
	return 1;
}