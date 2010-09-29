#include "modeledit.h"
#include "ui_modeledit.h"

ModelEdit::ModelEdit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ModelEdit)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/icon.ico"));
}

ModelEdit::~ModelEdit()
{
    delete ui;
}
