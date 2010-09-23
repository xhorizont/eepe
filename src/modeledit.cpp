#include "modeledit.h"
#include "ui_modeledit.h"

ModelEdit::ModelEdit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ModelEdit)
{
    ui->setupUi(this);
}

ModelEdit::~ModelEdit()
{
    delete ui;
}
