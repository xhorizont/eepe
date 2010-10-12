#include "avroutputdialog.h"
#include "ui_avroutputdialog.h"
#include <QtGui>

avrOutputDialog::avrOutputDialog(QWidget *parent, QProcess *prc) :
    QDialog(parent),
    ui(new Ui::avrOutputDialog)
{
    ui->setupUi(this);
    process = prc;

    connect(process,SIGNAL(started()),this,SLOT(doProcessStarted()));
    connect(process,SIGNAL(readyReadStandardOutput()),this,SLOT(doAddText()));
    connect(process,SIGNAL(finished(int)),this,SLOT(done(int)));
}

avrOutputDialog::~avrOutputDialog()
{
    delete ui;
}


void avrOutputDialog::addText(const QString &text)
{
    ui->plainTextEdit->appendPlainText(text);
}


void avrOutputDialog::doAddText()
{
    QString str = process->readAllStandardOutput();
    addText(str + "\n");
}

void avrOutputDialog::doProcessStarted()
{
    exec();
}
