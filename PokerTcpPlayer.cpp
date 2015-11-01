
#include <boost/asio.hpp>
#include <boost/asio/system_timer.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/array.hpp>

#include <queue>

#include "HoldEmTable.h"

#include "connection.h"
#include "tcpServer.h"
#include "PokerTcpPlayer.h"
#include "PokerPlayer.h"
#include "card.h"

using namespace std;

PokerTcpPlayer::PokerTcpPlayer(unsigned int chipcount, string name, connection* conn) : PokerPlayer(chipcount, name)
{
	_conn = conn;
}

struct RAData
{
	unsigned char _tag; // 0xBA
	unsigned char size;
	int _action;
};

void PokerTcpPlayer::Disconnect()
{
	_conn = NULL;
	boost::lock_guard<boost::mutex> lock(_mut);
	if( _waitingForAction)
	{		
		_lastActionRecieved = -1;
		_waitingForAction = false;
		_cond.notify_all();
	}
}

void PokerTcpPlayer::HandleRecv(void*data, int size)
{
	if( *(unsigned char*)data == 0xBA )
	{
		boost::lock_guard<boost::mutex> lock(_mut);
		if( _waitingForAction)
		{
			RAData* RA = (RAData*)data;
			_lastActionRecieved = RA->_action;
			_waitingForAction = false;
			_cond.notify_all();
		}
	}
}

struct AData
{
	unsigned char _tag; //0xAA
	unsigned char size;
};

int PokerTcpPlayer::Action(unsigned int mypot, unsigned int bet)
{
	_lastActionRecieved = -1;
	_waitingForAction = true;

	if( _conn )
	{
		AData A;
		memset(&A, NULL, sizeof(A));
		A._tag = 0xAA;
		A.size = sizeof(A);
		_conn->sendData(&A, sizeof(A));

		boost::unique_lock<boost::mutex> lock(_mut);
		while(_waitingForAction)
		{
			_cond.wait(lock);
		}

		return _lastActionRecieved;
	}
	else
	{
		return -1;
	}
}

struct NHSData
{
	unsigned char _tag; //0xAB
	unsigned char size;
	unsigned int _dealerposition;
};

void PokerTcpPlayer::NotifyHandStart(unsigned int dealerposition)
{
	if(_conn)
	{
		NHSData NHS;
		memset(&NHS, NULL, sizeof(NHS));
		NHS._tag=0xAB;
		NHS.size = sizeof(NHS);
		NHS._dealerposition = dealerposition;
		_conn->sendData(&NHS, sizeof(NHS));
	}
}

struct NPIData
{
	unsigned char _tag;   //0xAC
	unsigned char size;
	unsigned char _you;  
	unsigned char _position;
	unsigned int  _chipStackSize;
	char _name[16];
};

void PokerTcpPlayer::NotifyPlayerInfo(unsigned int position, bool you, string name, unsigned int chipStackSize)
{
	if(_conn)
	{
		NPIData NPI;
		memset(&NPI, NULL, sizeof(NPI));
		NPI._tag = 0xAC;
		NPI.size = sizeof(NPI);
		NPI._you=(unsigned char)you;
		NPI._position = (unsigned char)position;
		NPI._chipStackSize = chipStackSize; 
		memcpy(NPI._name, name.c_str(), name.length());
		_conn->sendData(&NPI, sizeof(NPI));
	}
}

struct NAData
{
	unsigned char _tag;   //0xAD
	unsigned char size;
	unsigned char _position;
	int _action;
};

void PokerTcpPlayer::NotifyAction(unsigned int position, int action)
{	
	if(_conn)
	{
		NAData NA;
		memset(&NA, NULL, sizeof(NA));
		NA._tag = 0xAD;
		NA.size = sizeof(NA);
		NA._position = (unsigned char)position;
		NA._action = action;
		_conn->sendData(&NA, sizeof(NA));
	}
}

struct NCData
{
	unsigned char _tag; //0xAE
	unsigned char size;
	unsigned char _type;
	unsigned char _postion;
	unsigned char _suit;
	unsigned char _value;
};

void PokerTcpPlayer::NotifyCard(cardtype type, unsigned int position, Card* card)
{
	if(_conn)
	{
		NCData NC;
		memset(&NC, NULL, sizeof(NC));
		NC._tag = 0xAE;
		NC.size = sizeof(NC);
		NC._type = (unsigned char)type;
		NC._postion = (unsigned char)position;
		NC._suit = (unsigned char)card->GetSuit();
		NC._value = (unsigned char)card->GetValue();
		_conn->sendData(&NC, sizeof(NC));
	}
}

struct NWData
{
	unsigned char _tag; //0xAF
	unsigned char size;
	unsigned char _position;
	unsigned int _potsize;
};

void PokerTcpPlayer::NotifyWinner(unsigned int position, unsigned int potsize)
{
	if(_conn)
	{
		NWData NW;
		memset(&NW, NULL, sizeof(NW));
		NW._tag = 0xAF;
		NW.size = sizeof(NW);
		NW._position = (unsigned char)position;
		NW._potsize = potsize;
		_conn->sendData(&NW, sizeof(NW));
	}
}

int PokerTcpPlayer::StillInGame()
{
	if(_conn != NULL )
		return 1;
	else
		return 0;
}