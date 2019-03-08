#include <vector>
#include <string>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Font.hpp> //for debug overlay NOTE: must include more later
#include <SFML/Graphics/Text.hpp> //for debug overlay NOTE: must include more later
#include "player.h"
#include "card.h"

#ifndef BATTLEVIEW
#define BATTLEVIEW

#define DEBUG_X_ZERO_POINT 0.f
#define DEBUG_Y_ZERO_POINT 0.f
#define DEBUG_LINE_SPACING 20.f
#define DEBUG_CAST_X_BUFFER 100.f
#define DEBUG_P2_OFFSET 200.f

enum castStatuses{na, p1, p2};

class BattleView{
private:
    Player p1;
    Player p2;
    std::vector<sf::Text> debugTextObjects;
    std::vector<sf::RectangleShape> debugRectObjects;
    bool debugSetting;

    unsigned short castSwitch; //0 -> empty, 1 -> player 1, 2 -> player 2, etc.
    Act* cast;

    sf::Font freeroadR;
    sf::Font freeroadB;
public:
    BattleView();

    Player* getPlayer();

    void setDebugSetting(bool set);
    bool getDebugSetting();
    std::vector<sf::Text> getDebugTextObjects();
    std::vector<sf::RectangleShape> getDebugRectObjects();

    void enterInput(char c);
    void enterInput(Player& p, char c);

    void updateCast();
    void update();

    void createDebugObjects();
    void updateDebugObjects();
};

#endif // BATTLEVIEW
