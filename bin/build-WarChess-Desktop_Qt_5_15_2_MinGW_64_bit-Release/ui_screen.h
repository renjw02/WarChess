/********************************************************************************
** Form generated from reading UI file 'screen.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SCREEN_H
#define UI_SCREEN_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Screen
{
public:
    QLabel *labelPictrue;
    QLabel *labelName;
    QProgressBar *progressBarHP;
    QLabel *labelHP;
    QProgressBar *progressBarES;
    QLabel *labelES;

    void setupUi(QWidget *Screen)
    {
        if (Screen->objectName().isEmpty())
            Screen->setObjectName(QString::fromUtf8("Screen"));
        Screen->resize(330, 200);
        labelPictrue = new QLabel(Screen);
        labelPictrue->setObjectName(QString::fromUtf8("labelPictrue"));
        labelPictrue->setGeometry(QRect(10, 30, 100, 100));
        labelName = new QLabel(Screen);
        labelName->setObjectName(QString::fromUtf8("labelName"));
        labelName->setGeometry(QRect(10, 135, 130, 35));
        progressBarHP = new QProgressBar(Screen);
        progressBarHP->setObjectName(QString::fromUtf8("progressBarHP"));
        progressBarHP->setGeometry(QRect(120, 30, 110, 35));
        progressBarHP->setValue(24);
        labelHP = new QLabel(Screen);
        labelHP->setObjectName(QString::fromUtf8("labelHP"));
        labelHP->setGeometry(QRect(240, 30, 90, 35));
        progressBarES = new QProgressBar(Screen);
        progressBarES->setObjectName(QString::fromUtf8("progressBarES"));
        progressBarES->setGeometry(QRect(120, 95, 110, 35));
        progressBarES->setValue(24);
        labelES = new QLabel(Screen);
        labelES->setObjectName(QString::fromUtf8("labelES"));
        labelES->setGeometry(QRect(240, 95, 70, 35));

        retranslateUi(Screen);

        QMetaObject::connectSlotsByName(Screen);
    } // setupUi

    void retranslateUi(QWidget *Screen)
    {
        Screen->setWindowTitle(QCoreApplication::translate("Screen", "Form", nullptr));
        labelPictrue->setText(QCoreApplication::translate("Screen", "TextLabel", nullptr));
        labelName->setText(QCoreApplication::translate("Screen", "TextLabel", nullptr));
        labelHP->setText(QCoreApplication::translate("Screen", "TextLabel", nullptr));
        labelES->setText(QCoreApplication::translate("Screen", "TextLabel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Screen: public Ui_Screen {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SCREEN_H
