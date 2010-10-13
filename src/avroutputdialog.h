#ifndef AVROUTPUTDIALOG_H
#define AVROUTPUTDIALOG_H

#include <QDialog>
#include <QtGui>

#define AVR_DIALOG_CLOSE_IF_SUCCESSFUL 0
#define AVR_DIALOG_KEEP_OPEN           1
#define AVR_DIALOG_FORCE_CLOSE         2


namespace Ui {
    class avrOutputDialog;
}

class avrOutputDialog : public QDialog
{
    Q_OBJECT

public:
    explicit avrOutputDialog(QWidget *parent, QString prog, QStringList arg, int closeBehaviour=AVR_DIALOG_CLOSE_IF_SUCCESSFUL);
    ~avrOutputDialog();

    void addText(const QString &text);
    void runAgain(QString prog, QStringList arg, int closeBehaviour=AVR_DIALOG_CLOSE_IF_SUCCESSFUL);
    void waitForFinish();

protected slots:
    void doAddTextStdOut();
    void doAddTextStdErr();
    void doProcessStarted();
    void doFinished(int code);

private:
    Ui::avrOutputDialog *ui;

    QProcess *process;
    QString cmdLine;
    int closeOpt;
};

#endif // AVROUTPUTDIALOG_H
