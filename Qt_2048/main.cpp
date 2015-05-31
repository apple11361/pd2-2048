#include "game2048.h"
#include <QApplication>
#include <QtGui>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    game2048 w;

    w.show();

    return a.exec();
}
