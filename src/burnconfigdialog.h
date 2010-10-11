#ifndef BURNCONFIGDIALOG_H
#define BURNCONFIGDIALOG_H

#include <QDialog>

namespace Ui {
    class burnConfigDialog;
}

class burnConfigDialog : public QDialog {
    Q_OBJECT
public:
    burnConfigDialog(QWidget *parent = 0);
    ~burnConfigDialog();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::burnConfigDialog *ui;
};

#endif // BURNCONFIGDIALOG_H
