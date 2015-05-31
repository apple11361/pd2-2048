#include "tile.h"


Tile::Tile()
{

}


Tile::~Tile()
{

}


QLabel *Tile::getLabel()
{
    return Tlabel;
}


void Tile::setLabel(QLabel *label)
{
    Tlabel = label;
}


int Tile::getNumber()
{
    return num;
}



void Tile::setNumber(int number)
{
    num = number;
}



void Tile::setText()
{
    if(num==0){
        Tlabel->setText("");
    }else{
        Tlabel->setNum(num);
    }

    switch (num) {
    case 0:
        Tlabel->setStyleSheet("QLabel{background-color: rgb(204, 192, 179, 150)}");
        break;

    case 2:
        Tlabel->setStyleSheet("background-image: url(:/image/2.png)");
        break;

    case 4:
        Tlabel->setStyleSheet("background-image: url(:/image/4.png)");
        break;

    case 8:
         Tlabel->setStyleSheet("background-image: url(:/image/8.png)");
        break;

    case 16:
         Tlabel->setStyleSheet("background-image: url(:/image/16.png)");
        break;

    case 32:
        Tlabel->setStyleSheet("background-image: url(:/image/32.png)");
        break;

    case 64:
        Tlabel->setStyleSheet("background-image: url(:/image/64.png)");
        break;

    case 128:
        Tlabel->setStyleSheet("background-image: url(:/image/128.png)");
        break;

    case 256:
        Tlabel->setStyleSheet("background-image: url(:/image/256.png)");
        break;

    case 512:
        Tlabel->setStyleSheet("background-image: url(:/image/512.png)");
        break;

    case 1024:
        Tlabel->setStyleSheet("background-image: url(:/image/1024.png)");
        break;

    case 2048:
        Tlabel->setStyleSheet("background-image: url(:/image/2048.png)");
        break;



    }
}



void Tile::setNumTxt(int number)
{
    num = number;
    setText();
}

