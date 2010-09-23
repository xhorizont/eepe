#ifndef MODELEDIT_H
#define MODELEDIT_H

#include <QDialog>

namespace Ui {
    class ModelEdit;
}

class ModelEdit : public QDialog
{
    Q_OBJECT

public:
    explicit ModelEdit(QWidget *parent = 0);
    ~ModelEdit();

private:
    Ui::ModelEdit *ui;
};

#endif // MODELEDIT_H
