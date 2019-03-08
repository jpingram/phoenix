#include <string>
#include "card.h"
#include "deck.h"
#include "act.h"
#include "rect.h"
#include "player.h"

Player::Player():deck(emptyDeck), status(Statuses::standby), eventCounter(0), eventTimeLimit(0),
    stunCounter(0), recoilCounter(0), currentFocusStage(1), focusProgress(0), reloadCount(0),
    maxHP(DEFAULT_PLAYER_HP), HP(DEFAULT_PLAYER_HP), dex(20), xSpeed(PLAYER_BASE_X_SPEED), ySpeed(PLAYER_BASE_Y_SPEED),
    xDir(Directions::right), hitbox(DEFAULT_PLAYER_X, DEFAULT_PLAYER_Y, PLAYER_WIDTH, PLAYER_HEIGHT),
    lowHitbox(DEFAULT_PLAYER_X, DEFAULT_PLAYER_Y+PLAYER_HEIGHT/2, PLAYER_WIDTH, PLAYER_HEIGHT/2), actions(){
};

Player::Player(unsigned short* s):deck(s), status(Statuses::standby), eventCounter(0),
    eventTimeLimit(0), stunCounter(0), recoilCounter(0), currentFocusStage(1), focusProgress(0), reloadCount(0),
    maxHP(DEFAULT_PLAYER_HP), HP(DEFAULT_PLAYER_HP), dex(20), xSpeed(PLAYER_BASE_X_SPEED), ySpeed(PLAYER_BASE_Y_SPEED),
    xDir(Directions::right), hitbox(DEFAULT_PLAYER_X, DEFAULT_PLAYER_Y, PLAYER_WIDTH, PLAYER_HEIGHT),
    lowHitbox(DEFAULT_PLAYER_X, DEFAULT_PLAYER_Y+PLAYER_HEIGHT/2, PLAYER_WIDTH, PLAYER_HEIGHT/2), actions(){
};

Player::Player(unsigned short* s, short x, short y):deck(s), status(Statuses::standby), eventCounter(0),
    eventTimeLimit(0), stunCounter(0), recoilCounter(0), currentFocusStage(1), focusProgress(0), reloadCount(0),
    maxHP(DEFAULT_PLAYER_HP), HP(DEFAULT_PLAYER_HP), dex(20), xSpeed(PLAYER_BASE_X_SPEED), ySpeed(PLAYER_BASE_Y_SPEED),
    xDir(Directions::right), hitbox(x, y, PLAYER_WIDTH, PLAYER_HEIGHT),
    lowHitbox(x, y+PLAYER_HEIGHT/2, PLAYER_WIDTH, PLAYER_HEIGHT/2), actions(){
};

Deck* Player::getDeck(){
    return &deck;
};

void Player::setMaxHP(float f){
    maxHP = f;
};

void Player::setHP(float f){
    HP = f;
};

void Player::setXSpeed(unsigned short x){
    xSpeed = x;
};

void Player::setYSpeed(unsigned short y){
    ySpeed = y;
};

float Player::getMaxHP(){
    return maxHP;
};

float Player::getHP(){
    return HP;
};

unsigned short Player::getDEX(){
    return dex;
};

unsigned short Player::getXSpeed(){
    return xSpeed;
};

unsigned short Player::getYSpeed(){
    return ySpeed;
};

void Player::setXDirection(short d){
    xDir = d;
};

short Player::getXDirection(){
    return xDir;
};

Rect* Player::getHitbox(){
    return &hitbox;
};

Rect* Player::getLowHitbox(){
    return &lowHitbox;
};

void Player::setStatus(unsigned short s){
    status = s; //actual place where status is reset

    eventCounter = 0;
    switch(s){
        case Statuses::standby:
            eventTimeLimit = 0;
            break;
        case Statuses::focusing:
            eventCounter = focusProgress;
            eventTimeLimit = FOCUS_TIME_LIMIT;
            break;
        case Statuses::focusing2:
            eventCounter = focusProgress;
            eventTimeLimit = FOCUS_TIME_LIMIT;
            break;
        case Statuses::focusing3:
            eventCounter = focusProgress;
            eventTimeLimit = FOCUS_TIME_LIMIT;
            break;
        case Statuses::focused:
            eventTimeLimit = 0; //set to zero so that updateStatus() cycles to reload immediately
            break;
        case Statuses::reloading:
            //increment reload counter
            reloadCount++;
            //reset focus stage to new starting stage
            if(reloadCount <= 1){
                currentFocusStage = 2;
            }else
            if(reloadCount >= 2){
                currentFocusStage = 3;
            }
            //set reload event timer
            eventTimeLimit = (RELOAD_TIME_LIMIT*deck.getStack().size())*(dex/20); //reload time limit per card * number of cards
            break;
        case Statuses::throwing:
            eventTimeLimit = THROW_SPEED;
            break;
        case Statuses::acting:
            recoilCounter = 0;
            eventTimeLimit = TEST_CAST_TIME;
            break;
    }

    return;
};

unsigned short Player::getStatus(){
    return status;
};

void Player::updateStatus(){
    //decrease stun counter if the player is stunned
    if(isStunned()){
        stunCounter--;
    }

    //decrease recoil counter if the player is in recoil
    if(isInRecoil()){
        recoilCounter--;
    }

    //IF THE STATUS IS NOT STANDBY, CHECK OTHER STATUS RELATED EVENTS
    if(status != Statuses::standby){
        //STATUS UPDATES UNAFFECTED BY EVENT TIMER
        //if acting and act has failed, reset status
        //(set event timer to zero, event timer logic for acting will fully reset)
        if(status == Statuses::acting){
            //error check, just to make sure there actually is an act there
            //shouldn't be an issue but still important
            if(!actions.empty()){
                //update the current act
                actions.front().update();
                //if the act has failed, make sure it ends
                if(actions.front().getStatus() == actStatuses::failed){
                    setEventTimeLimit(0);
                }
            }
        }

        //EVENT TIMER AFFECTED STATUS UPDATES
        eventCounter++;

        //UPDATE FOCUS COUNTER IF FOCUSING
        if(status == Statuses::focusing || status == Statuses::focusing2 || status == Statuses::focusing3){
            focusProgress = eventCounter;
        }

        if(eventCounter >= eventTimeLimit){
            //if throwing a card, if timer ends, act
            if(status == Statuses::throwing){
                setStatus(Statuses::acting);
            }else
            //if action is finished, remove current Act from actions vector
            //check if another Act is there, if not, set to standby
            //if yes, restart act timer for new Act
            if(status == Statuses::acting){
                actions.erase(actions.begin());
                if(actions.empty()){
                    setStatus(Statuses::standby);
                }else{
                    setStatus(Statuses::acting);
                }
            }else
            //if finished focusing, set player as focused, else, set to standby
            if(status == Statuses::focusing || status == Statuses::focusing2 || status == Statuses::focusing3){
                focusProgress = 0;
                setStatus(Statuses::focused);
            }else
            if(status == Statuses::focused){
                //set current focus stage to next appropriate level
                if(currentFocusStage == 2){
                    currentFocusStage = 1;
                    setStatus(Statuses::focusing);
                }else
                if(currentFocusStage == 3){
                    currentFocusStage = 2;
                    setStatus(Statuses::focusing2);
                }else{
                    //added in update function to prevent focus looping
                    //does not need to worry about updating cast in BattleView
                    //no cast because RELOAD is never used as an attack, item use, etc. just for reloading
                    deck.reload(); //must do before setStatus so Event Time Limit calculation in setStatus is accurate
                    setStatus(Statuses::reloading);
                }
            }else{
                //set to standby by default
                setStatus(Statuses::standby);
            }
        }
    }

    return;
};

void Player::setEventTimeLimit(unsigned short t){
    eventTimeLimit = t;
};


unsigned short Player::getCurrentFocusStage(){
    return currentFocusStage;
};

unsigned short Player::getFocusProgress(){
    return focusProgress;
};

unsigned short Player::getReloadCount(){
    return reloadCount;
};

void Player::stun(){
    stunCounter = DEFAULT_STUN_TIME;
};

bool Player::isStunned(){
    return stunCounter>0?true:false;
};

void Player::recoil(){
    recoilCounter = DEFAULT_RECOIL_TIME;
};

bool Player::isInRecoil(){
    return recoilCounter>0?true:false;
};

void Player::act(){
    if(deck.getCardsHeld() >= 1){
        //if card-to-cast is of type RELOAD, start focus
        if(deck.getStack().back()->getCateg() == Category::reload){
            //if on standby, set to correct focus stage
            if(getStatus() == Statuses::standby){
                //set focus status based on current focus stage
                if(currentFocusStage == 1){
                    setStatus(Statuses::focusing);
                }else
                if(currentFocusStage == 2){
                    setStatus(Statuses::focusing2);
                }else
                if(currentFocusStage == 3){
                    setStatus(Statuses::focusing3);
                }
            }
        }else{
            //ACT (THROW CARD)
            setAction(deck.throwCard());
            setStatus(Statuses::throwing);
        }
    }
};

void Player::setAction(Card *c){
    //calculate POWER
    unsigned short power = DEFAULT_POWER_SCALE;

    //create new act
    Act newAct(c->getCateg(), c->getType(), c->getValue(), c->getName(), power, xDir);
    newAct.setHitbox(hitbox.getX(), hitbox.getY(), hitbox.getW(), hitbox.getH());

    //add to actions
    actions.push_back(newAct);
};

//below is a utility function for manual entry of Act object variables
void Player::setAction(unsigned short c, unsigned short t, unsigned short v, std::string n, float p, short d){
    //create new act
    Act newAct(c, t, v, n, p, d);
    newAct.setHitbox(hitbox.getX(), hitbox.getY(), hitbox.getW(), hitbox.getH());

    //add to actions
    actions.push_back(newAct);
};

Act* Player::getCurrentAct(){
    if(actions.empty()){
        return nullptr;
    }
    return &(actions.front());
};

void Player::moveLeft(){
    hitbox.moveLeft(xSpeed);
    lowHitbox.moveLeft(xSpeed);
    xDir = Directions::left;
};

void Player::moveRight(){
    hitbox.moveRight(xSpeed);
    lowHitbox.moveRight(xSpeed);
    xDir = Directions::right;
};

void Player::moveUp(){
    hitbox.moveUp(ySpeed);
    lowHitbox.moveUp(ySpeed);
};

void Player::moveDown(){
    hitbox.moveDown(ySpeed);
    lowHitbox.moveDown(ySpeed);
};

void Player::moveLeft(short s){
    hitbox.moveLeft(s);
    lowHitbox.moveLeft(s);
    xDir = Directions::left;
};

void Player::moveRight(short s){
    hitbox.moveRight(s);
    lowHitbox.moveRight(s);
    xDir = Directions::right;
};

void Player::moveUp(short s){
    hitbox.moveUp(s);
    lowHitbox.moveUp(s);
};

void Player::moveDown(short s){
    hitbox.moveDown(s);
    lowHitbox.moveDown(s);
};

bool Player::checkHit(Rect* r){
    return hitbox.contains(r);
};

bool Player::checkLowHit(Rect* r){
    return lowHitbox.contains(r);
};

void Player::hit(float power){
    HP -= power;
    if(HP <= 0){
        HP = 0;
    }
    if(status == Statuses::focusing || status == Statuses::focusing2 || status == Statuses::focusing3){
        setStatus(Statuses::standby);
    }
    recoil();
};

void Player::heal(float power){
    HP += power;
    if(HP >= maxHP){
        HP = maxHP;
    }
};

unsigned short Player::getEventCounter(){
    return eventCounter;
};

unsigned short Player::getEventTimeLimit(){
    return eventTimeLimit;
};

std::string Player::getStatusText(){
    switch(status){
        case Statuses::standby: return "STANDBY"; break;
        case Statuses::focusing: return "FOCUSING"; break;
        case Statuses::focusing2: return "FOCUSING (2)"; break;
        case Statuses::focusing3: return "FOCUSING (3)"; break;
        case Statuses::focused: return "FOCUSED"; break;
        case Statuses::reloading: return "RELOADING"; break;
        case Statuses::throwing: return "THROWING"; break;
        case Statuses::acting: return "ACTING"; break;
        case Statuses::stunned: return "STUNNED"; break;
    }

    return "ERROR: UNOWN"; //should not get here
};

unsigned short Player::getStunCounter(){
    return stunCounter;
};

void Player::prependInput(char c){
    input.insert(input.begin(), c);

    //if input is longer than 200 characters, cut the end off
    if(input.length() >= 100){
        input.erase(input.end()-1);
    }
};

std::string Player::getInputText(){
    return input;
};
