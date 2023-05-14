#ifndef MYMAPONE_H
#define MYMAPONE_H

#include "mymap.h"
#include <QWidget>

class MyMapOne : public MyMap
{
    Q_OBJECT
public:
    explicit MyMapOne(QWidget *parent = nullptr);
    //画地图mapone
    void paintEvent(QPaintEvent *event) override;

    void startGame() override;

    //AI创建
    void initAI() override;

    //创建英雄 不同地图限制不同
    void createHero(int, int) override;

    //清除范围显示 重新画上出生点 不同地图不同
    //还要把所有的screen关掉
    void reset() override;

    //点击英雄 不同地图不同
    void clickHero(int, int, int) override;

    //用于调试 输出全场英雄血量信息
    void cout();



private:

signals:

public slots:
    void enterThisMap();

    void leaveMap();

};

#endif // MYMAPONE_H
