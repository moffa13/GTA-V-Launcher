/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QPushButton *playOnline;
    QPushButton *playMods;
    QPushButton *chooseMods;
    QLabel *byLabel;
    QPushButton *openOptionsButton;
    QLabel *gtaVersionLabel;

    void setupUi(QWidget *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(960, 600);
        MainWindow->setMinimumSize(QSize(960, 600));
        MainWindow->setMaximumSize(QSize(960, 600));
        playOnline = new QPushButton(MainWindow);
        playOnline->setObjectName(QString::fromUtf8("playOnline"));
        playOnline->setGeometry(QRect(700, 438, 200, 110));
        playMods = new QPushButton(MainWindow);
        playMods->setObjectName(QString::fromUtf8("playMods"));
        playMods->setGeometry(QRect(481, 438, 200, 80));
        chooseMods = new QPushButton(MainWindow);
        chooseMods->setObjectName(QString::fromUtf8("chooseMods"));
        chooseMods->setGeometry(QRect(481, 519, 200, 29));
        byLabel = new QLabel(MainWindow);
        byLabel->setObjectName(QString::fromUtf8("byLabel"));
        byLabel->setGeometry(QRect(10, 580, 471, 16));
        openOptionsButton = new QPushButton(MainWindow);
        openOptionsButton->setObjectName(QString::fromUtf8("openOptionsButton"));
        openOptionsButton->setGeometry(QRect(15, 15, 153, 57));
        gtaVersionLabel = new QLabel(MainWindow);
        gtaVersionLabel->setObjectName(QString::fromUtf8("gtaVersionLabel"));
        gtaVersionLabel->setGeometry(QRect(606, 585, 351, 20));
        gtaVersionLabel->setAlignment(Qt::AlignBottom|Qt::AlignRight|Qt::AlignTrailing);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QWidget *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Form", nullptr));
        playOnline->setText(QApplication::translate("MainWindow", "Play GTA V Online\n"
" (Mods disabled)", nullptr));
        playMods->setText(QApplication::translate("MainWindow", "Play GTA V", nullptr));
        chooseMods->setText(QApplication::translate("MainWindow", "Choose mods", nullptr));
        byLabel->setText(QApplication::translate("MainWindow", "Made By Moffa13 @ moffa13.com", nullptr));
        openOptionsButton->setText(QApplication::translate("MainWindow", "Settings", nullptr));
        gtaVersionLabel->setText(QApplication::translate("MainWindow", "GTA V", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
