#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

#include <QDialog>

namespace Ui {
    class preferencesDialog;
}

class preferencesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit preferencesDialog(QWidget *parent = 0);
    ~preferencesDialog();

private:
    Ui::preferencesDialog *ui;

    void populateLocale();
    void initSettings();

private slots:
    void write_values();
};

#endif // PREFERENCESDIALOG_H
