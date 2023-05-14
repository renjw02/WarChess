#ifndef GRID_H
#define GRID_H

#include <QWidget>

class Grid : public QWidget
{
    Q_OBJECT
public:
    explicit Grid(QWidget *parent = nullptr);
    static int level;

    int getposX() const;
    int getpoxY() const;
    void setpos(int x, int y);
    void setGridType(int t);
    int getGridType() const;
    void setBackType(int t);
    int getBackType() const;
    void setHeroType(int t);
    int getHeroType() const;
    void setInfluence(int t);
    int getInfluence() const;
    void setGridState(int t);
    int getGridState() const;
    void setDebuff(int t);
    int getDebuff() const;
    void setClickNum(int);

    void mousePressEvent(QMouseEvent *event) override;

     void paintEvent(QPaintEvent*) override;
//置于角色上才触发
//   void enterEvent(QEvent *event) override;
//   void leaveEvent(QEvent *event) override;

private:
    int posX,posY;
    int clickNum;   //第几次点击本格子
    int gridType;   //默认为0 出生地为1 攻击范围为2 移动范围为3
    int backType;   //背景格类型 普通为0 英雄为1 阻隔为2
    int debuff;     //格子负面效果 没有为0 扣血为1 减速为2
    int heroType;   //英雄类型 knight 1 paladin 2 genie 3 pastor 4 Berserker 5
    int gridState;  //格子状态 普通为0 浮起为1
    int influence;  //所属势力 默认为0 玩家为1 AI为2

signals:
    void createHero();
    //void reshow(int);
    void show(int);
    void clickTheHero(int);
    void reset(int);
    void moveTarget();
    void cureTarget();
    void attackTarget();
    void showAI(int);
    void closeAI(int);
};

#endif // GRID_H
