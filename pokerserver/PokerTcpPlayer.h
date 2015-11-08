#ifndef _POKERTCPPLAYER_H_
#define _POKERTCPPLAYER_H_

class PokerTcpPlayer : public PokerPlayer
{
public:
	PokerTcpPlayer(unsigned int chipcount, string name, connection* conn);
	int Action(unsigned int mypot, unsigned int bet);
	void NotifyHandStart(unsigned int dealerposition);
	void NotifyPlayerInfo(unsigned int position, bool you, string name, unsigned int chipStackSize);
	void NotifyAction(unsigned int position, int action);
	void NotifyCard(cardtype type, unsigned int position, Card* c);
	void NotifyWinner(unsigned int position, unsigned int potsize);
	int StillInGame();

	void HandleRecv(void*data, int size);
	void Disconnect();
private:
	connection* _conn;
	bool _waitingForAction;
	int _lastActionRecieved;
	boost::condition_variable _cond;
	boost::mutex _mut;
};

#endif