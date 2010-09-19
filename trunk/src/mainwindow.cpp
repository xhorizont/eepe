#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include "pers.h"
#include <stdio.h>
#include <stdlib.h>
#include <QListWidget>
#include <QListWidgetItem>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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


    FILE * pFile;
    pFile = fopen ( (const char*)fileName.data() , "rb" );
    if (pFile==NULL) return;

    // obtain file size:
    fseek (pFile , 0 , SEEK_END);
    long lSize = ftell (pFile)/2;
    rewind (pFile);

    if(lSize==EESIZE)
    {

        // copy the file into the buffer:
        long result = fread (&eeprom,1,lSize,pFile);
        if (result != lSize) lSize=0;

        /* the whole file is now loaded in the memory buffer. */
    }
    // terminate
    fclose (pFile);

    if(lSize==EESIZE)
    {
        ui->listWidget->clear();

        for(uint8_t i; i<MAX_MODELS; i++)
        {
            static char buf[sizeof(g_model.name)+5];
            eeLoadModelName(i,buf,sizeof(buf));
            QString str = buf;
            ui->listWidget->addItem(str);

        }
    }

}

void MainWindow::on_actionSave_activated()
{
    //QString QFileDialog::getSaveFileName();
}


