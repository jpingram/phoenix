#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>
#include "card.h"
#include "deck.h"
#include "player.h"
#include "data.h"
#include "battleview.h"

BattleView::BattleView():p1(deckTwo), p2(deckThree), debugTextObjects(), debugRectObjects(),
        debugSetting(true), castSwitch(castStatuses::na){
    //set p2 to default position and x direction
    p2.moveRight(DEFAULT_PLAYER_TWO_X_OFFSET);
    p2.moveLeft(0); //sets p2 to face left

    //hold card information for the current cast
    cast = nullptr;

    //SET UP FONTS FOR DEBUG OVERLAY
    freeroadR.loadFromMemory(freeroadRegular, freeroadRegular_length);
    freeroadB.loadFromMemory(freeroadBold, freeroadBold_length);

    //SET UP DEBUG OBJECTS AND INSERT THEM IN DEBUG OBJECT VECTOR
    createDebugObjects();

    update();

    return;
};

//returns a pointer to the p1 object
Player* BattleView::getPlayer(){
    return &p1;
};

//sets debug setting boolean, if true, the debug overlay should be displayed
void BattleView::setDebugSetting(bool set){
    debugSetting = set;
};

//returns the status of the debug setting boolean
bool BattleView::getDebugSetting(){
    return debugSetting;
};

//returns drawable text objects for debug overlay
std::vector<sf::Text> BattleView::getDebugTextObjects(){
    return debugTextObjects;
};

//returns drawable rectangle shape objects for debug overlay
std::vector<sf::RectangleShape> BattleView::getDebugRectObjects(){
    return debugRectObjects;
};

//translates input from main to appropriate parameters for enterInput(Player& p, char c)
void BattleView::enterInput(char c){
    switch(c){
        case 'f':
        case 'b':
        case 'c':
        case 'v':
        case 'n':
        case 'l':
        case 'r':
        case 'u':
        case 'd': enterInput(p1, c); break;

        case '-': enterInput(p2, 'f'); break;
        case '*': enterInput(p2, 'b'); break;
        case 'i': enterInput(p2, 'c'); break;
        case 'o': enterInput(p2, 'v'); break;
        case 'p': enterInput(p2, 'n'); break;
        case '4': enterInput(p2, 'l'); break;
        case '6': enterInput(p2, 'r'); break;
        case '8': enterInput(p2, 'u'); break;
        case '5': enterInput(p2, 'd'); break;
    }
};

//takes in input (denoted by char c) and does the appropriate action for given Player
//adds character (taken from parameter given by enterInput(char c) to the beginning of the input string
void BattleView::enterInput(Player& p, char c){
    //INPUT CASES
    //f: "[" -> forward card cycle (p1)
    //b: "l" -> backward card cycle (p1)
    //c: " " -> cast (p1)
    switch(c){
        case 'f':
            //unless there's no status in effect where cards can't be moved, move cards
            if(p.getStatus() == Statuses::standby || p.getStatus() == Statuses::acting){
                    p.getDeck()->rotateLeft();
            }
            break;
        case 'b':
            //unless there's no status in effect where cards can't be moved, move cards
            if(p.getStatus() == Statuses::standby || p.getStatus() == Statuses::acting){
                    p.getDeck()->rotateRight();
            }
            break;
        case 'c': //SPACE pressed
            //if it is legal to update cast, update cast
            if(p.getStatus() != Statuses::reloading && p.getStatus() != Statuses::throwing && p.getStatus() != Statuses::acting
                    && !p.isStunned() && !p.isInRecoil()){
                p.act();
            }
            break;
        case 'v': //SPACE held
            //if it is legal to update cast for reloading, update cast
            if(p.getStatus() != Statuses::reloading && p.getStatus() != Statuses::throwing && p.getStatus() != Statuses::acting
                    && !p.isStunned() && !p.isInRecoil()){
                if(p.getStatus() == Statuses::focusing || p.getStatus() == Statuses::focusing2 ||
                p.getStatus() == Statuses::focusing3 || p.getStatus() == Statuses::focused){
                    p.act();
                }
            }
            break;
        case 'n': //SPACE released
            //cancel p1 focus if active
            if(p.getStatus() == Statuses::focusing || p.getStatus() == Statuses::focusing2 ||
            p.getStatus() == Statuses::focusing3){
                p.setStatus(Statuses::standby);
            }
            break;
        case 'l':
            //if it's legal to move left, move player left
            if(p.getStatus() != focusing && p.getStatus() != acting && !p.isStunned() && !p.isInRecoil()){
                p.moveLeft();
            }
            break;
        case 'r':
            //if it's legal to move left, move player right
            if(p.getStatus() != focusing && p.getStatus() != acting && !p.isStunned() && !p.isInRecoil()){
                p.moveRight();
            }
            break;
        case 'u':
            //if it's legal to move left, move player up
            if(p.getStatus() != focusing && p.getStatus() != acting && !p.isStunned() && !p.isInRecoil()){
                p.moveUp();
            }
            break;
        case 'd':
            //if it's legal to move left, move player down
            if(p.getStatus() != focusing && p.getStatus() != acting && !p.isStunned() && !p.isInRecoil()){
                p.moveDown();
            }
            break;
    }

    //regardless of what the input char is, add it to beginning of Player's input string
    p.prependInput(c);
};

//updates the cast display object based on actions currently being used as an Act by either Player
//NEEDS TO BE UPDATED SOON
void BattleView::updateCast(){
    //if cast is empty, update cast to current active cast
    if(castSwitch == castStatuses::na){
        //if cast is empty and a Player is acting, set cast to that act
        //      Player 1, in this system takes precedent
        if(cast == nullptr){
            if(p1.getStatus() == Statuses::acting){
                castSwitch = castStatuses::p1;
                cast = p1.getCurrentAct();
            }else
            if(p2.getStatus() == Statuses::acting){
                castSwitch = castStatuses::p2;
                cast = p2.getCurrentAct();
            }
        }
    }else{
        //if a player is acting and their act has 'hazard' status, do a hit check
        //      if the act hitbox contains defending player hitbox (and vice versa)
        //           defending player is hit by the act
        //if a player tries to act while another player is acting
        //      find newly acting player
        //          if value of new throw is greater than cast, card break
        //              cast status is set to failed
        //              formally casting player is now stunned
        //              throwing player act is now casting (cast will be set on next iteration of loop)
        //          if equal
        //              new throw and cast both fail
        //          if less
        //              throwing player's act fails
        //if a player is acting, and their act is over,
        //or if no player is acting,
        //      set cast switch to standby
        //      using the current act instead of statuses in case the player has a queue of acts
        if(castSwitch == castStatuses::p1){
            if(cast->getStatus() == actStatuses::hazard){
                if(cast->getHitbox()->contains(p2.getHitbox()) || p2.getHitbox()->contains(cast->getHitbox())){
                    cast->setStatus(actStatuses::hit);
                    p2.hit(cast->getPower());
                }
            }

            if(p2.getStatus() == Statuses::acting){
                if(p2.getCurrentAct()->getValue() > cast->getValue() || p2.getCurrentAct()->getValue() == 0){ //CARD BREAK
                    cast->setStatus(actStatuses::failed);
                    if(!p1.isStunned()){
                        p1.stun();
                    }
                }else //DOUBLE FAIL
                if(p2.getCurrentAct()->getValue() == cast->getValue()){
                    cast->setStatus(actStatuses::failed);
                    p2.getCurrentAct()->setStatus(actStatuses::failed);
                }else{ //THROW FAIL
                    p2.getCurrentAct()->setStatus(actStatuses::failed);
                }
            }

            if(p1.getCurrentAct() == nullptr){
                castSwitch = castStatuses::na;
                cast = nullptr;
            }
        }else
        if(castSwitch == castStatuses::p2){
            if(cast->getStatus() == actStatuses::hazard){
                if(cast->getHitbox()->contains(p1.getHitbox())|| p1.getHitbox()->contains(cast->getHitbox())){
                    cast->setStatus(actStatuses::hit);
                    p1.hit(cast->getPower());
                }
            }

            if(p1.getStatus() == Statuses::acting){
                if(p1.getCurrentAct()->getValue() > cast->getValue() || p1.getCurrentAct()->getValue() == 0){ //CARD BREAK
                    cast->setStatus(actStatuses::failed);
                    if(!p2.isStunned()){
                        p2.stun();
                    }
                }else //DOUBLE FAIL
                if(p1.getCurrentAct()->getValue() == cast->getValue()){
                    cast->setStatus(actStatuses::failed);
                    p1.getCurrentAct()->setStatus(actStatuses::failed);
                }else{ //THROW FAIL
                    p1.getCurrentAct()->setStatus(actStatuses::failed);
                }
            }

            if(p2.getCurrentAct() == nullptr){
                castSwitch = castStatuses::na;
                cast = nullptr;
            }
        }else
        //more of a safety case than anything
        //if no player is acting, reset castSwitch and cast
        if(p1.getCurrentAct() == nullptr && p2.getCurrentAct() == nullptr){
            castSwitch = castStatuses::na;
            cast = nullptr;
        }
    }
};

//update all drawable objects in the battle view,
void BattleView::update(){
    //UPDATE CAST OBJECT
    updateCast();

    //update p1 status
    p1.updateStatus();

    //update p2 status
    p2.updateStatus();

    //update debug overlay objects with the text values taken from Player, cast, and input data
    updateDebugObjects();
};

void BattleView::createDebugObjects(){
    //debugTextObjects[0]
    sf::Text prev;
    prev.setFillColor(yellowish);
    prev.setFont(freeroadR);
    prev.setCharacterSize(15);
    prev.setPosition(DEBUG_X_ZERO_POINT, DEBUG_Y_ZERO_POINT + 3*DEBUG_LINE_SPACING); //mostly just so i know where it is, REFERENCE POINT FOR ALL DEBUG OBJECTS
    debugTextObjects.push_back(prev);

    //debugTextObjects[1]
    sf::Text curr;
    curr.setFillColor(yellowish);
    curr.setFont(freeroadB);
    curr.setCharacterSize(15);
    curr.setPosition(DEBUG_X_ZERO_POINT, DEBUG_Y_ZERO_POINT + 4*DEBUG_LINE_SPACING);
    debugTextObjects.push_back(curr);

    //debugTextObjects[2]
    sf::Text next;
    next.setFillColor(yellowish);
    next.setFont(freeroadR);
    next.setCharacterSize(15);
    next.setPosition(DEBUG_X_ZERO_POINT, DEBUG_Y_ZERO_POINT + 5*DEBUG_LINE_SPACING);
    debugTextObjects.push_back(next);

    //debugTextObjects[3]
    sf::Text castText;
    castText.setFont(freeroadB);
    castText.setCharacterSize(15);
    castText.setPosition(DEBUG_X_ZERO_POINT + DEBUG_CAST_X_BUFFER, DEBUG_Y_ZERO_POINT + 4*DEBUG_LINE_SPACING);
    debugTextObjects.push_back(castText);

    //debugTextObjects[4]
    sf::Text inputText;
    inputText.setFont(freeroadR);
    inputText.setCharacterSize(15);
    inputText.setPosition(DEBUG_X_ZERO_POINT, DEBUG_Y_ZERO_POINT + 7*DEBUG_LINE_SPACING);
    debugTextObjects.push_back(inputText);

    //debugTextObjects[5]
    sf::Text p1StatusText;
    p1StatusText.setFont(freeroadB);
    p1StatusText.setCharacterSize(15);
    p1StatusText.setPosition(DEBUG_X_ZERO_POINT, DEBUG_Y_ZERO_POINT + 0*DEBUG_LINE_SPACING);
    debugTextObjects.push_back(p1StatusText);

    //debugTextObjects[6]
    sf::Text p1EventCounterText;
    p1EventCounterText.setFont(freeroadR);
    p1EventCounterText.setCharacterSize(15);
    p1EventCounterText.setPosition(DEBUG_X_ZERO_POINT, DEBUG_Y_ZERO_POINT + DEBUG_LINE_SPACING);
    debugTextObjects.push_back(p1EventCounterText);

    //debugTextObjects[7]
    sf::Text p1EventLimitText;
    p1EventLimitText.setFont(freeroadR);
    p1EventLimitText.setCharacterSize(15);
    p1EventLimitText.setPosition(DEBUG_X_ZERO_POINT, DEBUG_Y_ZERO_POINT + 2*DEBUG_LINE_SPACING);
    debugTextObjects.push_back(p1EventLimitText);

    //debugTextObjects[15]
    sf::Text p1StunCounterText;
    p1EventLimitText.setFont(freeroadR);
    p1EventLimitText.setCharacterSize(15);
    p1EventLimitText.setPosition(DEBUG_X_ZERO_POINT, DEBUG_Y_ZERO_POINT + 10*DEBUG_LINE_SPACING);
    debugTextObjects.push_back(p1EventLimitText);

    //debugTextObjects[9]
    sf::Text castSwitchText;
    castSwitchText.setFont(freeroadR);
    castSwitchText.setCharacterSize(15);
    castSwitchText.setPosition(DEBUG_X_ZERO_POINT + DEBUG_CAST_X_BUFFER, DEBUG_Y_ZERO_POINT + 3*DEBUG_LINE_SPACING);
    debugTextObjects.push_back(castSwitchText);

    //debugTextObjects[10]
    sf::Text prevTwo;
    prevTwo.setFillColor(greenish);
    prevTwo.setFont(freeroadR);
    prevTwo.setCharacterSize(15);
    prevTwo.setPosition(DEBUG_X_ZERO_POINT + DEBUG_P2_OFFSET, DEBUG_Y_ZERO_POINT + 3*DEBUG_LINE_SPACING); //mostly just so i know where it is, REFERENCE POINT FOR ALL DEBUG OBJECTS
    debugTextObjects.push_back(prevTwo);

    //debugTextObjects[11]
    sf::Text currTwo;
    currTwo.setFillColor(greenish);
    currTwo.setFont(freeroadB);
    currTwo.setCharacterSize(15);
    currTwo.setPosition(DEBUG_X_ZERO_POINT + DEBUG_P2_OFFSET, DEBUG_Y_ZERO_POINT + 4*DEBUG_LINE_SPACING);
    debugTextObjects.push_back(currTwo);

    //debugTextObjects[12]
    sf::Text nextTwo;
    nextTwo.setFillColor(greenish);
    nextTwo.setFont(freeroadR);
    nextTwo.setCharacterSize(15);
    nextTwo.setPosition(DEBUG_X_ZERO_POINT + DEBUG_P2_OFFSET, DEBUG_Y_ZERO_POINT + 5*DEBUG_LINE_SPACING);
    debugTextObjects.push_back(nextTwo);

    //debugTextObjects[13]
    //placed just below the first input text
    sf::Text inputTextTwo;
    inputTextTwo.setFont(freeroadR);
    inputTextTwo.setCharacterSize(15);
    inputTextTwo.setPosition(DEBUG_X_ZERO_POINT, DEBUG_Y_ZERO_POINT + 8*DEBUG_LINE_SPACING);
    debugTextObjects.push_back(inputTextTwo);

    //debugTextObjects[14]
    sf::Text p2StatusText;
    p2StatusText.setFont(freeroadB);
    p2StatusText.setCharacterSize(15);
    p2StatusText.setPosition(DEBUG_X_ZERO_POINT + DEBUG_P2_OFFSET, DEBUG_Y_ZERO_POINT + 0*DEBUG_LINE_SPACING);
    debugTextObjects.push_back(p2StatusText);

    //debugTextObjects[15]
    sf::Text p2EventCounterText;
    p2EventCounterText.setFont(freeroadR);
    p2EventCounterText.setCharacterSize(15);
    p2EventCounterText.setPosition(DEBUG_X_ZERO_POINT + DEBUG_P2_OFFSET, DEBUG_Y_ZERO_POINT + DEBUG_LINE_SPACING);
    debugTextObjects.push_back(p2EventCounterText);

    //debugTextObjects[16]
    sf::Text p2EventLimitText;
    p2EventLimitText.setFont(freeroadR);
    p2EventLimitText.setCharacterSize(15);
    p2EventLimitText.setPosition(DEBUG_X_ZERO_POINT + DEBUG_P2_OFFSET, DEBUG_Y_ZERO_POINT + 2*DEBUG_LINE_SPACING);
    debugTextObjects.push_back(p2EventLimitText);

    //debugTextObjects[17]
    sf::Text p2StunCounter;
    p2StunCounter.setFont(freeroadR);
    p2StunCounter.setCharacterSize(15);
    p2StunCounter.setPosition(DEBUG_X_ZERO_POINT + DEBUG_P2_OFFSET, DEBUG_Y_ZERO_POINT + 10*DEBUG_LINE_SPACING);
    debugTextObjects.push_back(p2StunCounter);

    //debugTextObjects[18]
    sf::Text p1RecoilCounter;
    p1RecoilCounter.setFont(freeroadR);
    p1RecoilCounter.setCharacterSize(15);
    p1RecoilCounter.setPosition(DEBUG_X_ZERO_POINT, DEBUG_Y_ZERO_POINT + 11*DEBUG_LINE_SPACING);
    debugTextObjects.push_back(p1RecoilCounter);

    //debugTextObjects[19]
    sf::Text p2RecoilCounter;
    p2RecoilCounter.setFont(freeroadR);
    p2RecoilCounter.setCharacterSize(15);
    p2RecoilCounter.setPosition(DEBUG_X_ZERO_POINT + DEBUG_P2_OFFSET, DEBUG_Y_ZERO_POINT + 11*DEBUG_LINE_SPACING);
    debugTextObjects.push_back(p2RecoilCounter);

    //debugTextObjects[20]
    sf::Text p1HPText;
    p1HPText.setFont(freeroadR);
    p1HPText.setCharacterSize(15);
    p1HPText.setPosition(DEBUG_X_ZERO_POINT, DEBUG_Y_ZERO_POINT + 9*DEBUG_LINE_SPACING);
    debugTextObjects.push_back(p1HPText);

    //debugTextObjects[21]
    sf::Text p2HPText;
    p2HPText.setFont(freeroadR);
    p2HPText.setCharacterSize(15);
    p2HPText.setPosition(DEBUG_X_ZERO_POINT + DEBUG_P2_OFFSET, DEBUG_Y_ZERO_POINT + 9*DEBUG_LINE_SPACING);
    debugTextObjects.push_back(p2HPText);

    //debugTextObjects[22]
    sf::Text p1FaceText;
    p1FaceText.setFont(freeroadR);
    p1FaceText.setCharacterSize(15);
    p1FaceText.setPosition(DEBUG_X_ZERO_POINT, DEBUG_Y_ZERO_POINT + 6*DEBUG_LINE_SPACING);
    debugTextObjects.push_back(p1FaceText);

    //debugTextObjects[23]
    sf::Text p2FaceText;
    p2FaceText.setFont(freeroadR);
    p2FaceText.setCharacterSize(15);
    p2FaceText.setPosition(DEBUG_X_ZERO_POINT + DEBUG_P2_OFFSET, DEBUG_Y_ZERO_POINT + 6*DEBUG_LINE_SPACING);
    debugTextObjects.push_back(p2FaceText);

    //debugRectObjects[0]
    sf::RectangleShape p1Hitbox;
    p1Hitbox.setSize(sf::Vector2f(p1.getHitbox()->getW(), p1.getHitbox()->getH()));
    p1Hitbox.setPosition(p1.getHitbox()->getX(), p1.getHitbox()->getY());
    p1Hitbox.setFillColor(yellowish); //yellowish
    debugRectObjects.push_back(p1Hitbox);

    //debugRectObjects[1]
    sf::RectangleShape p1LowHitbox;
    p1LowHitbox.setSize(sf::Vector2f(p1.getLowHitbox()->getW(), p1.getLowHitbox()->getH()));
    p1LowHitbox.setPosition(p1.getLowHitbox()->getX(), p1.getLowHitbox()->getY());
    p1LowHitbox.setFillColor(darkYellowish); //a slightly darker yellow
    debugRectObjects.push_back(p1LowHitbox);

    //debugRectObjects[2]
    sf::RectangleShape p2Hitbox;
    p2Hitbox.setSize(sf::Vector2f(p2.getHitbox()->getW(), p2.getHitbox()->getH()));
    p2Hitbox.setPosition(p2.getHitbox()->getX(), p2.getHitbox()->getY());
    p2Hitbox.setFillColor(greenish); //greenish
    debugRectObjects.push_back(p2Hitbox);

    //debugObjects[3]
    sf::RectangleShape p2LowHitbox;
    p2LowHitbox.setSize(sf::Vector2f(p2.getLowHitbox()->getW(), p2.getLowHitbox()->getH()));
    p2LowHitbox.setPosition(p2.getLowHitbox()->getX(), p2.getLowHitbox()->getY());
    p2LowHitbox.setFillColor(darkGreenish); //a slightly darker green
    debugRectObjects.push_back(p2LowHitbox);

    //debugRectObjects[4]
    sf::RectangleShape castHitbox;
    castHitbox.setFillColor(sf::Color::Red); //a slightly darker green
    debugRectObjects.push_back(castHitbox);
};

void BattleView::updateDebugObjects(){
    //if there are only 2 cards in the deck, only show the current and next card (not previous card/ bottom of stack)
    if(p1.getDeck()->getCardsHeld() >= 3){
        debugTextObjects[0].setString(p1.getDeck()->getLast()->getName());
    }else{
        debugTextObjects[0].setString("");
    }

    //show current card (there should always be at least the reload card but checks for at least one just in case)
    if(p1.getDeck()->getCardsHeld() >= 1){
        debugTextObjects[1].setString(p1.getDeck()->getStack().back()->getName());
    }else{
        debugTextObjects[1].setString("(empty)");
    }

    //show next card in stack if size is over 1
    if(p1.getDeck()->getCardsHeld() >= 2){
        debugTextObjects[2].setString(p1.getDeck()->getNext()->getName());
    }else{
        debugTextObjects[2].setString("");
    }

    //CAST
    //update cast but only if cast is available
    if(cast != nullptr){
        debugTextObjects[3].setString(cast->getName());
    }else{
        debugTextObjects[3].setString("");
    }

    //OTHER p1 DEBUG OBJECTS
    //update p1 input string
    debugTextObjects[4].setString(p1.getInputText());

    //update display of p1 status
    debugTextObjects[5].setString(p1.getStatusText());

    std::string buffer; //exists as an excess string dump
    std::stringstream ss; //exists to print numerical stats

    //display p1 event counter but only if p1 status is not standby)
    if(p1.getStatus() == Statuses::standby){
        debugTextObjects[6].setString("");
    }else{
        ss.str(std::string());
        ss << p1.getEventCounter();
        debugTextObjects[6].setString(ss.str());
    }

    //display p1 event counter time limit but only if p1 status is not standby)
    if(p1.getStatus() == Statuses::standby){
        debugTextObjects[7].setString("");
    }else{
        ss.str(std::string());
        ss << p1.getEventCounter() << "-" << p1.getEventTimeLimit();
        debugTextObjects[7].setString(ss.str());
    }

    //display STUNNED if the stun status is active for p1
    debugTextObjects[8].setString("");
    if(p1.isStunned()){
        ss.str(std::string());
        ss << "STUNNED(" << p1.getStunCounter() << ")";
        debugTextObjects[8].setString(ss.str());
    }

    //display RECOIL if the recoil status is active for p1
    debugTextObjects[18].setString("");
    if(p1.isInRecoil()){
        debugTextObjects[18].setString("RECOIL");
    }

    //p1 HP text display "HP: [remaining HP]/[max HP]"
    //if HP = 0, display text color in red
    ss.str(std::string());
    ss << "HP: " << p1.getHP() << "/" << p1.getMaxHP();
    if(p1.getHP() == 0){
        debugTextObjects[20].setFillColor(sf::Color::Red);
    }else{
        debugTextObjects[20].setFillColor(sf::Color::White);
    }
    debugTextObjects[20].setString(ss.str());

    //display direction p1 is facing
    debugTextObjects[22].setString("");
    if(p1.getXDirection() == Directions::left){
        debugTextObjects[22].setString("Facing: LEFT");
    }else{
        debugTextObjects[22].setString("Facing: RIGHT");
    }

    //CAST SWITCH
    //display cast switch status by number
    ss.str(std::string());
    switch(castSwitch){
        case castStatuses::na: ss << "standby"; break;
        case castStatuses::p1: ss << "p1"; break;
        case castStatuses::p2: ss << "p2"; break;
    }
    debugTextObjects[9].setString(ss.str());

    //PLAYER 2 DEBUG OBJECTS
    //if there are only 2 cards in the deck, only show the current and next card (not previous card/ bottom of stack)
    if(p2.getDeck()->getCardsHeld() >= 3){
        debugTextObjects[10].setString(p2.getDeck()->getLast()->getName());
    }else{
        debugTextObjects[10].setString("");
    }

    //show current card (there should always be at least the reload card but checks for at least one just in case)
    if(p2.getDeck()->getCardsHeld() >= 1){
        debugTextObjects[11].setString(p2.getDeck()->getStack().back()->getName());
    }else{
        debugTextObjects[11].setString("(empty)");
    }

    //show next card in stack if size is over 1
    if(p2.getDeck()->getCardsHeld() >= 2){
        debugTextObjects[12].setString(p2.getDeck()->getNext()->getName());
    }else{
        debugTextObjects[12].setString("");
    }

    //update p2 input string
    debugTextObjects[13].setString(p2.getInputText());

    //update display of p2 status
    debugTextObjects[14].setString(p2.getStatusText());

    //reuses these objects from earlier
    //std::string buffer; //exists as an excess string dump
    //std::stringstream ss; //exists to print numerical stats

    //display p2 event counter but only if p2 status is not standby)
    if(p2.getStatus() == Statuses::standby){
        debugTextObjects[15].setString("");
    }else{
        ss.str(std::string());
        ss << p2.getEventCounter();
        debugTextObjects[15].setString(ss.str());
    }

    //display p2 event counter time limit but only if p2 status is not standby)
    if(p2.getStatus() == Statuses::standby){
        debugTextObjects[16].setString("");
    }else{
        ss.str(std::string());
        ss << p2.getEventCounter() << "-" << p2.getEventTimeLimit();
        debugTextObjects[16].setString(ss.str());
    }

    //display STUNNED if the stun status is active for p2
    debugTextObjects[17].setString("");
    if(p2.isStunned()){
        ss.str(std::string());
        ss << "STUNNED(" << p2.getStunCounter() << ")";
        debugTextObjects[17].setString(ss.str());
    }

    //display RECOIL if the recoil status is active for p1
    debugTextObjects[19].setString("");
    if(p2.isInRecoil()){
        debugTextObjects[19].setString("RECOIL");
    }

    //p2 HP text display "HP: [remaining HP]/[max HP]"
    //if HP = 0, display text color in red
    ss.str(std::string());
    ss << "HP: " << p2.getHP() << "/" << p2.getMaxHP();
    if(p2.getHP() == 0){
        debugTextObjects[21].setFillColor(sf::Color::Red);
    }else{
        debugTextObjects[21].setFillColor(sf::Color::White);
    }
    debugTextObjects[21].setString(ss.str());

    //display direction p1 is facing
    debugTextObjects[23].setString("");
    if(p2.getXDirection() == Directions::left){
        debugTextObjects[23].setString("Facing: LEFT");
    }else{
        debugTextObjects[23].setString("Facing: RIGHT");
    }

    //PLAYER HITBOX DISPLAY OBJECTS
    //really only needs to update positions
    debugRectObjects[0].setPosition(p1.getHitbox()->getX(), p1.getHitbox()->getY());
    debugRectObjects[1].setPosition(p1.getLowHitbox()->getX(), p1.getLowHitbox()->getY());
    debugRectObjects[2].setPosition(p2.getHitbox()->getX(), p2.getHitbox()->getY());
    debugRectObjects[3].setPosition(p2.getLowHitbox()->getX(), p2.getLowHitbox()->getY());

    //CAST HITBOX DISPLAY OBJECT
    //if there is a cast and the Act Status is 'hazard', display the appropriate hitbox, else, display nothing
    if(castSwitch != castStatuses::na){
        if(cast->getStatus() == actStatuses::hazard){
            debugRectObjects[4].setSize(sf::Vector2f(cast->getHitbox()->getW(), cast->getHitbox()->getH()));
            debugRectObjects[4].setPosition(cast->getHitbox()->getX(), cast->getHitbox()->getY());
        }
    }else{
        debugRectObjects[4].setSize(sf::Vector2f(0, 0));
        debugRectObjects[4].setPosition(-10, -10);
    }
};
