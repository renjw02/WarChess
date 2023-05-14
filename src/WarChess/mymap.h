#ifndef MYMAP_H
#define MYMAP_H

#include "player.h"
#include "grid.h"
#include <QWidget>
#include <QMediaPlayer>


class MyMap : public QWidget
{
    Q_OBJECT
public:
    explicit MyMap(QWidget *parent = nullptr);

    virtual void createHero(int, int) = 0;
    virtual void startGame() = 0;
    virtual void isGameOver();
    virtual void reset() = 0;
    virtual void clickHero(int, int, int) = 0;

    void setPlayerHero(int, QStringList);

    //范围设置
    void setMR(int, int, int);
    void setAR(int, int, int);

    //清屏
    void closeAllScreen();

    //AI控制
    virtual void initAI() = 0;
    void actAI();

    //移动判定 搜寻最短路
    bool moveable(int,int,int,int);
    void dijkstra(int,int,int,int);

    //攻击判定 范围内有无可攻击目标
    bool findTarget(int, int, int);

    //反击判断
    bool antiAttack(int,int,int,int,int);

    //英雄死亡判定
    void die();

    //动画制作
    void animMove(int,int,int,int);
    void animAttack(int,int,int,int);
    void animAntiAttack(int,int,int,int);
    void animCure(int,int,int,int);


protected:
    Grid* grid[10][15];     //地图上的网格
    Player1* player;    //玩家
    AI* enemy;  //AI

    int roundNum;   //回合数
    int currentPlayer;  //当前行动方 0是玩家 1是AI

    QMediaPlayer* musicPlayer;

private:
    bool flag;  //用于move的判断

signals:
    void changeTerm();
    void asignal();
    void anyoneDie();
    void gameOver();

public slots:

};



#endif // MYMAP_H
