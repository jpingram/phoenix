#include <sstream>
#include <string>
#include "card.h"

//DEFAULT: TEST ONE card
Card::Card():categ(Category::test), type(TType::one), value(1), name("ONE(1)"){};
//CONSTRUCTOR 2: Type 0 of given Category with value of 1
Card::Card(unsigned short c):categ(c), type(0), value(1), name(getNameFromList(c, 0, 1)){};
//CONSTRUCTOR 3: Card of Category c and Type t with a value of 1
Card::Card(unsigned short c, unsigned short t):categ(c), type(t), value(1), name(getNameFromList(c, t, 1)){};
//CONSTRUCTOR 4: MOST IMPORTANT, CREATE CARD USING INPUTTED CATEGORY, TYPE, AND VALUE
Card::Card(unsigned short c, unsigned short t, unsigned short v):categ(c), type(t), value(v), name(getNameFromList(c, t, v)){};

unsigned short Card::getCateg(){
    return categ;
};

unsigned short Card::getType(){
    return type;
};

unsigned short Card::getValue(){
    return value;
};

std::string Card::getName(){
    return name;
};

std::string getNameFromList(unsigned short c, unsigned short t, unsigned short v){
    short id = t;
    if(c > 0){
        id += RELOAD_CAT_SIZE;
    }
    if(c > 1){
        id += TEST_CAT_SIZE;
    }
    std::stringstream s;

    switch(id){
        case 0: s << "RELOAD"; break;
        case 1: s << "ZERO"; break;
        case 2: s << "ONE"; break;
        case 3: s << "TWO"; break;
        case 4: s << "THREE"; break;
        case 5: s << "FOUR"; break;
        case 6: s << "FIVE"; break;
        case 7: s << "SIX"; break;
        case 8: s << "SEVEN"; break;
        case 9: s << "EIGHT"; break;
        case 10: s << "NINE"; break;
    }

    if(id >= 0 && id <= 10){

        s << "(" << v << ")";
        return s.str();
    }

    return "NOT FOUND"; //error found
};
