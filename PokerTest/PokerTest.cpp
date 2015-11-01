// PokerTest.cpp : Defines the entry point for the console application.
//


#include <iostream>
#include "TestBase.h"

#include "HoldEmTable.h"
#include "PokerBot.h"
#include "PokerConsolePlayer.h"

#include "HandRankTests.h"
#include "DeckTest.h"
#include "TableTests.h"


using namespace std;

void RunTests( list<TestCase> testlist, int &pass, int &fail);

int main()
{
	int pass=0;
	int fail=0;

	system("del tablelog.txt");

	cout << "Poker Test App" << endl;
	cout << "----------------------------------" << endl;
	
	list<TestCase> TestList;
	
	DeckTests::RegisterTests(TestList);
	HandRankTests::RegisterTests(TestList);
	TableTests::RegisterTests(TestList);

	RunTests(TestList, pass, fail);
	
	double passrate = 100.0 * ((float)pass / (float)(pass+fail));

	cout << endl << "Test Summary" << endl;
	cout << "-----------------------------------" << endl;
	cout << "Pass Rate " << passrate << "%   (" << pass << "/" << pass+fail << ")" << endl;
	cout << "-----------------------------------" << endl;
		
	return 0;
}

void RunTests( list<TestCase> testlist, int &pass, int &fail)
{	
	for( list<TestCase>::iterator test=testlist.begin(); test !=testlist.end(); ++test )
	{
		int result = 0;
		try
		{
			Logger.Log(2, "--------------------------------------------------------------------");
			Logger.Log(2, "Starting test" );
			result = test->TestMethod();
		}
		catch(...)
		{
			Logger.Log(2, "Failed due to exception");
			result = 0;			
		}

		if( result == 1 )
		{
			cout << "PASS: " << test->TestName << endl;
			pass++;
		}
		else
		{
			cout << "FAIL: " << test->TestName << endl;
			fail++;
		}
	}
}




