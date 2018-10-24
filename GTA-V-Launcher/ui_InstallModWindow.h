/********************************************************************************
** Form generated from reading UI file 'InstallModWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_INSTALLMODWINDOW_H
#define UI_INSTALLMODWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_InstallModWindow
{
public:
    QVBoxLayout *verticalLayout_4;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_4;
    QLineEdit *modFileEdit;
    QPushButton *selectFileButton;
    QTreeView *filesInZipList;
    QHBoxLayout *horizontalLayout_5;
    QSpacerItem *horizontalSpacer;
    QPushButton *validateButton;

    void setupUi(QWidget *InstallModWindow)
    {
        if (InstallModWindow->objectName().isEmpty())
            InstallModWindow->setObjectName(QStringLiteral("InstallModWindow"));
        InstallModWindow->resize(480, 360);
        verticalLayout_4 = new QVBoxLayout(InstallModWindow);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        modFileEdit = new QLineEdit(InstallModWindow);
        modFileEdit->setObjectName(QStringLiteral("modFileEdit"));

        horizontalLayout_4->addWidget(modFileEdit);

        selectFileButton = new QPushButton(InstallModWindow);
        selectFileButton->setObjectName(QStringLiteral("selectFileButton"));

        horizontalLayout_4->addWidget(selectFileButton);


        verticalLayout_3->addLayout(horizontalLayout_4);


        verticalLayout_4->addLayout(verticalLayout_3);

        filesInZipList = new QTreeView(InstallModWindow);
        filesInZipList->setObjectName(QStringLiteral("filesInZipList"));
        filesInZipList->setEnabled(true);

        verticalLayout_4->addWidget(filesInZipList);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer);

        validateButton = new QPushButton(InstallModWindow);
        validateButton->setObjectName(QStringLiteral("validateButton"));

        horizontalLayout_5->addWidget(validateButton);


        verticalLayout_4->addLayout(horizontalLayout_5);


        retranslateUi(InstallModWindow);

        QMetaObject::connectSlotsByName(InstallModWindow);
    } // setupUi

    void retranslateUi(QWidget *InstallModWindow)
    {
        InstallModWindow->setWindowTitle(QApplication::translate("InstallModWindow", "Form", nullptr));
        selectFileButton->setText(QApplication::translate("InstallModWindow", "PushButton", nullptr));
        validateButton->setText(QApplication::translate("InstallModWindow", "PushButton", nullptr));
    } // retranslateUi

};

namespace Ui {
    class InstallModWindow: public Ui_InstallModWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INSTALLMODWINDOW_H
