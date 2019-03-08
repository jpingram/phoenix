#include <vector>
#include "card.h"

#ifndef DECK
#define DECK

class Deck{
private:
    std::vector<Card> fullStack; //full list of cards
    std::vector<Card*> activeStack; //list of active cards: MAIN STACK USED BY PLAYER
    std::vector<bool> reloadable; //array denoting which cards in the fullStack should be reloaded on reload() call
    short cardsHeld; //for LEGACY purposes (UPDATE: actually useful for current setup)
public:
    Deck(unsigned short* s);

    void setReloadable(int n); //fills reload-able to size n, mapped to cards in a FullStack of size n, all set to true
    std::vector<Card*> getStack();
    short getCardsHeld();

    void rotateLeft(); //forward
    void rotateRight(); //backward

    Card* throwCard();
    Card* getLast();
    Card* getNext();

    void reload();
};

extern unsigned short emptyDeck[];

extern unsigned short deckOne[];

extern unsigned short deckTwo[];

#endif // DECK
