#include "simulatordialog.h"
#include "ui_simulatordialog.h"

simulatorDialog::simulatorDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::simulatorDialog)
{
    ui->setupUi(this);
}

simulatorDialog::~simulatorDialog()
{
    delete ui;
}
