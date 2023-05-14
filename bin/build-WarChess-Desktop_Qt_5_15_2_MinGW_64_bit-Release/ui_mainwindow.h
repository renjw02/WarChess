/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "mymapone.h"
#include "mymaptwo.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionSet;
    QAction *actionHelp;
    QAction *actionRule;
    QWidget *centralwidget;
    QStackedWidget *stackedWidget;
    QWidget *page;
    QHBoxLayout *horizontalLayout_5;
    QWidget *widget_2;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_2;
    QWidget *widget_3;
    QHBoxLayout *horizontalLayout_4;
    QPushButton *btnLevelOne;
    QPushButton *btnLevelTwo;
    QWidget *page_2;
    MyMapOne *widgetMapOne;
    QWidget *page_3;
    MyMapTwo *widgetMapTwo;
    QToolBar *toolBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1200, 842);
        actionSet = new QAction(MainWindow);
        actionSet->setObjectName(QString::fromUtf8("actionSet"));
        actionHelp = new QAction(MainWindow);
        actionHelp->setObjectName(QString::fromUtf8("actionHelp"));
        actionRule = new QAction(MainWindow);
        actionRule->setObjectName(QString::fromUtf8("actionRule"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        stackedWidget = new QStackedWidget(centralwidget);
        stackedWidget->setObjectName(QString::fromUtf8("stackedWidget"));
        stackedWidget->setGeometry(QRect(0, 0, 1200, 800));
        stackedWidget->setMinimumSize(QSize(1200, 800));
        stackedWidget->setMaximumSize(QSize(1200, 800));
        page = new QWidget();
        page->setObjectName(QString::fromUtf8("page"));
        horizontalLayout_5 = new QHBoxLayout(page);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        widget_2 = new QWidget(page);
        widget_2->setObjectName(QString::fromUtf8("widget_2"));
        widget_2->setMinimumSize(QSize(800, 600));
        widget_2->setMaximumSize(QSize(800, 600));
        verticalLayout_3 = new QVBoxLayout(widget_2);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        label_2 = new QLabel(widget_2);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        QFont font;
        font.setPointSize(30);
        font.setBold(true);
        font.setItalic(false);
        font.setWeight(75);
        label_2->setFont(font);
        label_2->setAlignment(Qt::AlignCenter);

        verticalLayout_3->addWidget(label_2);

        widget_3 = new QWidget(widget_2);
        widget_3->setObjectName(QString::fromUtf8("widget_3"));
        horizontalLayout_4 = new QHBoxLayout(widget_3);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        btnLevelOne = new QPushButton(widget_3);
        btnLevelOne->setObjectName(QString::fromUtf8("btnLevelOne"));

        horizontalLayout_4->addWidget(btnLevelOne);

        btnLevelTwo = new QPushButton(widget_3);
        btnLevelTwo->setObjectName(QString::fromUtf8("btnLevelTwo"));

        horizontalLayout_4->addWidget(btnLevelTwo);


        verticalLayout_3->addWidget(widget_3);


        horizontalLayout_5->addWidget(widget_2);

        stackedWidget->addWidget(page);
        page_2 = new QWidget();
        page_2->setObjectName(QString::fromUtf8("page_2"));
        widgetMapOne = new MyMapOne(page_2);
        widgetMapOne->setObjectName(QString::fromUtf8("widgetMapOne"));
        widgetMapOne->setGeometry(QRect(0, 0, 1200, 800));
        widgetMapOne->setMinimumSize(QSize(1200, 800));
        widgetMapOne->setMaximumSize(QSize(1200, 800));
        stackedWidget->addWidget(page_2);
        page_3 = new QWidget();
        page_3->setObjectName(QString::fromUtf8("page_3"));
        widgetMapTwo = new MyMapTwo(page_3);
        widgetMapTwo->setObjectName(QString::fromUtf8("widgetMapTwo"));
        widgetMapTwo->setGeometry(QRect(0, 0, 1200, 800));
        widgetMapTwo->setMinimumSize(QSize(1200, 800));
        widgetMapTwo->setMaximumSize(QSize(1200, 800));
        stackedWidget->addWidget(page_3);
        MainWindow->setCentralWidget(centralwidget);
        toolBar = new QToolBar(MainWindow);
        toolBar->setObjectName(QString::fromUtf8("toolBar"));
        toolBar->setAllowedAreas(Qt::TopToolBarArea);
        toolBar->setFloatable(false);
        MainWindow->addToolBar(Qt::TopToolBarArea, toolBar);

        toolBar->addAction(actionSet);
        toolBar->addSeparator();
        toolBar->addAction(actionHelp);
        toolBar->addSeparator();
        toolBar->addAction(actionRule);

        retranslateUi(MainWindow);

        stackedWidget->setCurrentIndex(2);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        actionSet->setText(QCoreApplication::translate("MainWindow", "\350\256\276\347\275\256", nullptr));
        actionHelp->setText(QCoreApplication::translate("MainWindow", "\345\270\256\345\212\251", nullptr));
        actionRule->setText(QCoreApplication::translate("MainWindow", "\350\247\204\345\210\231", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "\346\210\230\346\243\213\346\270\270\346\210\217", nullptr));
        btnLevelOne->setText(QCoreApplication::translate("MainWindow", "LEVEL 1", nullptr));
        btnLevelTwo->setText(QCoreApplication::translate("MainWindow", "LEVEL 2", nullptr));
        toolBar->setWindowTitle(QCoreApplication::translate("MainWindow", "toolBar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
