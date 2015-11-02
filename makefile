
CFLAGS := -c -std=c++11 -I.

CORE_OBJECTS := card.o deck.o HandRank.o PokerBot.o PokerPlayer.o Rand.o HoldEmTable.o 
TEST_OBJECTS := PokerTest/PokerTest.o PokerTest/TestBase.o PokerTest/TestDeck.o PokerTest/PokerTestPlayer.o PokerTest/TestLogger.o PokerTest/HandRankTests.o PokerTest/DeckTests.o PokerTest/TableTests.o 

%.o: %.cpp
	g++ $(CFLAGS) -c $< -o $@

TEST: $(CORE_OBJECTS) $(TEST_OBJECTS)
	g++ $(CORE_OBJECTS) $(TEST_OBJECTS) -o pokertest
