#include "grid.h"
#include <QMouseEvent>
#include <QDebug>
#include <QPainter>
#include <QPen>
#include <QPushButton>
#include <QMessageBox>
#include <QHBoxLayout>



Grid::Grid(QWidget *parent) : QWidget(parent)
{
    gridType = 0;
    backType = 0;
    gridState = 0;
    clickNum = 1;
    influence = 0;
    debuff = 0;
}

//实现点击返回位置

void Grid::mousePressEvent(QMouseEvent *event)
{
    qDebug() << "点击了位置" << getposX() << getpoxY() << "backType为" << backType
             << "gridType为" << gridType << "girdstate为"  << gridState
             << "influence为" << influence;


    if (gridState == 0) //普通状态
    {
        if (gridType == 1 && backType == 0)   //出生地上且无英雄 弹出创建选择对话框
        {
            qDebug() << "点击了出生区域，是否创建英雄";
            emit createHero();
        }
        //点击玩家英雄
        if (backType == 1 && influence == 1)
        {
            if (event->button() == Qt::LeftButton)
            {
                if (clickNum == 1)
                {
                    emit show(heroType);
                    clickNum++;
                }
                else if (clickNum == 2)
                {
                    emit reset(heroType);
                    clickNum = 1;
                }
            }
            else if (event->button() == Qt::RightButton)
            {
                if(clickNum == 2)   //只有先把范围按出来才能操作
                {
                    emit clickTheHero(heroType);
                }
            }
        }

        //点击AI英雄
        if (backType == 1 && influence == 2)
        {
            if (clickNum == 1)
            {
                //qDebug() << "发信号了";
                emit showAI(heroType);
                clickNum++;
            }
            else if (clickNum == 2)
            {
                emit closeAI(heroType);
                clickNum = 1;
            }
        }


    }
    else if (gridState == 1)//浮起状态
    {
        if ((gridType == 3 || gridType == 2) && backType == 0)  //移动格或是攻击格但没英雄
        {
            //qDebug() << "发信号move";        //只发了一遍信号,没问题
            emit moveTarget();
        }
        if (gridType == 2 && backType == 1)   //攻击格且格上是enemy
        {
            if (influence == 2)
            {
                emit attackTarget();
            }
            else if (influence == 1)
            {
                emit cureTarget();
            }
        }
    }


}

void Grid::paintEvent(QPaintEvent* ev)
{
    //网格
    QPainter painter(this);
    QPen pen(Qt::SolidLine);

    if (gridType == 0)
    {
        pen.setColor(QColor(0, 0, 0));
        pen.setWidth(5);
        painter.setPen(pen);
    }
    else if (gridType == 1)  //出生地
    {
        pen.setColor(QColor(255,255,255));
        pen.setWidth(15);
        painter.setPen(pen);

    }
    else if (gridType == 2 && (backType == 0 || (backType == 1 && influence == 2)))     //攻击范围
    {

        pen.setColor(QColor(255,0,0));
        pen.setWidth(15);
        painter.setPen(pen);

    }
    else if (gridType == 3 && backType == 0)     //移动范围
    {
        pen.setColor(QColor(255,255,0));
        pen.setWidth(15);
        painter.setPen(pen);

    }

    painter.drawLine(0,0,0,79);
    painter.drawLine(0,0,79,0);
    painter.drawLine(0,79,79,79);
    painter.drawLine(79,0,79,79);

    //画英雄
    if (backType == 1)
    {
       // qDebug() << "进入画图";
        if (influence == 1)
        {
            if (heroType == 1)
            {
                //qDebug() << "正在画图";
                painter.drawPixmap(QRect(QPoint(posX, posY - 10),QSize(80, 80)),
                                   QPixmap("://picture/1.png"));
            }
            else if (heroType == 2)
            {
                //qDebug() << "正在画图";
                painter.drawPixmap(QRect(QPoint(posX, posY - 10),QSize(80, 80)),
                                   QPixmap("://picture/2.png"));
            }
            else if (heroType == 3)
            {
                //qDebug() << "正在画图";
                painter.drawPixmap(QRect(QPoint(posX, posY - 10),QSize(80, 80)),
                                   QPixmap("://picture/3.png"));
            }
            else if (heroType == 4)
            {
                painter.drawPixmap(QRect(QPoint(posX, posY - 10),QSize(80, 80)),
                                   QPixmap("://picture/16.png"));
            }
        }
        else if (influence == 2)    //画AI
        {
            if (heroType == 1)
            {
                //qDebug() << "正在画图";
                painter.drawPixmap(QRect(QPoint(posX - 10, posY - 3),QSize(80,80)),
                                   QPixmap("://picture/11.png"));
            }
            else if (heroType == 2)
            {
                //qDebug() << "正在画图";
                painter.drawPixmap(QRect(QPoint(posX - 10, posY - 3),QSize(80, 80)),
                                   QPixmap("://picture/9.png"));
            }
            else if (heroType == 3)
            {
                //qDebug() << "正在画图";
                painter.drawPixmap(QRect(QPoint(posX - 10, posY - 3),QSize(80, 80)),
                                   QPixmap("://picture/10.png"));
            }
            else if (heroType == 5)
            {
                //qDebug() << "正在画图";
                painter.drawPixmap(QRect(QPoint(posX - 10, posY - 3),QSize(80, 80)),
                                   QPixmap("://picture/15.png"));
            }
        }

    }
}

//void Grid::enterEvent(QEvent *event)
//{
//    qDebug() << "鼠标置于" << getposX() << getpoxY() << "之上" << "gridtype" << gridType;
//    if (backType == 1)      //英雄格
//    {
//        emit reshow();
//    }
//}

//void Grid::leaveEvent(QEvent *event)
//{
//    qDebug() << "鼠标离开" << getposX() << getpoxY();
//    if (backType == 1)
//    {
//        emit rehide();
//    }

//}

int Grid::getposX() const
{
    return posX;
}

int Grid::getpoxY() const
{
    return posY;
}

int Grid::getGridType() const
{
    return gridType;
}

void Grid::setGridType(int t)
{
    gridType = t;
    update();
}

int Grid::getBackType() const
{
    return backType;
}

void Grid::setBackType(int t)
{
    backType = t;
    update();
}

void Grid::setHeroType(int t)
{
    heroType = t;
    update();
}

int Grid::getHeroType() const
{
    return heroType;
}

void Grid::setGridState(int t)
{
    gridState = t;
    update();
}

int Grid::getGridState() const
{
    return gridState;
}

void Grid::setInfluence(int t)
{
    influence = t;
}

int Grid::getInfluence() const
{
    return influence;
}

void Grid::setDebuff(int t)
{
    debuff = t;
}

int Grid::getDebuff() const
{
    return debuff;
}

void Grid::setClickNum(int s)
{
    clickNum = s;
}

void Grid::setpos(int x, int y)
{
    posX = x;
    posY = y;
}
