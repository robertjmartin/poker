#include "stdafx.h"
#include "CppUnitTest.h"

#include "TestDeck.h"
#include "HoldEmTable.h"
#include "PokerTestPlayer.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace pokerlibtest
{
	TEST_CLASS(TableTest)
	{
	public:

		TEST_METHOD(Table_CreateTable)
		{
			HoldEmTable T(1);
			Assert::AreEqual(0u, T.GetNumberOfPlayers(), L"Verify table created");
		}

		TEST_METHOD(Table_AddPlayerTest)
		{
			HoldEmTable T(1);
			T.tablelog << "-----AddPlayerTest" << endl;
			// Add Player returns MAX_SEATS on error
			if (MAX_SEATS == T.AddPlayer(new PokerTestPlayer(1000)))
				Assert::Fail(L"AddPlayer failed");

			Assert::AreEqual(1u, T.GetNumberOfPlayers(), L"Verify player added");
		}

		TEST_METHOD(Table_FullTableTest)
		{
			HoldEmTable T(1);
			T.tablelog << "-----FullTableTest" << endl;

			for (int i = 0; i<MAX_SEATS; i++)
			{
				if (MAX_SEATS == T.AddPlayer(new PokerTestPlayer(1000)))
					Assert::Fail(L"AddPlayer failed");
			}

			// full table next add should fail
			Assert::AreEqual(MAX_SEATS, T.AddPlayer(new PokerTestPlayer(1000)));
		}

		TEST_METHOD(Table_PlayHandEmptyTable)
		{
			HoldEmTable T(1);
			T.tablelog << "-----PlayHandEmptyTable" << endl;
			T.playHand();
			Assert::AreEqual(0u, T.GetNumberOfPlayers(), L"Verify playhand on empty table");
		}

		TEST_METHOD(Table_PlayHandOnePlayer)
		{
			HoldEmTable T(1);
			T.tablelog << "-----PlayHandOnePlayer" << endl;
			Assert::AreNotEqual(MAX_SEATS, T.AddPlayer(new PokerTestPlayer(1000)), L"AddPlayer failed");
		
			T.playHand();
			Assert::AreEqual(1u, T.GetNumberOfPlayers(), L"Verify table after playing hand with 1 player");
		}

		TEST_METHOD(Table_SingleHandTwoPlayerCheck)
		{
			HoldEmTable T(1);
			T.tablelog << "-----SingleHandTwoPlayerCheck" << endl;
			PokerTestPlayer* Joe = new PokerTestPlayer(1000);
			PokerTestPlayer* Mike = new PokerTestPlayer(1000);

			T.AddPlayer(Joe);
			T.AddPlayer(Mike);

			// joe is small blind
			Joe->QueueActions(10, 0, 0, 0, -2);
			// mike is big blind
			Mike->QueueActions(0, 0, 0, 0, -2);

			T.playHand();

			if (!Joe->ActionListEmpty() || !Mike->ActionListEmpty())
			{
				T.tablelog << "--Test Failed Due to Extra Action Never Used" << endl;
				Assert::Fail(L"Test Failed Due to Extra Action Never Used");
			}

			//verify still 2000 chips in play
			unsigned int totalchips = 0;
			for (int i = 0; i < MAX_SEATS; i++)
				totalchips += T.GetChipCount(i);

			Assert::AreEqual(2000u, totalchips, L"Verify 2000 chips in play");
		}

		TEST_METHOD(Table_SingleHandThreePlayerCheck)
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
			Joe->QueueActions(0, 0, 0, 0, -2);
			// mike is small blind, calls then checks
			Mike->QueueActions(10, 0, 0, 0, -2);
			// tom calls then cheks
			Tom->QueueActions(20, 0, 0, 0, -2);

			T.playHand();

			//verify still 3000 chips in play
			unsigned int totalchips = 0;
			for (int i = 0; i < MAX_SEATS; i++)
				totalchips += T.GetChipCount(i);
			Assert::AreEqual(3000u, totalchips, L"Verify 3000 chips in play");
		}

		TEST_METHOD(Table_SingleHandTwoPlayerBet)
		{
			HoldEmTable T(1);
			T.tablelog << "-----SingleHandTwoPlayerBet Test" << endl;
			PokerTestPlayer* Joe = new PokerTestPlayer(1000);
			PokerTestPlayer* Mike = new PokerTestPlayer(1000);

			T.AddPlayer(Joe);
			T.AddPlayer(Mike);

			// joe is big blind, just checks
			Joe->QueueActions(0, 20, 20, 20, -2);
			// mike is small blind, calls then checks
			Mike->QueueActions(10, 20, 20, 20, -2);

			T.playHand();

			//verify still 2000 chips in play
			unsigned int totalchips = 0;
			for (int i = 0; i < MAX_SEATS; i++)
				totalchips += T.GetChipCount(i);
			Assert::AreEqual (2000u, totalchips, L"Verify 2000 chips in play");
		}

		TEST_METHOD(Table_SingleHand_TwoPlayer_SeatTwo_PairWin)
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
			Joe->QueueActions(10, 20, 20, 20, -2);
			// mike is big blind, checks then bets
			Mike->QueueActions(0, 20, 20, 20, -2);

			T.playHand();

			Assert::AreEqual(920u, T.GetChipCount(0), L"Player 0 should have 920");
			Assert::AreEqual(1080u, T.GetChipCount(1), L"Player 1 should have 1080");
		}

		TEST_METHOD(Table_SingleHand_ThreePlayer_SeatTwo_PairWin)
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

			Joe->QueueActions(20, 20, 20, 20, -2);
			Mike->QueueActions(10, 20, 20, 20, -2);
			Tom->QueueActions(0, 20, 20, 20, -2);

			T.playHand();

			Assert::AreEqual(920u, T.GetChipCount(0), L"Player 0 should have 920");
			Assert::AreEqual(1160u, T.GetChipCount(1), L"Player 1 should have 1160");		
		}

		TEST_METHOD(Table_SingleHand_ThreePlayer_SeatTwo_TwoPairWin)
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

			Joe->QueueActions(20, 20, 20, 20, -2);
			Mike->QueueActions(10, 20, 20, 20, -2);
			Tom->QueueActions(0, 20, 20, 20, -2);

			T.playHand();

			Assert::AreEqual(920u, T.GetChipCount(0), L"Player 0 should have 920");
			Assert::AreEqual(1160u, T.GetChipCount(1), L"Player 1 should have 1160");			
		}

		TEST_METHOD(Table_SingleHand_ThreePlayer_SeatTwo_ThreeOfAKindWin)
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

			Joe->QueueActions(20, 20, 20, 20, -2);
			Mike->QueueActions(10, 20, 20, 20, -2);
			Tom->QueueActions(0, 20, 20, 20, -2);

			T.playHand();

			Assert::AreEqual(920u, T.GetChipCount(0), L"Player 0 should have 920");
			Assert::AreEqual(1160u, T.GetChipCount(1), L"Player 1 should have 1160");			
		}		

		TEST_METHOD(Table_SingleHand_ThreePlayer_SeatTwo_FlushWin)
		{
			TestDeck deck;

			deck.SetNumberOfPlayers(3);
			deck.SetDealerPosition(0);
			deck.SetPlayersCards(0, new Card(2, Spades), new Card(7, Hearts));
			deck.SetPlayersCards(1, new Card(3, Hearts), new Card(11, Hearts));
			deck.SetPlayersCards(2, new Card(4, Spades), new Card(12, Spades));
			deck.SetCommunityCard(0, new Card(12, Hearts));
			deck.SetCommunityCard(1, new Card(4, Hearts));
			deck.SetCommunityCard(2, new Card(11, Spades));
			deck.SetCommunityCard(3, new Card(11, Clubs));
			deck.SetCommunityCard(4, new Card(5, Hearts));

			HoldEmTable T(1, &deck);
			T.tablelog << "-----SingleHand_ThreePlayer_SeatTwo_PairWin" << endl;
			PokerTestPlayer* Joe = new PokerTestPlayer(1000);
			PokerTestPlayer* Mike = new PokerTestPlayer(1000);
			PokerTestPlayer* Tom = new PokerTestPlayer(1000);

			T.AddPlayer(Joe);
			T.AddPlayer(Mike);
			T.AddPlayer(Tom);

			Joe->QueueActions(20, 20, 20, 20, -2);
			Mike->QueueActions(10, 20, 20, 20, -2);
			Tom->QueueActions(0, 20, 20, 20, -2);

			T.playHand();

			Assert::AreEqual(920u, T.GetChipCount(0), L"Player 0 should have 920");
			Assert::AreEqual(1160u, T.GetChipCount(1), L"Player 1 should have 1160");
		}
	};
}