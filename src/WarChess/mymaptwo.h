#ifndef MYMAPTWO_H
#define MYMAPTWO_H

#include "mymap.h"
#include <QWidget>

class MyMapTwo : public MyMap
{
    Q_OBJECT
public:
    explicit MyMapTwo(QWidget *parent = nullptr);
    //画地图maptwo
    void paintEvent(QPaintEvent *event) override;

    //AI创建
    void initAI() override;

    //点击英雄 不同地图不同
    void clickHero(int, int, int) override;

    //创建英雄 不同地图不同
    void createHero(int, int) override;

    void startGame() override;

    void reset() override;

signals:

public slots:
    void enterThisMap();
};

#endif // MYMAPTWO_H
