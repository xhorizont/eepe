#include "avroutputdialog.h"
#include "ui_avroutputdialog.h"
#include <QtGui>

avrOutputDialog::avrOutputDialog(QWidget *parent, QString prog, QStringList arg) :
    QDialog(parent),
    ui(new Ui::avrOutputDialog)
{
    ui->setupUi(this);

    this->setWindowTitle("AVRDUDE result");
    cmdLine = prog;
    foreach(QString str, arg) cmdLine.append(" " + str);

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

void avrOutputDialog::doFinished(int code=0)
{
    addText("\n============================================================");
    if(code)
        addText(tr("\nAVRDUDE done - ERROR: exit code %1").arg(code));
    else
    {
        addText(tr("\nAVRDUDE done - SUCCESSFUL"));
        accept();
    }
}

void avrOutputDialog::doProcessStarted()
{
    addText("Started AVRDUDE\n");
    addText(cmdLine);
    addText("\n============================================================\n");
}
