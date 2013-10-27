#include "reviewOutput.h"
#include "ui_reviewOutput.h"

//#include "mainwindow.h"
#include <QtGui>
#include <QString>
#include <inttypes.h>

extern QString AvrdudeOutput ;

reviewOutput::reviewOutput(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::reviewOutput)
{
  ui->setupUi(this) ;
	ui->outputText->setText( AvrdudeOutput ) ;
}

reviewOutput::~reviewOutput()
{
  delete ui ;
}




