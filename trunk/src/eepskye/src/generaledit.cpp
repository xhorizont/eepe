#include "generaledit.h"
#include "ui_generaledit.h"
#include "pers.h"
#include "file.h"
#include "helpers.h"
#include <QtGui>

#define BIT_WARN_THR     ( 0x01 )
#define BIT_WARN_SW      ( 0x02 )
#define BIT_WARN_MEM     ( 0x04 )
#define BIT_WARN_BEEP    ( 0x80 )
#define BIT_BEEP_VAL     ( 0x38 ) // >>3
#define BEEP_VAL_SHIFT   3

GeneralEdit::GeneralEdit( struct t_radioData *radioData, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GeneralEdit)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/icon.png"));
//    eeFile = eFile;

    rData = radioData ;
    switchDefPosEditLock = false;

    QSettings settings("er9x-eePskye", "eePskye");
    ui->tabWidget->setCurrentIndex(settings.value("generalEditTab", 0).toInt());

		memcpy(  &g_eeGeneral, &radioData->generalSettings, sizeof( g_eeGeneral) ) ;
//    eeFile->getGeneralSettings(&g_eeGeneral);

    QRegExp rx(CHAR_FOR_NAMES_REGEX);
    ui->ownerNameLE->setValidator(new QRegExpValidator(rx, this));

    populateSwitchCB(ui->backlightswCB,g_eeGeneral.lightSw);

    ui->ownerNameLE->setText(g_eeGeneral.ownerName);

    ui->contrastSB->setValue(g_eeGeneral.contrast);
    ui->battwarningDSB->setValue((double)g_eeGeneral.vBatWarn/10);
    ui->battcalibDSB->setValue((double)g_eeGeneral.vBatCalib/10);
    ui->battCalib->setValue((double)g_eeGeneral.vBatCalib/10);
    ui->backlightautoSB->setValue(g_eeGeneral.lightAutoOff*5);
    ui->backlightStickMove->setValue(g_eeGeneral.lightOnStickMove*5);
    ui->inactimerSB->setValue(g_eeGeneral.inactivityTimer+10);

    ui->soundModeCB->setCurrentIndex(g_eeGeneral.speakerMode);
    ui->speakerPitchSB->setValue(g_eeGeneral.speakerPitch);
    ui->hapticStengthSB->setValue(g_eeGeneral.hapticStrength);

    ui->thrrevChkB->setChecked(g_eeGeneral.throttleReversed);
    ui->inputfilterCB->setCurrentIndex(g_eeGeneral.filterInput);
    ui->thrwarnChkB->setChecked(!g_eeGeneral.disableThrottleWarning);   //Default is zero=checked
    ui->switchwarnChkB->setChecked(!g_eeGeneral.disableSwitchWarning); //Default is zero=checked
//    ui->memwarnChkB->setChecked(!g_eeGeneral.disableMemoryWarning);   //Default is zero=checked
    ui->alarmwarnChkB->setChecked(!g_eeGeneral.disableAlarmWarning);//Default is zero=checked
    ui->PotScrollEnableChkB->setChecked(!g_eeGeneral.disablePotScroll);//Default is zero=checked
//    ui->BandGapEnableChkB->setChecked(!g_eeGeneral.disableBG);//Default is zero=checked
    ui->beeperCB->setCurrentIndex(g_eeGeneral.beeperVal);
    ui->channelorderCB->setCurrentIndex(g_eeGeneral.templateSetup);
    ui->stickmodeCB->setCurrentIndex(g_eeGeneral.stickMode);
    
		ui->volumeSB->setValue(g_eeGeneral.volume);
//    ui->enablePpmsimChkB->setChecked(g_eeGeneral.enablePpmsim);
    ui->internalFrskyAlarmChkB->setChecked(g_eeGeneral.frskyinternalalarm);
    ui->OptrexDisplayChkB->setChecked(g_eeGeneral.optrexDisplay);

    ui->beepMinuteChkB->setChecked(g_eeGeneral.minuteBeep);
    ui->beepCountDownChkB->setChecked(g_eeGeneral.preBeep);
    ui->beepFlashChkB->setChecked(g_eeGeneral.flashBeep);
    ui->splashScreenChkB->setChecked(!g_eeGeneral.disableSplashScreen);
    ui->splashScreenNameChkB->setChecked(!g_eeGeneral.hideNameOnSplash);
    ui->brightSB->setValue(100-g_eeGeneral.bright ) ;
		ui->BtBaudrateCB->setCurrentIndex(g_eeGeneral.bt_baudrate) ;
		ui->RotaryDivisorCB->setCurrentIndex(g_eeGeneral.rotaryDivisor) ;
		ui->CurrentCalibSB->setValue(g_eeGeneral.current_calib ) ;
		ui->MaHalarmSB->setValue(g_eeGeneral.mAh_alarm*50 ) ;
    
    ui->ana1Neg->setValue(g_eeGeneral.calibSpanNeg[0]);
    ui->ana2Neg->setValue(g_eeGeneral.calibSpanNeg[1]);
    ui->ana3Neg->setValue(g_eeGeneral.calibSpanNeg[2]);
    ui->ana4Neg->setValue(g_eeGeneral.calibSpanNeg[3]);
    ui->ana5Neg->setValue(g_eeGeneral.calibSpanNeg[4]);
    ui->ana6Neg->setValue(g_eeGeneral.calibSpanNeg[5]);
    ui->ana7Neg->setValue(g_eeGeneral.calibSpanNeg[6]);

    ui->ana1Mid->setValue(g_eeGeneral.calibMid[0]);
    ui->ana2Mid->setValue(g_eeGeneral.calibMid[1]);
    ui->ana3Mid->setValue(g_eeGeneral.calibMid[2]);
    ui->ana4Mid->setValue(g_eeGeneral.calibMid[3]);
    ui->ana5Mid->setValue(g_eeGeneral.calibMid[4]);
    ui->ana6Mid->setValue(g_eeGeneral.calibMid[5]);
    ui->ana7Mid->setValue(g_eeGeneral.calibMid[6]);

    ui->ana1Pos->setValue(g_eeGeneral.calibSpanPos[0]);
    ui->ana2Pos->setValue(g_eeGeneral.calibSpanPos[1]);
    ui->ana3Pos->setValue(g_eeGeneral.calibSpanPos[2]);
    ui->ana4Pos->setValue(g_eeGeneral.calibSpanPos[3]);
    ui->ana5Pos->setValue(g_eeGeneral.calibSpanPos[4]);
    ui->ana6Pos->setValue(g_eeGeneral.calibSpanPos[5]);
    ui->ana7Pos->setValue(g_eeGeneral.calibSpanPos[6]);

    setSwitchDefPos();
    
		ui->weightSB_1->findChild<QLineEdit*>()->setReadOnly(true);
    ui->weightSB_2->findChild<QLineEdit*>()->setReadOnly(true);
    ui->weightSB_3->findChild<QLineEdit*>()->setReadOnly(true);
    ui->weightSB_4->findChild<QLineEdit*>()->setReadOnly(true);

		ui->stickgainLVCB->setCurrentIndex(g_eeGeneral.stickGain & STICK_LV_GAIN ? 1 : 0 ) ;
		ui->stickgainLHCB->setCurrentIndex(g_eeGeneral.stickGain & STICK_LH_GAIN ? 1 : 0 ) ;
		ui->stickgainRVCB->setCurrentIndex(g_eeGeneral.stickGain & STICK_RV_GAIN ? 1 : 0 ) ;
		ui->stickgainRHCB->setCurrentIndex(g_eeGeneral.stickGain & STICK_RH_GAIN ? 1 : 0 ) ;

		updateTrianerTab();

    connect(ui->modeCB_1, SIGNAL(currentIndexChanged(int)), this, SLOT(trainerTabValueChanged()));
    connect(ui->modeCB_2, SIGNAL(currentIndexChanged(int)), this, SLOT(trainerTabValueChanged()));
    connect(ui->modeCB_3, SIGNAL(currentIndexChanged(int)), this, SLOT(trainerTabValueChanged()));
    connect(ui->modeCB_4, SIGNAL(currentIndexChanged(int)), this, SLOT(trainerTabValueChanged()));

    connect(ui->weightSB_1, SIGNAL(editingFinished()), this, SLOT(trainerTabValueChanged()));
    connect(ui->weightSB_2, SIGNAL(editingFinished()), this, SLOT(trainerTabValueChanged()));
    connect(ui->weightSB_3, SIGNAL(editingFinished()), this, SLOT(trainerTabValueChanged()));
    connect(ui->weightSB_4, SIGNAL(editingFinished()), this, SLOT(trainerTabValueChanged()));

    connect(ui->sourceCB_1, SIGNAL(currentIndexChanged(int)), this, SLOT(trainerTabValueChanged()));
    connect(ui->sourceCB_2, SIGNAL(currentIndexChanged(int)), this, SLOT(trainerTabValueChanged()));
    connect(ui->sourceCB_3, SIGNAL(currentIndexChanged(int)), this, SLOT(trainerTabValueChanged()));
    connect(ui->sourceCB_4, SIGNAL(currentIndexChanged(int)), this, SLOT(trainerTabValueChanged()));

    connect(ui->swtchCB_1, SIGNAL(currentIndexChanged(int)), this, SLOT(trainerTabValueChanged()));
    connect(ui->swtchCB_2, SIGNAL(currentIndexChanged(int)), this, SLOT(trainerTabValueChanged()));
    connect(ui->swtchCB_3, SIGNAL(currentIndexChanged(int)), this, SLOT(trainerTabValueChanged()));
    connect(ui->swtchCB_4, SIGNAL(currentIndexChanged(int)), this, SLOT(trainerTabValueChanged()));

    connect(ui->trainerCalib_1, SIGNAL(editingFinished()), this, SLOT(trainerTabValueChanged()));
    connect(ui->trainerCalib_2, SIGNAL(editingFinished()), this, SLOT(trainerTabValueChanged()));
    connect(ui->trainerCalib_3, SIGNAL(editingFinished()), this, SLOT(trainerTabValueChanged()));
    connect(ui->trainerCalib_4, SIGNAL(editingFinished()), this, SLOT(trainerTabValueChanged()));

    connect(ui->PPM_MultiplierDSB, SIGNAL(editingFinished()), this, SLOT(trainerTabValueChanged()));

    connect(ui->weightSB_1, SIGNAL(valueChanged(int)), this, SLOT(validateWeightSB()));
    connect(ui->weightSB_2, SIGNAL(valueChanged(int)), this, SLOT(validateWeightSB()));
    connect(ui->weightSB_3, SIGNAL(valueChanged(int)), this, SLOT(validateWeightSB()));
    connect(ui->weightSB_4, SIGNAL(valueChanged(int)), this, SLOT(validateWeightSB()));
}

GeneralEdit::~GeneralEdit()
{
    delete ui;
}

void GeneralEdit::setSwitchDefPos()
{
    quint8 x = g_eeGeneral.switchWarningStates & SWP_IL5;
    if(x==SWP_IL1 || x==SWP_IL2 || x==SWP_IL3 || x==SWP_IL4 || x==SWP_IL5) //illegal states for ID0/1/2
    {
        g_eeGeneral.switchWarningStates &= ~SWP_IL5; // turn all off, make sure only one is on
        g_eeGeneral.switchWarningStates |=  SWP_ID0B;
    }

    switchDefPosEditLock = true;
    ui->switchDefPos_1->setChecked(g_eeGeneral.switchWarningStates & 0x01);
    ui->switchDefPos_2->setChecked(g_eeGeneral.switchWarningStates & 0x02);
    ui->switchDefPos_3->setChecked(g_eeGeneral.switchWarningStates & 0x04);
    ui->switchDefPos_4->setChecked(g_eeGeneral.switchWarningStates & 0x08);
    ui->switchDefPos_5->setChecked(g_eeGeneral.switchWarningStates & 0x10);
    ui->switchDefPos_6->setChecked(g_eeGeneral.switchWarningStates & 0x20);
    ui->switchDefPos_7->setChecked(g_eeGeneral.switchWarningStates & 0x40);
    ui->switchDefPos_8->setChecked(g_eeGeneral.switchWarningStates & 0x80);
    switchDefPosEditLock = false;
}

void GeneralEdit::updateSettings()
{
    int16_t sum=0;
    for(int i=0; i<12;i++) sum+=g_eeGeneral.calibMid[i];
    g_eeGeneral.chkSum = sum;
//    eeFile->putGeneralSettings(&g_eeGeneral);
    memcpy( &rData->generalSettings, &g_eeGeneral, sizeof( g_eeGeneral) ) ;


    emit modelValuesChanged();
}


void GeneralEdit::updateTrianerTab()
{
    on_tabWidget_selected(""); // updates channel name labels

    ui->modeCB_1->setCurrentIndex(g_eeGeneral.trainer.mix[0].mode);
    ui->weightSB_1->setValue(g_eeGeneral.trainer.mix[0].studWeight*13/4);
    ui->sourceCB_1->setCurrentIndex(g_eeGeneral.trainer.mix[0].srcChn);
    populateSwitchCB(ui->swtchCB_1,g_eeGeneral.trainer.mix[0].swtch);
    StudWeight1=g_eeGeneral.trainer.mix[0].studWeight*13/4;

    ui->modeCB_2->setCurrentIndex(g_eeGeneral.trainer.mix[1].mode);
    ui->weightSB_2->setValue(g_eeGeneral.trainer.mix[1].studWeight*13/4);
    ui->sourceCB_2->setCurrentIndex(g_eeGeneral.trainer.mix[1].srcChn);
    populateSwitchCB(ui->swtchCB_2,g_eeGeneral.trainer.mix[1].swtch);
    StudWeight2=g_eeGeneral.trainer.mix[1].studWeight*13/4;

    ui->modeCB_3->setCurrentIndex(g_eeGeneral.trainer.mix[2].mode);
    ui->weightSB_3->setValue(g_eeGeneral.trainer.mix[2].studWeight*13/4);
    ui->sourceCB_3->setCurrentIndex(g_eeGeneral.trainer.mix[2].srcChn);
    populateSwitchCB(ui->swtchCB_3,g_eeGeneral.trainer.mix[2].swtch);
    StudWeight3=g_eeGeneral.trainer.mix[2].studWeight*13/4;

    ui->modeCB_4->setCurrentIndex(g_eeGeneral.trainer.mix[0].mode);
    ui->weightSB_4->setValue(g_eeGeneral.trainer.mix[3].studWeight*13/4);
    ui->sourceCB_4->setCurrentIndex(g_eeGeneral.trainer.mix[3].srcChn);
    populateSwitchCB(ui->swtchCB_4,g_eeGeneral.trainer.mix[3].swtch);
    StudWeight4=g_eeGeneral.trainer.mix[3].studWeight*13/4;

    ui->trainerCalib_1->setValue(g_eeGeneral.trainer.calib[0]);
    ui->trainerCalib_2->setValue(g_eeGeneral.trainer.calib[1]);
    ui->trainerCalib_3->setValue(g_eeGeneral.trainer.calib[2]);
    ui->trainerCalib_4->setValue(g_eeGeneral.trainer.calib[3]);

    ui->PPM_MultiplierDSB->setValue(double(g_eeGeneral.PPM_Multiplier+10)/10);
}

void GeneralEdit::trainerTabValueChanged()
{
    g_eeGeneral.trainer.mix[0].mode       = ui->modeCB_1->currentIndex();
//    g_eeGeneral.trainer.mix[0].studWeight = ui->weightSB_1->value()*4/13;
    g_eeGeneral.trainer.mix[0].srcChn     = ui->sourceCB_1->currentIndex();
    g_eeGeneral.trainer.mix[0].swtch      = ui->swtchCB_1->currentIndex()-MAX_DRSWITCH;

    g_eeGeneral.trainer.mix[1].mode       = ui->modeCB_2->currentIndex();
//    g_eeGeneral.trainer.mix[1].studWeight = ui->weightSB_2->value()*4/13;
    g_eeGeneral.trainer.mix[1].srcChn     = ui->sourceCB_2->currentIndex();
    g_eeGeneral.trainer.mix[1].swtch      = ui->swtchCB_2->currentIndex()-MAX_DRSWITCH;

    g_eeGeneral.trainer.mix[2].mode       = ui->modeCB_3->currentIndex();
//    g_eeGeneral.trainer.mix[2].studWeight = ui->weightSB_3->value()*4/13;
    g_eeGeneral.trainer.mix[2].srcChn     = ui->sourceCB_3->currentIndex();
    g_eeGeneral.trainer.mix[2].swtch      = ui->swtchCB_3->currentIndex()-MAX_DRSWITCH;

    g_eeGeneral.trainer.mix[3].mode       = ui->modeCB_4->currentIndex();
//    g_eeGeneral.trainer.mix[3].studWeight = ui->weightSB_4->value()*4/13;
    g_eeGeneral.trainer.mix[3].srcChn     = ui->sourceCB_4->currentIndex();
    g_eeGeneral.trainer.mix[3].swtch      = ui->swtchCB_4->currentIndex()-MAX_DRSWITCH;

    g_eeGeneral.trainer.calib[0] = ui->trainerCalib_1->value();
    g_eeGeneral.trainer.calib[1] = ui->trainerCalib_2->value();
    g_eeGeneral.trainer.calib[2] = ui->trainerCalib_3->value();
    g_eeGeneral.trainer.calib[3] = ui->trainerCalib_4->value();

    g_eeGeneral.PPM_Multiplier = ((quint16)(ui->PPM_MultiplierDSB->value()*10))-10;

    updateSettings();
}

void GeneralEdit::validateWeightSB()
{
    ui->weightSB_1->blockSignals(true);
    ui->weightSB_2->blockSignals(true);
    ui->weightSB_3->blockSignals(true);
    ui->weightSB_4->blockSignals(true);

    if ((ui->weightSB_1->value()>StudWeight1) && (g_eeGeneral.trainer.mix[0].studWeight<31))
    {
      g_eeGeneral.trainer.mix[0].studWeight++;
    }
    else if ((ui->weightSB_1->value()<StudWeight1) && (g_eeGeneral.trainer.mix[0].studWeight>-31))
    {
      g_eeGeneral.trainer.mix[0].studWeight--;
    }
    ui->weightSB_1->setValue(g_eeGeneral.trainer.mix[0].studWeight*13/4);
    StudWeight1=ui->weightSB_1->value();
    
    if ((ui->weightSB_2->value()>StudWeight2) && (g_eeGeneral.trainer.mix[1].studWeight<31))
		{
      g_eeGeneral.trainer.mix[1].studWeight++;
    }
		else if ((ui->weightSB_2->value()<StudWeight2) && (g_eeGeneral.trainer.mix[1].studWeight>-31))
		{
      g_eeGeneral.trainer.mix[1].studWeight--;
    }
    ui->weightSB_2->setValue(g_eeGeneral.trainer.mix[1].studWeight*13/4);
    StudWeight2=ui->weightSB_2->value();
 
    if ((ui->weightSB_3->value()>StudWeight3) && (g_eeGeneral.trainer.mix[2].studWeight<31))
		{
      g_eeGeneral.trainer.mix[2].studWeight++;
    }
		else if ((ui->weightSB_3->value()<StudWeight3) && (g_eeGeneral.trainer.mix[2].studWeight>-31))
		{
      g_eeGeneral.trainer.mix[2].studWeight--;
    }
    ui->weightSB_3->setValue(g_eeGeneral.trainer.mix[2].studWeight*13/4);
    StudWeight3=ui->weightSB_3->value();
    
    if ((ui->weightSB_4->value()>StudWeight4) && (g_eeGeneral.trainer.mix[3].studWeight<31))
		{
      g_eeGeneral.trainer.mix[3].studWeight++;
    }
		else if ((ui->weightSB_4->value()<StudWeight4)  && (g_eeGeneral.trainer.mix[3].studWeight>-31))
		{
      g_eeGeneral.trainer.mix[3].studWeight--;
    }
    ui->weightSB_4->setValue(g_eeGeneral.trainer.mix[3].studWeight*13/4);
    StudWeight4=ui->weightSB_4->value();    
    
		ui->weightSB_1->blockSignals(false);
    ui->weightSB_2->blockSignals(false);
    ui->weightSB_3->blockSignals(false);
    ui->weightSB_4->blockSignals(false);
}

void GeneralEdit::on_contrastSB_editingFinished()
{
    g_eeGeneral.contrast = ui->contrastSB->value();
    updateSettings();
}

void GeneralEdit::on_volumeSB_editingFinished()
{
    g_eeGeneral.volume = ui->volumeSB->value() ;
    updateSettings();
}

void GeneralEdit::on_CurrentCalibSB_editingFinished()
{
    g_eeGeneral.current_calib = ui->CurrentCalibSB->value() ;
    updateSettings();
}

void GeneralEdit::on_MaHalarmSB_editingFinished()
{
    g_eeGeneral.mAh_alarm = ui->MaHalarmSB->value()/50 ;
    updateSettings();
}

void GeneralEdit::on_brightSB_editingFinished()
{
    g_eeGeneral.bright = 100-ui->brightSB->value() ;
    updateSettings();
}

void GeneralEdit::do_stick_gain()
{
	uint8_t value = 0 ;	
	
	if ( ui->stickgainLVCB->currentIndex() )
	{
		value |= STICK_LV_GAIN ;
	}
	if ( ui->stickgainLHCB->currentIndex() )
	{
		value |= STICK_LH_GAIN ;
	}
	if ( ui->stickgainRVCB->currentIndex() )
	{
		value |= STICK_RV_GAIN ;
	}
	if ( ui->stickgainRHCB->currentIndex() )
	{
		value |= STICK_RH_GAIN ;
	}
	
	g_eeGeneral.stickGain = value ;
}


void GeneralEdit::on_stickgainLVCB_currentIndexChanged(int index)
{
	do_stick_gain() ;
}

void GeneralEdit::on_stickgainLHCB_currentIndexChanged(int index)
{
	do_stick_gain() ;
}

void GeneralEdit::on_stickgainRVCB_currentIndexChanged(int index)
{
	do_stick_gain() ;
}

void GeneralEdit::on_stickgainRHCB_currentIndexChanged(int index)
{
	do_stick_gain() ;
}


void GeneralEdit::on_battwarningDSB_editingFinished()
{
    g_eeGeneral.vBatWarn = (int)(ui->battwarningDSB->value()*10);
    updateSettings();
}

void GeneralEdit::on_battcalibDSB_editingFinished()
{
    g_eeGeneral.vBatCalib = ui->battcalibDSB->value()*10;
    ui->battCalib->setValue(ui->battcalibDSB->value());
    updateSettings();
}

void GeneralEdit::on_backlightswCB_currentIndexChanged(int index)
{
    g_eeGeneral.lightSw = index-MAX_DRSWITCH;
    updateSettings();
}


void GeneralEdit::on_backlightautoSB_editingFinished()
{
    int i = ui->backlightautoSB->value()/5;
    if((i*5)!=ui->backlightautoSB->value())
        ui->backlightautoSB->setValue(i*5);

    g_eeGeneral.lightAutoOff = i;
    updateSettings();
}

void GeneralEdit::on_backlightStickMove_editingFinished()
{
    int i = ui->backlightStickMove->value()/5;
    if((i*5)!=ui->backlightStickMove->value())
        ui->backlightStickMove->setValue(i*5);

    g_eeGeneral.lightOnStickMove = i;
    updateSettings();
}

void GeneralEdit::on_inactimerSB_editingFinished()
{
    g_eeGeneral.inactivityTimer = ui->inactimerSB->value() - 10;
    updateSettings();
}

void GeneralEdit::on_thrrevChkB_stateChanged(int )
{
    g_eeGeneral.throttleReversed = ui->thrrevChkB->isChecked() ? 1 : 0;
    updateSettings();
}

//void GeneralEdit::on_enablePpmsimChkB_stateChanged(int )
//{
//    g_eeGeneral.enablePpmsim = ui->enablePpmsimChkB->isChecked() ? 1 : 0;
//    updateSettings();
//}

void GeneralEdit::on_internalFrskyAlarmChkB_stateChanged(int )
{
    g_eeGeneral.frskyinternalalarm = ui->internalFrskyAlarmChkB->isChecked() ? 1 : 0;
    updateSettings();
}
		
//void GeneralEdit::on_backlightinvertChkB_stateChanged(int )
//{
//    g_eeGeneral.blightinv = ui->backlightinvertChkB->isChecked() ? 1 : 0;
//    updateSettings();
//}

void GeneralEdit::on_inputfilterCB_currentIndexChanged(int index)
{
    g_eeGeneral.filterInput = index;
    updateSettings();
}

void GeneralEdit::on_thrwarnChkB_stateChanged(int )
{
    g_eeGeneral.disableThrottleWarning = ui->thrwarnChkB->isChecked() ? 0 : 1;
    updateSettings();
}

void GeneralEdit::on_switchwarnChkB_stateChanged(int )
{
    g_eeGeneral.disableSwitchWarning = ui->switchwarnChkB->isChecked() ? 0 : 1;
    updateSettings();
}

//void GeneralEdit::on_memwarnChkB_stateChanged(int )
//{
//    g_eeGeneral.disableMemoryWarning = ui->memwarnChkB->isChecked() ? 0 : 1;
//    updateSettings();
//}

void GeneralEdit::on_alarmwarnChkB_stateChanged(int )
{
    g_eeGeneral.disableAlarmWarning = ui->alarmwarnChkB->isChecked() ? 0 : 1;
    updateSettings();
}

void GeneralEdit::on_PotScrollEnableChkB_stateChanged(int )
{
    g_eeGeneral.disablePotScroll = ui->PotScrollEnableChkB->isChecked() ? 0 : 1;
    updateSettings();
}

//void GeneralEdit::on_BandGapEnableChkB_stateChanged(int )
//{
//    g_eeGeneral.disableBG = ui->BandGapEnableChkB->isChecked() ? 0 : 1;
//    updateSettings();
//}

void GeneralEdit::on_beeperCB_currentIndexChanged(int index)
{
    g_eeGeneral.beeperVal = index;
    updateSettings();
}

void GeneralEdit::on_channelorderCB_currentIndexChanged(int index)
{
    g_eeGeneral.templateSetup = index;
    updateSettings();
}

void GeneralEdit::on_stickmodeCB_currentIndexChanged(int index)
{
    g_eeGeneral.stickMode = index;
    updateSettings();
}

void GeneralEdit::on_RotaryDivisorCB_currentIndexChanged(int index)
{
    g_eeGeneral.rotaryDivisor = index;
    updateSettings();
}

void GeneralEdit::on_BtBaudrateCB_currentIndexChanged(int index)
{
    g_eeGeneral.bt_baudrate = index;
    updateSettings();
}

void GeneralEdit::on_ana1Neg_editingFinished()
{
    g_eeGeneral.calibSpanNeg[0] = ui->ana1Neg->value();
    updateSettings();
}

void GeneralEdit::on_ana2Neg_editingFinished()
{
    g_eeGeneral.calibSpanNeg[1] = ui->ana2Neg->value();
    updateSettings();
}

void GeneralEdit::on_ana3Neg_editingFinished()
{
    g_eeGeneral.calibSpanNeg[2] = ui->ana3Neg->value();
    updateSettings();
}

void GeneralEdit::on_ana4Neg_editingFinished()
{
    g_eeGeneral.calibSpanNeg[3] = ui->ana4Neg->value();
    updateSettings();
}

void GeneralEdit::on_ana5Neg_editingFinished()
{
    g_eeGeneral.calibSpanNeg[4] = ui->ana5Neg->value();
    updateSettings();
}

void GeneralEdit::on_ana6Neg_editingFinished()
{
    g_eeGeneral.calibSpanNeg[5] = ui->ana6Neg->value();
    updateSettings();
}

void GeneralEdit::on_ana7Neg_editingFinished()
{
    g_eeGeneral.calibSpanNeg[6] = ui->ana7Neg->value();
    updateSettings();
}



void GeneralEdit::on_ana1Mid_editingFinished()
{
    g_eeGeneral.calibMid[0] = ui->ana1Mid->value();
    updateSettings();
}

void GeneralEdit::on_ana2Mid_editingFinished()
{
    g_eeGeneral.calibMid[1] = ui->ana2Mid->value();
    updateSettings();
}

void GeneralEdit::on_ana3Mid_editingFinished()
{
    g_eeGeneral.calibMid[2] = ui->ana3Mid->value();
    updateSettings();
}

void GeneralEdit::on_ana4Mid_editingFinished()
{
    g_eeGeneral.calibMid[3] = ui->ana4Mid->value();
    updateSettings();
}

void GeneralEdit::on_ana5Mid_editingFinished()
{
    g_eeGeneral.calibMid[4] = ui->ana5Mid->value();
    updateSettings();
}

void GeneralEdit::on_ana6Mid_editingFinished()
{
    g_eeGeneral.calibMid[5] = ui->ana6Mid->value();
    updateSettings();
}

void GeneralEdit::on_ana7Mid_editingFinished()
{
    g_eeGeneral.calibMid[6] = ui->ana7Mid->value();
    updateSettings();
}



void GeneralEdit::on_ana1Pos_editingFinished()
{
    g_eeGeneral.calibSpanPos[0] = ui->ana1Pos->value();
    updateSettings();
}

void GeneralEdit::on_ana2Pos_editingFinished()
{
    g_eeGeneral.calibSpanPos[1] = ui->ana2Pos->value();
    updateSettings();
}

void GeneralEdit::on_ana3Pos_editingFinished()
{
    g_eeGeneral.calibSpanPos[2] = ui->ana3Pos->value();
    updateSettings();
}

void GeneralEdit::on_ana4Pos_editingFinished()
{
    g_eeGeneral.calibSpanPos[3] = ui->ana4Pos->value();
    updateSettings();
}

void GeneralEdit::on_ana5Pos_editingFinished()
{
    g_eeGeneral.calibSpanNeg[4] = ui->ana5Pos->value();
    updateSettings();
}

void GeneralEdit::on_ana6Pos_editingFinished()
{
    g_eeGeneral.calibSpanNeg[5] = ui->ana6Pos->value();
    updateSettings();
}

void GeneralEdit::on_ana7Pos_editingFinished()
{
    g_eeGeneral.calibSpanPos[6] = ui->ana7Pos->value();
    updateSettings();
}

void GeneralEdit::on_battCalib_editingFinished()
{
    g_eeGeneral.vBatCalib = ui->battCalib->value()*10;
    ui->battcalibDSB->setValue(ui->battCalib->value());
    updateSettings();
}


void GeneralEdit::on_tabWidget_currentChanged(int index)
{
    QSettings settings("er9x-eePe", "eePe");
    settings.setValue("generalEditTab",index);//ui->tabWidget->currentIndex());
}


void GeneralEdit::on_beepMinuteChkB_stateChanged(int )
{
    g_eeGeneral.minuteBeep = ui->beepMinuteChkB->isChecked() ? 1 : 0;
    updateSettings();
}

void GeneralEdit::on_beepCountDownChkB_stateChanged(int )
{
    g_eeGeneral.preBeep = ui->beepCountDownChkB->isChecked() ? 1 : 0;
    updateSettings();
}

void GeneralEdit::on_beepFlashChkB_stateChanged(int )
{
    g_eeGeneral.flashBeep = ui->beepFlashChkB->isChecked() ? 1 : 0;
    updateSettings();
}

void GeneralEdit::on_splashScreenChkB_stateChanged(int )
{
    g_eeGeneral.disableSplashScreen = ui->splashScreenChkB->isChecked() ? 0 : 1;
    updateSettings();
}

void GeneralEdit::on_ownerNameLE_editingFinished()
{
    memset(&g_eeGeneral.ownerName,' ',sizeof(g_eeGeneral.ownerName));
    for(quint8 i=0; i<(ui->ownerNameLE->text().length()); i++)
    {
        if(i>=sizeof(g_eeGeneral.ownerName)) break;
        g_eeGeneral.ownerName[i] = ui->ownerNameLE->text().toStdString()[i];
    }

    updateSettings();
}

void GeneralEdit::on_speakerPitchSB_editingFinished()
{
    g_eeGeneral.speakerPitch = ui->speakerPitchSB->value();
    updateSettings();
}

void GeneralEdit::on_hapticStengthSB_editingFinished()
{
    g_eeGeneral.hapticStrength = ui->hapticStengthSB->value();
    updateSettings();
}

void GeneralEdit::on_soundModeCB_currentIndexChanged(int index)
{
    g_eeGeneral.speakerMode = index;
    updateSettings();
}

void GeneralEdit::on_tabWidget_selected(QString )
{
    ui->chnLabel_1->setText(getSourceStr(g_eeGeneral.stickMode,1));
    ui->chnLabel_2->setText(getSourceStr(g_eeGeneral.stickMode,2));
    ui->chnLabel_3->setText(getSourceStr(g_eeGeneral.stickMode,3));
    ui->chnLabel_4->setText(getSourceStr(g_eeGeneral.stickMode,4));
}



void GeneralEdit::on_splashScreenNameChkB_stateChanged(int )
{
    g_eeGeneral.hideNameOnSplash = ui->splashScreenNameChkB->isChecked() ? 0 : 1;
    updateSettings();
}

void GeneralEdit::getGeneralSwitchDefPos(int i, bool val)
{
    if(val)
        g_eeGeneral.switchWarningStates |= (1<<(i-1));
    else
        g_eeGeneral.switchWarningStates &= ~(1<<(i-1));
}

void GeneralEdit::on_switchDefPos_1_stateChanged(int )
{
    if(switchDefPosEditLock) return;
    getGeneralSwitchDefPos(1,ui->switchDefPos_1->isChecked());
    updateSettings();
}
void GeneralEdit::on_switchDefPos_2_stateChanged(int )
{
    if(switchDefPosEditLock) return;
    getGeneralSwitchDefPos(2,ui->switchDefPos_2->isChecked());
    updateSettings();
}
void GeneralEdit::on_switchDefPos_3_stateChanged(int )
{
    getGeneralSwitchDefPos(3,ui->switchDefPos_3->isChecked());
    updateSettings();
}
void GeneralEdit::on_switchDefPos_4_stateChanged(int )
{
    if(switchDefPosEditLock) return;

    if(ui->switchDefPos_4->isChecked())
    {
        switchDefPosEditLock = true;
        ui->switchDefPos_5->setChecked(false);
        ui->switchDefPos_6->setChecked(false);
        switchDefPosEditLock = false;
    }
    else
        return;

    g_eeGeneral.switchWarningStates &= ~0x30; //turn off ID1/2
    getGeneralSwitchDefPos(4,ui->switchDefPos_4->isChecked());
    updateSettings();
}
void GeneralEdit::on_switchDefPos_5_stateChanged(int )
{
    if(switchDefPosEditLock) return;

    if(ui->switchDefPos_5->isChecked())
    {
        switchDefPosEditLock = true;
        ui->switchDefPos_4->setChecked(false);
        ui->switchDefPos_6->setChecked(false);
        switchDefPosEditLock = false;
    }
    else
        return;

    g_eeGeneral.switchWarningStates &= ~0x28; //turn off ID0/2
    getGeneralSwitchDefPos(5,ui->switchDefPos_5->isChecked());
    updateSettings();
}
void GeneralEdit::on_switchDefPos_6_stateChanged(int )
{
    if(switchDefPosEditLock) return;

    if(ui->switchDefPos_6->isChecked())
    {
        switchDefPosEditLock = true;
        ui->switchDefPos_4->setChecked(false);
        ui->switchDefPos_5->setChecked(false);
        switchDefPosEditLock = false;
    }
    else
        return;

    g_eeGeneral.switchWarningStates &= ~0x18; //turn off ID1/2
    getGeneralSwitchDefPos(6,ui->switchDefPos_6->isChecked());
    updateSettings();
}
void GeneralEdit::on_switchDefPos_7_stateChanged(int )
{
    if(switchDefPosEditLock) return;
    getGeneralSwitchDefPos(7,ui->switchDefPos_7->isChecked());
    updateSettings();
}
void GeneralEdit::on_switchDefPos_8_stateChanged(int )
{
    if(switchDefPosEditLock) return;
    getGeneralSwitchDefPos(8,ui->switchDefPos_8->isChecked());
    updateSettings();
}
