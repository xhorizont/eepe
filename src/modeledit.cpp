#include "modeledit.h"
#include "ui_modeledit.h"
#include "pers.h"
#include "helpers.h"

#include <QtGui>

#define BC_BIT_RUD (0x01)
#define BC_BIT_ELE (0x02)
#define BC_BIT_THR (0x04)
#define BC_BIT_AIL (0x08)
#define BC_BIT_P1  (0x10)
#define BC_BIT_P2  (0x20)
#define BC_BIT_P3  (0x40)

ModelEdit::ModelEdit(EEPFILE *eFile, uint8_t id, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ModelEdit)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/icon.ico"));

    eeFile = eFile;

    if(!eeFile->eeLoadGeneral())  eeFile->generalDefault();
    eeFile->getGeneralSettings(&g_eeGeneral);
    eeFile->getModel(&g_model,id);
    id_model = id;

    QSettings settings("er9x-eePe", "eePe");
    ui->tabWidget->setCurrentIndex(settings.value("modelEditTab", 0).toInt());

    tabModelEditSetup();
    tabLimits();
    tabTrims();
}

ModelEdit::~ModelEdit()
{
    delete ui;
}

void ModelEdit::updateSettings()
{
    eeFile->putModel(&g_model,id_model);
}

void ModelEdit::on_tabWidget_currentChanged(int index)
{
    QSettings settings("er9x-eePe", "eePe");
    settings.setValue("modelEditTab",index);//ui->tabWidget->currentIndex());
}

void ModelEdit::tabModelEditSetup()
{
    //name
    ui->modelNameLE->setText(g_model.name);

    //timer mode direction value
    populateTimerSwitchCB(ui->timerModeCB,g_model.tmrMode);
    int min = g_model.tmrVal/60;
    int sec = g_model.tmrVal%60;
    ui->timerValTE->setTime(QTime(0,min,sec));

    //trim inc, thro trim, thro expo, instatrim
    ui->trimIncCB->setCurrentIndex(g_model.trimInc);
    populateSwitchCB(ui->trimSWCB,g_model.trimSw);
    ui->thrExpoChkB->setChecked(g_model.thrExpo);
    ui->thrTrimChkB->setChecked(g_model.thrTrim);

    //center beep
    ui->bcRUDChkB->setChecked(g_model.beepANACenter & BC_BIT_RUD);
    ui->bcELEChkB->setChecked(g_model.beepANACenter & BC_BIT_ELE);
    ui->bcTHRChkB->setChecked(g_model.beepANACenter & BC_BIT_THR);
    ui->bcAILChkB->setChecked(g_model.beepANACenter & BC_BIT_AIL);
    ui->bcP1ChkB->setChecked(g_model.beepANACenter & BC_BIT_P1);
    ui->bcP2ChkB->setChecked(g_model.beepANACenter & BC_BIT_P2);
    ui->bcP3ChkB->setChecked(g_model.beepANACenter & BC_BIT_P3);

    //pulse polarity
    ui->pulsePolCB->setCurrentIndex(g_model.pulsePol);

    //protocol channels ppm delay (disable if needed)
    ui->protocolCB->setCurrentIndex(g_model.protocol);
    ui->ppmDelaySB->setValue(300+50*g_model.ppmDelay);
    ui->numChannelsSB->setValue(8+2*g_model.ppmNCH);
    ui->ppmDelaySB->setEnabled(!g_model.protocol);
    ui->numChannelsSB->setEnabled(!g_model.protocol);
}

void ModelEdit::tabLimits()
{
    ui->offsetDSB_1->setValue(g_model.limitData[0].offset/10);
    ui->offsetDSB_2->setValue(g_model.limitData[1].offset/10);
    ui->offsetDSB_3->setValue(g_model.limitData[2].offset/10);
    ui->offsetDSB_4->setValue(g_model.limitData[3].offset/10);
    ui->offsetDSB_5->setValue(g_model.limitData[4].offset/10);
    ui->offsetDSB_6->setValue(g_model.limitData[5].offset/10);
    ui->offsetDSB_7->setValue(g_model.limitData[6].offset/10);
    ui->offsetDSB_8->setValue(g_model.limitData[7].offset/10);
    ui->offsetDSB_9->setValue(g_model.limitData[8].offset/10);
    ui->offsetDSB_10->setValue(g_model.limitData[9].offset/10);
    ui->offsetDSB_11->setValue(g_model.limitData[10].offset/10);
    ui->offsetDSB_12->setValue(g_model.limitData[11].offset/10);
    ui->offsetDSB_13->setValue(g_model.limitData[12].offset/10);
    ui->offsetDSB_14->setValue(g_model.limitData[13].offset/10);
    ui->offsetDSB_15->setValue(g_model.limitData[14].offset/10);
    ui->offsetDSB_16->setValue(g_model.limitData[15].offset/10);

    ui->minSB_1->setValue(g_model.limitData[0].min-100);
    ui->minSB_2->setValue(g_model.limitData[1].min-100);
    ui->minSB_3->setValue(g_model.limitData[2].min-100);
    ui->minSB_4->setValue(g_model.limitData[3].min-100);
    ui->minSB_5->setValue(g_model.limitData[4].min-100);
    ui->minSB_6->setValue(g_model.limitData[5].min-100);
    ui->minSB_7->setValue(g_model.limitData[6].min-100);
    ui->minSB_8->setValue(g_model.limitData[7].min-100);
    ui->minSB_9->setValue(g_model.limitData[8].min-100);
    ui->minSB_10->setValue(g_model.limitData[9].min-100);
    ui->minSB_11->setValue(g_model.limitData[10].min-100);
    ui->minSB_12->setValue(g_model.limitData[11].min-100);
    ui->minSB_13->setValue(g_model.limitData[12].min-100);
    ui->minSB_14->setValue(g_model.limitData[13].min-100);
    ui->minSB_15->setValue(g_model.limitData[14].min-100);
    ui->minSB_16->setValue(g_model.limitData[15].min-100);

    ui->maxSB_1->setValue(g_model.limitData[0].max+100);
    ui->maxSB_2->setValue(g_model.limitData[1].max+100);
    ui->maxSB_3->setValue(g_model.limitData[2].max+100);
    ui->maxSB_4->setValue(g_model.limitData[3].max+100);
    ui->maxSB_5->setValue(g_model.limitData[4].max+100);
    ui->maxSB_6->setValue(g_model.limitData[5].max+100);
    ui->maxSB_7->setValue(g_model.limitData[6].max+100);
    ui->maxSB_8->setValue(g_model.limitData[7].max+100);
    ui->maxSB_9->setValue(g_model.limitData[8].max+100);
    ui->maxSB_10->setValue(g_model.limitData[9].max+100);
    ui->maxSB_11->setValue(g_model.limitData[10].max+100);
    ui->maxSB_12->setValue(g_model.limitData[11].max+100);
    ui->maxSB_13->setValue(g_model.limitData[12].max+100);
    ui->maxSB_14->setValue(g_model.limitData[13].max+100);
    ui->maxSB_15->setValue(g_model.limitData[14].max+100);
    ui->maxSB_16->setValue(g_model.limitData[15].max+100);

    ui->chInvCB_1->setCurrentIndex((g_model.limitData[0].revert) ? 1 : 0);
    ui->chInvCB_2->setCurrentIndex((g_model.limitData[1].revert) ? 1 : 0);
    ui->chInvCB_3->setCurrentIndex((g_model.limitData[2].revert) ? 1 : 0);
    ui->chInvCB_4->setCurrentIndex((g_model.limitData[3].revert) ? 1 : 0);
    ui->chInvCB_5->setCurrentIndex((g_model.limitData[4].revert) ? 1 : 0);
    ui->chInvCB_6->setCurrentIndex((g_model.limitData[5].revert) ? 1 : 0);
    ui->chInvCB_7->setCurrentIndex((g_model.limitData[6].revert) ? 1 : 0);
    ui->chInvCB_8->setCurrentIndex((g_model.limitData[7].revert) ? 1 : 0);
    ui->chInvCB_9->setCurrentIndex((g_model.limitData[8].revert) ? 1 : 0);
    ui->chInvCB_10->setCurrentIndex((g_model.limitData[9].revert) ? 1 : 0);
    ui->chInvCB_11->setCurrentIndex((g_model.limitData[10].revert) ? 1 : 0);
    ui->chInvCB_12->setCurrentIndex((g_model.limitData[11].revert) ? 1 : 0);
    ui->chInvCB_13->setCurrentIndex((g_model.limitData[12].revert) ? 1 : 0);
    ui->chInvCB_14->setCurrentIndex((g_model.limitData[13].revert) ? 1 : 0);
    ui->chInvCB_15->setCurrentIndex((g_model.limitData[14].revert) ? 1 : 0);
    ui->chInvCB_16->setCurrentIndex((g_model.limitData[15].revert) ? 1 : 0);
}

void ModelEdit::tabTrims()
{
    ui->spinBox_S1->setValue(g_model.trim[CONVERT_MODE(1)-1]);
    ui->spinBox_S2->setValue(g_model.trim[CONVERT_MODE(2)-1]);
    ui->spinBox_S3->setValue(g_model.trim[CONVERT_MODE(3)-1]);
    ui->spinBox_S4->setValue(g_model.trim[CONVERT_MODE(4)-1]);


    switch (g_eeGeneral.stickMode)
    {
        case (0):
            ui->Label_S1->setText("RUD");
            ui->Label_S2->setText("ELE");
            ui->Label_S3->setText("THR");
            ui->Label_S4->setText("AIL");
            break;
        case (1):
            ui->Label_S1->setText("RUD");
            ui->Label_S2->setText("THR");
            ui->Label_S3->setText("ELE");
            ui->Label_S4->setText("AIL");
            break;
        case (2):
            ui->Label_S1->setText("AIL");
            ui->Label_S2->setText("ELE");
            ui->Label_S3->setText("THR");
            ui->Label_S4->setText("RUD");
            break;
        case (3):
            ui->Label_S1->setText("AIL");
            ui->Label_S2->setText("THR");
            ui->Label_S3->setText("ELE");
            ui->Label_S4->setText("RUD");
            break;
    }

}


void ModelEdit::on_modelNameLE_textEdited(QString txt)
{
    //allow uppercase, number, ' ', '.', '_', '-' only
    ui->modelNameLE->setText(txt.toUpper());
}

void ModelEdit::on_modelNameLE_editingFinished()
{
    uint8_t temp = g_model.mdVers;
    const char *c = ui->modelNameLE->text().left(10).toAscii();
    strcpy((char*)&g_model.name,c);
    g_model.mdVers = temp;  //in case strcpy overruns
    updateSettings();
}

void ModelEdit::on_timerModeCB_currentIndexChanged(int index)
{
    g_model.tmrMode = index-TMR_NUM_OPTION;
    updateSettings();
}

void ModelEdit::on_timerDirCB_currentIndexChanged(int index)
{
    g_model.tmrDir = index;
    updateSettings();
}

void ModelEdit::on_trimIncCB_currentIndexChanged(int index)
{
    g_model.trimInc = index;
    updateSettings();
}

void ModelEdit::on_trimSWCB_currentIndexChanged(int index)
{
    g_model.trimSw = index-MAX_DRSWITCH;
    updateSettings();
}

void ModelEdit::on_pulsePolCB_currentIndexChanged(int index)
{
    g_model.pulsePol = index;
    updateSettings();
}

void ModelEdit::on_protocolCB_currentIndexChanged(int index)
{
    g_model.protocol = index;
    updateSettings();

    ui->ppmDelaySB->setEnabled(!g_model.protocol);
    ui->numChannelsSB->setEnabled(!g_model.protocol);
}

void ModelEdit::on_timerValTE_editingFinished()
{
    g_model.tmrVal = ui->timerValTE->time().minute()*60 + ui->timerValTE->time().second();
    updateSettings();
}

void ModelEdit::on_numChannelsSB_editingFinished()
{
    int i = (ui->numChannelsSB->value()-8)/2;
    if((i*2+8)!=ui->numChannelsSB->value()) ui->numChannelsSB->setValue(i*2+8);
    g_model.ppmNCH = i;
    updateSettings();
}

void ModelEdit::on_ppmDelaySB_editingFinished()
{
    int i = (ui->ppmDelaySB->value()-300)/50;
    if((i*50+300)!=ui->ppmDelaySB->value()) ui->ppmDelaySB->setValue(i*50+300);
    g_model.ppmDelay = i;
    updateSettings();
}


void ModelEdit::on_thrTrimChkB_toggled(bool checked)
{
    g_model.thrTrim = checked;
    updateSettings();
}

void ModelEdit::on_thrExpoChkB_toggled(bool checked)
{
    g_model.thrExpo = checked;
    updateSettings();
}

void ModelEdit::on_bcRUDChkB_toggled(bool checked)
{
    if(checked)
        g_model.beepANACenter |= BC_BIT_RUD;
    else
        g_model.beepANACenter &= ~BC_BIT_RUD;
    updateSettings();
}

void ModelEdit::on_bcELEChkB_toggled(bool checked)
{
    if(checked)
        g_model.beepANACenter |= BC_BIT_ELE;
    else
        g_model.beepANACenter &= ~BC_BIT_ELE;
    updateSettings();
}

void ModelEdit::on_bcTHRChkB_toggled(bool checked)
{
    if(checked)
        g_model.beepANACenter |= BC_BIT_THR;
    else
        g_model.beepANACenter &= ~BC_BIT_THR;
    updateSettings();
}

void ModelEdit::on_bcAILChkB_toggled(bool checked)
{
    if(checked)
        g_model.beepANACenter |= BC_BIT_AIL;
    else
        g_model.beepANACenter &= ~BC_BIT_AIL;
    updateSettings();
}

void ModelEdit::on_bcP1ChkB_toggled(bool checked)
{
    if(checked)
        g_model.beepANACenter |= BC_BIT_P1;
    else
        g_model.beepANACenter &= ~BC_BIT_P1;
    updateSettings();
}

void ModelEdit::on_bcP2ChkB_toggled(bool checked)
{
    if(checked)
        g_model.beepANACenter |= BC_BIT_P2;
    else
        g_model.beepANACenter &= ~BC_BIT_P2;
    updateSettings();
}

void ModelEdit::on_bcP3ChkB_toggled(bool checked)
{
    if(checked)
        g_model.beepANACenter |= BC_BIT_P3;
    else
        g_model.beepANACenter &= ~BC_BIT_P3;
    updateSettings();
}


void ModelEdit::on_spinBox_S1_valueChanged(int value)
{
        g_model.trim[CONVERT_MODE(1)-1] = value;
        updateSettings();
}

void ModelEdit::on_spinBox_S2_valueChanged(int value)
{
        g_model.trim[CONVERT_MODE(2)-1] = value;
        updateSettings();
}

void ModelEdit::on_spinBox_S3_valueChanged(int value)
{
        g_model.trim[CONVERT_MODE(3)-1] = value;
        updateSettings();
}

void ModelEdit::on_spinBox_S4_valueChanged(int value)
{
        g_model.trim[CONVERT_MODE(4)-1] = value;
        updateSettings();
}

void ModelEdit::on_offsetDSB_1_editingFinished()
{
    g_model.limitData[0].offset = ui->offsetDSB_1->value()*10;
    updateSettings();
}

void ModelEdit::on_offsetDSB_2_editingFinished()
{
    g_model.limitData[1].offset = ui->offsetDSB_2->value()*10;
    updateSettings();
}

void ModelEdit::on_offsetDSB_3_editingFinished()
{
    g_model.limitData[2].offset = ui->offsetDSB_3->value()*10;
    updateSettings();
}

void ModelEdit::on_offsetDSB_4_editingFinished()
{
    g_model.limitData[3].offset = ui->offsetDSB_4->value()*10;
    updateSettings();
}

void ModelEdit::on_offsetDSB_5_editingFinished()
{
    g_model.limitData[4].offset = ui->offsetDSB_5->value()*10;
    updateSettings();
}

void ModelEdit::on_offsetDSB_6_editingFinished()
{
    g_model.limitData[5].offset = ui->offsetDSB_6->value()*10;
    updateSettings();
}

void ModelEdit::on_offsetDSB_7_editingFinished()
{
    g_model.limitData[6].offset = ui->offsetDSB_7->value()*10;
    updateSettings();
}

void ModelEdit::on_offsetDSB_8_editingFinished()
{
    g_model.limitData[7].offset = ui->offsetDSB_8->value()*10;
    updateSettings();
}

void ModelEdit::on_offsetDSB_9_editingFinished()
{
    g_model.limitData[8].offset = ui->offsetDSB_9->value()*10;
    updateSettings();
}

void ModelEdit::on_offsetDSB_10_editingFinished()
{
    g_model.limitData[9].offset = ui->offsetDSB_10->value()*10;
    updateSettings();
}

void ModelEdit::on_offsetDSB_11_editingFinished()
{
    g_model.limitData[10].offset = ui->offsetDSB_11->value()*10;
    updateSettings();
}

void ModelEdit::on_offsetDSB_12_editingFinished()
{
    g_model.limitData[11].offset = ui->offsetDSB_12->value()*10;
    updateSettings();
}

void ModelEdit::on_offsetDSB_13_editingFinished()
{
    g_model.limitData[12].offset = ui->offsetDSB_13->value()*10;
    updateSettings();
}

void ModelEdit::on_offsetDSB_14_editingFinished()
{
    g_model.limitData[13].offset = ui->offsetDSB_14->value()*10;
    updateSettings();
}

void ModelEdit::on_offsetDSB_15_editingFinished()
{
    g_model.limitData[14].offset = ui->offsetDSB_15->value()*10;
    updateSettings();
}

void ModelEdit::on_offsetDSB_16_editingFinished()
{
    g_model.limitData[15].offset = ui->offsetDSB_16->value()*10;
    updateSettings();
}



void ModelEdit::on_minSB_1_editingFinished()
{
    g_model.limitData[0].min = ui->minSB_1->value()+100;
    updateSettings();
}

void ModelEdit::on_minSB_2_editingFinished()
{
    g_model.limitData[1].min = ui->minSB_2->value()+100;
    updateSettings();
}

void ModelEdit::on_minSB_3_editingFinished()
{
    g_model.limitData[2].min = ui->minSB_3->value()+10;
    updateSettings();
}

void ModelEdit::on_minSB_4_editingFinished()
{
    g_model.limitData[3].min = ui->minSB_4->value()+100;
    updateSettings();
}

void ModelEdit::on_minSB_5_editingFinished()
{
    g_model.limitData[4].min = ui->minSB_5->value()+100;
    updateSettings();
}

void ModelEdit::on_minSB_6_editingFinished()
{
    g_model.limitData[5].min = ui->minSB_6->value()+100;
    updateSettings();
}

void ModelEdit::on_minSB_7_editingFinished()
{
    g_model.limitData[6].min = ui->minSB_7->value()+100;
    updateSettings();
}

void ModelEdit::on_minSB_8_editingFinished()
{
    g_model.limitData[7].min = ui->minSB_8->value()+100;
    updateSettings();
}

void ModelEdit::on_minSB_9_editingFinished()
{
    g_model.limitData[8].min = ui->minSB_9->value()+100;
    updateSettings();
}

void ModelEdit::on_minSB_10_editingFinished()
{
    g_model.limitData[9].min = ui->minSB_10->value()+100;
    updateSettings();
}

void ModelEdit::on_minSB_11_editingFinished()
{
    g_model.limitData[10].min = ui->minSB_11->value()+100;
    updateSettings();
}

void ModelEdit::on_minSB_12_editingFinished()
{
    g_model.limitData[11].min = ui->minSB_12->value()+100;
    updateSettings();
}

void ModelEdit::on_minSB_13_editingFinished()
{
    g_model.limitData[12].min = ui->minSB_13->value()+100;
    updateSettings();
}

void ModelEdit::on_minSB_14_editingFinished()
{
    g_model.limitData[13].min = ui->minSB_14->value()+100;
    updateSettings();
}

void ModelEdit::on_minSB_15_editingFinished()
{
    g_model.limitData[14].min = ui->minSB_15->value()+100;
    updateSettings();
}

void ModelEdit::on_minSB_16_editingFinished()
{
    g_model.limitData[15].min = ui->minSB_16->value()+100;
    updateSettings();
}


void ModelEdit::on_maxSB_1_editingFinished()
{
    g_model.limitData[0].max = ui->maxSB_1->value()-100;
    updateSettings();
}

void ModelEdit::on_maxSB_2_editingFinished()
{
    g_model.limitData[1].max = ui->maxSB_2->value()-100;
    updateSettings();
}

void ModelEdit::on_maxSB_3_editingFinished()
{
    g_model.limitData[2].max = ui->maxSB_3->value()-100;
    updateSettings();
}

void ModelEdit::on_maxSB_4_editingFinished()
{
    g_model.limitData[3].max = ui->maxSB_4->value()-100;
    updateSettings();
}

void ModelEdit::on_maxSB_5_editingFinished()
{
    g_model.limitData[4].max = ui->maxSB_5->value()-100;
    updateSettings();
}

void ModelEdit::on_maxSB_6_editingFinished()
{
    g_model.limitData[5].max = ui->maxSB_6->value()-100;
    updateSettings();
}

void ModelEdit::on_maxSB_7_editingFinished()
{
    g_model.limitData[6].max = ui->maxSB_7->value()-100;
    updateSettings();
}

void ModelEdit::on_maxSB_8_editingFinished()
{
    g_model.limitData[7].max = ui->maxSB_8->value()-100;
    updateSettings();
}

void ModelEdit::on_maxSB_9_editingFinished()
{
    g_model.limitData[8].max = ui->maxSB_9->value()-100;
    updateSettings();
}

void ModelEdit::on_maxSB_10_editingFinished()
{
    g_model.limitData[9].max = ui->maxSB_10->value()-100;
    updateSettings();
}

void ModelEdit::on_maxSB_11_editingFinished()
{
    g_model.limitData[10].max = ui->maxSB_11->value()-100;
    updateSettings();
}

void ModelEdit::on_maxSB_12_editingFinished()
{
    g_model.limitData[11].max = ui->maxSB_12->value()-100;
    updateSettings();
}

void ModelEdit::on_maxSB_13_editingFinished()
{
    g_model.limitData[12].max = ui->maxSB_13->value()-100;
    updateSettings();
}

void ModelEdit::on_maxSB_14_editingFinished()
{
    g_model.limitData[13].max = ui->maxSB_14->value()-100;
    updateSettings();
}

void ModelEdit::on_maxSB_15_editingFinished()
{
    g_model.limitData[14].max = ui->maxSB_16->value()-100;
    updateSettings();
}

void ModelEdit::on_maxSB_16_editingFinished()
{
    g_model.limitData[15].max = ui->maxSB_16->value()-100;
    updateSettings();
}


void ModelEdit::on_chInvCB_1_currentIndexChanged(int index)
{
    g_model.limitData[0].revert = index;
    updateSettings();
}

void ModelEdit::on_chInvCB_2_currentIndexChanged(int index)
{
    g_model.limitData[1].revert = index;
    updateSettings();
}

void ModelEdit::on_chInvCB_3_currentIndexChanged(int index)
{
    g_model.limitData[2].revert = index;
    updateSettings();
}

void ModelEdit::on_chInvCB_4_currentIndexChanged(int index)
{
    g_model.limitData[3].revert = index;
    updateSettings();
}

void ModelEdit::on_chInvCB_5_currentIndexChanged(int index)
{
    g_model.limitData[4].revert = index;
    updateSettings();
}

void ModelEdit::on_chInvCB_6_currentIndexChanged(int index)
{
    g_model.limitData[5].revert = index;
    updateSettings();
}

void ModelEdit::on_chInvCB_7_currentIndexChanged(int index)
{
    g_model.limitData[6].revert = index;
    updateSettings();
}

void ModelEdit::on_chInvCB_8_currentIndexChanged(int index)
{
    g_model.limitData[7].revert = index;
    updateSettings();
}

void ModelEdit::on_chInvCB_9_currentIndexChanged(int index)
{
    g_model.limitData[8].revert = index;
    updateSettings();
}

void ModelEdit::on_chInvCB_10_currentIndexChanged(int index)
{
    g_model.limitData[9].revert = index;
    updateSettings();
}

void ModelEdit::on_chInvCB_11_currentIndexChanged(int index)
{
    g_model.limitData[10].revert = index;
    updateSettings();
}

void ModelEdit::on_chInvCB_12_currentIndexChanged(int index)
{
    g_model.limitData[11].revert = index;
    updateSettings();
}

void ModelEdit::on_chInvCB_13_currentIndexChanged(int index)
{
    g_model.limitData[12].revert = index;
    updateSettings();
}

void ModelEdit::on_chInvCB_14_currentIndexChanged(int index)
{
    g_model.limitData[13].revert = index;
    updateSettings();
}

void ModelEdit::on_chInvCB_15_currentIndexChanged(int index)
{
    g_model.limitData[14].revert = index;
    updateSettings();
}

void ModelEdit::on_chInvCB_16_currentIndexChanged(int index)
{
    g_model.limitData[15].revert = index;
    updateSettings();
}

