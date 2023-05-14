#include "mymap.h"
#include <QPainter>
#include <QGridLayout>
#include <QDebug>
#include <QMessageBox>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include <math.h>
#include <QTimer>
#include <QLabel>
#include <QEventLoop>
#include <QPalette>
#include <QParallelAnimationGroup>
#include <QGraphicsOpacityEffect>

int min(int a,int b)
{
    return (a < b) ? a : b;
}

MyMap::MyMap(QWidget *parent) : QWidget(parent)
{

}


void MyMap::setPlayerHero(int num, QStringList str)
{
    player->setAllowHeroNumber(num);
    player->setAllowHeroType(str);

    qDebug() << "本局限用英雄" << num << str;
}



void MyMap::actAI()
{
    //先清屏
    closeAllScreen();

    //事件阻塞 等待本单位进攻动画结束
    QEventLoop loop;//定义一个新的事件循环
    QTimer::singleShot(1000, &loop, SLOT(quit()));//创建单次定时器，槽函数为事件循环的退出函数
    loop.exec();//事件循环开始执行，程序会卡在这里，直到定时时间到，本循环被退出
    //先move 能attack就attack

    //move
    double min = 100;  //最近单位的距离
    int x,y;    //目标单位坐标
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 15; j++)
        {
            if (grid[i][j]->getBackType() == 1 && grid[i][j]->getInfluence() == 1)
            {
                double tmp = enemy->cloest(j,i);
                if (tmp < min)
                {
                    min = tmp;
                    x = j;
                    y = i;
                }
            }
        }
    }

    for (auto it = enemy->myheroes.begin(); it != enemy->myheroes.end(); it++)
    {
        double min_ = 100;
        int tx,ty;
        int range = (*it)->getMR();
        if (grid[(*it)->getPoxY()][(*it)->getPosX()]->getDebuff() == 2) //减速格上范围减小
        {
            range = range - 2;
        }
        //找到移动范围内跟目标点最近的点
        for (int i = 0; i < 10; i++)
        {
            for (int j = 0; j < 15; j++)
            {
                if ((sqrt(pow((*it)->getPosX() - j, 2) + pow((*it)->getPoxY() - i, 2)) <= range)
                        && grid[i][j]->getBackType() == 0)
                {
                    double tmp = sqrt(pow(x - j, 2) + pow(y - i, 2));
                    if (tmp < min_)
                    {
                        min_ = tmp;
                        tx = j;
                        ty = i;
                    }
                }
            }
        }

        if (moveable((*it)->getPosX(),(*it)->getPoxY(),tx,ty))
        {
            animMove((*it)->getPosX(),(*it)->getPoxY(),tx,ty);
            //新位置画上英雄
            grid[ty][tx]->setBackType(1);
            grid[ty][tx]->setInfluence(2);
            grid[ty][tx]->setHeroType((*it)->getName());
            //原位置英雄删去
            grid[(*it)->getPoxY()][(*it)->getPosX()]->setBackType(0);
            grid[(*it)->getPoxY()][(*it)->getPosX()]->setInfluence(0);
            //更新英雄位置
            (*it)->setPos(tx,ty);
        }
    }

    //attack   
    for (auto it = enemy->myheroes.begin(); it != enemy->myheroes.end(); it++)
    {
        bool hadDone = false;
        //先搜索攻击范围内有无目标
        for (int i = 0; i < 10; i++)
        {
            for (int j = 0; j < 15; j++)
            {
                if (grid[(*it)->getPoxY()][(*it)->getPosX()]->getHeroType() == 1 ||
                        grid[(*it)->getPoxY()][(*it)->getPosX()]->getHeroType() == 2 ||
                        grid[(*it)->getPoxY()][(*it)->getPosX()]->getHeroType() == 5)
                {
                    if ((sqrt(pow((*it)->getPoxY() - i, 2) + pow((*it)->getPosX() - j, 2)) <= 1)
                            && (sqrt(pow((*it)->getPoxY() - i, 2) + pow((*it)->getPosX() - j, 2)) > 0)
                            && grid[i][j]->getBackType() == 1 && grid[i][j]->getInfluence() == 1
                            && hadDone == false)
                    {
                        hadDone = true;
                        //qDebug() << "进入判断" << grid[i][j]->getposX() << grid[i][j]->getpoxY();
                        //有目标 攻击
                        player->my(grid[i][j]->getHeroType())->minusrHP((*it)->getATK());
                        //攻击动画
                        animAttack((*it)->getPosX(),(*it)->getPoxY(),j,i);
                        //攻击音效
                        musicPlayer = new QMediaPlayer(this);//设置背景音乐
                        musicPlayer->setMedia(QUrl("qrc:/picture/attack2.m4a"));
                        musicPlayer->setVolume(50);//音量
                        musicPlayer->play();

                        QEventLoop loop;//定义一个新的事件循环
                        QTimer::singleShot(1000, &loop, SLOT(quit()));//创建单次定时器，槽函数为事件循环的退出函数
                        loop.exec();//事件循环开始执行，程序会卡在这里，直到定时时间到，本循环被退出

                        //判断反击
                        if (antiAttack((*it)->getPoxY(),(*it)->getPosX(),grid[i][j]->getHeroType(),i,j))
                        {
                            //自身受到反击,伤害为目标攻击力的一半
                            (*it)->minusrHP((player->my(grid[i][j]->getHeroType())->getATK()) / 2);
                            //反击动画
                            animAntiAttack(j,i,(*it)->getPosX(),(*it)->getPoxY());
                            //反击音效
                            musicPlayer->setMedia(QUrl("qrc:/picture/antiattack.m4a"));
                            musicPlayer->setVolume(70);//音量
                            musicPlayer->play();
                        }
                        else
                        {
                            //被攻击无法反击音效
                            musicPlayer->setMedia(QUrl("qrc:/picture/beattack.m4a"));
                            musicPlayer->setVolume(50);//音量
                            musicPlayer->play();
                        }

                        //事件阻塞 等待本单位进攻动画结束
                        QEventLoop loop1;//定义一个新的事件循环
                        QTimer::singleShot(800, &loop1, SLOT(quit()));//创建单次定时器，槽函数为事件循环的退出函数
                        loop1.exec();//事件循环开始执行，程序会卡在这里，直到定时时间到，本循环被退出


                        //判断有无英雄死亡
                        emit anyoneDie();
                    }
                }
                else if (grid[(*it)->getPoxY()][(*it)->getPosX()]->getHeroType() == 3)
                {
                    if ((sqrt(pow((*it)->getPoxY() - i, 2) + pow((*it)->getPosX() - j, 2)) <= 3)
                            && (sqrt(pow((*it)->getPoxY() - i, 2) + pow((*it)->getPosX() - j, 2)) > 0)
                            && grid[i][j]->getBackType() == 1 && grid[i][j]->getInfluence() == 1
                            && hadDone == false)
                    {
                        hadDone = true;
                        //qDebug() << "进入判断" << grid[i][j]->getposX() << grid[i][j]->getpoxY();
                        //有目标 攻击
                        player->my(grid[i][j]->getHeroType())->minusrHP((*it)->getATK());
                        //攻击动画
                        animAttack((*it)->getPosX(),(*it)->getPoxY(),j,i);
                        //攻击音效
                        musicPlayer = new QMediaPlayer(this);//设置背景音乐
                        musicPlayer->setMedia(QUrl("qrc:/picture/attack2.m4a"));
                        musicPlayer->setVolume(50);//音量
                        musicPlayer->play();

                        QEventLoop loop;//定义一个新的事件循环
                        QTimer::singleShot(1000, &loop, SLOT(quit()));//创建单次定时器，槽函数为事件循环的退出函数
                        loop.exec();//事件循环开始执行，程序会卡在这里，直到定时时间到，本循环被退出

                        //判断反击
                        if (antiAttack((*it)->getPoxY(),(*it)->getPosX(),grid[i][j]->getHeroType(),i,j))
                        {
                            //自身受到反击,伤害为目标攻击力的一半
                            (*it)->minusrHP((player->my(grid[i][j]->getHeroType())->getATK()) / 2);
                            //反击动画
                            animAntiAttack(j,i,(*it)->getPosX(),(*it)->getPoxY());
                            //反击音效
                            musicPlayer->setMedia(QUrl("qrc:/picture/antiattack.m4a"));
                            musicPlayer->setVolume(70);//音量
                            musicPlayer->play();
                        }
                        else
                        {
                            //被攻击无法反击音效
                            musicPlayer->setMedia(QUrl("qrc:/picture/beattack.m4a"));
                            musicPlayer->setVolume(50);//音量
                            musicPlayer->play();
                        }

                        //事件阻塞 等待本单位进攻动画结束
                        QEventLoop loop1;//定义一个新的事件循环
                        QTimer::singleShot(800, &loop1, SLOT(quit()));//创建单次定时器，槽函数为事件循环的退出函数
                        loop1.exec();//事件循环开始执行，程序会卡在这里，直到定时时间到，本循环被退出

                        //判断有无英雄死亡
                        emit anyoneDie();

                    }
                }
            }
        }


    }

    //事件阻塞 等待语音结束
    QEventLoop loop2;//定义一个新的事件循环
    QTimer::singleShot(1500, &loop2, SLOT(quit()));//创建单次定时器，槽函数为事件循环的退出函数
    loop2.exec();//事件循环开始执行，程序会卡在这里，直到定时时间到，本循环被退出

    emit changeTerm();
}

void MyMap::die()
{
    //玩家方
    for (auto it = player->myheroes.begin(); it != player->myheroes.end();)
    {
        //英雄阵亡 我方英雄数减少 图片去掉
        if ((*it)->getrHP() <= 0)
        {
            qDebug() << "玩家的" << grid[(*it)->getPoxY()][(*it)->getPosX()]->getHeroType()
                    << "死亡" << "当前剩余人数" << player->getCurrentHeroNum() - 1;
            //事件阻塞 等待音效结束
            QEventLoop loop;//定义一个新的事件循环
            QTimer::singleShot(2000, &loop, SLOT(quit()));//创建单次定时器，槽函数为事件循环的退出函数
            loop.exec();//事件循环开始执行，程序会卡在这里，直到定时时间到，本循环被退出

            //死亡音效
            musicPlayer = new QMediaPlayer(this);
            musicPlayer->setMedia(QUrl("qrc:/picture/die.m4a"));
            musicPlayer->setVolume(100);//音量
            musicPlayer->play();

            QEventLoop loop1;//定义一个新的事件循环
            QTimer::singleShot(2000, &loop1, SLOT(quit()));//创建单次定时器，槽函数为事件循环的退出函数
            loop1.exec();//事件循环开始执行，程序会卡在这里，直到定时时间到，本循环被退出

            player->setCurrentNum(player->getCurrentHeroNum() - 1);
            grid[(*it)->getPoxY()][(*it)->getPosX()]->setBackType(0);
            grid[(*it)->getPoxY()][(*it)->getPosX()]->setInfluence(0);
            //qDebug() << "运行到这";
            if (player->getCurrentHeroNum() > 0)
            {
                it = player->myheroes.erase(it);
                if (player->getCurrentHeroNum() == 1)
                {
                    //只剩一人音效
                    musicPlayer = new QMediaPlayer(this);
                    musicPlayer->setMedia(QUrl("qrc:/picture/lastone.m4a"));
                    musicPlayer->setVolume(50);//音量
                    musicPlayer->play();
                }
            }
            else if (player->getCurrentHeroNum() == 0)
            {
                //不用去掉了 游戏结束
                isGameOver();
                break;
            }
        }
        else
        {
            it++;
        }
    }

    //AI方
    for (auto it = enemy->myheroes.begin(); it != enemy->myheroes.end();)
    {
        //英雄阵亡 AI方英雄数减少 图片去掉
        if ((*it)->getrHP() <= 0)
        {
            qDebug() << "AI的" << grid[(*it)->getPoxY()][(*it)->getPosX()]->getHeroType()
                    << "死亡";

            //事件阻塞 等待音效结束
            QEventLoop loop;//定义一个新的事件循环
            QTimer::singleShot(2000, &loop, SLOT(quit()));//创建单次定时器，槽函数为事件循环的退出函数
            loop.exec();//事件循环开始执行，程序会卡在这里，直到定时时间到，本循环被退出

            //死亡音效
            musicPlayer = new QMediaPlayer(this);
            musicPlayer->setMedia(QUrl("qrc:/picture/die.m4a"));
            musicPlayer->setVolume(100);//音量
            musicPlayer->play();

            QEventLoop loop1;//定义一个新的事件循环
            QTimer::singleShot(2000, &loop1, SLOT(quit()));//创建单次定时器，槽函数为事件循环的退出函数
            loop1.exec();//事件循环开始执行，程序会卡在这里，直到定时时间到，本循环被退出

            enemy->setCurrentNum(enemy->getCurrentHeroNum() - 1);
            grid[(*it)->getPoxY()][(*it)->getPosX()]->setBackType(0);
            grid[(*it)->getPoxY()][(*it)->getPosX()]->setInfluence(0);
            if (enemy->getCurrentHeroNum() > 0)
            {
                it = enemy->myheroes.erase(it);
                if (enemy->getCurrentHeroNum() == 1)
                {
                    //只剩一人音效
                    musicPlayer = new QMediaPlayer(this);
                    musicPlayer->setMedia(QUrl("qrc:/picture/lastone.m4a"));
                    musicPlayer->setVolume(50);//音量
                    musicPlayer->play();
                }
            }
            else if (enemy->getCurrentHeroNum() == 0)
            {
                //不用去掉了 游戏结束
                isGameOver();
                break;
            }
        }
        else
        {
            it++;
        }
    }
}

void MyMap::isGameOver()
{
    //判断游戏是否结束
    if (player->getCurrentHeroNum() == 0)//玩家输
    {
        QEventLoop loop1;//定义一个新的事件循环
        QTimer::singleShot(2000, &loop1, SLOT(quit()));//创建单次定时器，槽函数为事件循环的退出函数
        loop1.exec();//事件循环开始执行，程序会卡在这里，直到定时时间到，本循环被退出

        musicPlayer = new QMediaPlayer(this);//设置背景音乐
        musicPlayer->setMedia(QUrl("qrc:/picture/gameover.m4a"));
        musicPlayer->setVolume(40);//音量
        musicPlayer->play();

        QMessageBox::information(this, "GAMEOVER", "You lose");
        emit gameOver();
    }
    else if (enemy->getCurrentHeroNum() == 0)//玩家赢
    {
        QEventLoop loop1;//定义一个新的事件循环
        QTimer::singleShot(2000, &loop1, SLOT(quit()));//创建单次定时器，槽函数为事件循环的退出函数
        loop1.exec();//事件循环开始执行，程序会卡在这里，直到定时时间到，本循环被退出

        musicPlayer = new QMediaPlayer(this);//设置背景音乐
        musicPlayer->setMedia(QUrl("qrc:/picture/win.m4a"));
        musicPlayer->setVolume(40);//音量
        musicPlayer->play();

        QMessageBox::information(this, "GAMEOVER", "You win");
        emit gameOver();
    }
}

bool MyMap::moveable(int startx, int starty, int endx, int endy)
{
    flag = false;
    dijkstra(startx,starty,endx,endy);

    return flag;
}

void MyMap::dijkstra(int sx, int sy, int ex, int ey)
{
    int dist[10][15];
    bool visited[10][15];
    int prex[10][15];
    int prey[10][15];

    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 15; j++)
        {
            //直接后继
            if (sqrt(pow(j - sx, 2) + pow(i - sy, 2)) == 1 && grid[i][j]->getBackType() == 0)
            {
                dist[i][j] = 1;
                prex[i][j] = sx;
                prey[i][j] = sy;
            }
            else dist[i][j] = 200;
            visited[i][j] = false;
        }
    }
    dist[sy][sx] = 0;
    visited[sy][sx] = true;

    for (int k = 2; k < 150; k++)
    {
        //找dist中最小的
        int smallest = 200;
        int tmpx = 0,tmpy = 0;
        for (int i = 0; i < 10; i++)
        {
            for (int j = 0; j < 15; j++)
            {
                if (dist[i][j] < smallest && visited[i][j] == false)
                {
                    smallest = dist[i][j];
                    tmpx = j;
                    tmpy = i;
                }
            }
        }
        visited[tmpy][tmpx] = true;

        //找tmp的直接后继
        for (int i = 0; i < 10; i++)
        {
            for (int j = 0; j < 15; j++)
            {
                //直接后继 且未被搜过
                if (sqrt(pow(j - tmpx, 2) + pow(i - tmpy, 2)) == 1 && visited[i][j] == false
                        && grid[i][j]->getBackType() == 0)
                {
                    dist[i][j] = min(dist[i][j],dist[tmpy][tmpx] + 1);
                    prex[i][j] = tmpx;
                    prey[i][j] = tmpy;
                }
            }
        }
    }

    //dijkstra结束
    if (dist[ey][ex] != 200)
    {
        flag = true;
    }
}

void MyMap::closeAllScreen()
{
    //玩家
    for (auto it = player->myheroes.begin(); it != player->myheroes.end(); it++)
    {
        if ((*it)->getScreen() == true)
            (*it)->closeData();
    }
    //AI
    for (auto it = enemy->myheroes.begin(); it != enemy->myheroes.end(); it++)
    {
        if ((*it)->getScreen() == true)
            (*it)->closeData();
    }
}

void MyMap::setMR(int x, int y, int range)
{
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 15; j++)
        {
            if (sqrt(pow(x - i, 2) + pow(y - j, 2)) <= range)
            {
                grid[i][j]->setGridType(3);
            }
        }
    }
}

void MyMap::setAR(int x, int y, int range)
{
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 15; j++)
        {
            if (sqrt(pow(x - i, 2) + pow(y - j, 2)) <= range)
            {
                grid[i][j]->setGridType(2);
            }
        }
    }
}

bool MyMap::findTarget(int heroType, int x, int y)
{
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 15; j++)
        {
            if (heroType == 1 || heroType == 2)
            {
                if ((sqrt(pow(x - i, 2) + pow(y - j, 2)) <= 1) && (sqrt(pow(x - i, 2) + pow(y - j, 2)) > 0)
                        && grid[i][j]->getBackType() == 1 && grid[i][j]->getInfluence() == 2)
                {
                    //qDebug() << "进入判断" << grid[i][j]->getposX() << grid[i][j]->getpoxY();
                    return true;
                }
            }
            else if (heroType == 3)
            {
                if ((sqrt(pow(x - i, 2) + pow(y - j, 2)) <= 3) && (sqrt(pow(x - i, 2) + pow(y - j, 2)) > 0)
                        && grid[i][j]->getBackType() == 1 && grid[i][j]->getInfluence() == 2)
                {
                    //qDebug() << "进入判断" << grid[i][j]->getposX() << grid[i][j]->getpoxY();
                    return true;
                }
            }
            else if (heroType == 4)
            {
                if ((sqrt(pow(x - i, 2) + pow(y - j, 2)) <= 2) && (sqrt(pow(x - i, 2) + pow(y - j, 2)) > 0)
                        && grid[i][j]->getBackType() == 1 && grid[i][j]->getInfluence() == 1)
                {
                    //qDebug() << "进入判断" << grid[i][j]->getposX() << grid[i][j]->getpoxY();
                    return true;
                }
            }
        }
    }
    return false;
}

bool MyMap::antiAttack(int i1, int j1, int beattacker, int i2, int j2)
{
    //pastor不能反击
    if (beattacker == 4) return false;

    int r;
    if (beattacker == 1 || beattacker == 2 || beattacker == 5) r = 1;
    else if (beattacker == 3) r = 3;

    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 15; j++)
        {
            //beattacker的攻击范围内
            if (sqrt(pow(i - i2, 2) + pow(j - j2, 2)) <= r && sqrt(pow(i - i2, 2) + pow(j - j2, 2)) >= 0)
            {
                if (i == i1 && j == j1) return true;
            }
        }
    }
    return false;
}

void MyMap::animMove(int sx, int sy, int ex, int ey)
{
    int dist[10][15];
    bool visited[10][15];
    int prex[10][15];
    int prey[10][15];

    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 15; j++)
        {
            //直接后继
            if (sqrt(pow(j - sx, 2.0) + pow(i - sy, 2.0)) == 1.0 && grid[i][j]->getBackType() == 0)
            {
                dist[i][j] = 1;
                prex[i][j] = sx;
                prey[i][j] = sy;
            }
            else
            {
                dist[i][j] = 200;
                prex[i][j] = -1;
                prey[i][j] = -1;
            }
            visited[i][j] = false;

        }
    }
    dist[sy][sx] = 0;
    visited[sy][sx] = true;


    for (int k = 2; k < 150; k++)
    {
        //找dist中最小的
        int smallest = 200;
        int tmpx = 0,tmpy = 0;
        for (int i = 0; i < 10; i++)
        {
            for (int j = 0; j < 15; j++)
            {
                if (dist[i][j] < smallest && visited[i][j] == false)
                {
                    smallest = dist[i][j];
                    tmpx = j;
                    tmpy = i;
                }
            }
        }
        visited[tmpy][tmpx] = true;

        //找tmp的直接后继
        for (int i = 0; i < 10; i++)
        {
            for (int j = 0; j < 15; j++)
            {
                //直接后继 且未被搜过
                if (sqrt(pow(j - tmpx, 2.0) + pow(i - tmpy, 2.0)) == 1.0 && visited[i][j] == false
                        && grid[i][j]->getBackType() == 0)
                {
                    if (dist[i][j] <= dist[tmpy][tmpx] + 1)
                    {

                    }
                    else
                    {
                        dist[i][j] = dist[tmpy][tmpx] + 1;
                        prex[i][j] = tmpx;
                        prey[i][j] = tmpy;
                    }
                }
            }
        }

    }

//    for (int i = 0; i < 10; i++)
//        for (int j = 0;j < 15; j++)
//            qDebug() << "dist[" << i << "][" << j << "]" << dist[i][j] << visited[i][j] << prey[i][j] << prex[i][j];


    int path[150][2] = {0};
    int num = 0;
    int x = prex[ey][ex];
    int y = prey[ey][ex];
    //qDebug() << sy << sx;
    for (int k = 0; k < 150; k++)
    {

        path[num][0] = x;
        path[num][1] = y;
        num++;
        if (x == sx && y == sy) break;
        x = prex[path[num - 1][1]][path[num - 1][0]];
        y = prey[path[num - 1][1]][path[num - 1][0]];

    }


    //动画
    grid[sy][sx]->setGridType(0);
    QPropertyAnimation * pWidgetProcessUp = new QPropertyAnimation(grid[sy][sx], "geometry");
    pWidgetProcessUp->setDuration(1000);//设置动画执行时间，单位毫秒
    pWidgetProcessUp->setStartValue(QRect(sx * 80, sy * 80, 80, 80));//初始值
    pWidgetProcessUp->setKeyValueAt(0,QRect(sx * 80, sy * 80, 80, 80));
    for (int g = num - 2; g >= 0; g--)
    {
        //qDebug() << path[g][0] << path[g][1] << num << double((1.0 / num) * (num - g - 1));
        pWidgetProcessUp->setKeyValueAt(double((1.0 / num) * (num - g - 1)),QRect(path[g][0] * 80, path[g][1] * 80, 80, 80));
    }
    pWidgetProcessUp->setEndValue(QRect(ex * 80, ey * 80, 80, 80));//结束值
    pWidgetProcessUp->setEasingCurve(QEasingCurve::Linear);//设置速度曲线
    pWidgetProcessUp->start(QAbstractAnimation::DeleteWhenStopped);

    //事件阻塞 等待本单位移动动画结束
    QEventLoop loop;//定义一个新的事件循环
    QTimer::singleShot(1000, &loop, SLOT(quit()));//创建单次定时器，槽函数为事件循环的退出函数
    loop.exec();//事件循环开始执行，程序会卡在这里，直到定时时间到，本循环被退出

    QPropertyAnimation * pWidgetProcessUp2 = new QPropertyAnimation(grid[sy][sx], "geometry");
    pWidgetProcessUp2->setDuration(1);//设置动画执行时间，单位毫秒
    pWidgetProcessUp2->setStartValue(QRect(ex * 80, ey * 80, 80, 80));//初始值
    pWidgetProcessUp2->setEndValue(QRect(sx * 80, sy * 80, 80, 80));//结束值
    pWidgetProcessUp2->setEasingCurve(QEasingCurve::Linear);//设置速度曲线
    pWidgetProcessUp2->start(QAbstractAnimation::DeleteWhenStopped);

}

void MyMap::animAttack(int sx, int sy, int ex, int ey)
{
    QSequentialAnimationGroup *pPosGroup = new QSequentialAnimationGroup(this);

    QPropertyAnimation * pWidgetProcessUp = new QPropertyAnimation(grid[sy][sx], "geometry");
    QPropertyAnimation * pWidgetProcessUp2 = new QPropertyAnimation(grid[sy][sx], "geometry");


    pWidgetProcessUp->setDuration(500);//设置动画执行时间，单位毫秒
    pWidgetProcessUp2->setDuration(500);//设置动画执行时间，单位毫秒


    pWidgetProcessUp->setStartValue(QRect(sx * 80, sy * 80, 80, 80));//初始值
    pWidgetProcessUp->setEndValue(QRect(ex * 80, ey * 80, 80, 80));//结束值

    pWidgetProcessUp2->setStartValue(QRect(ex * 80, ey * 80, 80, 80));//初始值
    pWidgetProcessUp2->setEndValue(QRect(sx * 80, sy * 80, 80, 80));//结束值


    pWidgetProcessUp->setEasingCurve(QEasingCurve::Linear);//设置速度曲线
    pWidgetProcessUp2->setEasingCurve(QEasingCurve::Linear);//设置速度曲线


    pPosGroup->addAnimation(pWidgetProcessUp);
    pPosGroup->addAnimation(pWidgetProcessUp2);


    pPosGroup->start(QAbstractAnimation::DeleteWhenStopped);

}

void MyMap::animAntiAttack(int sx, int sy, int ex, int ey)
{
    QSequentialAnimationGroup *pPosGroup = new QSequentialAnimationGroup(this);

    QPropertyAnimation * pWidgetProcessUp = new QPropertyAnimation(grid[sy][sx], "geometry");
    QPropertyAnimation * pWidgetProcessUp2 = new QPropertyAnimation(grid[sy][sx], "geometry");


    pWidgetProcessUp->setDuration(500);//设置动画执行时间，单位毫秒
    pWidgetProcessUp2->setDuration(500);//设置动画执行时间，单位毫秒


    pWidgetProcessUp->setStartValue(QRect(sx * 80, sy * 80, 80, 80));//初始值
    pWidgetProcessUp->setEndValue(QRect(ex * 80, ey * 80, 80, 80));//结束值

    pWidgetProcessUp2->setStartValue(QRect(ex * 80, ey * 80, 80, 80));//初始值
    pWidgetProcessUp2->setEndValue(QRect(sx * 80, sy * 80, 80, 80));//结束值


    pWidgetProcessUp->setEasingCurve(QEasingCurve::Linear);//设置速度曲线
    pWidgetProcessUp2->setEasingCurve(QEasingCurve::Linear);//设置速度曲线


    pPosGroup->addAnimation(pWidgetProcessUp);
    pPosGroup->addAnimation(pWidgetProcessUp2);


    pPosGroup->start(QAbstractAnimation::DeleteWhenStopped);
}

void MyMap::animCure(int sx, int sy, int ex, int ey)
{
    QParallelAnimationGroup* anim = new QParallelAnimationGroup(this);
    QGraphicsOpacityEffect* pButtonOpacity1 = new QGraphicsOpacityEffect(this);
    pButtonOpacity1->setOpacity(1);
    grid[sy][sx]->setGraphicsEffect(pButtonOpacity1);
    QGraphicsOpacityEffect* pButtonOpacity2 = new QGraphicsOpacityEffect(this);
    pButtonOpacity2->setOpacity(1);
    grid[ey][ex]->setGraphicsEffect(pButtonOpacity2);

    QPropertyAnimation *animation1 = new QPropertyAnimation(pButtonOpacity1, "opacity");
    QPropertyAnimation *animation2 = new QPropertyAnimation(pButtonOpacity2, "opacity");

    animation1->setDuration(1500);
    animation2->setDuration(1500);

    animation1->setStartValue(1);
    animation1->setKeyValueAt(0.5,0.5);
    animation1->setEndValue(1);

    animation2->setStartValue(1);
    animation2->setKeyValueAt(0.5,0.5);
    animation2->setEndValue(1);

    anim->addAnimation(animation1);
    anim->addAnimation(animation2);

    anim->start(QAbstractAnimation::DeleteWhenStopped);
}






