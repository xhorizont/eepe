#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include "pers.h"

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

}

void MainWindow::on_actionSave_activated()
{
    //QString QFileDialog::getSaveFileName();
}
