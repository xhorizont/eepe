#include "curvedialog.h"
#include "ui_curvedialog.h"
#include "edge.h"
#include "node.h"
#include "pers.h"

#include <QtGui>

curveDialog::curveDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::curveDialog)
{
    ui->setupUi(this);


    QGraphicsScene *scene = new QGraphicsScene(this);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);

    //scene->setSceneRect(-200, -200, 400, 400);
    ui->graphicsView->setScene(scene);
}

curveDialog::~curveDialog()
{
    delete ui;
}


void curveDialog::setCurve(QList<int> *crv)
{
    Node *nodel = 0;
    Node *nodex = 0;
    for (int i = 0; i < crv->size(); ++i)
    {
        nodel = nodex;
        nodex = new Node(ui->graphicsView);

        nodex->setPos(i*10,crv->at(i));
        ui->graphicsView->scene()->addItem(nodex);
        if(i>0) ui->graphicsView->scene()->addItem(new Edge(nodel, nodex));

    }
}

QList<int> curveDialog::getCurve()
{
    QList<int> values;
    foreach (QGraphicsItem *item, ui->graphicsView->scene()->items())
    {
        if (Node *node = qgraphicsitem_cast<Node *>(item))
                values << node->pos().ry();
    }

    return values;
}


