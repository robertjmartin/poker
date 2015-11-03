#include "stdafx.h"
#include "CppUnitTest.h"

#include "card.h"
#include "HandRank.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace pokerlibtest
{
	TEST_CLASS(DeckTest)
	{
	public:

		TEST_METHOD(HandRank_FiveCardHighCard)
		{
			Assert::AreEqual(
				0x00075432u,
				HandRank::GetHandRank(5, Card(7, Hearts), Card(4, Hearts), Card(5, Hearts), Card(3, Hearts), Card(2, Clubs)),
				L"Verify HandRank Five Card High Card");
		}

		TEST_METHOD(HandRank_SevenCardHighCard)
		{
			Assert::AreEqual(
				0x000BA754u,
				HandRank::GetHandRank(7, Card(10, Clubs), Card(11, Diamonds), Card(7, Hearts), Card(4, Hearts), Card(5, Hearts), Card(3, Hearts), Card(2, Clubs)),
				L"Verify HandRank Seven Card High Card");
		}

		TEST_METHOD(HandRank_HighCardCompare)
		{
			unsigned int a = HandRank::GetHandRank(5, Card(7, Hearts), Card(4, Hearts), Card(5, Hearts), Card(3, Hearts), Card(2, Clubs));
			unsigned int b = HandRank::GetHandRank(5, Card(7, Hearts), Card(4, Hearts), Card(5, Hearts), Card(3, Hearts), Card(10, Clubs));
			Assert::IsTrue(b > a, L"Verify two high card hands ranked correctly");
		}

		TEST_METHOD(HandRank_Pair)
		{
			Assert::AreEqual(
				0x00120754u,
				HandRank::GetHandRank(5, Card(7, Hearts), Card(2, Hearts), Card(4, Hearts), Card(5, Hearts), Card(2, Clubs)),
				L"Verify HandRank Five card hand pair");
		}

		TEST_METHOD(HandRank_PairBeatHighCard)
		{
			unsigned int a = HandRank::GetHandRank(5, Card(2, Hearts), Card(4, Hearts), Card(5, Hearts), Card(3, Hearts), Card(2, Clubs));
			unsigned int b = HandRank::GetHandRank(5, Card(7, Hearts), Card(4, Hearts), Card(5, Hearts), Card(3, Hearts), Card(10, Clubs));
			Assert::IsTrue(a > b, L"Verify pair beats high card");
		}

		TEST_METHOD(HandRank_StraightBeatHighCard)
		{
			unsigned int a = HandRank::GetHandRank(5, Card(2, Hearts), Card(3, Hearts), Card(4, Hearts), Card(5, Hearts), Card(6, Clubs));
			unsigned int b = HandRank::GetHandRank(5, Card(7, Hearts), Card(4, Hearts), Card(5, Hearts), Card(3, Hearts), Card(10, Clubs));
			Assert::IsTrue(a > b, L"Verify straight beats high card");
		}

		TEST_METHOD(HandRank_AceLowStraightBeatPair)
		{
			unsigned int a = HandRank::GetHandRank(5, Card(14, Hearts), Card(2, Hearts), Card(3, Hearts), Card(4, Hearts), Card(5, Clubs));
			unsigned int b = HandRank::GetHandRank(5, Card(7, Hearts), Card(4, Hearts), Card(4, Hearts), Card(3, Hearts), Card(10, Clubs));
			Assert::IsTrue(a > b, L"Verify ace low straight beats pair");
		}

		TEST_METHOD(HandRank_FlushBeatThreeOfAKind)
		{
			unsigned int a = HandRank::GetHandRank(5, Card(14, Hearts), Card(1, Hearts), Card(3, Hearts), Card(4, Hearts), Card(3, Hearts));
			unsigned int b = HandRank::GetHandRank(5, Card(7, Hearts), Card(7, Spades), Card(4, Spades), Card(7, Spades), Card(10, Clubs));
			Assert::IsTrue(a > b, L"Verify flush beats three of a kind");
		}


	};
}
