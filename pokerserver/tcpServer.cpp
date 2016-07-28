#include <boost/asio.hpp>
#include <boost/asio/system_timer.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/array.hpp>
#include <queue>
#include <map>

#include "../pokerlib/HoldEmTable.h"

#include "connection.h"
#include "../pokerlib/PokerPlayer.h"
#include "PokerTcpPlayer.h"
#include "tcpserver.h"

using namespace std;

HoldEmTable tcp_server::Table(1);

tcp_server::tcp_server(boost::asio::io_service& io_service)
	: acceptor_(io_service, tcp::endpoint(tcp::v4(), 10080))
{
    connectioncount_ = 0;
    start_accept();
}

void tcp_server::start_accept()
{
    connection::pointer new_connection = connection::create(acceptor_.get_io_service(), this);

    acceptor_.async_accept(new_connection->socket(),
                           boost::bind(&tcp_server::handle_accept,
                           this,
                           new_connection,
                           boost::asio::placeholders::error));
}

void tcp_server::handle_accept(connection::pointer new_connection, const boost::system::error_code& error)
{
    if (!error)
    {
        connectioncount_++;
        new_connection->start();
    }

    start_accept();
}

void tcp_server::handle_disconnect(connection* conn)
{    
    connectioncount_--;
}

bool tcp_server::sendto(std::string name, const char* msg)
{
    map<std::string, connection*>::iterator it;
    std::string logmsg = "Send To[";
    logmsg += name + "]" + msg;
  
    if (name == "broadcast")
    {
        broadcast(msg);
        return true;
    }

    it = connections_.find(name);

    if (it != connections_.end())
    {
        (*it).second->send(msg);
        return true;
    }
    else
    {
        return false;
    }
}

void tcp_server::broadcast(const char* msg)
{
    map<std::string, connection*>::iterator it;
    for (it=connections_.begin(); it != connections_.end(); it++)
    {
        (*it).second->send(msg);
    }
}

unsigned int tcp_server::GetConnectionCount()
{
    //return connectioncount_;
    return connections_.size();
}
