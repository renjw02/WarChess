#ifndef PLAYER_H
#define PLAYER_H

#include "hero.h"
#include <QObject>

class Player : public QObject
{
    Q_OBJECT
public:
    explicit Player(QObject *parent = nullptr);

//    virtual void move(int,int) = 0;
//    virtual void attack(int,int,QString) = 0;
//    virtual void rest() = 0;

//    virtual void setAllowHeroNumber(int);
//    virtual void setAllowHeroType(QStringList);
    //void addHero(Hero*, int, int);

signals:

};

class Player1 : public Player
{
public:
    Player1();
//    void move(int,int) override;
//    void attack(int,int,QString) override;
//    void rest() override;

    void setAllowHeroNumber(int);
    void setAllowHeroType(QStringList);

    int getAllowHeroNum() const;
    int getCurrentHeroNum() const;
    void addHero(Hero*, int, int);
    void setCurrentNum(int);

    bool has(Hero*);
    bool isfinish() const;  //关于游戏能否开始的判断
    bool canAct();
    void resetAction();

    Hero* my(int);
    QVector<Hero*> myheroes;    //我的英雄

private:
    int currentHeroNumber;   //当前player拥有英雄数量
    int allowHeroNumber;    //允许player使用英雄数量
    QStringList allowHeroType;  //允许player使用英雄类型
    bool finishPrepare;         //游戏是否能开始


};

class AI : public Player
{
public:
    AI();
    int getCurrentHeroNum() const;
    void addHero(Hero*, int, int);
    void setCurrentNum(int);

    double cloest(int, int);

    Hero* my(int);

    QVector<Hero*> myheroes;    //我的英雄
private:
    int currentHeroNumber;   //当前AI拥有英雄数量

};

#endif // PLAYER_H
