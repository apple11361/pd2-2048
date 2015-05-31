#ifndef TILE
#define TILE

#include <QLabel>


class Tile
{

public:
    explicit Tile();
    ~Tile();

    int getNumber();
    QLabel *getLabel();
    void setNumber(int number);
    void setText();
    void setNumTxt(int number);
    void setLabel(QLabel *label);

private:
    int num;
    QLabel *Tlabel;


};


#endif // TILE

