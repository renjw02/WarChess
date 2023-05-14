#ifndef SCREEN_H
#define SCREEN_H

#include <QWidget>

namespace Ui {
class Screen;
}

class Screen : public QWidget
{
    Q_OBJECT

public:
    explicit Screen(int HP,int rHP,int ES,int name,int influence,QWidget *parent = nullptr);
    ~Screen();



private:
    Ui::Screen *ui;

    int name;
    int HP;
    int rHP;
    int ES;
};

#endif // SCREEN_H
