
#include "HoldEmTable.h"
#include "HandRank.h"


HoldEmTable::HoldEmTable(int TableID) 
{
	_deck = new Deck();
	Init(TableID);
}

HoldEmTable::HoldEmTable(int TableID, Deck* deck)
{
	_deck = deck;
	Init(TableID);
}

void HoldEmTable::Init(int TableID)
{
	_dealerposition = MAX_SEATS;
	for(int i=0; i < MAX_SEATS; i++ )
	{
		_seat[i] = NULL;
		_pot[i] = NULL;
	}

	_currentblind=20;
	_option = -1;
	tablelog.open("tablelog.txt", ios::app );
	tablelog << "Init Table ID: " << TableID << endl;
}

unsigned int HoldEmTable::AddPlayer( PokerPlayer* newPlayer )
{
	int s = GetNextEmptyPosition(0);
	if( s != MAX_SEATS )
	{
		tablelog << "Add player at position " << s << endl;
		_seat[s] = newPlayer;
		_seat[s]->_myposition = s;
		if( _dealerposition == MAX_SEATS )
		{
			_dealerposition = s;
			tablelog << "Position " << s << " set at dealer" << endl;
		}

		NotifyAllChipStack();
	}
	return s;
}

unsigned int HoldEmTable::GetNumberOfPlayers()
{
	int count = 0;
	for(int i=0; i<MAX_SEATS; i++)
	{
		if( _seat[i] != NULL )
			count++;
	}
	return count;
}

unsigned int HoldEmTable::GetChipCount(unsigned int s)
{
	if( _seat[s] != NULL )
		return _seat[s]->GetChipCount();
	else
		return 0;
}

void HoldEmTable::playHand()
{
	tablelog << "playHand()" << endl;

	if( GetNumberOfPlayers() < 2 )
		return;

	NotifyAllHandStart();
	NotifyAllChipStack();
	
	_deck->AddCards(_discard);
	_deck->AddCards(_communitycards);

	_deck->Shuffle(7);
	_actionto=_dealerposition;

	CollectBlindsAndDeal();
	CollectBets();
	DealCommunityCards(3);
	_option = GetLastToAct(_actionto);
	CollectBets();
	DealCommunityCards(1);
	_option = GetLastToAct(_actionto);
	CollectBets();
	DealCommunityCards(1);
	_option = GetLastToAct(_actionto);
	CollectBets();

	DistributePot();

	for( int i = 0; i < MAX_SEATS; i++ )
	{
		if( _seat[i] != NULL && !_seat[i]->StillInGame() )
		{
			_seat[i] = NULL;
		}
	}
}

// todo everyonefolds, split pot, all-in, notif
void HoldEmTable::DistributePot()
{
	tablelog << "DistributePot()" << endl;
	
	unsigned int WinnerPos = MAX_SEATS;
	unsigned int WinnerRank = 0;

	if( _communitycards.size() != 5 )
		throw new exception();

	for(int i=0; i<MAX_SEATS; i++)
	{
		if( _seat[i] != NULL && _seat[i]->_hand[0] != NULL )
		{
			unsigned int HandRank = HandRank::GetHandRank(7, 
												 Card(*_seat[i]->_hand[0]),
												 Card(*_seat[i]->_hand[1] ),
												 Card(*_communitycards.at(0)),
												 Card(*_communitycards.at(1)),
												 Card(*_communitycards.at(2)),
												 Card(*_communitycards.at(3)),
												 Card(*_communitycards.at(4)));
			if( HandRank > WinnerRank )
			{
				WinnerPos = i;
				WinnerRank = HandRank;
			}
		}
	}

	int totalprize = 0;
	for(int i=0; i<MAX_SEATS; i++)
	{
		_seat[WinnerPos]->_chipcount += _pot[i];
		totalprize += _pot[i];
		_pot[i] = 0;
	}

	NotifyAllWinner(WinnerPos, totalprize);

	for( int i=0; i <MAX_SEATS; i++)
	{
		if( _seat[i] != NULL && _seat[i]->_hand[0] != NULL)
		{
			_discard.push_back(_seat[i]->CollectCard());
			_discard.push_back(_seat[i]->CollectCard());
		}
	}
}

void HoldEmTable::DealCommunityCards(int x)
{
	_discard.push_back(_deck->DrawCard()); //burn
	for(int i=0; i< x; i++ )
	{
		Card* c = _deck->DrawCard();
		_communitycards.push_back(c);
		tablelog << "Dealt Community Card " << c->GetValue() << " of " << c->GetSuitString() << endl;
		NotifyAllCommunityCard(c);
	}
}

void HoldEmTable::CollectBlindsAndDeal()
{
	int smblind = _currentblind / 2;
	int cardsdealtperplayer = 0;

	if( 2 != GetNumberOfPlayers() )
		_actionto = GetNextPosition(_dealerposition);

	_pot[_actionto] += _seat[_actionto]->CollectBlind(smblind);
	if( _pot[_actionto] == smblind )
	{
		tablelog << "Position " << _actionto << " paid small blind " << smblind << endl;
		NotifyAllAction(_actionto, smblind );
	}
	else
	{
		BootPlayer(_actionto);
	}

	_actionto = GetNextPosition(_actionto);

	_pot[_actionto] += _seat[_actionto]->CollectBlind(_currentblind);
	if( _pot[_actionto] == _currentblind )
	{
		tablelog << "Position " << _actionto << " paid big blind " << _currentblind << endl;
		NotifyAllAction(_actionto, _currentblind );
		_option = _actionto;
	}
	else
	{
		BootPlayer(_actionto);
	}

	_actionto = GetNextPosition(_actionto);

	unsigned int dealto = _dealerposition;
	do{ dealto = GetNextPosition(dealto);
		if( dealto == _dealerposition )
			cardsdealtperplayer++;

		Card* c = _deck->DrawCard();
		_seat[dealto]->DealCard(c);
		tablelog << "Dealt Card "  << c->GetValue() << " of " << c->GetSuitString() << " to position " << dealto << endl;
	} while( cardsdealtperplayer != 2 );
}

void HoldEmTable::CollectBets()
{
	int minbet = _currentblind;
	do 
	{
		if( _option == _actionto )
			_option = -1;

		int action = _seat[_actionto]->GetAction(_pot[_actionto], minbet);
		NotifyAllAction(_actionto, action);
		tablelog << "Position " << _actionto << " action: " << action << endl;
		if( action == -1 )
		{
			_discard.push_back(_seat[_actionto]->CollectCard());
			_discard.push_back(_seat[_actionto]->CollectCard());
		}
		else
		{
			_pot[_actionto] += action;
			minbet = _pot[_actionto];
		}

		_actionto = GetNextActivePosition(_actionto);

	} while( !BettingComplete() );

}

bool HoldEmTable::BettingComplete()
{
	if( _option != -1 )
		return false;

	int start = GetNextActivePosition(_dealerposition);
	int x = start;
	int bet = _pot[x];
	
	do
	{	x = GetNextActivePosition(x);
		if( bet != _pot[x] )
			return false;
		
	} while( x != start );

	return true;
}

void HoldEmTable::BootPlayer(int s)
{
	_seat[s] = NULL;
}

void HoldEmTable::NotifyAllHandStart()
{
	for(int i=0; i<MAX_SEATS; i++)
	{
		if(_seat[i] != NULL) 
			_seat[i]->NotifyHandStart(_dealerposition);
	}
}

void HoldEmTable::NotifyAllChipStack()
{
	for(int i=0; i<MAX_SEATS; i++)
	{		
		if(_seat[i] != NULL) 
		{
			for(int p=0; p<MAX_SEATS; p++)
			{
				if(_seat[p] != NULL)
					_seat[i]->NotifyPlayerInfo(p, p==i, _seat[p]->GetName(), _seat[p]->_chipcount);
			}
		}
	}
}

void HoldEmTable::NotifyAllCommunityCard(Card* card)
{
	for(int i=0; i<MAX_SEATS; i++)
	{
		if(_seat[i] != NULL) 
			_seat[i]->NotifyCard(CommunityCard, i, card);
	}
}


void HoldEmTable::NotifyAllWinner(unsigned int position, unsigned int size)
{
	for(int i=0; i<MAX_SEATS; i++)
	{
		if(_seat[i] != NULL) 
		{
			_seat[i]->NotifyCard(VisibleHandCard, position, _seat[position]->_hand[0]);
			_seat[i]->NotifyCard(VisibleHandCard, position, _seat[position]->_hand[1]);
			_seat[i]->NotifyWinner(position, size);
		}
	}
}

void HoldEmTable::NotifyAllAction(unsigned int position, int action)
{
	for(int i=0; i<MAX_SEATS; i++)
	{
		if(_seat[i] != NULL) 
			_seat[i]->NotifyAction(position, action);
	}
}

unsigned int HoldEmTable::GetNextPosition(unsigned int x)
{
	int start = x;
	do{ if(++x == MAX_SEATS)
			x = 0;
	    if( x == start )
			return MAX_SEATS;
	} while( _seat[x] == NULL );
	
	return x;
}

unsigned int HoldEmTable::GetNextActivePosition(unsigned int x)
{
	int start = x;
	do{ if(++x == MAX_SEATS)
			x = 0;
	} while( _seat[x] == NULL || _seat[x]->_hand[0] == NULL);
	
	return x;
}

unsigned int HoldEmTable::GetNextEmptyPosition(unsigned int x)
{
	int start = x;
	while( _seat[x] != NULL )
	{
		if(++x == MAX_SEATS)
			x = 0;
	    if( x == start )
			return MAX_SEATS;
	}

	return x;
}
unsigned int HoldEmTable::GetLastToAct(unsigned int firstpos)
{
	int x = firstpos;
	int y = GetNextActivePosition(x);

	while( y != firstpos)
	{
		x = y;
		y = GetNextActivePosition(y);
	}

	return x;
	
}