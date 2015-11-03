#include "stdafx.h"
#include "CppUnitTest.h"

#include "deck.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace pokerlibtest
{		
	TEST_CLASS(DeckTest)
	{
	public:
		
		TEST_METHOD(CreateDeckTest)
		{
			Deck D;
			Assert::AreEqual(D.GetSize(), 52, L"Verify Deck is created");			
		}

		TEST_METHOD(ShuffleTest)
		{
			Deck D;
			D.Shuffle(100);
			Assert::AreEqual(D.GetSize(), 52, L"Verify deck valid after shuffling");
		}

		TEST_METHOD(DealCardTest)
		{
			Deck D;
			D.DrawCard();
			Assert::AreEqual(D.GetSize(), 51, L"Verify draw card from deck");
		}

		TEST_METHOD(DealAllCardTest)
		{
			Deck D;
			while (D.DrawCard() != NULL);
			Assert::AreEqual(D.GetSize(), 0, L"Verify draw all cards from deck");
		}
	};
}