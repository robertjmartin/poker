
#include <iostream>

#include <boost/asio.hpp>
#include <boost/asio/system_timer.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/thread.hpp>
#include <boost/array.hpp>

#include <queue>

#include "connection.h"
#include "../pokerlib/HoldEmTable.h"
#include "../pokerlib/PokerBot.h"
#include "PokerTcpPlayer.h"

#include "tcpServer.h"

using namespace std;

void TableManager(tcp_server* s)
{	
	while (1)
	{
		while (!s->newPlayers.empty())
		{
			PokerTcpPlayer* p = s->newPlayers.front();
			
			if (MAX_SEATS != tcp_server::Table.AddPlayer((PokerPlayer*)p))
			{
				s->newPlayers.pop();
			}
			else
			{
				break;
			}
		}	
				
		if (tcp_server::Table.GetNumberOfPlayers() >= 2)
		{
			tcp_server::Table.playHand();
			Sleep(3000);
		}
		else
		{
			Sleep(1000);
		}
	}
}

int main()
{	
    boost::asio::io_service io_service;
	tcp_server* s = new tcp_server(io_service);

	boost::thread workerThread(TableManager, s);

	s->Table.AddPlayer(new PokerBot(5000));
	s->Table.AddPlayer(new PokerBot(5000));
	s->Table.AddPlayer(new PokerBot(5000));
	s->Table.AddPlayer(new PokerBot(5000));
	s->Table.AddPlayer(new PokerBot(5000));

	io_service.run();
}