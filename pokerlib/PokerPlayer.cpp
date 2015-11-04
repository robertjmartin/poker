
#include "PokerPlayer.h"

unsigned int PokerPlayer::_nextPlayerNameId;

PokerPlayer::PokerPlayer(unsigned int chipcount)
{
	_chipcount = chipcount;
	_hand[0] = NULL;
	_hand[1] = NULL;
	_myposition = 99; 
    
	char buf[32];
	sprintf_s(buf, "Player%d", ++PokerPlayer::_nextPlayerNameId);
	_name = buf;
}

PokerPlayer::PokerPlayer(unsigned int chipcount, string name)
{
	_chipcount = chipcount;
	_hand[0] = NULL;
	_hand[1] = NULL;
	_myposition = 99; 
	_name = name;
}

unsigned int PokerPlayer::GetChipCount()
{
	return _chipcount;
}

string PokerPlayer::GetName()
{
	return _name;
}

void PokerPlayer::DealCard(Card* C)
{
	if( _hand[0] == NULL )
		_hand[0] = C;
	else if( _hand[1] == NULL )
		_hand[1] = C;
	else
		throw new exception();

	NotifyCard( HiddenHandCard, _myposition, C);
}

Card* PokerPlayer::CollectCard()
{
	if( _hand[0] != NULL )
	{
		Card* c = _hand[0];
		_hand[0] = NULL;
		return c;
	}
	else if(_hand[1] != NULL )
	{
		Card* c = _hand[1];
		_hand[1] = NULL;
		return c;
	}
	else 
		return NULL;
}

int PokerPlayer::GetAction(unsigned int mypot, unsigned int minbet)
{
	unsigned int action = Action(mypot, minbet);

	if( action == -1 || mypot + action < minbet )
		return -1;

	if( action > _chipcount )
		action = _chipcount;

	_chipcount -= action;
	return action;
}

unsigned int PokerPlayer::CollectBlind(unsigned int x)
{
	if( _chipcount >= x )
	{
		_chipcount -= x;
		return x;
	}
	return 0;

}

