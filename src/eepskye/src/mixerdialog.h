#ifndef MIXERDIALOG_H
#define MIXERDIALOG_H

#include <QDialog>
#include "pers.h"

namespace Ui {
    class MixerDialog;
}

class MixerDialog : public QDialog {
    Q_OBJECT
public:
    MixerDialog(QWidget *parent, SKYMixData *mixdata, int stickMode, QString * comment, int modelVersion, int eeType);
    ~MixerDialog();

//    QString getComment();

protected:
    void changeEvent(QEvent *e);

private slots:
    void valuesChanged();


private:
    SKYMixData *md;
    Ui::MixerDialog *ui;
    QString * mixCommennt;
		int leeType ;
};

#endif // MIXERDIALOG_H
