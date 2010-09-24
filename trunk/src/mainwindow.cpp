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
#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    unSaved = false;

    ui->listWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(ui->listWidget, SIGNAL(customContextMenuRequested(const QPoint&)),
        this, SLOT(ShowContextMenu(const QPoint&)));
}

MainWindow::~MainWindow()
{
    if(unSaved)
    {
         if(question("EEPROM Not Saved, Save now?")) on_actionSave_activated();
    }
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
    if(unSaved)
    {
         if(question("EEPROM Not Saved, Save now?")) on_actionSave_activated();
    }

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open EEPROM"), ".", tr("EEPROM Files (*.bin)"));

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) //assume binary file
    {
        alert("Could not open " + fileName);
        return;
    }
    if(file.size()!=EESIZE)
    {
        alert("File size is incorrect");
        file.close();
        return;
    }

    long result = file.read((char*)&eeprom,EESIZE);
    file.close();

    if (result!=EESIZE)
    {
        alert("Error reading file");
        return;
    }

    currentFileName = fileName;
    unSaved = false;
    memcpy(&eeFs,&eeprom,sizeof(eeFs));

    RefreshList();

    //TODO
    //File not opened
    //result != EESIZE
    //format wrong

}


bool MainWindow::question(QString msg)
{
    QMessageBox msgBox;
    msgBox.setText(msg);
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setEscapeButton(QMessageBox::No);
    msgBox.setWindowTitle("Question");
    msgBox.setIcon(QMessageBox::Question);
    return (msgBox.exec() == QMessageBox::Yes);
}

void MainWindow::alert(QString msg)
{
    QMessageBox msgBox;
    msgBox.setText(msg);
    msgBox.setStandardButtons(QMessageBox::Close);
    msgBox.setEscapeButton(QMessageBox::Close);
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setWindowTitle("alert");
}

void MainWindow::on_actionSave_activated()
{
    QString tstr = currentFileName;
    if(currentFileName.isEmpty()) on_actionSave_As_activated();

    QFile file(currentFileName);
    if(file.exists() && (tstr!=currentFileName))  // ask confirmation only if overwriting different file
    {
        if(!question("File " + currentFileName + " exists, overwrite?")) return;
    }
    if(!file.open(QIODevice::WriteOnly))  //assume binary file
    {
        alert("Unable to open file for writing.");
        return;
    }

    //TODO
    //File exists
    //Unable to open file
    //result!=EESIZE
    long result = file.write((char*)&eeprom,EESIZE);
    file.close();

    if(result!=EESIZE) alert("Error saving file");

    unSaved = false;

}


void MainWindow::on_actionSave_As_activated()
{
    currentFileName = QFileDialog::getSaveFileName(this, tr("Open EEPROM"), ".", tr("EEPROM Files (*.bin)"));
    if(!currentFileName.isEmpty()) on_actionSave_activated();
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


void MainWindow::DeleteSelectedModel()
{
    int id=ui->listWidget->currentIndex().row();
    if(!id) return;
    id--;
    static char buf[sizeof(g_model.name)+10];
    eeLoadModelName(id,buf,sizeof(buf));
    QString str = QString(buf).mid(4,10);
    if(question("Delete model " + str + "?"))
    {
        EFile::rm(FILE_MODEL(id)); //delete file
        RefreshList();
        unSaved=true;
    }
}


void MainWindow::DuplicateSelectedModel()
{
    //duplicate
}

void MainWindow::CutSelectedModel()
{
    //duplicate
}

void MainWindow::PasteSelectedModel()
{
    //duplicate
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
        //TODO error checking
        unSaved=true;
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
        //TODO error checking
        if(eeLoadGeneral())
        {
            unSaved=true;
            GeneralEdit t;
            t.exec();
        }
    }
}


void MainWindow::ShowContextMenu(const QPoint& pos)
{
    QPoint globalPos = ui->listWidget->mapToGlobal(pos);

    QMenu contextMenu;
    contextMenu.addAction(tr("&Delete"),this,SLOT(DeleteSelectedModel()),tr("Delete"));
    contextMenu.addAction(tr("D&uplicate"),this,SLOT(DuplicateSelectedModel()),tr("Ctrl+D"));
    contextMenu.addAction(tr("&Cut"),this,SLOT(CutSelectedModel()),tr("Ctrl+X"));
    contextMenu.addAction(tr("&Paste"),this,SLOT(PasteSelectedModel()),tr("Ctrl+V"));


    QAction* selectedItem = contextMenu.exec(globalPos);
    if (selectedItem)
    {
        //something was chosen - do something
        //if(int i=ui->listWidget->currentIndex().row()) DeleteModel(i-1);

    }
}



