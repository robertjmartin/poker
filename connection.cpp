#include <boost/asio.hpp>
#include <boost/asio/system_timer.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/thread.hpp>
#include <boost/array.hpp>
#include <map>
#include <queue>
#include <iostream>

#include "HoldEmTable.h"
#include "connection.h"

#include "PokerTcpPlayer.h"
#include "tcpserver.h"
using namespace std;

connection::connection(boost::asio::io_service& io_service, tcp_server* server)
: _socket(io_service)
{	
    _server = server;
	_player = NULL;
}

connection::pointer connection::create(boost::asio::io_service& io_service, tcp_server* server)
{
    return pointer(new connection(io_service, server));
}

tcp::socket& connection::socket()
{
    return _socket;
}

void connection::start()
{   
    memset( buf_.c_array(), 0, 1024 );
    _socket.async_read_some(boost::asio::buffer(buf_),
                            boost::bind(&connection::handle_read, shared_from_this(),
                            boost::asio::placeholders::error, 
                            boost::asio::placeholders::bytes_transferred));
}

int connection::send( const char* msg )
{
    return _socket.write_some( boost::asio::buffer(msg, strlen(msg)) );
}

int connection::sendData( const void* data, unsigned int size)
{
	return _socket.write_some( boost::asio::buffer(data, size) );
}



void connection::handle_read( const::boost::system::error_code& e, std::size_t bytes_transfered)
{
    if(!e && bytes_transfered > 0)
    { 
        std::string data = buf_.c_array();

        // remove line ending (allows testing through PUTTY)
        size_t nl = data.find('\r');
        if( nl != std::string::npos )
        {
            data.erase(nl);
        }
		
		//cout << "recieved: " << data << endl;

		if( _player == NULL)
		{
			if(data.substr(0,5) == "SPReg")
			{
				_player = new PokerTcpPlayer(1000, data.substr(5, string::npos), this);
				this->_server->newPlayers.push(_player);
			}
		}
		else
		{
			_player->HandleRecv( buf_.c_array(), buf_.size());
		}
      
		// prepare for next read
        memset( buf_.c_array(), 0, 1024 );
        _socket.async_read_some(boost::asio::buffer(buf_),
                                boost::bind(&connection::handle_read, shared_from_this(),
                                    boost::asio::placeholders::error,
                                    boost::asio::placeholders::bytes_transferred));
    }
	else
	{
		this->_player->Disconnect();
	}
}


