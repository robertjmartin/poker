
#include <iostream>
#include "PokerConsolePlayer.h"

using namespace std;

PokerConsolePlayer::PokerConsolePlayer(unsigned int chipcount) : PokerPlayer(chipcount)
{
}

int PokerConsolePlayer::Action(unsigned int mypot, unsigned int bet)
{
	int newbet;
	cout << "Your Chipcount: " << GetChipCount() << endl;
	cout << "Your Bet (-1 to fold):";
	cin >> newbet;
	return newbet;
}

void PokerConsolePlayer::NotifyHandStart(unsigned int dealerposition)
{
}

void PokerConsolePlayer::NotifyPlayerInfo(unsigned int position, bool you, string name, unsigned int chipStackSize)
{
}

void PokerConsolePlayer::NotifyAction(unsigned int position, int action)
{
	cout << "Notify: Seat " << position << " has bet " << action << endl;
}

void PokerConsolePlayer::NotifyCard(cardtype type, unsigned int position, Card* card)
{
	cout << "Dealt Card: " << card->GetValue() << " of " << card->GetSuitString() << endl;
}

void PokerConsolePlayer::NotifyWinner(unsigned int position, unsigned int potsize)
{
}

int PokerConsolePlayer::StillInGame()
{
	return 1;
}