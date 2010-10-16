#ifndef BURNCONFIGDIALOG_H
#define BURNCONFIGDIALOG_H

#include <QDialog>
#include <QtGui>

#define MEM_TYPE_EEPROM 1
#define MEM_TYPE_FLASH  2

#define OPR_TYPE_READ  1
#define OPR_TYPE_WRITE 2

namespace Ui {
    class burnConfigDialog;
}

class burnConfigDialog : public QDialog {
    Q_OBJECT
public:
    burnConfigDialog(QWidget *parent = 0);
    ~burnConfigDialog();

    QString getTempDir();
    QString getAVRDUDE();
    QStringList getAVRArgs();
    QString getProgrammer();
    QString getPort();

    void listProgrammers();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::burnConfigDialog *ui;


private slots:
    void on_readFuses_clicked();
    void on_resetFuses_clicked();
    void on_avrArgs_editingFinished();
    void on_pushButton_4_clicked();
    void on_temp_location_editingFinished();
    void on_pushButton_3_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_clicked();
    void on_avrdude_location_editingFinished();
    void on_avrdude_programmer_currentIndexChanged(QString );
    void on_avrdude_port_currentIndexChanged(QString );

    void getSettings();
    void putSettings();
};

#endif // BURNCONFIGDIALOG_H
