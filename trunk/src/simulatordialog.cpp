#include "simulatordialog.h"
#include "ui_simulatordialog.h"

simulatorDialog::simulatorDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::simulatorDialog)
{
    ui->setupUi(this);

    QGraphicsScene *leftScene = new QGraphicsScene(ui->leftStick);
    leftScene->setItemIndexMethod(QGraphicsScene::NoIndex);
    ui->leftStick->setScene(leftScene);

    QGraphicsScene *rightScene = new QGraphicsScene(ui->rightStick);
    rightScene->setItemIndexMethod(QGraphicsScene::NoIndex);
    ui->rightStick->setScene(rightScene);

}

simulatorDialog::~simulatorDialog()
{
    delete ui;
}
