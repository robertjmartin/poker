#include "TestBase.h"
#include "TableTests.h"
#include "TestDeck.h"
#include "HoldEmTable.h"
#include "PokerTestPlayer.h"

namespace TableTests
{
	int CreateTableTest()
	{
		HoldEmTable T(1);
		T.tablelog << "-----CreateTableTest" << endl;
		return 1;
	}

	int AddPlayerTest()
	{
		HoldEmTable T(1);
		T.tablelog << "-----AddPlayerTest" << endl;
		// Add Player returns MAX_SEATS on error
		if( MAX_SEATS == T.AddPlayer( new PokerTestPlayer(1000)))
			return 0;
	
		return( 1 == T.GetNumberOfPlayers());
	}

	int FullTableTest()
	{
		HoldEmTable T(1);
		T.tablelog << "-----FullTableTest" << endl;

		for( int i=0; i<MAX_SEATS; i++ )
		{
			if( MAX_SEATS == T.AddPlayer( new PokerTestPlayer(1000)))
				return 0;
		}

		// full table next add should fail
		return MAX_SEATS == T.AddPlayer( new PokerTestPlayer(1000));

	}

	int PlayHandEmptyTable()
	{
		HoldEmTable T(1);
		T.tablelog << "-----PlayHandEmptyTable" << endl;
		T.playHand();
		return 1;
	}

	int PlayHandOnePlayer()
	{
		HoldEmTable T(1);
		T.tablelog << "-----PlayHandOnePlayer" << endl;
		if( MAX_SEATS == T.AddPlayer( new PokerTestPlayer(1000)))
			return 0;
		T.playHand();
		return 1;
	}

	int SingleHandTwoPlayerCheck()
	{
		HoldEmTable T(1);
		T.tablelog << "-----SingleHandTwoPlayerCheck" << endl;
		PokerTestPlayer* Joe = new PokerTestPlayer(1000);
		PokerTestPlayer* Mike = new PokerTestPlayer(1000);
		
		T.AddPlayer(Joe);
		T.AddPlayer(Mike);
		
		// joe is small blind
		Joe->QueueActions( 10, 0, 0, 0, -2);
		// mike is big blind
		Mike->QueueActions( 0, 0, 0, 0, -2);

		T.playHand();

		if(!Joe->ActionListEmpty() || !Mike->ActionListEmpty())
		{
			T.tablelog << "--Test Failed Due to Extra Action Never Used" << endl;
			return false;
		}
		
		//verify still 2000 chips in play
		unsigned int totalchips = 0;
		for(int i=0; i < MAX_SEATS; i++)
			totalchips += T.GetChipCount(i);
		return (2000 == totalchips);
	}

	int SingleHandThreePlayerCheck()
	{
		HoldEmTable T(1);
		T.tablelog << "-----SingleHandThreePlayerCheck" << endl;
		PokerTestPlayer* Joe = new PokerTestPlayer(1000);
		PokerTestPlayer* Mike = new PokerTestPlayer(1000);
		PokerTestPlayer* Tom = new PokerTestPlayer(1000);
				
		T.AddPlayer(Tom); //dealer
		T.AddPlayer(Mike);
		T.AddPlayer(Joe);

		// joe is big blind, just checks
		Joe->QueueActions( 0, 0, 0, 0, -2);
		// mike is small blind, calls then checks
		Mike->QueueActions( 10, 0, 0, 0, -2);
		// tom calls then cheks
		Tom->QueueActions( 20, 0, 0, 0, -2);

		T.playHand();
		
		//verify still 3000 chips in play
		unsigned int totalchips = 0;
		for(int i=0; i < MAX_SEATS; i++)
			totalchips += T.GetChipCount(i);
		return (3000 == totalchips);
	}

	int SingleHandTwoPlayerBet()
	{
		HoldEmTable T(1);
		T.tablelog << "-----SingleHandTwoPlayerBet Test" << endl;
		PokerTestPlayer* Joe = new PokerTestPlayer(1000);
		PokerTestPlayer* Mike = new PokerTestPlayer(1000);

		T.AddPlayer(Joe);
		T.AddPlayer(Mike);

		// joe is big blind, just checks
		Joe->QueueActions( 0, 20, 20, 20, -2);
		// mike is small blind, calls then checks
		Mike->QueueActions( 10, 20, 20, 20, -2);

		T.playHand();
		
		//verify still 2000 chips in play
		unsigned int totalchips = 0;
		for(int i=0; i < MAX_SEATS; i++)
			totalchips += T.GetChipCount(i);
		return (2000 == totalchips);
	}
	
	int SingleHand_TwoPlayer_SeatTwo_PairWin()
	{
		TestDeck deck;

		deck.SetNumberOfPlayers(2);
		deck.SetDealerPosition(0);
		deck.SetPlayersCards(0, new Card(2, Spades), new Card(7, Hearts));
		deck.SetPlayersCards(1, new Card(3, Spades), new Card(10, Spades));
		deck.SetCommunityCard(0, new Card(3, Hearts));
		deck.SetCommunityCard(1, new Card(4, Hearts));
		deck.SetCommunityCard(2, new Card(11, Spades));
		deck.SetCommunityCard(3, new Card(12, Clubs));
		deck.SetCommunityCard(4, new Card(5, Clubs));
				
		HoldEmTable T(1, &deck);
		T.tablelog << "-----SingleHand_TwoPlayer_SeatTwo_PairWin" << endl;
		PokerTestPlayer* Joe = new PokerTestPlayer(1000);
		PokerTestPlayer* Mike = new PokerTestPlayer(1000);

		T.AddPlayer(Joe);
		T.AddPlayer(Mike);

		// joe is small blind, checks and calls
		Joe->QueueActions( 10, 20, 20, 20, -2);
		// mike is big blind, checks then bets
		Mike->QueueActions( 0, 20, 20, 20, -2);

		T.playHand();

		// verify Mike has 1160, Joe has 840
		if( T.GetChipCount(0) != 920 || T.GetChipCount(1) != 1080)
			return false;
		return true;
	}

	int SingleHand_ThreePlayer_SeatTwo_PairWin()
	{
		TestDeck deck;

		deck.SetNumberOfPlayers(3);
		deck.SetDealerPosition(0);
		deck.SetPlayersCards(0, new Card(2, Spades), new Card(7, Hearts));
		deck.SetPlayersCards(1, new Card(3, Spades), new Card(10, Spades));
		deck.SetPlayersCards(2, new Card(6, Spades), new Card(13, Spades));
		deck.SetCommunityCard(0, new Card(3, Hearts));
		deck.SetCommunityCard(1, new Card(4, Hearts));
		deck.SetCommunityCard(2, new Card(11, Spades));
		deck.SetCommunityCard(3, new Card(12, Clubs));
		deck.SetCommunityCard(4, new Card(5, Clubs));
				
		HoldEmTable T(1, &deck);
		T.tablelog << "-----SingleHand_ThreePlayer_SeatTwo_PairWin" << endl;
		PokerTestPlayer* Joe = new PokerTestPlayer(1000);
		PokerTestPlayer* Mike = new PokerTestPlayer(1000);
		PokerTestPlayer* Tom = new PokerTestPlayer(1000);

		T.AddPlayer(Joe);
		T.AddPlayer(Mike);
		T.AddPlayer(Tom);

		Joe->QueueActions( 20, 20, 20, 20, -2);
		Mike->QueueActions( 10, 20, 20, 20, -2);
		Tom->QueueActions( 0, 20, 20, 20, -2);

		T.playHand();

		// verify Mike has 1160, Joe has 840
		if( T.GetChipCount(0) != 920 || T.GetChipCount(1) != 1160)
			return false;
		return true;
	}

	int SingleHand_ThreePlayer_SeatTwo_TwoPairWin()
	{
		TestDeck deck;

		deck.SetNumberOfPlayers(3);
		deck.SetDealerPosition(0);
		deck.SetPlayersCards(0, new Card(2, Spades), new Card(7, Hearts));
		deck.SetPlayersCards(1, new Card(3, Spades), new Card(11, Hearts));
		deck.SetPlayersCards(2, new Card(6, Spades), new Card(12, Spades));
		deck.SetCommunityCard(0, new Card(3, Hearts));
		deck.SetCommunityCard(1, new Card(4, Hearts));
		deck.SetCommunityCard(2, new Card(11, Spades));
		deck.SetCommunityCard(3, new Card(12, Clubs));
		deck.SetCommunityCard(4, new Card(5, Clubs));
				
		HoldEmTable T(1, &deck);
		T.tablelog << "-----SingleHand_ThreePlayer_SeatTwo_PairWin" << endl;
		PokerTestPlayer* Joe = new PokerTestPlayer(1000);
		PokerTestPlayer* Mike = new PokerTestPlayer(1000);
		PokerTestPlayer* Tom = new PokerTestPlayer(1000);

		T.AddPlayer(Joe);
		T.AddPlayer(Mike);
		T.AddPlayer(Tom);

		Joe->QueueActions( 20, 20, 20, 20, -2);
		Mike->QueueActions( 10, 20, 20, 20, -2);
		Tom->QueueActions( 0, 20, 20, 20, -2);

		T.playHand();

		// verify Mike has 1160, Joe has 840
		if( T.GetChipCount(0) != 920 || T.GetChipCount(1) != 1160)
			return false;
		return true;
	}

	int SingleHand_ThreePlayer_SeatTwo_ThreeOfAKindWin()
	{
		TestDeck deck;

		deck.SetNumberOfPlayers(3);
		deck.SetDealerPosition(0);
		deck.SetPlayersCards(0, new Card(2, Spades), new Card(7, Hearts));
		deck.SetPlayersCards(1, new Card(3, Spades), new Card(11, Hearts));
		deck.SetPlayersCards(2, new Card(4, Spades), new Card(12, Spades));
		deck.SetCommunityCard(0, new Card(12, Hearts));
		deck.SetCommunityCard(1, new Card(4, Hearts));
		deck.SetCommunityCard(2, new Card(11, Spades));
		deck.SetCommunityCard(3, new Card(11, Clubs));
		deck.SetCommunityCard(4, new Card(5, Clubs));
				
		HoldEmTable T(1, &deck);
		T.tablelog << "-----SingleHand_ThreePlayer_SeatTwo_PairWin" << endl;
		PokerTestPlayer* Joe = new PokerTestPlayer(1000);
		PokerTestPlayer* Mike = new PokerTestPlayer(1000);
		PokerTestPlayer* Tom = new PokerTestPlayer(1000);

		T.AddPlayer(Joe);
		T.AddPlayer(Mike);
		T.AddPlayer(Tom);

		Joe->QueueActions( 20, 20, 20, 20, -2);
		Mike->QueueActions( 10, 20, 20, 20, -2);
		Tom->QueueActions( 0, 20, 20, 20, -2);

		T.playHand();

		// verify Mike has 1160, Joe has 840
		if( T.GetChipCount(0) != 920 || T.GetChipCount(1) != 1160)
			return false;
		return true;
	}


	void RegisterTests(list<TestCase> &TestList )
	{
		TestList.push_back(TestCase("Create Holdem Table Test", CreateTableTest));
		TestList.push_back(TestCase("Add Player Test", AddPlayerTest));
		TestList.push_back(TestCase("Full Table Test", FullTableTest));
		TestList.push_back(TestCase("Play Hand Empty Table", PlayHandEmptyTable));
		TestList.push_back(TestCase("Play Hand One Player", PlayHandOnePlayer));
		TestList.push_back(TestCase("Single Hand Two Player Check", SingleHandTwoPlayerCheck));
		TestList.push_back(TestCase("Single Hand Three Player Check", SingleHandThreePlayerCheck));
		TestList.push_back(TestCase("Single Hand Two Player Bet", SingleHandTwoPlayerBet));
		TestList.push_back(TestCase("Single Hand Two Player Seat2 Pair Win", SingleHand_TwoPlayer_SeatTwo_PairWin));
		TestList.push_back(TestCase("Single Hand Three Player Seat2 Pair Win", SingleHand_ThreePlayer_SeatTwo_PairWin));
		TestList.push_back(TestCase("Single Hand Three Player Seat2 Two Pair Win", SingleHand_ThreePlayer_SeatTwo_TwoPairWin));
		TestList.push_back(TestCase("Single Hand Three Player Seat1 Three of A Kind Win", SingleHand_ThreePlayer_SeatTwo_ThreeOfAKindWin));
	}
}
