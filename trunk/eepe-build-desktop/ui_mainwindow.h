/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Tue 21. Sep 09:54:16 2010
**      by: Qt User Interface Compiler version 4.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QListWidget>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_eePe
{
public:
    QAction *actionNew;
    QAction *actionSave;
    QAction *actionSave_As;
    QAction *actionQuit;
    QAction *actionOpen;
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QListWidget *listWidget;
    QMenuBar *menuBar;
    QMenu *menuFile;

    void setupUi(QMainWindow *eePe)
    {
        if (eePe->objectName().isEmpty())
            eePe->setObjectName(QString::fromUtf8("eePe"));
        eePe->resize(596, 491);
        actionNew = new QAction(eePe);
        actionNew->setObjectName(QString::fromUtf8("actionNew"));
        actionSave = new QAction(eePe);
        actionSave->setObjectName(QString::fromUtf8("actionSave"));
        actionSave_As = new QAction(eePe);
        actionSave_As->setObjectName(QString::fromUtf8("actionSave_As"));
        actionQuit = new QAction(eePe);
        actionQuit->setObjectName(QString::fromUtf8("actionQuit"));
        actionOpen = new QAction(eePe);
        actionOpen->setObjectName(QString::fromUtf8("actionOpen"));
        centralWidget = new QWidget(eePe);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        listWidget = new QListWidget(centralWidget);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));

        gridLayout->addWidget(listWidget, 0, 0, 1, 1);

        eePe->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(eePe);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 596, 20));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        eePe->setMenuBar(menuBar);

        menuBar->addAction(menuFile->menuAction());
        menuFile->addAction(actionNew);
        menuFile->addAction(actionOpen);
        menuFile->addAction(actionSave);
        menuFile->addAction(actionSave_As);
        menuFile->addSeparator();
        menuFile->addAction(actionQuit);

        retranslateUi(eePe);

        QMetaObject::connectSlotsByName(eePe);
    } // setupUi

    void retranslateUi(QMainWindow *eePe)
    {
        eePe->setWindowTitle(QApplication::translate("eePe", "MainWindow", 0, QApplication::UnicodeUTF8));
        actionNew->setText(QApplication::translate("eePe", "New", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionNew->setToolTip(QApplication::translate("eePe", "Create New EEPROM", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionNew->setShortcut(QApplication::translate("eePe", "Ctrl+N", 0, QApplication::UnicodeUTF8));
        actionSave->setText(QApplication::translate("eePe", "Save", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionSave->setToolTip(QApplication::translate("eePe", "Save EEPROM", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionSave->setShortcut(QApplication::translate("eePe", "Ctrl+S", 0, QApplication::UnicodeUTF8));
        actionSave_As->setText(QApplication::translate("eePe", "Save As", 0, QApplication::UnicodeUTF8));
        actionSave_As->setShortcut(QApplication::translate("eePe", "Ctrl+Shift+S", 0, QApplication::UnicodeUTF8));
        actionQuit->setText(QApplication::translate("eePe", "Quit", 0, QApplication::UnicodeUTF8));
        actionQuit->setShortcut(QApplication::translate("eePe", "Ctrl+Q", 0, QApplication::UnicodeUTF8));
        actionOpen->setText(QApplication::translate("eePe", "Open", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionOpen->setToolTip(QApplication::translate("eePe", "Open EEPROM", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionOpen->setShortcut(QApplication::translate("eePe", "Ctrl+O", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("eePe", "File", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class eePe: public Ui_eePe {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
