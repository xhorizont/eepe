#ifndef CUSTOMIZESPLASHDIALOG_H
#define CUSTOMIZESPLASHDIALOG_H

#include <QtGui>
#include <QDialog>

namespace Ui {
    class customizeSplashDialog;
}

class customizeSplashDialog : public QDialog
{
    Q_OBJECT

public:
    explicit customizeSplashDialog(QWidget *parent = 0);
    ~customizeSplashDialog();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_buttonInvertColor_clicked();


private:
    Ui::customizeSplashDialog *ui;
};

#endif // CUSTOMIZESPLASHDIALOG_H
