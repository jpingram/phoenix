#include <string>

#ifndef CARD
#define CARD

enum Category {reload, test, chi};
enum TType {zero, one, two, three, four, five, six, seven, eight, nine};
enum CType {gold, mercury, copper, silver, iron, tin, lead};

#define RELOAD_CAT_SIZE 1
#define TEST_CAT_SIZE 10
#define CHI_CAT_SIZE 7

class Card{
private:
    unsigned short categ;
    unsigned short type;
    unsigned short value;
    std::string name;
public:
    Card();
    Card(unsigned short c);
    Card(unsigned short c, unsigned short t);
    Card(unsigned short c, unsigned short t, unsigned short v);

    unsigned short getCateg();
    unsigned short getType();
    unsigned short getValue();
    std::string getName();
};

std::string getNameFromList(unsigned short c, unsigned short t, unsigned short v);

#endif
