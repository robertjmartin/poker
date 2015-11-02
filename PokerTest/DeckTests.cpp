

#include "TestBase.h"
#include "DeckTest.h"
#include "deck.h"

namespace DeckTests
{
	int CreateDeckTest()
	{
		Deck D;
		return (D.GetSize() == 52);
	}

	int ShuffleTest()
	{
		Deck D;
		D.Shuffle(100);
		return (D.GetSize() == 52);
	}

	int DealCardTest()
	{
		Deck D;
		D.DrawCard();
		return (D.GetSize() == 51);
	}

	int DealAllCardsTest()
	{
		Deck D;
		while( D.DrawCard() != NULL );
		return 1;
	}
	
	void RegisterTests(list<TestCase> &TestList )
	{
		TestList.push_back(TestCase("Create Deck Test", CreateDeckTest));
		TestList.push_back(TestCase("Shuffle Test", ShuffleTest));
		TestList.push_back(TestCase("Deal Card Test", DealCardTest));
		TestList.push_back(TestCase("Deal All Cards Test", DealAllCardsTest));
	
	}

}

