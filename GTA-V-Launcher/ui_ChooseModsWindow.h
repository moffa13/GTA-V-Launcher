/********************************************************************************
** Form generated from reading UI file 'ChooseModsWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHOOSEMODSWINDOW_H
#define UI_CHOOSEMODSWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QListView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ChooseModsWindow
{
public:
    QVBoxLayout *verticalLayout;
    QListView *listViewEnabled;
    QHBoxLayout *horizontalLayout;
    QHBoxLayout *horizontalLayout_2;
    QHBoxLayout *horizontalLayout_4;
    QPushButton *setEnabledButton;
    QPushButton *enableAllButton;
    QPushButton *installModButton;
    QPushButton *deleteModButton;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *disableAllButton;
    QPushButton *setDisabledButton;
    QListView *listViewDisabled;
    QHBoxLayout *horizontalLayout_5;
    QSpacerItem *horizontalSpacer;
    QPushButton *resetConfigButton;
    QPushButton *validateButton;

    void setupUi(QWidget *ChooseModsWindow)
    {
        if (ChooseModsWindow->objectName().isEmpty())
            ChooseModsWindow->setObjectName(QString::fromUtf8("ChooseModsWindow"));
        ChooseModsWindow->resize(645, 478);
        verticalLayout = new QVBoxLayout(ChooseModsWindow);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        listViewEnabled = new QListView(ChooseModsWindow);
        listViewEnabled->setObjectName(QString::fromUtf8("listViewEnabled"));

        verticalLayout->addWidget(listViewEnabled);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        setEnabledButton = new QPushButton(ChooseModsWindow);
        setEnabledButton->setObjectName(QString::fromUtf8("setEnabledButton"));

        horizontalLayout_4->addWidget(setEnabledButton);

        enableAllButton = new QPushButton(ChooseModsWindow);
        enableAllButton->setObjectName(QString::fromUtf8("enableAllButton"));

        horizontalLayout_4->addWidget(enableAllButton);


        horizontalLayout_2->addLayout(horizontalLayout_4);

        installModButton = new QPushButton(ChooseModsWindow);
        installModButton->setObjectName(QString::fromUtf8("installModButton"));

        horizontalLayout_2->addWidget(installModButton);

        deleteModButton = new QPushButton(ChooseModsWindow);
        deleteModButton->setObjectName(QString::fromUtf8("deleteModButton"));

        horizontalLayout_2->addWidget(deleteModButton);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        disableAllButton = new QPushButton(ChooseModsWindow);
        disableAllButton->setObjectName(QString::fromUtf8("disableAllButton"));

        horizontalLayout_3->addWidget(disableAllButton);

        setDisabledButton = new QPushButton(ChooseModsWindow);
        setDisabledButton->setObjectName(QString::fromUtf8("setDisabledButton"));

        horizontalLayout_3->addWidget(setDisabledButton);


        horizontalLayout_2->addLayout(horizontalLayout_3);


        horizontalLayout->addLayout(horizontalLayout_2);


        verticalLayout->addLayout(horizontalLayout);

        listViewDisabled = new QListView(ChooseModsWindow);
        listViewDisabled->setObjectName(QString::fromUtf8("listViewDisabled"));

        verticalLayout->addWidget(listViewDisabled);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer);

        resetConfigButton = new QPushButton(ChooseModsWindow);
        resetConfigButton->setObjectName(QString::fromUtf8("resetConfigButton"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(resetConfigButton->sizePolicy().hasHeightForWidth());
        resetConfigButton->setSizePolicy(sizePolicy);

        horizontalLayout_5->addWidget(resetConfigButton);

        validateButton = new QPushButton(ChooseModsWindow);
        validateButton->setObjectName(QString::fromUtf8("validateButton"));

        horizontalLayout_5->addWidget(validateButton);


        verticalLayout->addLayout(horizontalLayout_5);


        retranslateUi(ChooseModsWindow);

        QMetaObject::connectSlotsByName(ChooseModsWindow);
    } // setupUi

    void retranslateUi(QWidget *ChooseModsWindow)
    {
        ChooseModsWindow->setWindowTitle(QApplication::translate("ChooseModsWindow", "Select active mods", nullptr));
        setEnabledButton->setText(QApplication::translate("ChooseModsWindow", "Set enabled", nullptr));
        enableAllButton->setText(QApplication::translate("ChooseModsWindow", "Enable all", nullptr));
        installModButton->setText(QApplication::translate("ChooseModsWindow", "Install a mod", nullptr));
        deleteModButton->setText(QApplication::translate("ChooseModsWindow", "Delete mod", nullptr));
        disableAllButton->setText(QApplication::translate("ChooseModsWindow", "Disable all", nullptr));
        setDisabledButton->setText(QApplication::translate("ChooseModsWindow", "Set disabled", nullptr));
        resetConfigButton->setText(QApplication::translate("ChooseModsWindow", "Reset from config", nullptr));
        validateButton->setText(QApplication::translate("ChooseModsWindow", "Confirm", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ChooseModsWindow: public Ui_ChooseModsWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHOOSEMODSWINDOW_H
