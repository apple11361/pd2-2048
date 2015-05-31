#ifndef GAME2048_H
#define GAME2048_H
#include <QWidget>
#include <QPalette>
#include <QVector>
#include <QList>
#include <QPoint>
#include <QKeyEvent>
#include <QParallelAnimationGroup>
#include <QPropertyAnimation>
#include <QEasingCurve>
#include <QResizeEvent>
#include "tile.h"

namespace Ui {
class game2048;
}

class game2048 : public QWidget
{
    Q_OBJECT

public:
    explicit game2048(QWidget *parent = 0);
    ~game2048();

    void initgame();// 程式初始化, 設定一些變數.

    void setupTiles();// 將放置在frame上用來顯示tile的所有label設定給tilesMap 2維陣列中對應的tile.

    void startGame();// 開始遊戲.

    void showBestScore();// 顯示遊戲開始後, 出現過的最高分.

    void rndaddNum();// 在所有的空tile處, 隨機選一個位置, 產生2分或4分的tile.

    void slideLeft();// 向左移動tile.

    void slideRight();// 向右移動tile.

    void slideUp();// 向上移動tile.

    void slideDown();// 向下移動tile.

    void addTempScore(int s);// 累加 tempscore.

    void addScore();// 累加 score.

    void showScore();// 顯示新的score.

    void tempscoreAni();//動畫顯示單次操作所得分數.

    void saveBestScore();// 保存新的最高分.

    void slideAni();// 動畫顯示tile移動.

    void mergeAni();// 動畫顯示tile合併.

    void checkGameOver();// 檢查遊戲是否已經結束.



public slots:
    void frndaddNum();// 當隨機產生tile的動畫結束時, 要做的工作.

    void finishSlide();//當tile移動的動畫顯示結束時, 要做的工作.

    void finishMerge();// 當tile合併的動畫顯示結束時, 要做的工作.



private slots:
    void on_PBNewGame_clicked();//NewGame按鍵被按下時, 要做的工作.

private:
    void showEvent(QShowEvent*);// 遊戲視窗每次被顯示在螢幕上就會產生QShowEvent事件. 第一次顯示時做一些基本設定.

    void resizeEvent(QResizeEvent *);// 遊戲視窗改變大小時, 調整遊戲畫面的顯示位置.

    void keyPressEvent(QKeyEvent *event);// 處理鍵盤按鍵事件.

private:
    Ui::game2048 *ui;
    QPalette palette;// 用來繪製遊戲視窗背景的調色板.
    QImage game2048bg;// 當遊戲視窗背景的圖片.
    QVector< QVector<Tile*> > tilesMap;// tile的2維陣列.
    QVector< QVector<QPoint*> > tilesCord;// 所有tile的座標.
    QVector< QVector<QLabel*> > tilesbg;// 對應所有tile的label. 做動畫效果時, tile被移開後, 用來當空tile背景用.
    int score, bestscore, tempscore;// score:遊戲分數, bestscore:出現過的最高分, tempscore:單次操作的得分.
    int Twidth, Theight;// Twidth:tile的寬度, Theight:tile的高度

    //emptyPointsList:存放空tile的格子位置的List, slideStartList:存放需要動畫移動的tile的起始格子位置的List.
    //slideEndList:存放需要動畫移動的tile的終點格子位置的List, mergeList:存放需要做tile合併動畫的格子位置的List.
    QList<QPoint> emptyPointsList, slideStartList, slideEndList, mergeList;

    // *gameoverlabel:顯示"Game Over"的Label, *tempscorelabel:顯示單次操作得分的Label.
    QLabel *gameoverlabel, *youwin, *tempscorelabel , *gameoverpic , *youwinpic;

    // first:是否剛建立遊戲的flag, aniSMfinished:tile移動和合併動畫是否結束的flag, aniRfinisfed:隨機產生1個tile的動畫是否結束的flag.
    bool first, aniSMfinished, aniRfinisfed,ifwin;
};

#endif // GAME2048_H
