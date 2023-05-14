#include "mymaptwo.h"
#include "mymap.h"
#include <QPainter>
#include <QGridLayout>
#include <QDialog>
#include <QPushButton>
#include <QMessageBox>
#include <QEventLoop>
#include <QTimer>

MyMapTwo::MyMapTwo(QWidget *parent) : MyMap(parent)
{
    roundNum = 1;
    currentPlayer = 0;
    qDebug() << "玩家回合" << "round" << roundNum;

    player = new Player1;
    enemy = new AI;

    //初始化网格
    QGridLayout* layout = new QGridLayout(this);
    layout->setMargin(0);
    layout->setSpacing(0);
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 15; j++)
        {
            grid[i][j] = new Grid;
            grid[i][j]->setFixedSize(80,80);
            layout->addWidget(grid[i][j],i,j);
            grid[i][j]->setpos(j,i);
        }
    }
    //设置出生区域
    grid[9][0]->setGridType(1);
    grid[8][0]->setGridType(1);
    grid[9][1]->setGridType(1);
    grid[8][1]->setGridType(1);
    grid[9][2]->setGridType(1);
    grid[8][2]->setGridType(1);
    grid[9][3]->setGridType(1);
    grid[8][3]->setGridType(1);


    //初始化AI
    initAI();

    //AI的显示信号
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 15; j++)
        {
            connect(grid[i][j],&Grid::showAI,this,[=](int heroType){
                closeAllScreen();
                enemy->my(heroType)->showData(2,this);
            });

            connect(grid[i][j],&Grid::closeAI,this,[=](int heroType){
                enemy->my(heroType)->closeData();
            });
        }
    }


    //接收初始化英雄信号
    for (int i = 8; i < 10; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            connect(grid[i][j],&Grid::createHero,this,[=](){
                createHero(i, j);
            });
        }
    }
    //英雄初始化完成


    //接受显示英雄移动范围MR和攻击范围AR信号 显示信息
    //接受重置信号
    //接受点击英雄信号
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 15; j++)
        {

            void(Grid::*sgn1)(int) = &Grid::show;
            void(Grid::*sgn2)(int) = &Grid::reset;
            connect(grid[i][j],sgn1,[=](int heroType){
                int mrange = player->my(heroType)->getMR();
                int arange = player->my(heroType)->getAR();
                //先清屏
                reset();
                closeAllScreen();
                //设置范围
                setMR(i,j,mrange);
                setAR(i,j,arange);
                player->my(heroType)->showData(1,this);

//                qDebug() << "左键第一次点击" << "hero" << heroType << "move" << player->my(heroType)->hadMove
//                         << "attack" << player->my(heroType)->hadAttack
//                         << "rest" << player->my(heroType)->hadRest;
//                qDebug() << "canact()" << player->canAct();
            });
            connect(grid[i][j],sgn2,[=](int heroType){
                reset();
                player->my(heroType)->closeData();
            });

            void(Grid::*sgnclk)(int) = &Grid::clickTheHero;
            connect(grid[i][j],sgnclk,[=](int heroType){
                clickHero(heroType,i,j);
                player->my(heroType)->closeData();
            });
        }
    }


    //是否交换攻守
    connect(this,&MyMap::asignal,[=](){
        //不能动了
        if (!player->canAct())
        {
            //qDebug() << "不能走";
            emit changeTerm();
            reset();
        }
    });

    //是否英雄阵亡
    connect(this,&MyMap::anyoneDie,[=](){
        die();
    });



    //轮到谁谁走
    connect(this,&MyMap::changeTerm,[=](){
       if (currentPlayer == 0)
       {
           //AI回合 禁用玩家所有操作
           //玩家回合结束 此时位于岩浆上的英雄全部受到伤害2点
           currentPlayer = 1;
           for (int i = 0; i < 10; i++)
           {
               for (int j = 0; j < 15; j++)
               {
                   grid[i][j]->setEnabled(false);
                   //玩家方的英雄 且背景为岩浆
                   if (grid[i][j]->getBackType() == 1 && grid[i][j]->getDebuff() == 1
                           && grid[i][j]->getInfluence() == 1)
                   {
                       player->my(grid[i][j]->getHeroType())->minusrHP(2);
                       //有伤害 判断有无英雄死亡
                       emit anyoneDie();
                   }
               }
           }

           qDebug() << "AI回合";
           actAI();
       }
       else if (currentPlayer == 1)
       {
           qDebug() << "AI回合结束";
           currentPlayer = 0;
           for (int i = 0; i < 10; i++)
           {
               for (int j = 0; j < 15; j++)
               {
                   grid[i][j]->setEnabled(true);
                   //AI方的英雄 且背景为岩浆
                   if (grid[i][j]->getBackType() == 1 && grid[i][j]->getDebuff() == 1
                           && grid[i][j]->getInfluence() == 2)
                   {
                       enemy->my(grid[i][j]->getHeroType())->minusrHP(2);
                       //有伤害 判断有无英雄死亡
                       emit anyoneDie();
                   }
               }
           }
           player->resetAction();
           roundNum++;
           qDebug() << "玩家回合" << "round" << roundNum;
       }
       //cout();
    });


}

void MyMapTwo::clickHero(int heroType, int x, int y)
{
    //qDebug() << "点击英雄准备操作";
    //不在connect里的只跑了一遍
    //弹窗询问下一步操作
    QDialog* dial = new QDialog(this);
    //dial->setAttribute(Qt::WA_DeleteOnClose);
    QHBoxLayout* layout = new QHBoxLayout(dial);
    dial->show();
    QPushButton* btnMove = new QPushButton("move",dial);
    QPushButton* btnAttack = new QPushButton("attack/cure",dial);
    QPushButton* btnRest = new QPushButton("rest",dial);
    layout->addWidget(btnMove);
    layout->addWidget(btnAttack);
    layout->addWidget(btnRest);


    //英雄没建完不弹窗
    if (!player->isfinish())
    {
        qDebug() << "没建完";
        dial->hide();
    }
    if (player->my(heroType)->hadMove)
    {
        btnMove->setEnabled(false);
    }
    if (player->my(heroType)->hadAttack)
    {
        btnAttack->setEnabled(false);
    }
    if (player->my(heroType)->hadRest)
    {
        btnRest->setEnabled(false);
    }


    //信号连接 点完之后要禁用
    connect(btnMove,&QPushButton::clicked,[=](){
        //所有移动范围内格子变为浮起
        //qDebug() << "点击move";
        //此处只跑了一遍
        for (int i = 0; i < 10; i++)
        {
            for (int j = 0; j < 15; j++)
            {
                if (grid[i][j]->getGridType() == 3 || grid[i][j]->getGridType() == 2)
                {
                    //qDebug() << "设置浮起";
                    grid[i][j]->setGridState(1);
                }
            }
        }

        //当前坐标（y，x） 即grid[x][y]
        //得到目标位置坐标
        for (int i = 0; i < 10; i++)
        {
            for (int j = 0; j < 15; j++)
            {
                disconnect(grid[i][j],&Grid::moveTarget,0,0);
                connect(grid[i][j],&Grid::moveTarget,[=](){
                    if (moveable(y,x,j,i))
                    {
                        //move动画
                        animMove(y,x,j,i);
                        //目标位置画上英雄
                        qDebug() << "move目标位置" << i << j;
                        grid[i][j]->setBackType(1);
                        grid[i][j]->setHeroType(heroType);
                        grid[i][j]->setInfluence(1);
                        //本回合移动了
                        player->my(heroType)->hadMove = true;
                        player->my(heroType)->setPos(j,i);
                        //move禁用
                        btnMove->setEnabled(false);
                        //关闭范围显示 关闭浮动
                        reset();
                        //原位置英雄删掉
                        grid[x][y]->setBackType(0);
                        grid[x][y]->setInfluence(0);

                        //若此时攻击范围内无目标
                        if (!findTarget(heroType,i,j))
                        {
                            player->my(heroType)->hadAttack = true;
                            player->my(heroType)->hadRest = true;
                        }

                        emit asignal();
                    }
                    else
                    {
                        QMessageBox::information(this, "attention", "You can not move there!");
                        //关闭范围显示 关闭浮动
                        reset();
                    }

                });

            }
        }



        //弹窗关闭
        dial->close();


    });

    connect(btnAttack,&QPushButton::clicked,[=](){
        //所有攻击范围内格子变为浮起
        //qDebug() << "点击move";
        for (int i = 0; i < 10; i++)
        {
            for (int j = 0; j < 15; j++)
            {
                if (grid[i][j]->getGridType() == 2)
                {
                    //qDebug() << "设置浮起";
                    grid[i][j]->setGridState(1);
                }
            }
        }
        //当前坐标（y，x） 即grid[x][y]
        //得到目标位置坐标

        //不是牧师
        if (heroType != 4)
        {
            for (int i = 0; i < 10; i++)
            {
                for (int j = 0; j < 15; j++)
                {
                    disconnect(grid[i][j],&Grid::attackTarget,0,0);
                    connect(grid[i][j],&Grid::attackTarget,[=](){
                        qDebug() << "attack目标位置" << i << j;

                        //攻击目标，目标扣血
                        enemy->my(grid[i][j]->getHeroType())->minusrHP(player->my(heroType)->getATK());
                        //攻击动画
                        animAttack(y,x,j,i);

                        //攻击音效
                        musicPlayer = new QMediaPlayer(this);//设置背景音乐
                        musicPlayer->setMedia(QUrl("qrc:/picture/attack2.m4a"));
                        musicPlayer->setVolume(50);//音量
                        musicPlayer->play();

                        //事件阻塞 等待本单位音效结束
                        QEventLoop loop;//定义一个新的事件循环
                        QTimer::singleShot(1000, &loop, SLOT(quit()));//创建单次定时器，槽函数为事件循环的退出函数
                        loop.exec();//事件循环开始执行，程序会卡在这里，直到定时时间到，本循环被退出


                        //判断反击
                        if (antiAttack(x,y,grid[i][j]->getHeroType(),i,j))
                        {
                            //自身受到反击,伤害为目标攻击力的一半
                            player->my(heroType)->minusrHP((enemy->my(grid[i][j]->getHeroType())->getATK()) / 2);
                            //反击动画
                            animAntiAttack(j,i,y,x);
                            //反击音效
                            musicPlayer->setMedia(QUrl("qrc:/picture/antiattack.m4a"));
                            musicPlayer->setVolume(50);//音量
                            musicPlayer->play();
                        }
                        else
                        {
                            //无法反击音效
                            musicPlayer->setMedia(QUrl("qrc:/picture/beattack.m4a"));
                            musicPlayer->setVolume(50);//音量
                            musicPlayer->play();
                        }


                        //判断有无英雄死亡
                        emit anyoneDie();


                        //
                        // cout();
                        //

                        //攻击结束 该英雄本轮结束
                        player->my(heroType)->hadAttack = true;
                        player->my(heroType)->hadMove = true;
                        player->my(heroType)->hadRest = true;
                        btnMove->setEnabled(false);
                        btnAttack->setEnabled(false);
                        btnRest->setEnabled(false);
                        //关闭范围显示 关闭浮动
                        reset();

                        //判断玩家回合是否结束
                        emit asignal();
                    });
                }
            }

        }
        //是牧师
        if (heroType == 4)
        {
            for (int i = 0; i < 10; i++)
            {
                for (int j = 0; j < 15; j++)
                {
                    disconnect(grid[i][j],&Grid::cureTarget,0,0);
                    connect(grid[i][j],&Grid::cureTarget,[=](){
                        qDebug() << "attack目标位置" << i << j;
                        //目标加血
                        player->my(grid[i][j]->getHeroType())->addrHP(2);
                        //自身回血
                        player->my(heroType)->addrHP(1);
                        //治疗动画
                        animCure(y,x,j,i);

                        //治疗结束 该英雄本轮结束
                        player->my(heroType)->hadAttack = true;
                        player->my(heroType)->hadMove = true;
                        player->my(heroType)->hadRest = true;
                        btnMove->setEnabled(false);
                        btnAttack->setEnabled(false);
                        btnRest->setEnabled(false);
                        //关闭范围显示 关闭浮动
                        reset();

                        //判断玩家回合是否结束
                        emit asignal();


                    });
                }
            }
        }


        //关闭弹窗
        dial->close();
    });

    connect(btnRest,&QPushButton::clicked,[=](){
        player->my(heroType)->hadRest = true;
        qDebug() << "点击rest 本轮休息";

        //休息音效
        musicPlayer = new QMediaPlayer(this);
        musicPlayer->setMedia(QUrl("qrc:/picture/rest.m4a"));
        musicPlayer->setVolume(50);//音量
        musicPlayer->play();

        btnMove->setEnabled(false);
        btnAttack->setEnabled(false);
        btnRest->setEnabled(false);
        player->my(heroType)->hadMove = true;
        player->my(heroType)->hadAttack = true;

        //关闭范围显示 关闭浮动
        reset();
        dial->close();

        emit asignal();
    });

    if (!findTarget(heroType,x,y))
    {
        btnAttack->setEnabled(false);
    }

    if (player->my(heroType)->hadRest && player->my(heroType)->hadAttack && player->my(heroType)->hadMove)
    {
        //qDebug() << "不能走";
        dial->hide();
    }

    qDebug() << "第二次点击" << "hero" << heroType << "move" << player->my(heroType)->hadMove
             << "attack" << player->my(heroType)->hadAttack
             << "rest" << player->my(heroType)->hadRest;
}

void MyMapTwo::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    //画出背景地图 15 * 10
    int x = 0,y = 0;
    int edgeLength = 80;
    //横线 11条
    for(int i = 0; i < 11; i++)
    {
        painter.drawLine(0, y + edgeLength * i, 1200, y + edgeLength * i);

    }
    //竖线 16条
    for(int i = 0; i < 16; i++)
    {
        painter.drawLine(x + edgeLength * i, 0, x + edgeLength * i, 800);
    }

    //在格子内加入图片，设置不同地形
    for (int i = 0; i < 10; i++)  //row
    {
        for (int j = 0; j < 15; j++)  //colomn
        {
            painter.drawPixmap(QRect(QPoint(j * 80, i * 80),QSize(edgeLength, edgeLength)),
                               QPixmap("://picture/14.png"));
            //设置阻隔地形
            if ((j == 0 && (i == 3 || i == 4 || i == 5)) || (i == 7 && (j == 0 || j == 1 || j == 2 || j == 3))
                    || (i == 0 && (j == 6 || j == 7 || j == 8)) || (i == 2 && (j == 10 || j == 11)) ||
                    (i == 5 && j == 11) || (i == 6 && (j == 10 || j == 11 || j == 12)) ||
                    (i == 9 && (j == 9 || j == 10)) || (i == 0 && j == 1) || (i == 8 && j == 5) ||
                    (i == 9 && (j == 5 || j == 6)))
            {
                painter.drawPixmap(QRect(QPoint(j * 80, i * 80),QSize(edgeLength, edgeLength)),
                                   QPixmap("://picture/13.png"));
                grid[i][j]->setBackType(2);
            }


            //设置扣血地形
            if ((i == 1 && j == 4) || (i == 2 && (j == 3 || j == 4 || j == 5)) || (i == 3 && (j == 5 || j == 6))
                    || (i == 4 && j == 8) || (i == 6 && j == 6) || (i == 7 && j == 7) ||
                    (i == 8 && j == 13) || (j == 2 && (i == 4 || i == 5)) || (i == 5 && j == 3))
            {
                    painter.drawPixmap(QRect(QPoint(j * 80, i * 80),QSize(edgeLength, edgeLength)),
                                       QPixmap("://picture/12.png"));
                    grid[i][j]->setDebuff(1);
            }

        }
     }

    //写上第几回合
    QFont font("宋体",20,QFont::Bold);
    painter.setPen(Qt::blue);
    painter.setFont(font);
    painter.drawText(500,60,QString("ROUND" + QString::number(roundNum)));
}

void MyMapTwo::initAI()
{
    grid[0][13]->setBackType(1);
    grid[0][13]->setInfluence(2);
    grid[0][13]->setHeroType(1);
    Knight* knightA = new Knight;
    enemy->addHero(knightA,13,0);
    grid[0][12]->setBackType(1);
    grid[0][12]->setInfluence(2);
    grid[0][12]->setHeroType(2);
    Paladin* paladinA = new Paladin;
    enemy->addHero(paladinA,12,0);
    grid[1][14]->setBackType(1);
    grid[1][14]->setInfluence(2);
    grid[1][14]->setHeroType(3);
    Genie* genieA = new Genie;
    enemy->addHero(genieA,14,1);
    grid[2][14]->setBackType(1);
    grid[2][14]->setInfluence(2);
    grid[2][14]->setHeroType(5);
    Berserker* berserkerA = new Berserker;
    enemy->addHero(berserkerA,14,2);
}


void MyMapTwo::createHero(int x, int y)
{
    //qDebug() << "调用createhero";
    QDialog* dial = new QDialog(this);
    //dial->setAttribute(Qt::WA_DeleteOnClose);
    QHBoxLayout* layout = new QHBoxLayout(dial);
    dial->show();
    QPushButton* btnKnight = new QPushButton("Knight",dial);
    QPushButton* btnPaladin = new QPushButton("Paladin",dial);
    QPushButton* btnGenie = new QPushButton("Genie",dial);
    QPushButton* btnPastor = new QPushButton("Pastor",dial);
    QPushButton* btnCancel = new QPushButton("取消创建",dial);
    layout->addWidget(btnKnight);
    layout->addWidget(btnPaladin);
    layout->addWidget(btnGenie);
    layout->addWidget(btnPastor);
    layout->addWidget(btnCancel);


    //信号连接
    connect(btnKnight,&QPushButton::clicked,[=](){
        //场上英雄增加
        //qDebug() << "点击创建knight";
        Knight* knight = new Knight;
        //每种英雄只能创建一个
        if (player->has(knight))
        {
            QMessageBox::information(this, "attention", "You already had a knight");
        }
        else
        {
            qDebug() << "创建了一个 knight";
            player->addHero(knight,y,x);

            //创建音效
            musicPlayer = new QMediaPlayer(this);//设置背景音乐
            musicPlayer->setMedia(QUrl("qrc:/picture/create.m4a"));
            musicPlayer->setVolume(40);//音量
            musicPlayer->play();

            //让grid生成英雄图像 将该格变为英雄格
            grid[x][y]->setBackType(1);
            grid[x][y]->setHeroType(1);
            grid[x][y]->setInfluence(1);
        }
        //关闭窗口
        dial->close();
    });


    connect(btnPaladin,&QPushButton::clicked,[=](){
        //场上英雄增加
        Paladin* paladin = new Paladin;
        if (player->has(paladin))
        {
            QMessageBox::information(this, "attention", "You already had a paladin");
        }
        else
        {
            qDebug() << "创建了一个 paladin";
            player->addHero(paladin,y,x);

            //创建音效
            musicPlayer = new QMediaPlayer(this);//设置背景音乐
            musicPlayer->setMedia(QUrl("qrc:/picture/create.m4a"));
            musicPlayer->setVolume(40);//音量
            musicPlayer->play();

            //让grid生成英雄图像 将该格变为英雄格
            grid[x][y]->setBackType(1);
            grid[x][y]->setHeroType(2);
            grid[x][y]->setInfluence(1);

        }
        //关闭窗口
        dial->close();
    });

    connect(btnGenie,&QPushButton::clicked,[=](){
        //场上英雄增加
        Genie* genie = new Genie;
        if (player->has(genie))
        {
            QMessageBox::information(this, "attention", "You already had a genie");
        }
        else
        {
            qDebug() << "创建了一个 genie";
            player->addHero(genie,y,x);

            //创建音效
            musicPlayer = new QMediaPlayer(this);//设置背景音乐
            musicPlayer->setMedia(QUrl("qrc:/picture/create.m4a"));
            musicPlayer->setVolume(40);//音量
            musicPlayer->play();

            //让grid生成英雄图像 将该格变为英雄格
            grid[x][y]->setBackType(1);
            grid[x][y]->setHeroType(3);
            grid[x][y]->setInfluence(1);

        }
        //关闭窗口
        dial->close();
    });


    connect(btnPastor,&QPushButton::clicked,[=](){
        //场上英雄增加
        Pastor* pastor = new Pastor;
        if (player->has(pastor))
        {
            QMessageBox::information(this, "attention", "You already had a pastor");
        }
        else
        {
            qDebug() << "创建了一个 pastor";
            player->addHero(pastor,y,x);

            //创建音效
            musicPlayer = new QMediaPlayer(this);//设置背景音乐
            musicPlayer->setMedia(QUrl("qrc:/picture/create.m4a"));
            musicPlayer->setVolume(40);//音量
            musicPlayer->play();

            //让grid生成英雄图像 将该格变为英雄格
            grid[x][y]->setBackType(1);
            grid[x][y]->setHeroType(4);
            grid[x][y]->setInfluence(1);

        }
        //关闭窗口
        dial->close();
    });

    connect(btnCancel,&QPushButton::clicked,[=](){
        qDebug() << "关闭选择英雄窗口";
        //关闭窗口
        dial->close();
    });

    //只能创建四个  创建完成
    if (player->isfinish())
    {
        //qDebug() << "进入判断";
        btnKnight->setEnabled(false);
        btnPaladin->setEnabled(false);
        btnGenie->setEnabled(false);
        btnPastor->setEnabled(false);
    }

}

void MyMapTwo::startGame()
{
    //--------------------level 2 允许英雄 4 类型 knight paladin genie pastor-----------------------
    qDebug() << "设置英雄数量和类型";
    setPlayerHero(4,QStringList() << "knight" << "paladin" << "genie" << "pastor");
    qDebug() << "完成设置英雄数量和类型";

}

void MyMapTwo::reset()
{
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 15; j++)
        {
            grid[i][j]->setGridType(0);
            grid[i][j]->setGridState(0);
            grid[i][j]->setClickNum(1);
        }
    }
    grid[9][0]->setGridType(1);
    grid[8][0]->setGridType(1);
    grid[9][1]->setGridType(1);
    grid[8][1]->setGridType(1);
    grid[9][2]->setGridType(1);
    grid[8][2]->setGridType(1);
    grid[9][3]->setGridType(1);
    grid[8][3]->setGridType(1);
}

void MyMapTwo::enterThisMap()
{
    //qDebug() << "进入第二幅图";
//    player = new Player1;
//    enemy = new AI;



    //进入地图，开始游戏
    qDebug() << "进入了地图2，游戏开始";
    startGame();
}
