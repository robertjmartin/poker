
#ifndef _DECK_H_
#define _DECK_H_

#include<vector>
#include "card.h"
#include "rand.h"

using namespace std;

class Deck
{
public:
   Deck();
   virtual void Shuffle(int count);
   int GetSize();
   Card* DrawCard();
   void AddCards(vector<Card*>&);
  
protected:
   vector<Card*> _cards;

};

#endif
