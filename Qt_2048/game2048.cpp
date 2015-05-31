#include <stdlib.h>
#include <time.h>
#include "game2048.h"
#include "ui_game2048.h"



game2048::game2048(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::game2048)
{
    ui->setupUi(this);

    initgame();
}



game2048::~game2048()
{
    delete ui;
}



void game2048::initgame()// 程式初始化, 設定一些變數.
{
    gameoverlabel = new QLabel(ui->frame_1_1);// 建立顯示"Game Over"的Label, 指定ui->frame_1_1為父層.
    gameoverlabel->hide();// 先隱藏起來, 遊戲結束時才顯示出來.
    QFont font = gameoverlabel->font();// 開始設定gameoverlabel的字型,字型大小等.
    font.setBold(true);
    font.setFamily("Arial");
    font.setPointSize(48);
    gameoverlabel->setFont(font);
    gameoverlabel->setGeometry(QRect(0, 0, 522, 500));// 設定gameoverlabel的x,y位置,寬度,高度.
    gameoverlabel->setAlignment(Qt::AlignCenter);// 水平和垂直置中.
    gameoverlabel->setStyleSheet("QLabel{color: rgb(0, 0, 0)}");// 設定顯示的文字為黑色.
    gameoverlabel->setAttribute(Qt::WA_TranslucentBackground, true);// 設定背景透明.
    gameoverlabel->setText("Game Over !");// 要顯示的字串.

    youwin = new QLabel(ui->frame_1_1);
    youwin -> hide();
    font = youwin -> font();
    font.setBold(true);
    font.setFamily("Arial");
    font.setPointSize(48);
    youwin->setFont(font);
    youwin->setGeometry(QRect(0,0,522,500));
    youwin->setAlignment(Qt::AlignCenter);
    youwin->setStyleSheet("QLabel{color: rgb(0, 0, 0)}");// 設定顯示的文字為黑色.
    youwin->setAttribute(Qt::WA_TranslucentBackground, true);// 設定背景透明.
    youwin->setText("You Win !");// 要顯示的字串.

    gameoverpic = new QLabel(this);
    gameoverpic->setStyleSheet("image: url(:/image/gameover.png)");


    youwinpic = new QLabel(this);
    youwinpic->setStyleSheet("image: url(:/image/youwin.png)");



    game2048bg = QImage(":/image/game2048.png");// 載入遊戲視窗的背景
    palette.setBrush(this->backgroundRole(), QBrush(game2048bg));// 把背景圖片game2048bg設為調色板palette的刷子.
    this->setPalette(palette);// 設定遊戲視窗的調色板(Palette)為palette.
    this->setAutoFillBackground(true);// 設定遊戲視窗自動填充背景.

    srand(time(NULL));// 初始化亂數.

    tempscorelabel = new QLabel(ui->frame_1);// 用來顯示每次操作所得到分數的 Label.

    first = true;// 紀錄遊戲是否第一次建立, showEvent()函數會用來判斷遊戲是否剛建立.
    aniRfinisfed = true;// 隨機產生tile的動畫是否結束的flag設為true.
    aniSMfinished = true;// 移動或合併tile的動畫是否結束的flag設為true.



    setupTiles();// 將放置在frame上用來顯示tile的所有label設定給tilesMap 2維陣列中對應的tile.
    bestscore = 0;
    showBestScore();
}



void game2048::setupTiles()
{
    QWidget *wdt;
    Tile *tile;
    QLabel *label;

    tilesbg.resize(4);// 讓tilesbg成為4x4的2維陣列.
    for (int i = 0; i < 4; i++)
        tilesbg[i].resize(4);

    tilesMap.resize(4);// 讓tilesMap成為4x4的2維陣列.
    for (int i = 0; i < 4; i++)
        tilesMap[i].resize(4);

    for(int y=0; y<4; y++){
        for (int x=0; x<4; x++){
            wdt = ui->gridLayout->itemAtPosition(y,x)->widget();// 取得gridLayout方格中(y,x)位置的QWidget物件.
            label = static_cast<QLabel*>(wdt);// 將wdt轉型為QLabel物件.
            QFont font = label->font();// 開始設定label的字型,字型大小等.
            font.setBold(true);
            font.setFamily("Tunga");
            font.setPointSize(24);
            label->setFont(font);

            tile = new Tile;// 建立1個tile.
            tile->setLabel(label);// 將label設給tile的member data:Tlabel
            tilesMap[x][y] = tile;// tile存入tilesMap陣列.

            QLabel *templabel = new QLabel(ui->frame_1_1);// 建立一個對應的要做空tile背景的Label.
            templabel->setGeometry(QRect(x*125+18, y*120+17, 111, 104));// 設定正確的座標位置.
            templabel->setStyleSheet("QLabel{background-color: rgb(204, 192, 179)}");// 設定背景顏色.
            templabel->hide();// 先隱藏起來, 不顯示.
            tilesbg[x][y] = templabel;// 存入tilesbg陣列.
        }
    }
}



void game2048::startGame()// // 開始遊戲.
{
    ifwin = false;
    aniRfinisfed = true;// 隨機產生tile的動畫是否結束的flag設為true.
    aniSMfinished = true;// 移動或合併tile的動畫是否結束的flag設為true.
    tempscore = 0;
    score = 0;
    showScore();
    gameoverlabel->hide();
    youwin->hide();
    gameoverpic->hide();
    youwinpic->hide();
    ui->frame_1_1->setStyleSheet("QFrame{background-color: rgb(187, 173, 160, 150)}");// 將遊戲結束時變更的方格背景色恢復為原來的背景色.

    for (int y=0; y<4; y++) {// 將所有tile設為空tile.
        for (int x=0; x<4; x++) {
            tilesMap[x][y]->setNumTxt(0);
        }
    }

    rndaddNum();// 隨機產生1個tile.
    rndaddNum();// 隨機產生1個tile.
}



void game2048::rndaddNum()// 在所有的空tile處, 隨機選一個位置, 產生2分或4分的tile.
{
    QPoint p;
    int index, number;
    QLabel *label;
    emptyPointsList.clear();// 清除List.

    for(int y=0; y<4; y++){// 將所有空tile加入List.
        for(int x=0; x<4; x++){
            if(tilesMap[x][y]->getNumber()==0){
                p.rx() = x;
                p.ry() = y;
                emptyPointsList.append(p);
            }
        }
    }

    if(emptyPointsList.size()>0){// 如果還有空tile, 將隨機產生1個tile.
        index = rand()%emptyPointsList.size();// 從0到emptyPointsList.size()-1隨機選1個數字.
        p = emptyPointsList.takeAt(index);//取得List中index位置的Point, 並將它從List中移除.
        number = (rand()%100)>=10?2:4;// 以1和9的比率隨機產生4和2.
        tilesMap[p.rx()][p.ry()]->setNumTxt(number);// 將產生的number設給方格中p.rx(),p.ry()位置的tile.
        label = tilesMap[p.rx()][p.ry()]->getLabel();// 取得這個tile的Label, 將對這個Label做動畫效果.

        // 建立1個可以做動畫的QPropertyAnimation物件, 以label為動畫對象, 以label的Geometry Property為依據,
        // 改變label的Geometry值, 產生動畫效果.
        QPropertyAnimation *ani_rndaddNum = new QPropertyAnimation(label, "geometry");

        // connect ani_rndaddNum的finished()signal和game2048的frndaddNum()slot.
        connect(ani_rndaddNum, QPropertyAnimation::finished, this, game2048::frndaddNum);
        ani_rndaddNum->setDuration(150);// 設定動畫持續時間.
        ani_rndaddNum->setStartValue(QRect(label->x()+(Twidth/2), label->y()+(Theight/2), 0, 0));// 設定起始Geometry值.
        ani_rndaddNum->setEndValue(QRect(label->x(), label->y(), Twidth, Theight));// 設定結束Geometry值.
        ani_rndaddNum->start();// 動畫開始.
    }
}



void game2048::frndaddNum()// 當隨機產生tile的動畫結束時, ani_rndaddNum的finished()signal會觸發這個slot函數.
{
    aniRfinisfed = true;//// 隨機產生tile的動畫是否結束的flag設為true.
}




void game2048::slideLeft()//向左移動tile.
{
    bool slide = false, merge = false;//設定移動和合併的flag為false.

    //startP:需要移動的tile原始位置, endp:需要移動的tile新位置, mergeP:發生tile合併的位置.
    //因為要做動畫才需要記錄這些位置, 如果不做動畫就不必記錄這些位置.
    QPoint startP, endP, mergeP;

    slideStartList.clear();//清除紀錄tile原始位置的List.
    slideEndList.clear();//清除紀錄tile新位置的List.
    mergeList.clear();//清除紀錄發生tile合併的位置.

    for(int y=0; y<4; y++){
        for(int x=0; x<4; x++){
            for (int xR=x+1; xR<4; xR++){
                if(tilesMap[xR][y]->getNumber()>0){//正在檢查的x,y位置的右方tile數字>0.
                    if(tilesMap[x][y]->getNumber()==0){//正在檢查的x,y位置數字為0,需要移動tile.
                        startP.setX(xR);//將移動的tile原始位置xR設給startP的x.
                        startP.setY(y);//將移動的tile原始位置y設給startP的y.
                        endP.setX(x);//將移動的tile新位置x設給endP的x.
                        endP.setY(y);//將移動的tile新位置y設給endP的y.
                        slideStartList.append(startP);//將移動的tile原始位置加入slideStartList.
                        slideEndList.append(endP);//將移動的tile新位置加入slideEndList.

                        //將被移動的tile的數字設給新位置的tile.
                        tilesMap[x][y]->setNumber(tilesMap[xR][y]->getNumber());

                        //將被移動的tile的數字設為0.
                        tilesMap[xR][y]->setNumber(0);
                        slide = true;//發生移動.

                        //因為有tile移到x,y這個位置, 需要再從這個位置開始檢查, 回到迴圈開頭會x++, 所以先x--.
                        x--;
                    }else if(tilesMap[x][y]->getNumber()==tilesMap[xR][y]->getNumber()){//兩個tile數字相同, 需要合併.
                        startP.setX(xR);//將被合併的tile原始位置xR設給startP的x.
                        startP.setY(y);//將被合併的tile原始位置y設給startP的y.
                        endP.setX(x);//將被合併的tile新位置x設給startP的x.
                        endP.setY(y);//將被合併的tile新位置y設給startP的y.
                        mergeP.setX(x);//將發生tile合併的位置x設給mergeP的x.
                        mergeP.setY(y);//將發生tile合併的位置y設給mergeP的y.
                        slideStartList.append(startP);//將被合併的tile原始位置加入slideStartList.
                        slideEndList.append(endP);//將被合併的tile新位置加入slideEndList.
                        mergeList.append(mergeP);//將發生tile合併的位置加入mergeList.
                        tilesMap[x][y]->setNumber(tilesMap[x][y]->getNumber()*2);//合併處的tile數字*2.
                        tilesMap[xR][y]->setNumber(0);//被合併處的tile數字設為0.
                        addTempScore(tilesMap[x][y]->getNumber());//累加tempscore.
                        merge = true;//發生合併.
                    }

                    break;
                }
            }
        }
    }

    if(slide || merge){//如果有移動或合併,執行slideAni()做動畫.
        slideAni();
    }else{
        aniSMfinished = true;//沒有移動和合併,aniSMfinished設為true.
        aniRfinisfed = true;//沒有移動和合併,aniRfinisfed設為true.
    }

    if(merge)addScore();//如果有合併,累加分數.
}



void game2048::slideRight()
{
    bool slide = false, merge = false;
    QPoint startP, endP, mergeP;

    slideStartList.clear();
    slideEndList.clear();
    mergeList.clear();

    for(int y=0; y<4; y++){
        for(int x=3; x>=0; x--){
            for (int xL=x-1; xL>=0; xL--){
                if(tilesMap[xL][y]->getNumber()>0){
                    if(tilesMap[x][y]->getNumber()==0){
                        startP.setX(xL);
                        startP.setY(y);
                        endP.setX(x);
                        endP.setY(y);
                        slideStartList.append(startP);
                        slideEndList.append(endP);
                        tilesMap[x][y]->setNumber(tilesMap[xL][y]->getNumber());
                        tilesMap[xL][y]->setNumber(0);
                        slide = true;
                        x++;
                    }else if(tilesMap[x][y]->getNumber()==tilesMap[xL][y]->getNumber()){
                        startP.setX(xL);
                        startP.setY(y);
                        endP.setX(x);
                        endP.setY(y);
                        mergeP.setX(x);
                        mergeP.setY(y);
                        slideStartList.append(startP);
                        slideEndList.append(endP);
                        mergeList.append(mergeP);
                        tilesMap[x][y]->setNumber(tilesMap[x][y]->getNumber()*2);
                        tilesMap[xL][y]->setNumber(0);
                        addTempScore(tilesMap[x][y]->getNumber());
                        merge = true;
                    }

                    break;
                }
            }
        }
    }

    if(slide || merge){
        slideAni();
    }else{
        aniSMfinished = true;
        aniRfinisfed = true;
    }

    if(merge)addScore();
}



void game2048::slideUp()
{
    bool slide = false, merge = false;
    QPoint startP, endP, mergeP;

    slideStartList.clear();
    slideEndList.clear();
    mergeList.clear();

    for(int x=0; x<4; x++){
        for(int y=0; y<4; y++){
            for (int yD=y+1; yD<4; yD++){
                if(tilesMap[x][yD]->getNumber()>0){
                    if(tilesMap[x][y]->getNumber()==0){
                        startP.setX(x);
                        startP.setY(yD);
                        endP.setX(x);
                        endP.setY(y);
                        slideStartList.append(startP);
                        slideEndList.append(endP);
                        tilesMap[x][y]->setNumber(tilesMap[x][yD]->getNumber());
                        tilesMap[x][yD]->setNumber(0);
                        slide = true;
                        y--;
                    }else if(tilesMap[x][y]->getNumber()==tilesMap[x][yD]->getNumber()){
                        startP.setX(x);
                        startP.setY(yD);
                        endP.setX(x);
                        endP.setY(y);
                        mergeP.setX(x);
                        mergeP.setY(y);
                        slideStartList.append(startP);
                        slideEndList.append(endP);
                        mergeList.append(mergeP);
                        tilesMap[x][y]->setNumber(tilesMap[x][y]->getNumber()*2);
                        tilesMap[x][yD]->setNumber(0);
                        addTempScore(tilesMap[x][y]->getNumber());
                        merge = true;
                    }

                    break;
                }
            }
        }
    }

    if(slide || merge){
        slideAni();
    }else{
        aniSMfinished = true;
        aniRfinisfed = true;
    }

    if(merge)addScore();
}



void game2048::slideDown()
{
    bool slide = false, merge = false;
    QPoint startP, endP, mergeP;

    slideStartList.clear();
    slideEndList.clear();
    mergeList.clear();

    for(int x=0; x<4; x++){
        for(int y=3; y>=0; y--){
            for (int yU=y-1; yU>=0; yU--){
                if(tilesMap[x][yU]->getNumber()>0){
                    if(tilesMap[x][y]->getNumber()==0){
                        startP.setX(x);
                        startP.setY(yU);
                        endP.setX(x);
                        endP.setY(y);
                        slideStartList.append(startP);
                        slideEndList.append(endP);
                        tilesMap[x][y]->setNumber(tilesMap[x][yU]->getNumber());
                        tilesMap[x][yU]->setNumber(0);
                        slide = true;
                        y++;
                    }else if(tilesMap[x][y]->getNumber()==tilesMap[x][yU]->getNumber()){
                        startP.setX(x);
                        startP.setY(yU);
                        endP.setX(x);
                        endP.setY(y);
                        mergeP.setX(x);
                        mergeP.setY(y);
                        slideStartList.append(startP);
                        slideEndList.append(endP);
                        mergeList.append(mergeP);
                        tilesMap[x][y]->setNumber(tilesMap[x][y]->getNumber()*2);
                        tilesMap[x][yU]->setNumber(0);
                        addTempScore(tilesMap[x][y]->getNumber());
                        merge = true;
                    }

                    break;
                }
            }
        }
    }

    if(slide || merge){
        slideAni();
    }else{
        aniSMfinished = true;
        aniRfinisfed = true;
    }

    if(merge)addScore();
}



void game2048::addTempScore(int s)
{
    tempscore+=s;
}



void game2048::addScore()
{
    score+=tempscore;

    showScore();
    tempscoreAni();
    if(score>bestscore){
        bestscore = score;
        showBestScore();
    }

    tempscore = 0;
}



void game2048::showScore()
{
    ui->lcdScore->display(score);
}



void game2048::showBestScore()
{
    ui->lcdbestScore->display(bestscore);
}



void game2048::tempscoreAni()
{
    tempscorelabel->setGeometry(ui->frame_1->x(), ui->frame_1->y()-100, 90, 30);
    tempscorelabel->setAlignment(Qt::AlignHCenter);
    QFont font = tempscorelabel->font();
    font.setPointSize(16);
    font.setFamily("Tunga");
    tempscorelabel->setFont(font);
    tempscorelabel->setStyleSheet("QLabel{color: rgb(0, 0, 0, 100)}");

    QString tsString;
    tsString.setNum(tempscore);
    tempscorelabel->setText("+ "+tsString);

    QPropertyAnimation *ani_tempscorelabel = new QPropertyAnimation(tempscorelabel, "geometry", ui->frame_1);
    ani_tempscorelabel->setDuration(900);
    ani_tempscorelabel->setStartValue(QRect(0, 35, 90, 30));
    ani_tempscorelabel->setEndValue(QRect(0, -30, 90, 30));
    ani_tempscorelabel->start();
}



void game2048::checkGameOver()
{
    bool gameover = true;

    for(int y=0; y<4; y++)
    {
        for(int x=0; x<4; x++)
        {
            if(tilesMap[x][y]->getNumber()==2048)
            {
                ifwin = true;
                break;
            }
            if(tilesMap[x][y]->getNumber()==0||
                (x>0&&(tilesMap[x][y]->getNumber()==tilesMap[x-1][y]->getNumber()))||
                (x<3&&(tilesMap[x][y]->getNumber()==tilesMap[x+1][y]->getNumber()))||
                (y>0&&(tilesMap[x][y]->getNumber()==tilesMap[x][y-1]->getNumber()))||
                (y<3&&(tilesMap[x][y]->getNumber()==tilesMap[x][y+1]->getNumber())))
            {
                gameover = false;

            }
        }
        if(ifwin==true)break;
    }

    if(ifwin)
    {
        ui->frame_1_1->setStyleSheet("QFrame{background-color: rgb(187, 173, 160, 200)}");
        youwin->show();
        youwinpic->show();
        gameover = false;
    }
    if(gameover)
    {
        ui->frame_1_1->setStyleSheet("QFrame{background-color: rgb(187, 173, 160, 200)}");
        gameoverlabel->show();
        gameoverpic->show();
    }
}



void game2048::slideAni()
{
    QLabel *startlabel, *endlabel;
    QPoint startP, endP;

    QParallelAnimationGroup *ani_Slide = new QParallelAnimationGroup;
    connect(ani_Slide, QParallelAnimationGroup::finished, this, game2048::finishSlide);

    for(int i=0; i<slideStartList.size(); i++){
        startP = slideStartList.at(i);
        startlabel = tilesMap[startP.rx()][startP.ry()]->getLabel();
        endP = slideEndList.at(i);
        endlabel = tilesMap[endP.rx()][endP.ry()]->getLabel();
        tilesbg[startP.rx()][startP.ry()]->show();//顯示空tile的背景色
        QPropertyAnimation *ani_s = new QPropertyAnimation(startlabel, "geometry");
        ani_s->setDuration(70);
        ani_s->setStartValue(QRect(startlabel->x(), startlabel->y(), Twidth, Theight));
        ani_s->setEndValue(QRect(endlabel->x(), endlabel->y(), Twidth, Theight));
        ani_Slide->addAnimation(ani_s);
    }

    ani_Slide->start();
}



void game2048::finishSlide()
{
    QPoint startP, endP, *cordP;
    QLabel *label;

    for(int i=0; i<slideStartList.size(); i++){
        startP = slideStartList.at(i);
        endP = slideEndList.at(i);

        tilesbg[startP.rx()][startP.ry()]->hide();//關閉空tile的背景色
        label = tilesMap[startP.rx()][startP.ry()]->getLabel();
        cordP = tilesCord[startP.rx()][startP.ry()];
        label->setGeometry(QRect(cordP->rx(), cordP->ry(), Twidth, Theight));
        tilesMap[startP.rx()][startP.ry()]->setText();
        tilesMap[endP.rx()][endP.ry()]->setText();
    }

    if(mergeList.size()>0){
        mergeAni();
    }else{
        aniSMfinished = true;
    }

    rndaddNum();
    checkGameOver();
}



void game2048::mergeAni()//tiles合併的動畫程序
{
    QPoint mergeP;
    QLabel *label;

    QParallelAnimationGroup *ani_Merge = new QParallelAnimationGroup;
    connect(ani_Merge, QParallelAnimationGroup::finished, this, game2048::finishMerge);

    for(int i=0; i<mergeList.size(); i++){
        mergeP = mergeList.at(i);
        label = tilesMap[mergeP.rx()][mergeP.ry()]->getLabel();

        QPropertyAnimation *ani_M = new QPropertyAnimation(label, "geometry");
        ani_M->setDuration(150);
        ani_M->setStartValue(QRect(label->x()+((Twidth-90)/2), label->y()+((Theight-90)/2), 90, 90));
        ani_M->setEndValue(QRect(label->x(), label->y(), Twidth, Theight));
        ani_M->setEasingCurve(QEasingCurve::InOutBack);

        ani_Merge->addAnimation(ani_M);
    }

    ani_Merge->start();
}



void game2048::finishMerge()
{
    aniSMfinished = true;
}



void game2048::showEvent(QShowEvent*)
{
    if(first){
        QLabel *label;
        QPoint *cordP;
        Twidth = tilesMap[0][0]->getLabel()->width();
        Theight = tilesMap[0][0]->getLabel()->height();


        tilesCord.resize(4);
        for (int i = 0; i < 4; i++)
            tilesCord[i].resize(4);


        for(int y=0; y<4; y++){
            for (int x=0; x<4; x++){
                label = tilesMap[x][y]->getLabel();
                cordP = new QPoint;
                cordP->setX(label->x());
                cordP->setY(label->y());
                tilesCord[x][y] = cordP;
            }
        }

        first = false;
        startGame();
    }
}



void game2048::resizeEvent(QResizeEvent *)
{
    if(this->width()>=ui->frame_1->width() && this->height()>=ui->frame_1->height()){
        ui->frame_1->setGeometry(QRect((this->width()-ui->frame_1->width())/2, (this->height()-ui->frame_1->height())/2, ui->frame_1->width(), ui->frame_1->height()));
    }

    if(this->width()>=ui->frame_1->width() && this->height()<ui->frame_1->height()){
        ui->frame_1->setGeometry(QRect((this->width()-ui->frame_1->width())/2, 10, ui->frame_1->width(), ui->frame_1->height()));
    }

    if(this->width()<ui->frame_1->width() && this->height()>=ui->frame_1->height()){
        ui->frame_1->setGeometry(QRect(10, (this->height()-ui->frame_1->height())/2, ui->frame_1->width(), ui->frame_1->height()));
    }

    if(this->width()<ui->frame_1->width() && this->height()<ui->frame_1->height()){
        ui->frame_1->setGeometry(QRect(10, 10, ui->frame_1->width(), ui->frame_1->height()));
    }

    gameoverpic->setGeometry(QRect(ui->frame_1->x()-120,ui->frame_1->y()+330,110,104));
    youwinpic->setGeometry(QRect(ui->frame_1->x()-120,ui->frame_1->y()+330,110,104));

}



void game2048::on_PBNewGame_clicked()
{
    startGame();
}



void game2048::keyPressEvent(QKeyEvent *event)
{
    if(!aniSMfinished || !aniRfinisfed||ifwin){
         return;
     }else{
         aniSMfinished = false;
         aniRfinisfed = false;
     }

     switch (event->key()){
     case Qt::Key_Left:
         slideLeft();
         break;

     case Qt::Key_Right:
         slideRight();
         break;

     case Qt::Key_Up:
         slideUp();
         break;

     case Qt::Key_Down:
         slideDown();
         break;
     default:
         aniSMfinished = true;
         aniRfinisfed = true;
     }
}
