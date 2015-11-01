

#include "TestBase.h"
#include "HandRankTests.h"
#include "card.h"
#include "HandRank.h"

namespace HandRankTests
{
	int FiveCardHighCardTest()
	{
		return (0x00075432 == HandRank::GetHandRank(5, Card(7, Hearts), Card(4, Hearts), Card(5, Hearts), Card(3, Hearts), Card(2, Clubs)));
	}

	int SevenCardHighCardTest()
	{
		return (0x000BA754 == HandRank::GetHandRank(7, Card(10, Clubs), Card(11, Diamonds), Card(7, Hearts), Card(4, Hearts), Card(5, Hearts), Card(3, Hearts), Card(2, Clubs)));
	}

	int HighCardCompareTest()
	{
		unsigned int a = HandRank::GetHandRank(5, Card(7, Hearts), Card(4, Hearts), Card(5, Hearts), Card(3, Hearts), Card(2, Clubs));
		unsigned int b = HandRank::GetHandRank(5, Card(7, Hearts), Card(4, Hearts), Card(5, Hearts), Card(3, Hearts), Card(10, Clubs));
		return (b > a);
	}

	int PairTest()
	{
		return (0x00120754 == HandRank::GetHandRank(5, Card(7, Hearts), Card(2, Hearts), Card(4, Hearts), Card(5, Hearts), Card(2, Clubs)));
	}

	int PairBeatHighCardTest()
	{
		unsigned int a = HandRank::GetHandRank(5, Card(2, Hearts), Card(4, Hearts), Card(5, Hearts), Card(3, Hearts), Card(2, Clubs));
		unsigned int b = HandRank::GetHandRank(5, Card(7, Hearts), Card(4, Hearts), Card(5, Hearts), Card(3, Hearts), Card(10, Clubs));
		return (a > b);
	}
	
	int StraightBeatHighCardTest()
	{
		unsigned int a = HandRank::GetHandRank(5, Card(2, Hearts), Card(3, Hearts), Card(4, Hearts), Card(5, Hearts), Card(6, Clubs));
		unsigned int b = HandRank::GetHandRank(5, Card(7, Hearts), Card(4, Hearts), Card(5, Hearts), Card(3, Hearts), Card(10, Clubs));
		return (a > b);
	}

	int AceLowStraightBeatPairTest()
	{
		unsigned int a = HandRank::GetHandRank(5, Card(14, Hearts), Card(2, Hearts), Card(3, Hearts), Card(4, Hearts), Card(5, Clubs));
		unsigned int b = HandRank::GetHandRank(5, Card(7, Hearts), Card(4, Hearts), Card(4, Hearts), Card(3, Hearts), Card(10, Clubs));
		return (a > b);
	}

	int FlushBeatThreeOfAKindTest()
	{
		unsigned int a = HandRank::GetHandRank(5, Card(14, Hearts), Card(1, Hearts), Card(3, Hearts), Card(4, Hearts), Card(3, Hearts));
		unsigned int b = HandRank::GetHandRank(5, Card(7, Hearts), Card(7, Spades), Card(4, Spades), Card(7, Spades), Card(10, Clubs));
		return (a > b);
	}
		
	void RegisterTests(list<TestCase> &TestList )
	{
		TestList.push_back(TestCase("Five Card High Card Test", FiveCardHighCardTest));
		TestList.push_back(TestCase("Seven Card High Card Test", SevenCardHighCardTest));
		TestList.push_back(TestCase("High Card Compare Test ", HighCardCompareTest));
		TestList.push_back(TestCase("Pair Test", PairTest));
		TestList.push_back(TestCase("Pair Beat High Card Test", PairBeatHighCardTest));
		TestList.push_back(TestCase("StraightBeatHighCardTest", StraightBeatHighCardTest));
		TestList.push_back(TestCase("AceLowStraightBeatPairTest", AceLowStraightBeatPairTest));
		TestList.push_back(TestCase("FlushBeatThreeOfAKindTest", FlushBeatThreeOfAKindTest));
	}

}