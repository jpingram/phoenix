#include <string>
#include "rect.h"

#ifndef ACT
#define ACT

#define TEST_DEFAULT_POWER 1.0
#define TEST_HAZARD_TIME 50

#define TEST_HITBOX_WIDTH 10
#define TEST_HITBOX_HEIGHT 10
#define TEST_X_ORIGIN (xOrigin+(width/2)-(TEST_HITBOX_WIDTH/2))
#define TEST_Y_ORIGIN (yOrigin+(height/2)-(TEST_HITBOX_HEIGHT/2))
#define TEST_DISTANCE 20 //radius of effect of test attack
#define TEST_ROTE_SCALE 80 //scale in which the arc of each test hitbox is placed along attack radius

enum actStatuses{active, hazard, hit, failed, broken};

class Act{
private:
    unsigned short categ; //card category
    unsigned short type; //card type
    unsigned short value; //value of card(s)/act
    std::string name;
    float powerScale; //buff/debuffs act's power
    short xDir; //defined in constructor but only used internally, no need for setter
    unsigned short status;

    Rect hitbox;

    float power; //strength of act (damage, heal points, etc)
    float duration; //duration of act
    float reach; //used to determine length of hitbox
    float recovery; //if card is broken, how long is the formally acting Player stunned for?

    float xSpeed; //defined in getStats() but only used internally, no need for getter
    float ySpeed; //defined in getStats() but only used internally, no need for getter

    unsigned short actEventTimer; //starts at 0 and increases by 1 with each call to update()
    unsigned short hazardTimeCue; //if reached, set status to hazard
public:
    Act();
    Act(unsigned short c, unsigned short t, unsigned short v, std::string n, float p, short d);

    unsigned short getCateg();
    unsigned short getType();
    unsigned short getValue();
    std::string getName();
    short getXDirection();

    unsigned short getStatus();
    void setStatus(unsigned short s);

    //sets hitbox location based on act type and x & y origin coordinates as well as width and height input
    void setHitbox(short xOrigin, short yOrigin, short width, short height);
    Rect* getHitbox();

    void setStats(); //done after categ and type are set
    float getPower();
    float getDuration();
    float getReach();
    float getRecovery();

    void update();
};

#endif // ACT
