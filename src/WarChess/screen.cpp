#include "screen.h"
#include "ui_screen.h"
#include <QDebug>

Screen::Screen(int HP,int rHP,int ES,int name,int influence,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Screen)
{
    ui->setupUi(this);
    setFixedSize(330,200);
    QPalette pa;
    pa.setColor(QPalette::WindowText,Qt::white);
    QFont ft;
    ft.setPointSize(10);
    ft.setBold(true);

    ui->progressBarHP->setFormat("%v");
    ui->progressBarHP->setMinimum(0);  // 最小值
    ui->progressBarHP->setMaximum(HP);  // 最大值
    ui->progressBarHP->setValue(rHP);  // 当前进度
    ui->progressBarHP->setTextVisible(false);
    ui->labelHP->setText(QString(QString::number(rHP) + "/" + QString::number(HP))); 
    ui->labelHP->setPalette(pa);
    ui->labelHP->setFont(ft);

    ui->progressBarES->setMaximum(ES);
    ui->progressBarES->setValue(ES);
    ui->progressBarES->setTextVisible(false);
    ui->labelES->setText(QString::number(ES));
    ui->labelES->setPalette(pa);
    ui->labelES->setFont(ft);

    if (name == 1)
    {
        ui->labelName->setText("knight");
        if (influence == 1)
        {
            ui->labelPictrue->setPixmap(QPixmap("://picture/1.png"));
        }
        else if (influence == 2)
        {
            ui->labelPictrue->setPixmap(QPixmap("://picture/11.png"));
        }
    }
    else if (name == 2)
    {
        ui->labelName->setText("paladin");
        if (influence == 1)
        {
            ui->labelPictrue->setPixmap(QPixmap("://picture/2.png"));
        }
        else if (influence == 2)
        {
            ui->labelPictrue->setPixmap(QPixmap("://picture/9.png"));
        }
    }
    else if (name == 3)
    {
        ui->labelName->setText("genie");
        if (influence == 1)
        {
            ui->labelPictrue->setPixmap(QPixmap("://picture/3.png"));
        }
        else if (influence == 2)
        {
            ui->labelPictrue->setPixmap(QPixmap("://picture/10.png"));
        }
    }
    else if (name == 4)
    {
        ui->labelName->setText("pastor");
        ui->labelPictrue->setPixmap(QPixmap("://picture/16.png"));
    }
    else if (name == 5)
    {
        ui->labelName->setText("berserker");
        ui->labelPictrue->setPixmap(QPixmap("://picture/15.png"));
    }

    ui->labelName->setPalette(pa);
    ui->labelName->setFont(ft);
    ui->labelPictrue->setScaledContents(true);
}

Screen::~Screen()
{
    delete ui;
}


