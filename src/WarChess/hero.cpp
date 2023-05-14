#include "hero.h"
#include "screen.h"
#include <QDebug>

Hero::Hero(QObject *parent) : QObject(parent)
{
    hadMove = 0;
    hadAttack = 0;
    hadRest = 0;
    hadScreen = 0;
}

int Hero::getPosX() const
{
    return posX;
}

int Hero::getPoxY() const
{
    return posY;
}

int Hero::getMR() const
{
    return MR;
}

int Hero::getAR() const
{
    return AR;
}

void Hero::setPos(int x, int y)
{
    posX = x;
    posY = y;
}

Knight::Knight()
{
    HP = 7;
    ATK = 4;
    MR = 5;
    AR = 1;
    name = 1;
    rHP = 7;
    ES = 1;
}

Paladin::Paladin()
{

    HP = 10;
    ATK = 3;
    MR = 3;
    AR = 1;
    name = 2;
    rHP = 10;
    ES = 2;
}


Genie::Genie()
{
    HP = 6;
    ATK = 4;
    MR = 4;
    AR = 3;
    name = 3;
    rHP = 6;
    ES = 1;
}

Pastor::Pastor()
{
    HP = 3;
    ATK = 1;    //治疗力
    MR = 3;
    AR = 2;
    name = 4;
    rHP = 3;
    ES = 2;
}

Berserker::Berserker()
{
    HP = 2;
    ATK = 999;
    MR = 5;
    AR = 1;
    name = 5;
    rHP = 2;
    ES = 1;
}



void Hero::showData(int influence,QWidget* parent)
{
    //qDebug() << "进入showData";
    myScreen = new Screen(HP,rHP,ES,name,influence,parent);
    //myScreen->setAttribute(Qt::WA_DeleteOnClose);
    hadScreen = 1;

    if (posY < 5 && posX < 7)
    {
        myScreen->move(posX * 80 + 100,posY * 80 + 100);
    }
    else if (posY < 5 && posX >= 7)
    {
        myScreen->move(posX * 80 - 250,posY * 80 + 100);
    }
    else if (posY >= 5 && posX < 7)
    {
        myScreen->move(posX * 80 + 100,posY * 80 - 200);
    }
    else if (posY >= 5 && posX >= 7)
    {
        myScreen->move(posX * 80 - 250,posY * 80 - 200);
    }

    myScreen->show();
}

void Hero::closeData()
{
    hadScreen = 0;
    myScreen->close();
}

bool Hero::getScreen() const
{
    return hadScreen;
}

int Hero::getName() const
{
    return name;
}

int Hero::getrHP() const
{
    return rHP;
}

void Hero::minusrHP(int h)
{
    //实际伤害 造成的伤害 - 护甲
    h = h - ES;

    if (h < 0) h = 0;
    rHP = rHP - h;
}

void Hero::addrHP(int h)
{
    rHP = rHP + h;
    if (rHP > HP) rHP = HP;
}

int Hero::getATK() const
{
    return ATK;
}

int Hero::getHP() const
{
    return HP;
}

int Hero::getES() const
{
    return ES;
}

bool Hero::operator == (Hero* other)
{
    //qDebug() << "进入了重载的运算符";
    if (other->name == name)
    {
        return true;
    }
    return false;
}
