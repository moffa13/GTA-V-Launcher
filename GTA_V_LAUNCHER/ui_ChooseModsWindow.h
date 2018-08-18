/********************************************************************************
** Form generated from reading UI file 'ChooseModsWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHOOSEMODSWINDOW_H
#define UI_CHOOSEMODSWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
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
            ChooseModsWindow->setObjectName(QStringLiteral("ChooseModsWindow"));
        ChooseModsWindow->resize(645, 478);
        verticalLayout = new QVBoxLayout(ChooseModsWindow);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        listViewEnabled = new QListView(ChooseModsWindow);
        listViewEnabled->setObjectName(QStringLiteral("listViewEnabled"));

        verticalLayout->addWidget(listViewEnabled);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        setEnabledButton = new QPushButton(ChooseModsWindow);
        setEnabledButton->setObjectName(QStringLiteral("setEnabledButton"));

        horizontalLayout_4->addWidget(setEnabledButton);

        enableAllButton = new QPushButton(ChooseModsWindow);
        enableAllButton->setObjectName(QStringLiteral("enableAllButton"));

        horizontalLayout_4->addWidget(enableAllButton);


        horizontalLayout_2->addLayout(horizontalLayout_4);

        deleteModButton = new QPushButton(ChooseModsWindow);
        deleteModButton->setObjectName(QStringLiteral("deleteModButton"));

        horizontalLayout_2->addWidget(deleteModButton);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        disableAllButton = new QPushButton(ChooseModsWindow);
        disableAllButton->setObjectName(QStringLiteral("disableAllButton"));

        horizontalLayout_3->addWidget(disableAllButton);

        setDisabledButton = new QPushButton(ChooseModsWindow);
        setDisabledButton->setObjectName(QStringLiteral("setDisabledButton"));

        horizontalLayout_3->addWidget(setDisabledButton);


        horizontalLayout_2->addLayout(horizontalLayout_3);


        horizontalLayout->addLayout(horizontalLayout_2);


        verticalLayout->addLayout(horizontalLayout);

        listViewDisabled = new QListView(ChooseModsWindow);
        listViewDisabled->setObjectName(QStringLiteral("listViewDisabled"));

        verticalLayout->addWidget(listViewDisabled);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer);

        resetConfigButton = new QPushButton(ChooseModsWindow);
        resetConfigButton->setObjectName(QStringLiteral("resetConfigButton"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(resetConfigButton->sizePolicy().hasHeightForWidth());
        resetConfigButton->setSizePolicy(sizePolicy);

        horizontalLayout_5->addWidget(resetConfigButton);

        validateButton = new QPushButton(ChooseModsWindow);
        validateButton->setObjectName(QStringLiteral("validateButton"));

        horizontalLayout_5->addWidget(validateButton);


        verticalLayout->addLayout(horizontalLayout_5);


        retranslateUi(ChooseModsWindow);

        QMetaObject::connectSlotsByName(ChooseModsWindow);
    } // setupUi

    void retranslateUi(QWidget *ChooseModsWindow)
    {
        ChooseModsWindow->setWindowTitle(QApplication::translate("ChooseModsWindow", "ChooseModsWindow", Q_NULLPTR));
        setEnabledButton->setText(QApplication::translate("ChooseModsWindow", "PushButton", Q_NULLPTR));
        enableAllButton->setText(QApplication::translate("ChooseModsWindow", "PushButton", Q_NULLPTR));
        deleteModButton->setText(QApplication::translate("ChooseModsWindow", "PushButton", Q_NULLPTR));
        disableAllButton->setText(QApplication::translate("ChooseModsWindow", "PushButton", Q_NULLPTR));
        setDisabledButton->setText(QApplication::translate("ChooseModsWindow", "PushButton", Q_NULLPTR));
        resetConfigButton->setText(QApplication::translate("ChooseModsWindow", "PushButton", Q_NULLPTR));
        validateButton->setText(QApplication::translate("ChooseModsWindow", "PushButton", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class ChooseModsWindow: public Ui_ChooseModsWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHOOSEMODSWINDOW_H
