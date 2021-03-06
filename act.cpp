#include <string>
#include <math.h>
#include "act.h"
#include "card.h"
#include "rect.h"

Act::Act():categ(1), type(1), value(1), name("ONE"), powerScale(1.0), xDir(1), status(actStatuses::active),
        hitbox(), power(), duration(), reach(), recovery(), xSpeed(), ySpeed(),
        actEventTimer(0), hazardTimeCue(0){
            setStats();
        }; //default: makes an act of a "ONE" card with a power of 1
Act::Act(unsigned short c, unsigned short t, unsigned short v, std::string n, float p, short d)
        :categ(c), type(t), value(v), name(n), powerScale(p), xDir(d), status(actStatuses::active),
        hitbox(), power(), duration(), reach(), recovery(), xSpeed(), ySpeed(),
        actEventTimer(0), hazardTimeCue(0){
            setStats();
        };

unsigned short Act::getCateg(){
    return categ;
};

unsigned short Act::getType(){
    return type;
};

unsigned short Act::getValue(){
    return value;
};

std::string Act::getName(){
    return name;
};

short Act::getXDirection(){
    return xDir;
};

unsigned short Act::getStatus(){
    return status;
};

void Act::setStatus(unsigned short s){
    status = s;
};

//sets hitbox location based on act type and x & y origin coordinates as well as width and height input
void Act::setHitbox(short xOrigin, short yOrigin, short width, short height){
    switch(categ){
        case Category::test:
        { //brackets added to prevent [-fpermissive]/cross initialization of 'double angle'
            double angle = 0.0;
            switch(type){
                case TType::zero: angle = 0.0; break;
                case TType::one: angle = 45; break;
                case TType::two: angle = 30; break;
                case TType::three: angle = 20; break;
                case TType::four: angle = 10; break;
                case TType::five: angle = 0.0; break;
                case TType::six: angle = -10; break;
                case TType::seven: angle = -20; break;
                case TType::eight: angle = -30; break;
                case TType::nine: angle = -45; break;
                default: angle = 180.0; break;
            }
            hitbox.setX(TEST_X_ORIGIN + xDir*TEST_DISTANCE + xDir*TEST_ROTE_SCALE*cos(angle*3.1416/180));
            hitbox.setY(TEST_Y_ORIGIN + TEST_ROTE_SCALE*sin(angle*3.1416/180));
            hitbox.setW(TEST_HITBOX_WIDTH);
            hitbox.setH(TEST_HITBOX_HEIGHT);
            break;
        }
        case Category::chi:
        {
            hitbox.setX(CHI_X_ORIGIN);
            hitbox.setY(CHI_Y_ORIGIN);
            hitbox.setW(CHI_HITBOX_WIDTH);
            hitbox.setH(CHI_HITBOX_HEIGHT);
            break;
        }
        default:
        {
            //just for error contingency purposes
            hitbox.setX(0);
            hitbox.setY(0);
            hitbox.setH(10);
            hitbox.setW(10);
            break;
        }
    }

    //if the act is using facing left, set starting x coordinate one "width length" away from current position
    if(xDir == -1){
        hitbox.moveLeft(hitbox.getW());
    }
};

Rect* Act::getHitbox(){
    return &hitbox;
};

//determine stats based on category and type if Act
void Act::setStats(){
    switch(categ){
        case Category::test:
            //all test cards have the same stats
            power = TEST_DEFAULT_POWER;
            duration = TEST_CAST_TIME;
            reach = 1.0;
            recovery = 1.0;

            xSpeed = 0.0;
            ySpeed = 0.0;

            hazardTimeCue = TEST_HAZARD_TIME;
            break;
        case Category::chi:
        {
            switch(type){
                case CType::gold:
                    power = CHI_POWER_A;
                    duration = CHI_DURATION_B;
                    reach = CHI_REACH_A;
                    recovery = CHI_RECOVERY_A;
                    break;
                case CType::mercury:
                    power = CHI_POWER_D;
                    duration = CHI_DURATION_S;
                    reach = CHI_REACH_C;
                    recovery = CHI_RECOVERY_C;
                    break;
                case CType::copper:
                    power = CHI_POWER_C;
                    duration = CHI_DURATION_C;
                    reach = CHI_REACH_C;
                    recovery = CHI_RECOVERY_C;
                    break;
                case CType::silver:
                    power = CHI_POWER_A;
                    duration = CHI_DURATION_B;
                    reach = CHI_REACH_B;
                    recovery = CHI_RECOVERY_B;
                    break;
                case CType::iron:
                    power = CHI_POWER_S;
                    duration = CHI_DURATION_D;
                    reach = CHI_REACH_B;
                    recovery = CHI_RECOVERY_C;
                    break;
                case CType::tin:
                    power = CHI_POWER_C;
                    duration = CHI_DURATION_D;
                    reach = CHI_REACH_S;
                    recovery = CHI_RECOVERY_B;
                    break;
                case CType::lead:
                    power = CHI_POWER_C;
                    duration = CHI_DURATION_D;
                    reach = CHI_REACH_C;
                    recovery = CHI_RECOVERY_S;
                    break;
            }
            duration *= CHI_BASE_DURATION;

            //chi attack hit-boxes do not move
            xSpeed = 0.0;
            ySpeed = 0.0;

            hazardTimeCue = duration*CHI_HAZARD_CUE_SCALE;
            break;
        }
        default:
            //added just for error contingency purposes
            power = 1.0;
            duration = 1.0;
            reach = 1.0;
            recovery = 1.0;

            xSpeed = 0.0;
            ySpeed = 0.0;

            hazardTimeCue = TEST_HAZARD_TIME;
            break;
    }
};

float Act::getPower(){
    return power;
};

float Act::getDuration(){
    return duration;
};

float Act::getReach(){
    return reach;
};

float Act::getRecovery(){
    return recovery;
};

void Act::update(){
    //update hazard timer, if cue is reach, set status to hazard
    actEventTimer++;
    if(actEventTimer >= hazardTimeCue && status == actStatuses::active){
        setStatus(actStatuses::hazard);
    }
    //overflow check
    if(actEventTimer >= 255){
        actEventTimer = 0;
    }
};
