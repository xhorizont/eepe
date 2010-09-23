#ifndef GENERALEDIT_H
#define GENERALEDIT_H

#include <QDialog>

namespace Ui {
    class GeneralEdit;
}

class GeneralEdit : public QDialog
{
    Q_OBJECT

public:
    explicit GeneralEdit(QWidget *parent = 0);
    ~GeneralEdit();

private:
    Ui::GeneralEdit *ui;
};

#endif // GENERALEDIT_H
