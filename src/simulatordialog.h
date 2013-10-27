#ifndef SIMULATORDIALOG_H
#define SIMULATORDIALOG_H

#include <QDialog>
#include "node.h"
#include <inttypes.h>
#include "pers.h"
#include "modeledit.h"

#define TMR_OFF     0
#define TMR_RUNNING 1
#define TMR_BEEPING 2
#define TMR_STOPPED 3

#define FLASH_DURATION 10

namespace Ui {
    class simulatorDialog;
}

class simulatorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit simulatorDialog( QWidget *parent = 0);
    ~simulatorDialog();

    void loadParams(const EEGeneral gg, const ModelData gm);
		void setType( uint8_t type ) ;

private:
    Ui::simulatorDialog *ui;
    Node *nodeLeft;
    Node *nodeRight;
    QTimer *timer;
    QString modelName;
    qint16	ee_type ;

    qint8   *trimptr[4];
    quint16 g_tmr10ms;
    qint16  chanOut[NUM_CHNOUT];
    qint16  calibratedStick[7+2+3];
    qint16  StickValues[4] ;
    qint16  g_ppmIns[8];
    qint16  ex_chans[NUM_CHNOUT];
    qint8   trim[4];
    qint16  sDelay[MAX_MIXERS];
    qint32  act[MAX_MIXERS];
    qint16  anas [NUM_XCHNRAW+1+MAX_GVARS];
    qint32  chans[NUM_CHNOUT];
    quint8  bpanaCenter;
    bool    swOn[MAX_MIXERS];
    quint16 one_sec_precount;
		qint16		CsTimer[NUM_CSW+EXTRA_CSW] ;

    quint16 s_timeCumTot;
    quint16 s_timeCumAbs;
    quint16 s_timeCumSw;
    quint16 s_timeCumThr;
    quint16 s_timeCum16ThrP;
    quint8  s_timerState;
    quint8  beepAgain;
    quint16 g_LightOffCounter;
    qint16  s_timerVal;
    quint16 s_time;
    quint16 s_cnt;
    quint16 s_sum;
    quint8  sw_toggled;
		quint8	CurrentPhase ;

		quint8  current_limits ;

    ModelData g_model;
    EEGeneral g_eeGeneral;

		int chVal(int val) ;
    void setupSticks();
    void setupTimer();
    void resizeEvent(QResizeEvent *event  = 0);

		uint32_t adjustMode( uint32_t x ) ;
    void getValues();
    void setValues();
    void perOut(bool init=false);
    void centerSticks();
    void timerTick();

    bool keyState(EnumKeys key);
    qint16 getValue(qint8 i);
    bool getSwitch(int swtch, bool nc, qint8 level=0);
    void beepWarn();
    void beepWarn1();
    void beepWarn2();

    int beepVal;
    int beepShow;

    int16_t intpol(int16_t x, uint8_t idx);
		int8_t REG100_100(int8_t x) ;
		int8_t REG(int8_t x, int8_t min, int8_t max) ;

		uint32_t getFlightPhase() ;
		int16_t getRawTrimValue( uint8_t phase, uint8_t idx ) ;
		uint32_t getTrimFlightPhase( uint8_t phase, uint8_t idx ) ;
		int16_t getTrimValue( uint8_t phase, uint8_t idx ) ;
		void setTrimValue(uint8_t phase, uint8_t idx, int16_t trim) ;

protected:
		void closeEvent(QCloseEvent *event) ;

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
