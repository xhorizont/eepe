/********************************************************************************
** Form generated from reading UI file 'modeleditwindow.ui'
**
** Created: Thu Sep 23 08:41:55 2010
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MODELEDITWINDOW_H
#define UI_MODELEDITWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QTabWidget>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ModelEdit
{
public:
    QWidget *centralwidget;
    QTabWidget *tabWidget;
    QWidget *tab;
    QWidget *tab_2;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *ModelEdit)
    {
        if (ModelEdit->objectName().isEmpty())
            ModelEdit->setObjectName(QString::fromUtf8("ModelEdit"));
        ModelEdit->resize(800, 600);
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(ModelEdit->sizePolicy().hasHeightForWidth());
        ModelEdit->setSizePolicy(sizePolicy);
        centralwidget = new QWidget(ModelEdit);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setGeometry(QRect(0, 0, 801, 561));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(tabWidget->sizePolicy().hasHeightForWidth());
        tabWidget->setSizePolicy(sizePolicy1);
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        tabWidget->addTab(tab_2, QString());
        ModelEdit->setCentralWidget(centralwidget);
        menubar = new QMenuBar(ModelEdit);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 25));
        ModelEdit->setMenuBar(menubar);
        statusbar = new QStatusBar(ModelEdit);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        ModelEdit->setStatusBar(statusbar);

        retranslateUi(ModelEdit);

        QMetaObject::connectSlotsByName(ModelEdit);
    } // setupUi

    void retranslateUi(QMainWindow *ModelEdit)
    {
        ModelEdit->setWindowTitle(QApplication::translate("ModelEdit", "Edit Model", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("ModelEdit", "Tab 1", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("ModelEdit", "Tab 2", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ModelEdit: public Ui_ModelEdit {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MODELEDITWINDOW_H
