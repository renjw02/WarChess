#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "mymap.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setToolBar(Ui::MainWindow* ui,QWidget*);

signals:
    void enterMap1();
    void enterMap2();
    void leaveGame();
    void gameover();

private:
    Ui::MainWindow *ui;

    //选择地图的指针
    //MyMap* mymap;
};
#endif // MAINWINDOW_H
