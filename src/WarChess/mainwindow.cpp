#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mymap.h"
#include <QGroupBox>
#include <QAction>
#include <QTreeWidget>
#include <QGroupBox>
#include <QGridLayout>
#include <QRadioButton>
#include <QPainter>
#include <QDebug>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QListWidget>
#include <QListWidgetItem>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    setFixedSize(1200,842);
    //connect 设置 和 帮助
    setToolBar(ui,this);

    //开始进入首页
    ui->stackedWidget->setCurrentIndex(0);

    //点击level 1进入第一幅图
    connect(ui->btnLevelOne,&QPushButton::clicked,[=](){
        ui->stackedWidget->setCurrentIndex(1);
        emit enterMap1();

    });

    //点击level 2进入第一幅图
    connect(ui->btnLevelTwo,&QPushButton::clicked,[=](){
        ui->stackedWidget->setCurrentIndex(2);
        emit enterMap2();
        //qDebug() << "点击level 2";
        //mymap = new MyMapOne(this);
    });

    connect(this,SIGNAL(enterMap1()),ui->widgetMapOne,SLOT(enterThisMap()));
    connect(this,SIGNAL(enterMap2()),ui->widgetMapTwo,SLOT(enterThisMap()));

    connect(this,&MainWindow::leaveGame,[=](){
       //ui->widgetMapOne->leaveMap();
    });

    connect(ui->widgetMapOne,&MyMap::gameOver,this,&MainWindow::gameover);
    connect(ui->widgetMapTwo,&MyMap::gameOver,this,&MainWindow::gameover);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::setToolBar(Ui::MainWindow* ui,QWidget* parent)
{
    //设置
    QGroupBox* groupBoxSet = new QGroupBox("设置",parent);
    groupBoxSet->setFixedSize(800,600);
    groupBoxSet->move(200,100);
    QGridLayout* layout = new QGridLayout;
    groupBoxSet->setLayout(layout);
    QLabel* label = new QLabel("音效");
    QRadioButton* rbtnOpen = new QRadioButton("开");
    QRadioButton* rbtnClose = new QRadioButton("关");
    QPushButton* btnReturn = new QPushButton("返回");
    QPushButton* btnContinue = new QPushButton("继续");
    layout->addWidget(label,0,0);
    layout->addWidget(rbtnOpen,0,1);
    layout->addWidget(rbtnClose,0,2);
    layout->addWidget(btnReturn,1,0,1,3);
    layout->addWidget(btnContinue,2,0,1,3);

    rbtnOpen->setChecked(true);
    groupBoxSet->setStyleSheet("background-color: rgb(195, 195, 195)");
    groupBoxSet->setVisible(false);
    groupBoxSet->setAttribute(Qt::WA_DeleteOnClose);
    //点设置弹出设置窗口
    QObject::connect(ui->actionSet,&QAction::triggered,groupBoxSet,&QGroupBox::show);

    //点继续关闭窗口
    QObject::connect(btnContinue,&QPushButton::clicked,groupBoxSet,&QGroupBox::hide);

    //点返回返回主界面
    QObject::connect(btnReturn,&QPushButton::clicked,[=](){
        ui->stackedWidget->setCurrentIndex(0);
        emit gameover();
    });

    //音效的开关
    //音乐
    QMediaPlaylist *playlist = new QMediaPlaylist(this);
    playlist->addMedia(QUrl("qrc:/picture/mymusic.mp3"));
    playlist->setPlaybackMode(QMediaPlaylist::Loop);

    QMediaPlayer *playerM = new QMediaPlayer(this);//设置背景音乐
    playerM->setPlaylist(playlist);
    playerM->setVolume(30);//音量
    playerM->play();
    QObject::connect(rbtnOpen,&QRadioButton::toggled,playerM,&QMediaPlayer::play);
    QObject::connect(rbtnClose,&QRadioButton::toggled,playerM,&QMediaPlayer::stop);

    //帮助
    QTreeWidget* treeWidget = new QTreeWidget(parent);
    treeWidget->setFixedSize(800,600);
    treeWidget->move(200,100);
    treeWidget->setVisible(false);
    treeWidget->setAttribute(Qt::WA_DeleteOnClose);
    treeWidget->setHeaderLabels(QStringList() << "英雄" << "英雄介绍");
    QTreeWidgetItem* knight = new QTreeWidgetItem(QStringList() << "骑士");
    QTreeWidgetItem* paladin = new QTreeWidgetItem(QStringList() << "圣骑士");
    QTreeWidgetItem* genie = new QTreeWidgetItem(QStringList() << "精灵");
    QTreeWidgetItem* pastor = new QTreeWidgetItem(QStringList() << "牧师");
    QTreeWidgetItem* berserker = new QTreeWidgetItem(QStringList() << "狂战士");
    treeWidget->addTopLevelItem(knight);
    treeWidget->addTopLevelItem(paladin);
    treeWidget->addTopLevelItem(genie);
    treeWidget->addTopLevelItem(pastor);
    treeWidget->addTopLevelItem(berserker);

    QTreeWidgetItem* treK1 = new QTreeWidgetItem(QStringList() << " " << "血量 7，攻击力 4，");
    QTreeWidgetItem* treK2 = new QTreeWidgetItem(QStringList() << " " << "移动 5，攻击范围 1,");
    QTreeWidgetItem* treK3 = new QTreeWidgetItem(QStringList() << " " << "护盾 1");
    knight->addChild(treK1);
    knight->addChild(treK2);
    knight->addChild(treK3);

    QTreeWidgetItem* treP1 = new QTreeWidgetItem(QStringList() << " " << "血量 10，攻击力 3，");
    QTreeWidgetItem* treP2 = new QTreeWidgetItem(QStringList() << " " << "移动 3，攻击范围 1,");
    QTreeWidgetItem* treP3 = new QTreeWidgetItem(QStringList() << " " << "护盾 2");
    paladin->addChild(treP1);
    paladin->addChild(treP2);
    paladin->addChild(treP3);

    QTreeWidgetItem* treG1 = new QTreeWidgetItem(QStringList() << " " << "血量 6，攻击力 4，");
    QTreeWidgetItem* treG2 = new QTreeWidgetItem(QStringList() << " " << "移动 4，攻击范围 3,");
    QTreeWidgetItem* treG3 = new QTreeWidgetItem(QStringList() << " " << "护盾 1");
    genie->addChild(treG1);
    genie->addChild(treG2);
    genie->addChild(treG3);

    QTreeWidgetItem* trePa1 = new QTreeWidgetItem(QStringList() << " " << "血量 3，治疗 1，");
    QTreeWidgetItem* trePa2 = new QTreeWidgetItem(QStringList() << " " << "移动 3，攻击范围 2,");
    QTreeWidgetItem* trePa3 = new QTreeWidgetItem(QStringList() << " " << "护盾 2");
    pastor->addChild(trePa1);
    pastor->addChild(trePa2);
    pastor->addChild(trePa3);

    QTreeWidgetItem* treB1 = new QTreeWidgetItem(QStringList() << " " << "血量 2，攻击力 999，");
    QTreeWidgetItem* treB2 = new QTreeWidgetItem(QStringList() << " " << "移动 5，攻击范围 1,");
    QTreeWidgetItem* treB3 = new QTreeWidgetItem(QStringList() << " " << "护盾 1");
    berserker->addChild(treB1);
    berserker->addChild(treB2);
    berserker->addChild(treB3);

    //点帮助弹出帮助文档,再点关闭
    QObject::connect(ui->actionHelp,&QAction::triggered,treeWidget,[=](){
        if (treeWidget->isHidden())
        {
            treeWidget->show();
        }
        else
        {
            treeWidget->hide();
        }
    });


    //规则
    QTreeWidget* treeWidget1 = new QTreeWidget(parent);
    treeWidget1->setFixedSize(800,600);
    treeWidget1->move(200,100);
    treeWidget1->setVisible(false);
    treeWidget1->setAttribute(Qt::WA_DeleteOnClose);
    treeWidget1->setHeaderLabels(QStringList() << "战棋");
    QTreeWidgetItem* rule = new QTreeWidgetItem(QStringList() << "游戏规则");
    QTreeWidgetItem* rules1 = new QTreeWidgetItem(QStringList() << "  本游戏为回合制游戏，一个大回合内玩家和AI依");
    QTreeWidgetItem* rules2 = new QTreeWidgetItem(QStringList() << "次进行回合。玩家回合内，玩家所有英雄可选择");
    QTreeWidgetItem* rules3 = new QTreeWidgetItem(QStringList() << "move、attack/cure、rest。选择rest则该英雄本");
    QTreeWidgetItem* rules4 = new QTreeWidgetItem(QStringList() << "回合不能再行动。attack/pure之后无法再移动。");
    treeWidget1->addTopLevelItem(rule);
    rule->addChild(rules1);
    rule->addChild(rules2);
    rule->addChild(rules3);
    rule->addChild(rules4);
    QTreeWidgetItem* operation = new QTreeWidgetItem(QStringList() << "操作");
    QTreeWidgetItem* oper1 = new QTreeWidgetItem(QStringList() << "  地图上白色框为出生点，红色为攻击范围，黄色为");
    QTreeWidgetItem* oper2 = new QTreeWidgetItem(QStringList() << "移动范围。出生点创建完英雄后，左键第一次点击");
    QTreeWidgetItem* oper3 = new QTreeWidgetItem(QStringList() << "英雄显示信息和范围，左键第二次点击英雄关闭显");
    QTreeWidgetItem* oper4 = new QTreeWidgetItem(QStringList() << "示。通过左键显示范围后，右键点击英雄进行该英");
    QTreeWidgetItem* oper5 = new QTreeWidgetItem(QStringList() << "雄本回合的操作。");
    treeWidget1->addTopLevelItem(operation);
    operation->addChild(oper1);
    operation->addChild(oper2);
    operation->addChild(oper3);
    operation->addChild(oper4);
    operation->addChild(oper5);


    //点rule弹出,再点关闭
    QObject::connect(ui->actionRule,&QAction::triggered,treeWidget1,[=](){
        if (treeWidget1->isHidden())
        {
            treeWidget1->show();
        }
        else
        {
            treeWidget1->hide();
        }
    });

}
