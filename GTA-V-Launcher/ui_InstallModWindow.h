/********************************************************************************
** Form generated from reading UI file 'InstallModWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
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
            InstallModWindow->setObjectName(QString::fromUtf8("InstallModWindow"));
        InstallModWindow->resize(480, 360);
        verticalLayout_4 = new QVBoxLayout(InstallModWindow);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        modFileEdit = new QLineEdit(InstallModWindow);
        modFileEdit->setObjectName(QString::fromUtf8("modFileEdit"));

        horizontalLayout_4->addWidget(modFileEdit);

        selectFileButton = new QPushButton(InstallModWindow);
        selectFileButton->setObjectName(QString::fromUtf8("selectFileButton"));

        horizontalLayout_4->addWidget(selectFileButton);


        verticalLayout_3->addLayout(horizontalLayout_4);


        verticalLayout_4->addLayout(verticalLayout_3);

        filesInZipList = new QTreeView(InstallModWindow);
        filesInZipList->setObjectName(QString::fromUtf8("filesInZipList"));
        filesInZipList->setEnabled(true);

        verticalLayout_4->addWidget(filesInZipList);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer);

        validateButton = new QPushButton(InstallModWindow);
        validateButton->setObjectName(QString::fromUtf8("validateButton"));

        horizontalLayout_5->addWidget(validateButton);


        verticalLayout_4->addLayout(horizontalLayout_5);


        retranslateUi(InstallModWindow);

        QMetaObject::connectSlotsByName(InstallModWindow);
    } // setupUi

    void retranslateUi(QWidget *InstallModWindow)
    {
        InstallModWindow->setWindowTitle(QApplication::translate("InstallModWindow", "Install a mod (BETA)", nullptr));
        selectFileButton->setText(QApplication::translate("InstallModWindow", "Select a mod file (*.asi, *.dll, *.zip, *.rar)", nullptr));
        validateButton->setText(QApplication::translate("InstallModWindow", "Confirm", nullptr));
    } // retranslateUi

};

namespace Ui {
    class InstallModWindow: public Ui_InstallModWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INSTALLMODWINDOW_H
