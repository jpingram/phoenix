#include "card.h"
#include "deck.h"
#include "act.h"
#include "rect.h"
#include <vector>
#include <string>

#ifndef PLAYER
#define PLAYER

enum Statuses{standby, focusing, focusing2, focusing3, focused, reloading, throwing, acting, stunned};
enum Directions{left=-1, right=1};

#define FOCUS_TIME_LIMIT 100
#define RELOAD_TIME_LIMIT 12
#define DEFAULT_STUN_TIME 50
#define DEFAULT_RECOIL_TIME 50
#define THROW_SPEED 10

#define TEST_CAST_TIME 60
#define DEFAULT_POWER_SCALE 1.0

#define PLAYER_HEIGHT 40
#define PLAYER_WIDTH 20
#define DEFAULT_PLAYER_HP 10.0
#define DEFAULT_PLAYER_X 100
#define DEFAULT_PLAYER_TWO_X_OFFSET 100
#define DEFAULT_PLAYER_Y 250
#define PLAYER_BASE_X_SPEED 4
#define PLAYER_BASE_Y_SPEED 2

class Player{
private:
    Deck deck;
    unsigned short status;

    unsigned short eventCounter;
    unsigned short eventTimeLimit; //set on a case by case basis
    unsigned short stunCounter; //used to track stun time
    unsigned short recoilCounter; //used to track recoil time

    unsigned short currentFocusStage; //starts at 1 -> focusing, 2 ->focusing2, 3 -> focusing3
    unsigned short focusProgress;
    unsigned short reloadCount;

    //Player stats
    float maxHP; //maximum level of HP
    float HP; //current remaining HP
    unsigned short dex;
    unsigned short xSpeed;
    unsigned short ySpeed;

    //Player location dependent objects
    short xDir;
    Rect hitbox;
    Rect lowHitbox;

    std::vector<Act> actions;
    std::string input;
public:
    Player(); //default player, starts with an empty deck (of reload card)
    Player(unsigned short* s); //default position
    Player(unsigned short* s, short x, short y);

    Deck* getDeck();

    void setMaxHP(float f);
    void setHP(float f);
    void setXSpeed(unsigned short x);
    void setYSpeed(unsigned short y);

    float getMaxHP();
    float getHP();
    unsigned short getDEX();
    unsigned short getXSpeed();
    unsigned short getYSpeed();

    void setXDirection(short d);
    short getXDirection();
    Rect* getHitbox();
    Rect* getLowHitbox();

    void setStatus(unsigned short s); //also resets eventTimeLimit
    unsigned short getStatus();
    void updateStatus();
    void setEventTimeLimit(unsigned short t); //manual change function for event time limit
    unsigned short getCurrentFocusStage();
    unsigned short getFocusProgress();
    unsigned short getReloadCount();

    void stun(); //sets stunCounter
    bool isStunned(); //returns true if the Player has an active stunned status

    void recoil(); // sets recoilCounter
    bool isInRecoil(); //returns true if the Player has an active recoil status

    void act();
    void setAction(Card *c);
    void setAction(unsigned short c, unsigned short t, unsigned short v, std::string n, float p, short d);
    Act* getCurrentAct();

    //move using speed variables
    void moveLeft();
    void moveRight();
    void moveUp();
    void moveDown();

    //manual move functions
    void moveLeft(short f);
    void moveRight(short f);
    void moveUp(short f);
    void moveDown(short f);

    bool checkHit(Rect* r);
    bool checkLowHit(Rect* r);
    void hit(float power); //HP--
    void heal(float power); //HP++

    unsigned short getEventCounter(); //for display purposes
    unsigned short getEventTimeLimit(); //for display purposes
    unsigned short getStunCounter(); //for display purposes
    std::string getStatusText(); //for display purposes

    void prependInput(char c);
    std::string getInputText(); //for display purposes
};

#endif // PLAYER
