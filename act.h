#include <string>
#include "rect.h"

#ifndef ACT
#define ACT

#define TEST_DEFAULT_POWER 10.0
#define TEST_CAST_TIME 60
#define TEST_HAZARD_TIME 50

#define TEST_HITBOX_WIDTH 10
#define TEST_HITBOX_HEIGHT 10
#define TEST_X_ORIGIN (xOrigin+(width/2)-(TEST_HITBOX_WIDTH/2))
#define TEST_Y_ORIGIN (yOrigin+(height/2)-(TEST_HITBOX_HEIGHT/2))
#define TEST_DISTANCE 20 //radius of effect of test attack
#define TEST_ROTE_SCALE 80 //scale in which the arc of each test hitbox is placed along attack radius

#define CHI_POWER_E 1.5
#define CHI_POWER_D 2.0
#define CHI_POWER_C 3.0
#define CHI_POWER_B 4.0
#define CHI_POWER_A 4.5
#define CHI_POWER_S 5.0

#define CHI_DURATION_E 0.8
#define CHI_DURATION_D 0.76
#define CHI_DURATION_C 0.7
#define CHI_DURATION_B 0.62
#define CHI_DURATION_A 0.55
#define CHI_DURATION_S 0.47

#define CHI_REACH_E 1.0
#define CHI_REACH_D 1.25
#define CHI_REACH_C 1.5
#define CHI_REACH_B 1.75
#define CHI_REACH_A 1.85
#define CHI_REACH_S 2.0

#define CHI_RECOVERY_E 1.5
#define CHI_RECOVERY_D 1.2
#define CHI_RECOVERY_C 0.9
#define CHI_RECOVERY_B 0.75
#define CHI_RECOVERY_A 0.6
#define CHI_RECOVERY_S 0.5

#define CHI_BASE_DURATION 60
#define CHI_HAZARD_CUE_SCALE 0.8

#define CHI_X_ORIGIN xOrigin+((width/2)+(xDir*width*.75))
#define CHI_Y_ORIGIN yOrigin
#define CHI_HITBOX_WIDTH (width*reach)-(width/2)
#define CHI_HITBOX_HEIGHT height/2

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
