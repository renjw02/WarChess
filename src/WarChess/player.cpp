#include "player.h"
#include <QDebug>
#include <math.h>

Player::Player(QObject *parent) : QObject(parent)
{

}

Player1::Player1()
{
    currentHeroNumber = 0;
    finishPrepare = 0;

}

AI::AI()
{
    currentHeroNumber = 0;
}

double AI::cloest(int x,int y)
{
    double min = 100;
    for (auto it = myheroes.begin(); it != myheroes.end(); it++)
    {
        double tmp = sqrt(pow((*it)->getPosX() - x, 2) + pow((*it)->getPoxY() - y, 2));
        if (tmp < min)
            min = tmp;
    }
    return min;
}



void Player1::setAllowHeroNumber(int num)
{
    allowHeroNumber = num;
    qDebug() << num;
}

void Player1::setAllowHeroType(QStringList str)
{
    allowHeroType = str;
    qDebug() << str;
}

int Player1::getCurrentHeroNum() const
{
    return currentHeroNumber;
}

int AI::getCurrentHeroNum() const
{
    return currentHeroNumber;
}

int Player1::getAllowHeroNum() const
{
    return allowHeroNumber;
}

void Player1::addHero(Hero* h, int x, int y)
{
    currentHeroNumber++;
    if (currentHeroNumber == allowHeroNumber)
    {
        finishPrepare = true;
    }
    myheroes.push_back(h);
    myheroes[currentHeroNumber - 1]->setPos(x,y);
    //qDebug() << "加入一个knight" << "当前英雄数" << currentHeroNumber;
}

void AI::addHero(Hero* h, int x, int y)
{
    currentHeroNumber++;

    myheroes.push_back(h);
    myheroes[currentHeroNumber - 1]->setPos(x,y);
}

bool Player1::isfinish() const
{
    return finishPrepare;
}

bool Player1::canAct()
{
    for (auto it = myheroes.begin(); it != myheroes.end(); it++)
    {
        if (!((*it)->hadMove && (*it)->hadAttack && (*it)->hadRest))
        {
            //qDebug() << "能走";
            return true;
        }
    }
   // qDebug() << "不能走";
    return false;
}

bool Player1::has(Hero* h)
{
    //qDebug() << "进入has函数判断是否已有该英雄";
    for (auto it = myheroes.begin(); it != myheroes.end(); it++)
    {

        if (*(*it) == h)
        {
            return true;
        }
    }
    return false;
}

void Player1::setCurrentNum(int i)
{
    currentHeroNumber = i;
}

void AI::setCurrentNum(int i)
{
    currentHeroNumber = i;
}

void Player1::resetAction()
{
    qDebug() << "重置了按钮";
    for (auto it = myheroes.begin(); it != myheroes.end(); it++)
    {
        (*it)->hadMove = false;
        (*it)->hadAttack = false;
        (*it)->hadRest = false;
    }
}

Hero* AI::my(int heroType)
{
    for (auto it = myheroes.begin(); it != myheroes.end(); it++)
    {

        if ((*it)->getName() == heroType)
        {
            return (*it);
        }
    }
}

Hero* Player1::my(int heroType)
{
    for (auto it = myheroes.begin(); it != myheroes.end(); it++)
    {

        if ((*it)->getName() == heroType)
        {
            return (*it);
        }
    }
}
