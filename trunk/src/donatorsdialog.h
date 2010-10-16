#ifndef DONATORSDIALOG_H
#define DONATORSDIALOG_H

#include <QDialog>

namespace Ui {
    class donatorsDialog;
}

class donatorsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit donatorsDialog(QWidget *parent = 0);
    ~donatorsDialog();

private:
    Ui::donatorsDialog *ui;
};

#endif // DONATORSDIALOG_H
