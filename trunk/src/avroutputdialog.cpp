#include "avroutputdialog.h"
#include "ui_avroutputdialog.h"
#include <QtGui>

avrOutputDialog::avrOutputDialog(QWidget *parent, QString prog, QStringList arg) :
    QDialog(parent),
    ui(new Ui::avrOutputDialog)
{
    ui->setupUi(this);

    this->setWindowTitle("AVRDUDE result");

    process = new QProcess(this);

    connect(process,SIGNAL(started()),this,SLOT(doProcessStarted()));
    connect(process,SIGNAL(readyReadStandardOutput()),this,SLOT(doAddText()));
    connect(process,SIGNAL(finished(int)),this,SLOT(doFinished(int)));
    process->start(prog,arg);
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
    QByteArray data = process->readAllStandardOutput();
    QString text = QString(data);
    addText(text);

}

void avrOutputDialog::doFinished(int code=0)
{
    addText(tr("\nAVRDUDE done - exit code: %1").arg(code));
}

void avrOutputDialog::doProcessStarted()
{
    addText("Started AVRDUDE\n\n");
}
