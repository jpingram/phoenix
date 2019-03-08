#include <iostream>
#include <vector>
#include "card.h"
#include "deck.h"

Deck::Deck(unsigned short* s):fullStack(), activeStack(), reloadable(), cardsHeld(s[0]){
    try{
        short cardNum = s[0];

        //set cardsHeld (pulled cards plus reload card
        cardsHeld = cardNum+1;

        //every card is reload-able at the start (counts reload card)
        setReloadable(cardNum+1);

        //add cards from data array
        for(int i = 1; i < cardNum*3; i+=3){ //iterate through array
            Card newCard(s[i], s[i+1], s[i+2]);
            fullStack.insert(fullStack.begin(), newCard); //information comes three at a time
        }

        //regardless of size of array, place reload card at the end of the deck
        Card reloadCard(0, 0, 0);
        fullStack.insert(fullStack.begin(), reloadCard);

        //fill active stack with all cards on full stack
        for(unsigned int i = 0; i < fullStack.size(); i++){
            activeStack.push_back(&(fullStack[i]));
        }
    }catch(std::exception& e){
        std::cout << "Deck() Failed: " << e.what() << std::endl;
        //return -1;
    }

    //return 0;
};

void Deck::setReloadable(int n){
    //every card is reload-able at the start ('int n' counts reload card)
    for(int i = 0; i < n; i++){
        reloadable.push_back(true);
    }

    return;
};

std::vector<Card*> Deck::getStack(){
    return activeStack;
};

short Deck::getCardsHeld(){
    return cardsHeld;
};

void Deck::rotateLeft(){ //forward
    if(cardsHeld > 1){
        Card* temp = activeStack.back();
        activeStack.pop_back();
        activeStack.insert(activeStack.begin(), temp);
    }

    return;
};

void Deck::rotateRight(){ //backward
    if(cardsHeld > 1){
        Card* temp = activeStack[0];
        activeStack.erase(activeStack.begin());
        activeStack.push_back(temp);
    }

    return;
};

Card* Deck::throwCard(){
    cardsHeld--;
    Card* last = activeStack.back();
    activeStack.pop_back();

    return last;
};

Card* Deck::getLast(){
    return activeStack[0];
};

//return card second from the top of the active stack
Card* Deck::getNext(){
    if(cardsHeld >= 2){
        return activeStack.at(cardsHeld-2);
    }else
        return getLast();
};

void Deck::reload(){
    //clear activeStack first
    while(activeStack.size() != 0){
        activeStack.erase(activeStack.begin());
    }

    //reset cardsHeld
    cardsHeld = 0;

    //active stack with reload-able cards from full stack
    //NOTE: reload-able array is no longer in reverse order of full stack
    for(unsigned int i = 0; i < fullStack.size(); i++){
        if(reloadable[i] == true){
            activeStack.push_back(&(fullStack[i]));
            cardsHeld++;
        }
    }

    return;
};

//default empty deck (should be set up in deck with just reload card)
unsigned short emptyDeck[] = {0};

//sample test deck for use with Deck()
unsigned short deckOne[]  =
    {10,
    1, 9, 9,
    1, 8, 8,
    1, 7, 7,
    1, 6, 6,
    1, 5, 5,
    1, 4, 4,
    1, 3, 3,
    1, 2, 2,
    1, 1, 1,
    1, 0, 0};

//sample test deck for use with Deck()
unsigned short deckTwo[]  =
    {10,
    1, 0, 9,
    1, 0, 8,
    1, 0, 7,
    1, 0, 6,
    1, 0, 5,
    1, 0, 4,
    1, 0, 3,
    1, 0, 2,
    1, 0, 1,
    1, 0, 0};
