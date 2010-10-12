#ifndef AVROUTPUTDIALOG_H
#define AVROUTPUTDIALOG_H

#include <QDialog>
#include <QtGui>

namespace Ui {
    class avrOutputDialog;
}

class avrOutputDialog : public QDialog
{
    Q_OBJECT

public:
    explicit avrOutputDialog(QWidget *parent, QString prog, QStringList arg);
    ~avrOutputDialog();

    void addText(const QString &text);

protected slots:
    void doAddText();
    void doProcessStarted();
    void doFinished(int code);

private:
    Ui::avrOutputDialog *ui;

    QProcess *process;
};

#endif // AVROUTPUTDIALOG_H
