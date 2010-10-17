#include "donatorsdialog.h"
#include "ui_donatorsdialog.h"
#include <QtGui>

donatorsDialog::donatorsDialog(QWidget *parent) :
    QDialog(parent, Qt::WindowTitleHint | Qt::WindowSystemMenuHint),
    ui(new Ui::donatorsDialog)
{
    ui->setupUi(this);

    QFile file(":/donators");
    if(file.open( QIODevice::ReadOnly | QIODevice::Text ) )
    {
        ui->plainTextEdit->insertPlainText(file.readAll());
    }

    //ui->plainTextEdit->insertPlainText();
}

donatorsDialog::~donatorsDialog()
{
    delete ui;
}
