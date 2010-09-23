#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include "pers.h"
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <QListWidget>
#include "about.h"
#include "modeledit.h"
#include "generaledit.h"
#include <QTabWidget>
#include <QHeaderView>




MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->listWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(ui->listWidget, SIGNAL(customContextMenuRequested(const QPoint&)),
        this, SLOT(ShowContextMenu(const QPoint&)));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::on_actionQuit_activated()
{
   exit(0);
}

void MainWindow::on_actionOpen_activated()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open EEPROM"), ".", tr("EEPROM Files (*.bin)"));

    QFile file(fileName);
         if (!file.open(QIODevice::ReadOnly)) //assume binary file
             return;
    if(file.size()!=EESIZE)
    {
        file.close();
        return;
    }

    long result = file.read((char*)&eeprom,EESIZE);
    file.close();

    if (result!=EESIZE) return;

    memcpy(&eeFs,&eeprom,sizeof(eeFs));

    RefreshList();

}

void MainWindow::on_actionSave_activated()
{
    //QString QFileDialog::getSaveFileName();

}


void MainWindow::RefreshList()
{
    ui->listWidget->clear();
    ui->listWidget->addItem("General Settings");

    for(uint8_t i=0; i<MAX_MODELS; i++)
    {
        static char buf[sizeof(g_model.name)+10];
        eeLoadModelName(i,buf,sizeof(buf));
        QString str = QString(buf);
        ui->listWidget->addItem(str);
    }
}



void MainWindow::on_actionAbout_activated()
{
    About a;
    a.exec();
}


void MainWindow::on_listWidget_doubleClicked(QModelIndex index)
{

    int i = index.row();

    if(i)
    {
        eeLoadModel((uint8_t)i-1);
        ModelEdit t;
        uint8_t temp = g_model.mdVers;
        g_model.mdVers = 0;
        t.setWindowTitle(QString("Editing: ") + QString(g_model.name));
        g_model.mdVers = temp;
        t.exec();
    }
    else
    {
        if(eeLoadGeneral())
        {
            GeneralEdit t;
            t.exec();
        }
    }
}


void MainWindow::ShowContextMenu(const QPoint& pos)
{
    // for most widgets
    QPoint globalPos = ui->listWidget->mapToGlobal(pos);
    // for QAbstractScrollArea and derived classes you would use:
    // QPoint globalPos = myWidget->viewport()->mapToGlobal(pos);

    QMenu myMenu;
    myMenu.addAction("Delete");
    myMenu.addAction("Duplicate");
    myMenu.addAction("Move");
    // ...

    QAction* selectedItem = myMenu.exec(globalPos);
    if (selectedItem)
    {
        // something was chosen, do stuff
    }
    else
    {
        // nothing was chosen
    }
}


