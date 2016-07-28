#ifndef _TCP_SERVER_H_
#define _TCP_SERVER_H_



class connection;

using boost::asio::ip::tcp;

class tcp_server
{
    public:
        tcp_server(boost::asio::io_service& io_service);
        void handle_disconnect(connection* conn);
        unsigned int GetConnectionCount();
        void broadcast(const char* msg);

		std::queue<PokerTcpPlayer*> newPlayers;
		static HoldEmTable Table; // todo replace with dyamic table generator and lobby

    private:
        void start_accept();
        void handle_accept(connection::pointer new_connection, const boost::system::error_code& error);

        tcp::acceptor acceptor_;
        std::map<std::string, connection*> connections_;
        unsigned int connectioncount_;
};

#endif