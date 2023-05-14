#ifndef HERO_H
#define HERO_H

#include "screen.h"
#include <QObject>

//knight 1 paladin 2 genie 3 pastor 4 Berserker 5

class Hero : public QObject
{
    Q_OBJECT
public:
    explicit Hero(QObject *parent = nullptr);
//    virtual void move() = 0;
//    virtual void attack() = 0;
//    virtual void rest() = 0;

    //得到英雄基本数据的接口
    int getPosX() const;
    int getPoxY() const;
    int getName() const;
    int getMR() const;
    int getAR() const;
    int getrHP() const;
    int getATK() const;
    int getHP() const;
    int getES() const;
    bool getScreen() const;

    //血量机制
    void minusrHP(int);
    void addrHP(int);

    //更新英雄坐标
    void setPos(int, int);

    bool operator == (Hero* other);

    //与显示器的连接
    virtual void showData(int,QWidget*);
    virtual void closeData();

    bool hadMove;
    bool hadAttack;
    bool hadRest;

protected:
    int HP;         //血量
    int ATK;        //攻击力
    int MR;         //移动范围
    int AR;         //攻击范围
    int rHP;        //当前血量
    int ES;         //护盾

    int name;
    int posX,posY;

    Screen* myScreen;    //显示器
    bool hadScreen;
signals:

};

class Knight : public Hero
{
public:
    Knight();


};

class Paladin : public Hero
{
public:
    Paladin();


};

class Genie : public Hero
{
public:
    Genie();


};

class Pastor : public Hero
{
public:
    Pastor();


};

class Berserker : public Hero
{
public:
    Berserker();


};



#endif // HERO_H
