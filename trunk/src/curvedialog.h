#ifndef CURVEDIALOG_H
#define CURVEDIALOG_H

#include <QDialog>

namespace Ui {
    class curveDialog;
}

class curveDialog : public QDialog
{
    Q_OBJECT

public:
    explicit curveDialog(QWidget *parent = 0);
    ~curveDialog();

    void setCurve(QList<int> *crv);
    QList<int> getCurve();




private:
    Ui::curveDialog *ui;

};

#endif // CURVEDIALOG_H
