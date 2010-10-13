#ifndef SIMULATORDIALOG_H
#define SIMULATORDIALOG_H

#include <QDialog>

namespace Ui {
    class simulatorDialog;
}

class simulatorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit simulatorDialog(QWidget *parent = 0);
    ~simulatorDialog();

private:
    Ui::simulatorDialog *ui;
};

#endif // SIMULATORDIALOG_H
