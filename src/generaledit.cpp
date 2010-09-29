#include "generaledit.h"
#include "ui_generaledit.h"

GeneralEdit::GeneralEdit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GeneralEdit)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/icon.ico"));
}

GeneralEdit::~GeneralEdit()
{
    delete ui;
}
