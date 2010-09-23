/********************************************************************************
** Form generated from reading UI file 'generaleditwindow.ui'
**
** Created: Thu Sep 23 08:41:55 2010
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GENERALEDITWINDOW_H
#define UI_GENERALEDITWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_GeneralEdit
{
public:
    QWidget *centralwidget;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *GeneralEdit)
    {
        if (GeneralEdit->objectName().isEmpty())
            GeneralEdit->setObjectName(QString::fromUtf8("GeneralEdit"));
        GeneralEdit->resize(800, 600);
        centralwidget = new QWidget(GeneralEdit);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        GeneralEdit->setCentralWidget(centralwidget);
        menubar = new QMenuBar(GeneralEdit);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 25));
        GeneralEdit->setMenuBar(menubar);
        statusbar = new QStatusBar(GeneralEdit);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        GeneralEdit->setStatusBar(statusbar);

        retranslateUi(GeneralEdit);

        QMetaObject::connectSlotsByName(GeneralEdit);
    } // setupUi

    void retranslateUi(QMainWindow *GeneralEdit)
    {
        GeneralEdit->setWindowTitle(QApplication::translate("GeneralEdit", "Edit General Settings", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class GeneralEdit: public Ui_GeneralEdit {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GENERALEDITWINDOW_H
