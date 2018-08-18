/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.11.1
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

    void setupUi(QWidget *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(960, 600);
        MainWindow->setMinimumSize(QSize(960, 600));
        MainWindow->setMaximumSize(QSize(960, 600));
        playOnline = new QPushButton(MainWindow);
        playOnline->setObjectName(QStringLiteral("playOnline"));
        playOnline->setGeometry(QRect(700, 438, 200, 110));
        playMods = new QPushButton(MainWindow);
        playMods->setObjectName(QStringLiteral("playMods"));
        playMods->setGeometry(QRect(481, 438, 200, 80));
        chooseMods = new QPushButton(MainWindow);
        chooseMods->setObjectName(QStringLiteral("chooseMods"));
        chooseMods->setGeometry(QRect(481, 519, 200, 29));
        byLabel = new QLabel(MainWindow);
        byLabel->setObjectName(QStringLiteral("byLabel"));
        byLabel->setGeometry(QRect(10, 580, 47, 13));
        openOptionsButton = new QPushButton(MainWindow);
        openOptionsButton->setObjectName(QStringLiteral("openOptionsButton"));
        openOptionsButton->setGeometry(QRect(15, 15, 153, 57));

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QWidget *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Form", nullptr));
        playOnline->setText(QApplication::translate("MainWindow", "PushButton", nullptr));
        playMods->setText(QApplication::translate("MainWindow", "PushButton", nullptr));
        chooseMods->setText(QApplication::translate("MainWindow", "PushButton", nullptr));
        byLabel->setText(QString());
        openOptionsButton->setText(QApplication::translate("MainWindow", "PushButton", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
