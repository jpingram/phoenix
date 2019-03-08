#include <SFML/Graphics.hpp>
#include <iostream>
#include "battleview.h"

#define LB sf::Keyboard::LBracket
#define L sf::Keyboard::L
#define SPACE sf::Keyboard::Space
#define A sf::Keyboard::A
#define D sf::Keyboard::D
#define W sf::Keyboard::W
#define S sf::Keyboard::S

#define SUBT sf::Keyboard::Subtract
#define MULT sf::Keyboard::Multiply
#define INS sf::Keyboard::Numpad0
#define FOUR sf::Keyboard::Numpad4
#define SIX sf::Keyboard::Numpad6
#define EIGHT sf::Keyboard::Numpad8
#define FIVE sf::Keyboard::Numpad5

#define FRAMERATE_CAP 80

int main(){
    //create window and window related objects
    sf::RenderWindow window(sf::VideoMode(600, 400), "Alpha 2.2");

    //set Max Frame Rate to the FERAMERATE_CAP macro
    window.setFramerateLimit(FRAMERATE_CAP);

    //window focus boolean
    bool hasFocus = true;

    //key pressed booleans
    bool leftBPressed = false;
    bool lPressed = false;
    bool spacePressed = false;
    bool aPressed = false;
    bool dPressed = false;
    bool wPressed = false;
    bool sPressed = false;

    bool subtPressed = false;
    bool multPressed = false;
    bool insPressed = false;
    bool fourPressed = false;
    bool sixPressed = false;
    bool eightPressed = false;
    bool fivePressed = false;

    //the battle view object!
    BattleView bv;

    //MAIN LOOP
    while(window.isOpen()){
        //EVENT OBJECT AND EVENT POLLING LOOP
        sf::Event event;
        while(window.pollEvent(event)){
            //WINDOW CLOSE EVENT, activated on pressing the close button
            if (event.type == sf::Event::Closed){
                window.close();
            }

            //CHECK FOR WINDOW FOCUS BEFORE ALLOWING KEYBOARD INPUT
            if (event.type == sf::Event::LostFocus){
                hasFocus = false;
            }
            if (event.type == sf::Event::GainedFocus){
                hasFocus = true;
            }
        }//end while(window.pollEvent(event))

        //IF WINDOW HAS FOCUS, CHECK KEY INPUTS
        if(hasFocus){
            //PLAYER 1 INPUTS
            if(sf::Keyboard::isKeyPressed(LB)){ //is key pressed?
                if(!leftBPressed){ //if not pressed to start, set to pressed and do action
                    leftBPressed = true;
                    bv.enterInput('f');
                }
            }else{ //if the key isn't pressed, set the key's flag to false
                leftBPressed = false;
            }

            if(sf::Keyboard::isKeyPressed(L)){ //is key pressed?
                if(!lPressed){ //if not pressed to start, set to pressed and do action
                    lPressed = true;
                    bv.enterInput('b');
                }
            }else{ //if the key isn't pressed, set the key's flag to false
                lPressed = false;
            }

            if(sf::Keyboard::isKeyPressed(SPACE)){ //is key pressed?
                if(!spacePressed){ //if not pressed to start, set to pressed and do action
                    spacePressed = true;
                    bv.enterInput('c');
                }else{ //SPACE held input
                    bv.enterInput('v');
                }
            }else{ //if the key isn't pressed, set the key's flag to false
                //if SPACE was held but is now released, give BattleView SPACE released input
                if(spacePressed){
                    bv.enterInput('n'); //give BattleView SPACE released input
                }
                spacePressed = false;
            }

            if(sf::Keyboard::isKeyPressed(A)){ //is key pressed?
                if(!aPressed){ //if not pressed to start, set to pressed and do action
                    aPressed = true;
                    bv.enterInput('l');
                }else{ //held input
                    bv.enterInput('l');
                }
            }else{ //if the key isn't pressed, set the key's flag to false
                aPressed = false;
            }

            if(sf::Keyboard::isKeyPressed(D)){ //is key pressed?
                if(!dPressed){ //if not pressed to start, set to pressed and do action
                    dPressed = true;
                    bv.enterInput('r');
                }else{ //held input
                    bv.enterInput('r');
                }
            }else{ //if the key isn't pressed, set the key's flag to false
                dPressed = false;
            }

            if(sf::Keyboard::isKeyPressed(W)){ //is key pressed?
                if(!wPressed){ //if not pressed to start, set to pressed and do action
                    wPressed = true;
                    bv.enterInput('u');
                }else{ //held input
                    bv.enterInput('u');
                }
            }else{ //if the key isn't pressed, set the key's flag to false
                wPressed = false;
            }

            if(sf::Keyboard::isKeyPressed(S)){ //is key pressed?
                if(!sPressed){ //if not pressed to start, set to pressed and do action
                    sPressed = true;
                    bv.enterInput('d');
                }else{ //held input
                    bv.enterInput('d');
                }
            }else{ //if the key isn't pressed, set the key's flag to false
                sPressed = false;
            }

            //PLAYER 2 INPUTS
            if(sf::Keyboard::isKeyPressed(SUBT)){ //is key pressed?
                if(!subtPressed){ //if not pressed to start, set to pressed and do action
                    subtPressed = true;
                    bv.enterInput('-');
                }
            }else{ //if the key isn't pressed, set the key's flag to false
                subtPressed = false;
            }

            if(sf::Keyboard::isKeyPressed(MULT)){ //is key pressed?
                if(!multPressed){ //if not pressed to start, set to pressed and do action
                    multPressed = true;
                    bv.enterInput('*');
                }
            }else{ //if the key isn't pressed, set the key's flag to false
                multPressed = false;
            }

            if(sf::Keyboard::isKeyPressed(INS)){ //is key pressed?
                if(!insPressed){ //if not pressed to start, set to pressed and do action
                    insPressed = true;
                    bv.enterInput('i');
                }else{ //INS held input
                    bv.enterInput('o');
                }
            }else{ //if the key isn't pressed, set the key's flag to false
                //if SPACE was held but is now released, give BattleView SPACE released input
                if(insPressed){
                    bv.enterInput('p'); //give BattleView SPACE released input
                }
                insPressed = false;
            }

            if(sf::Keyboard::isKeyPressed(FOUR)){ //is key pressed?
                if(!fourPressed){ //if not pressed to start, set to pressed and do action
                    fourPressed = true;
                    bv.enterInput('4');
                }else{ //held input
                    bv.enterInput('4');
                }
            }else{ //if the key isn't pressed, set the key's flag to false
                fourPressed = false;
            }

            if(sf::Keyboard::isKeyPressed(SIX)){ //is key pressed?
                if(!sixPressed){ //if not pressed to start, set to pressed and do action
                    sixPressed = true;
                    bv.enterInput('6');
                }else{ //held input
                    bv.enterInput('6');
                }
            }else{ //if the key isn't pressed, set the key's flag to false
                sixPressed = false;
            }

            if(sf::Keyboard::isKeyPressed(EIGHT)){ //is key pressed?
                if(!eightPressed){ //if not pressed to start, set to pressed and do action
                    eightPressed = true;
                    bv.enterInput('8');
                }else{ //held input
                    bv.enterInput('8');
                }
            }else{ //if the key isn't pressed, set the key's flag to false
                eightPressed = false;
            }

            if(sf::Keyboard::isKeyPressed(FIVE)){ //is key pressed?
                if(!fivePressed){ //if not pressed to start, set to pressed and do action
                    fivePressed = true;
                    bv.enterInput('5');
                }else{ //held input
                    bv.enterInput('5');
                }
            }else{ //if the key isn't pressed, set the key's flag to false
                fivePressed = false;
            }
        }//end if(hasFocus)

        //WINDOW DRAW FUNCTIONS
        window.clear(); //start by clearing the window

        //update battle view objects and draw them
        bv.update();
        if(bv.getDebugSetting() == true){
            for(unsigned short i = 0; i < bv.getDebugTextObjects().size(); i++){
                window.draw(bv.getDebugTextObjects().at(i));
            }
            for(unsigned short i = 0; i < bv.getDebugRectObjects().size(); i++){
                window.draw(bv.getDebugRectObjects().at(i));
            }
        }
        window.display(); //finally, display!
    }//end while(window.isOpen())

    return 0;
}
