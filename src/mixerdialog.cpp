#include "mixerdialog.h"
#include "ui_mixerdialog.h"
#include "pers.h"
#include "helpers.h"

MixerDialog::MixerDialog(QWidget *parent, MixData *mixdata, int stickMode) :
    QDialog(parent),
    ui(new Ui::MixerDialog)
{
    ui->setupUi(this);
    md = mixdata;

    populateSourceCB(ui->sourceCB, stickMode, md->srcRaw);
    ui->sourceCB->removeItem(0);

    ui->destCB->addItem(tr("dest %1").arg(md->destCh));
}

MixerDialog::~MixerDialog()
{
    delete ui;
}

void MixerDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
