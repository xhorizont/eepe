#ifndef SIMULATORDIALOG_H
#define SIMULATORDIALOG_H

#include <QDialog>
#include "node.h"
#include <inttypes.h>
#include "pers.h"

namespace Ui {
    class simulatorDialog;
}

class simulatorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit simulatorDialog(QWidget *parent = 0);
    ~simulatorDialog();

    void loadParams(const EEGeneral gg, const ModelData gm);

private:
    Ui::simulatorDialog *ui;
    Node *nodeLeft;
    Node *nodeRight;
    QTimer *timer;

    quint16 g_tmr10ms;
    qint16  chanOut[NUM_CHNOUT];
    qint16  calibratedStick[7];
    qint16  g_ppmIns[8];
    qint16  ex_chans[NUM_CHNOUT];
    qint16  trim[4];
    qint16  sDelay[MAX_MIXERS];
    qint32  act[MAX_MIXERS];

    ModelData g_model;
    EEGeneral g_eeGeneral;

    void setupSticks();
    void setupTimer();
    void resizeEvent(QResizeEvent *event  = 0);

    void getValues();
    void setValues();
    void perOut(bool init=false);
    void centerSticks();

    bool keyState(EnumKeys key);
    bool getSwitch(int swtch, bool nc);
    void beepWarn1();

    int beepVal;
    int beepShow;

    int16_t intpol(int16_t x, uint8_t idx);
private slots:
    void on_FixRightY_clicked(bool checked);
    void on_FixRightX_clicked(bool checked);
    void on_FixLeftY_clicked(bool checked);
    void on_FixLeftX_clicked(bool checked);
    void on_holdRightY_clicked(bool checked);
    void on_holdRightX_clicked(bool checked);
    void on_holdLeftY_clicked(bool checked);
    void on_holdLeftX_clicked(bool checked);
    void timerEvent();


};

#endif // SIMULATORDIALOG_H
