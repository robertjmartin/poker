
#ifndef _TCPCONNECTION_H_
#define _TCPCONNECTION_H_

using boost::asio::ip::tcp;

class tcp_server;
class PokerTcpPlayer;

class connection : public boost::enable_shared_from_this<connection>
{
public:

    connection(boost::asio::io_service& io_service, tcp_server* server);
    typedef boost::shared_ptr<connection> pointer;
    static pointer create(boost::asio::io_service& io_service, tcp_server* server);
    int send(const char* msg);
	int sendData( const void* data, unsigned int size);
    tcp::socket& socket();
    void start();

private:

    void handle_read( const::boost::system::error_code& e,
                      std::size_t bytes_transfered);

	boost::array<char,1024> buf_;

    tcp_server* _server;
    tcp::socket _socket;
	PokerTcpPlayer* _player;
};

#endif
