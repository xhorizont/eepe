#include "avroutputdialog.h"
#include "ui_avroutputdialog.h"
#include <QtGui>

avrOutputDialog::avrOutputDialog(QWidget *parent, QString prog, QStringList arg, int closeBehaviour) :
    QDialog(parent),
    ui(new Ui::avrOutputDialog)
{
    ui->setupUi(this);

    this->setWindowTitle("AVRDUDE result");

    cmdLine = prog;
    foreach(QString str, arg) cmdLine.append(" " + str);
    closeOpt = closeBehaviour;

    process = new QProcess(this);

    connect(process,SIGNAL(readyReadStandardError()), this, SLOT(doAddTextStdErr()));
    connect(process,SIGNAL(started()),this,SLOT(doProcessStarted()));
    connect(process,SIGNAL(readyReadStandardOutput()),this,SLOT(doAddTextStdOut()));
    connect(process,SIGNAL(finished(int)),this,SLOT(doFinished(int)));
    process->start(prog,arg);
}

avrOutputDialog::~avrOutputDialog()
{
    delete ui;
}

void avrOutputDialog::runAgain(QString prog, QStringList arg, int closeBehaviour)
{
    cmdLine = prog;
    foreach(QString str, arg) cmdLine.append(" " + str);
    closeOpt = closeBehaviour;
    process->start(prog,arg);
}

void avrOutputDialog::waitForFinish()
{
    process->waitForFinished();
}

void avrOutputDialog::addText(const QString &text)
{
    int val = ui->plainTextEdit->verticalScrollBar()->maximum();
    ui->plainTextEdit->insertPlainText(text);
    if(val!=ui->plainTextEdit->verticalScrollBar()->maximum())
        ui->plainTextEdit->verticalScrollBar()->setValue(ui->plainTextEdit->verticalScrollBar()->maximum());
}


void avrOutputDialog::doAddTextStdOut()
{
    QByteArray data = process->readAllStandardOutput();
    QString text = QString(data);
    addText(text);
}

void avrOutputDialog::doAddTextStdErr()
{
    QByteArray data = process->readAllStandardError();
    QString text = QString(data);
    addText(text);
}

#define HLINE_SEPARATOR "================================================================================="
void avrOutputDialog::doFinished(int code=0)
{
    addText("\n" HLINE_SEPARATOR);
    if(code)
        addText(tr("\nAVRDUDE done - exit code %1").arg(code));
    else
        addText(tr("\nAVRDUDE done - SUCCESSFUL"));
    addText("\n" HLINE_SEPARATOR "\n");


    switch(closeOpt)
    {
    case (AVR_DIALOG_CLOSE_IF_SUCCESSFUL): if(!code) accept();break;
    case (AVR_DIALOG_FORCE_CLOSE): if(code) reject(); else accept(); break;
    default: //AVR_DIALOG_KEEP_OPEN
        break;
    }


}

void avrOutputDialog::doProcessStarted()
{
    addText(HLINE_SEPARATOR "\n");
    addText("Started AVRDUDE\n");
    addText(cmdLine);
    addText("\n" HLINE_SEPARATOR "\n");
}


