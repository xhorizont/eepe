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
    tabCurves();
    tabSwitches();
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
    ui->offsetDSB_1->setValue(g_model.limitData[0].offset/10);   connect(ui->offsetDSB_1,SIGNAL(editingFinished()),this,SLOT(limitEdited()));
    ui->offsetDSB_2->setValue(g_model.limitData[1].offset/10);   connect(ui->offsetDSB_2,SIGNAL(editingFinished()),this,SLOT(limitEdited()));
    ui->offsetDSB_3->setValue(g_model.limitData[2].offset/10);   connect(ui->offsetDSB_3,SIGNAL(editingFinished()),this,SLOT(limitEdited()));
    ui->offsetDSB_4->setValue(g_model.limitData[3].offset/10);   connect(ui->offsetDSB_4,SIGNAL(editingFinished()),this,SLOT(limitEdited()));
    ui->offsetDSB_5->setValue(g_model.limitData[4].offset/10);   connect(ui->offsetDSB_5,SIGNAL(editingFinished()),this,SLOT(limitEdited()));
    ui->offsetDSB_6->setValue(g_model.limitData[5].offset/10);   connect(ui->offsetDSB_6,SIGNAL(editingFinished()),this,SLOT(limitEdited()));
    ui->offsetDSB_7->setValue(g_model.limitData[6].offset/10);   connect(ui->offsetDSB_7,SIGNAL(editingFinished()),this,SLOT(limitEdited()));
    ui->offsetDSB_8->setValue(g_model.limitData[7].offset/10);   connect(ui->offsetDSB_8,SIGNAL(editingFinished()),this,SLOT(limitEdited()));
    ui->offsetDSB_9->setValue(g_model.limitData[8].offset/10);   connect(ui->offsetDSB_9,SIGNAL(editingFinished()),this,SLOT(limitEdited()));
    ui->offsetDSB_10->setValue(g_model.limitData[9].offset/10);  connect(ui->offsetDSB_10,SIGNAL(editingFinished()),this,SLOT(limitEdited()));
    ui->offsetDSB_11->setValue(g_model.limitData[10].offset/10); connect(ui->offsetDSB_11,SIGNAL(editingFinished()),this,SLOT(limitEdited()));
    ui->offsetDSB_12->setValue(g_model.limitData[11].offset/10); connect(ui->offsetDSB_12,SIGNAL(editingFinished()),this,SLOT(limitEdited()));
    ui->offsetDSB_13->setValue(g_model.limitData[12].offset/10); connect(ui->offsetDSB_13,SIGNAL(editingFinished()),this,SLOT(limitEdited()));
    ui->offsetDSB_14->setValue(g_model.limitData[13].offset/10); connect(ui->offsetDSB_14,SIGNAL(editingFinished()),this,SLOT(limitEdited()));
    ui->offsetDSB_15->setValue(g_model.limitData[14].offset/10); connect(ui->offsetDSB_15,SIGNAL(editingFinished()),this,SLOT(limitEdited()));
    ui->offsetDSB_16->setValue(g_model.limitData[15].offset/10); connect(ui->offsetDSB_16,SIGNAL(editingFinished()),this,SLOT(limitEdited()));

    ui->minSB_1->setValue(g_model.limitData[0].min-100);   connect(ui->minSB_1,SIGNAL(editingFinished()),this,SLOT(limitEdited()));
    ui->minSB_2->setValue(g_model.limitData[1].min-100);   connect(ui->minSB_2,SIGNAL(editingFinished()),this,SLOT(limitEdited()));
    ui->minSB_3->setValue(g_model.limitData[2].min-100);   connect(ui->minSB_3,SIGNAL(editingFinished()),this,SLOT(limitEdited()));
    ui->minSB_4->setValue(g_model.limitData[3].min-100);   connect(ui->minSB_4,SIGNAL(editingFinished()),this,SLOT(limitEdited()));
    ui->minSB_5->setValue(g_model.limitData[4].min-100);   connect(ui->minSB_5,SIGNAL(editingFinished()),this,SLOT(limitEdited()));
    ui->minSB_6->setValue(g_model.limitData[5].min-100);   connect(ui->minSB_6,SIGNAL(editingFinished()),this,SLOT(limitEdited()));
    ui->minSB_7->setValue(g_model.limitData[6].min-100);   connect(ui->minSB_7,SIGNAL(editingFinished()),this,SLOT(limitEdited()));
    ui->minSB_8->setValue(g_model.limitData[7].min-100);   connect(ui->minSB_8,SIGNAL(editingFinished()),this,SLOT(limitEdited()));
    ui->minSB_9->setValue(g_model.limitData[8].min-100);   connect(ui->minSB_9,SIGNAL(editingFinished()),this,SLOT(limitEdited()));
    ui->minSB_10->setValue(g_model.limitData[9].min-100);  connect(ui->minSB_10,SIGNAL(editingFinished()),this,SLOT(limitEdited()));
    ui->minSB_11->setValue(g_model.limitData[10].min-100); connect(ui->minSB_11,SIGNAL(editingFinished()),this,SLOT(limitEdited()));
    ui->minSB_12->setValue(g_model.limitData[11].min-100); connect(ui->minSB_12,SIGNAL(editingFinished()),this,SLOT(limitEdited()));
    ui->minSB_13->setValue(g_model.limitData[12].min-100); connect(ui->minSB_13,SIGNAL(editingFinished()),this,SLOT(limitEdited()));
    ui->minSB_14->setValue(g_model.limitData[13].min-100); connect(ui->minSB_14,SIGNAL(editingFinished()),this,SLOT(limitEdited()));
    ui->minSB_15->setValue(g_model.limitData[14].min-100); connect(ui->minSB_15,SIGNAL(editingFinished()),this,SLOT(limitEdited()));
    ui->minSB_16->setValue(g_model.limitData[15].min-100); connect(ui->minSB_16,SIGNAL(editingFinished()),this,SLOT(limitEdited()));

    ui->maxSB_1->setValue(g_model.limitData[0].max+100);   connect(ui->maxSB_1,SIGNAL(editingFinished()),this,SLOT(limitEdited()));
    ui->maxSB_2->setValue(g_model.limitData[1].max+100);   connect(ui->maxSB_2,SIGNAL(editingFinished()),this,SLOT(limitEdited()));
    ui->maxSB_3->setValue(g_model.limitData[2].max+100);   connect(ui->maxSB_3,SIGNAL(editingFinished()),this,SLOT(limitEdited()));
    ui->maxSB_4->setValue(g_model.limitData[3].max+100);   connect(ui->maxSB_4,SIGNAL(editingFinished()),this,SLOT(limitEdited()));
    ui->maxSB_5->setValue(g_model.limitData[4].max+100);   connect(ui->maxSB_5,SIGNAL(editingFinished()),this,SLOT(limitEdited()));
    ui->maxSB_6->setValue(g_model.limitData[5].max+100);   connect(ui->maxSB_6,SIGNAL(editingFinished()),this,SLOT(limitEdited()));
    ui->maxSB_7->setValue(g_model.limitData[6].max+100);   connect(ui->maxSB_7,SIGNAL(editingFinished()),this,SLOT(limitEdited()));
    ui->maxSB_8->setValue(g_model.limitData[7].max+100);   connect(ui->maxSB_8,SIGNAL(editingFinished()),this,SLOT(limitEdited()));
    ui->maxSB_9->setValue(g_model.limitData[8].max+100);   connect(ui->maxSB_9,SIGNAL(editingFinished()),this,SLOT(limitEdited()));
    ui->maxSB_10->setValue(g_model.limitData[9].max+100);  connect(ui->maxSB_10,SIGNAL(editingFinished()),this,SLOT(limitEdited()));
    ui->maxSB_11->setValue(g_model.limitData[10].max+100); connect(ui->maxSB_11,SIGNAL(editingFinished()),this,SLOT(limitEdited()));
    ui->maxSB_12->setValue(g_model.limitData[11].max+100); connect(ui->maxSB_12,SIGNAL(editingFinished()),this,SLOT(limitEdited()));
    ui->maxSB_13->setValue(g_model.limitData[12].max+100); connect(ui->maxSB_13,SIGNAL(editingFinished()),this,SLOT(limitEdited()));
    ui->maxSB_14->setValue(g_model.limitData[13].max+100); connect(ui->maxSB_14,SIGNAL(editingFinished()),this,SLOT(limitEdited()));
    ui->maxSB_15->setValue(g_model.limitData[14].max+100); connect(ui->maxSB_15,SIGNAL(editingFinished()),this,SLOT(limitEdited()));
    ui->maxSB_16->setValue(g_model.limitData[15].max+100); connect(ui->maxSB_16,SIGNAL(editingFinished()),this,SLOT(limitEdited()));

    ui->chInvCB_1->setCurrentIndex((g_model.limitData[0].revert) ? 1 : 0);   connect(ui->chInvCB_1,SIGNAL(currentIndexChanged(int)),this,SLOT(limitEdited()));
    ui->chInvCB_2->setCurrentIndex((g_model.limitData[1].revert) ? 1 : 0);   connect(ui->chInvCB_2,SIGNAL(currentIndexChanged(int)),this,SLOT(limitEdited()));
    ui->chInvCB_3->setCurrentIndex((g_model.limitData[2].revert) ? 1 : 0);   connect(ui->chInvCB_3,SIGNAL(currentIndexChanged(int)),this,SLOT(limitEdited()));
    ui->chInvCB_4->setCurrentIndex((g_model.limitData[3].revert) ? 1 : 0);   connect(ui->chInvCB_4,SIGNAL(currentIndexChanged(int)),this,SLOT(limitEdited()));
    ui->chInvCB_5->setCurrentIndex((g_model.limitData[4].revert) ? 1 : 0);   connect(ui->chInvCB_5,SIGNAL(currentIndexChanged(int)),this,SLOT(limitEdited()));
    ui->chInvCB_6->setCurrentIndex((g_model.limitData[5].revert) ? 1 : 0);   connect(ui->chInvCB_6,SIGNAL(currentIndexChanged(int)),this,SLOT(limitEdited()));
    ui->chInvCB_7->setCurrentIndex((g_model.limitData[6].revert) ? 1 : 0);   connect(ui->chInvCB_7,SIGNAL(currentIndexChanged(int)),this,SLOT(limitEdited()));
    ui->chInvCB_8->setCurrentIndex((g_model.limitData[7].revert) ? 1 : 0);   connect(ui->chInvCB_8,SIGNAL(currentIndexChanged(int)),this,SLOT(limitEdited()));
    ui->chInvCB_9->setCurrentIndex((g_model.limitData[8].revert) ? 1 : 0);   connect(ui->chInvCB_9,SIGNAL(currentIndexChanged(int)),this,SLOT(limitEdited()));
    ui->chInvCB_10->setCurrentIndex((g_model.limitData[9].revert) ? 1 : 0);  connect(ui->chInvCB_10,SIGNAL(currentIndexChanged(int)),this,SLOT(limitEdited()));
    ui->chInvCB_11->setCurrentIndex((g_model.limitData[10].revert) ? 1 : 0); connect(ui->chInvCB_11,SIGNAL(currentIndexChanged(int)),this,SLOT(limitEdited()));
    ui->chInvCB_12->setCurrentIndex((g_model.limitData[11].revert) ? 1 : 0); connect(ui->chInvCB_12,SIGNAL(currentIndexChanged(int)),this,SLOT(limitEdited()));
    ui->chInvCB_13->setCurrentIndex((g_model.limitData[12].revert) ? 1 : 0); connect(ui->chInvCB_13,SIGNAL(currentIndexChanged(int)),this,SLOT(limitEdited()));
    ui->chInvCB_14->setCurrentIndex((g_model.limitData[13].revert) ? 1 : 0); connect(ui->chInvCB_14,SIGNAL(currentIndexChanged(int)),this,SLOT(limitEdited()));
    ui->chInvCB_15->setCurrentIndex((g_model.limitData[14].revert) ? 1 : 0); connect(ui->chInvCB_15,SIGNAL(currentIndexChanged(int)),this,SLOT(limitEdited()));
    ui->chInvCB_16->setCurrentIndex((g_model.limitData[15].revert) ? 1 : 0); connect(ui->chInvCB_16,SIGNAL(currentIndexChanged(int)),this,SLOT(limitEdited()));
}

void ModelEdit::tabCurves()
{
   ui->curvePt1_1->setValue(g_model.curves5[0][0]);connect(ui->curvePt1_1,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));
   ui->curvePt2_1->setValue(g_model.curves5[0][1]);connect(ui->curvePt2_1,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));
   ui->curvePt3_1->setValue(g_model.curves5[0][2]);connect(ui->curvePt3_1,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));
   ui->curvePt4_1->setValue(g_model.curves5[0][3]);connect(ui->curvePt4_1,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));
   ui->curvePt5_1->setValue(g_model.curves5[0][4]);connect(ui->curvePt5_1,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));

   ui->curvePt1_2->setValue(g_model.curves5[1][0]);connect(ui->curvePt1_2,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));
   ui->curvePt2_2->setValue(g_model.curves5[1][1]);connect(ui->curvePt2_2,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));
   ui->curvePt3_2->setValue(g_model.curves5[1][2]);connect(ui->curvePt3_2,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));
   ui->curvePt4_2->setValue(g_model.curves5[1][3]);connect(ui->curvePt4_2,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));
   ui->curvePt5_2->setValue(g_model.curves5[1][4]);connect(ui->curvePt5_2,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));

   ui->curvePt1_3->setValue(g_model.curves5[2][0]);connect(ui->curvePt1_3,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));
   ui->curvePt2_3->setValue(g_model.curves5[2][1]);connect(ui->curvePt2_3,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));
   ui->curvePt3_3->setValue(g_model.curves5[2][2]);connect(ui->curvePt3_3,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));
   ui->curvePt4_3->setValue(g_model.curves5[2][3]);connect(ui->curvePt4_3,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));
   ui->curvePt5_3->setValue(g_model.curves5[2][4]);connect(ui->curvePt5_3,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));

   ui->curvePt1_4->setValue(g_model.curves5[3][0]);connect(ui->curvePt1_4,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));
   ui->curvePt2_4->setValue(g_model.curves5[3][1]);connect(ui->curvePt2_4,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));
   ui->curvePt3_4->setValue(g_model.curves5[3][2]);connect(ui->curvePt3_4,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));
   ui->curvePt4_4->setValue(g_model.curves5[3][3]);connect(ui->curvePt4_4,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));
   ui->curvePt5_4->setValue(g_model.curves5[3][4]);connect(ui->curvePt5_4,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));

   ui->curvePt1_5->setValue(g_model.curves5[4][0]);connect(ui->curvePt1_5,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));
   ui->curvePt2_5->setValue(g_model.curves5[4][1]);connect(ui->curvePt2_5,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));
   ui->curvePt3_5->setValue(g_model.curves5[4][2]);connect(ui->curvePt3_5,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));
   ui->curvePt4_5->setValue(g_model.curves5[4][3]);connect(ui->curvePt4_5,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));
   ui->curvePt5_5->setValue(g_model.curves5[4][4]);connect(ui->curvePt5_5,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));

   ui->curvePt1_6->setValue(g_model.curves5[5][0]);connect(ui->curvePt1_6,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));
   ui->curvePt2_6->setValue(g_model.curves5[5][1]);connect(ui->curvePt2_6,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));
   ui->curvePt3_6->setValue(g_model.curves5[5][2]);connect(ui->curvePt3_6,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));
   ui->curvePt4_6->setValue(g_model.curves5[5][3]);connect(ui->curvePt4_6,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));
   ui->curvePt5_6->setValue(g_model.curves5[5][4]);connect(ui->curvePt5_6,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));

   ui->curvePt1_7->setValue(g_model.curves5[6][0]);connect(ui->curvePt1_7,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));
   ui->curvePt2_7->setValue(g_model.curves5[6][1]);connect(ui->curvePt2_7,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));
   ui->curvePt3_7->setValue(g_model.curves5[6][2]);connect(ui->curvePt3_7,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));
   ui->curvePt4_7->setValue(g_model.curves5[6][3]);connect(ui->curvePt4_7,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));
   ui->curvePt5_7->setValue(g_model.curves5[6][4]);connect(ui->curvePt5_7,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));

   ui->curvePt1_8->setValue(g_model.curves5[7][0]);connect(ui->curvePt1_8,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));
   ui->curvePt2_8->setValue(g_model.curves5[7][1]);connect(ui->curvePt2_8,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));
   ui->curvePt3_8->setValue(g_model.curves5[7][2]);connect(ui->curvePt3_8,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));
   ui->curvePt4_8->setValue(g_model.curves5[7][3]);connect(ui->curvePt4_8,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));
   ui->curvePt5_8->setValue(g_model.curves5[7][4]);connect(ui->curvePt5_8,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));

   ui->curvePt1_9->setValue(g_model.curves9[0][0]);connect(ui->curvePt1_9,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));
   ui->curvePt2_9->setValue(g_model.curves9[0][1]);connect(ui->curvePt2_9,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));
   ui->curvePt3_9->setValue(g_model.curves9[0][2]);connect(ui->curvePt3_9,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));
   ui->curvePt4_9->setValue(g_model.curves9[0][3]);connect(ui->curvePt4_9,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));
   ui->curvePt5_9->setValue(g_model.curves9[0][4]);connect(ui->curvePt5_9,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));
   ui->curvePt6_9->setValue(g_model.curves9[0][5]);connect(ui->curvePt6_9,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));
   ui->curvePt7_9->setValue(g_model.curves9[0][6]);connect(ui->curvePt7_9,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));
   ui->curvePt8_9->setValue(g_model.curves9[0][7]);connect(ui->curvePt8_9,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));
   ui->curvePt9_9->setValue(g_model.curves9[0][8]);connect(ui->curvePt9_9,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));

   ui->curvePt1_10->setValue(g_model.curves9[1][0]);connect(ui->curvePt1_10,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));
   ui->curvePt2_10->setValue(g_model.curves9[1][1]);connect(ui->curvePt2_10,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));
   ui->curvePt3_10->setValue(g_model.curves9[1][2]);connect(ui->curvePt3_10,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));
   ui->curvePt4_10->setValue(g_model.curves9[1][3]);connect(ui->curvePt4_10,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));
   ui->curvePt5_10->setValue(g_model.curves9[1][4]);connect(ui->curvePt5_10,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));
   ui->curvePt6_10->setValue(g_model.curves9[1][5]);connect(ui->curvePt6_10,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));
   ui->curvePt7_10->setValue(g_model.curves9[1][6]);connect(ui->curvePt7_10,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));
   ui->curvePt8_10->setValue(g_model.curves9[1][7]);connect(ui->curvePt8_10,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));
   ui->curvePt9_10->setValue(g_model.curves9[1][8]);connect(ui->curvePt9_10,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));

   ui->curvePt1_11->setValue(g_model.curves9[2][0]);connect(ui->curvePt1_11,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));
   ui->curvePt2_11->setValue(g_model.curves9[2][1]);connect(ui->curvePt2_11,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));
   ui->curvePt3_11->setValue(g_model.curves9[2][2]);connect(ui->curvePt3_11,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));
   ui->curvePt4_11->setValue(g_model.curves9[2][3]);connect(ui->curvePt4_11,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));
   ui->curvePt5_11->setValue(g_model.curves9[2][4]);connect(ui->curvePt5_11,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));
   ui->curvePt6_11->setValue(g_model.curves9[2][5]);connect(ui->curvePt6_11,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));
   ui->curvePt7_11->setValue(g_model.curves9[2][6]);connect(ui->curvePt7_11,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));
   ui->curvePt8_11->setValue(g_model.curves9[2][7]);connect(ui->curvePt8_11,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));
   ui->curvePt9_11->setValue(g_model.curves9[2][8]);connect(ui->curvePt9_11,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));

   ui->curvePt1_12->setValue(g_model.curves9[3][0]);connect(ui->curvePt1_12,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));
   ui->curvePt2_12->setValue(g_model.curves9[3][1]);connect(ui->curvePt2_12,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));
   ui->curvePt3_12->setValue(g_model.curves9[3][2]);connect(ui->curvePt3_12,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));
   ui->curvePt4_12->setValue(g_model.curves9[3][3]);connect(ui->curvePt4_12,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));
   ui->curvePt5_12->setValue(g_model.curves9[3][4]);connect(ui->curvePt5_12,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));
   ui->curvePt6_12->setValue(g_model.curves9[3][5]);connect(ui->curvePt6_12,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));
   ui->curvePt7_12->setValue(g_model.curves9[3][6]);connect(ui->curvePt7_12,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));
   ui->curvePt8_12->setValue(g_model.curves9[3][7]);connect(ui->curvePt8_12,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));
   ui->curvePt9_12->setValue(g_model.curves9[3][8]);connect(ui->curvePt9_12,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));

   ui->curvePt1_13->setValue(g_model.curves9[4][0]);connect(ui->curvePt1_13,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));
   ui->curvePt2_13->setValue(g_model.curves9[4][1]);connect(ui->curvePt2_13,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));
   ui->curvePt3_13->setValue(g_model.curves9[4][2]);connect(ui->curvePt3_13,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));
   ui->curvePt4_13->setValue(g_model.curves9[4][3]);connect(ui->curvePt4_13,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));
   ui->curvePt5_13->setValue(g_model.curves9[4][4]);connect(ui->curvePt5_13,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));
   ui->curvePt6_13->setValue(g_model.curves9[4][5]);connect(ui->curvePt6_13,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));
   ui->curvePt7_13->setValue(g_model.curves9[4][6]);connect(ui->curvePt7_13,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));
   ui->curvePt8_13->setValue(g_model.curves9[4][7]);connect(ui->curvePt8_13,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));
   ui->curvePt9_13->setValue(g_model.curves9[4][8]);connect(ui->curvePt9_13,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));

   ui->curvePt1_14->setValue(g_model.curves9[5][0]);connect(ui->curvePt1_14,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));
   ui->curvePt2_14->setValue(g_model.curves9[5][1]);connect(ui->curvePt2_14,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));
   ui->curvePt3_14->setValue(g_model.curves9[5][2]);connect(ui->curvePt3_14,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));
   ui->curvePt4_14->setValue(g_model.curves9[5][3]);connect(ui->curvePt4_14,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));
   ui->curvePt5_14->setValue(g_model.curves9[5][4]);connect(ui->curvePt5_14,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));
   ui->curvePt6_14->setValue(g_model.curves9[5][5]);connect(ui->curvePt6_14,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));
   ui->curvePt7_14->setValue(g_model.curves9[5][6]);connect(ui->curvePt7_14,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));
   ui->curvePt8_14->setValue(g_model.curves9[5][7]);connect(ui->curvePt8_14,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));
   ui->curvePt9_14->setValue(g_model.curves9[5][8]);connect(ui->curvePt9_14,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));

   ui->curvePt1_15->setValue(g_model.curves9[6][0]);connect(ui->curvePt1_15,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));
   ui->curvePt2_15->setValue(g_model.curves9[6][1]);connect(ui->curvePt2_15,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));
   ui->curvePt3_15->setValue(g_model.curves9[6][2]);connect(ui->curvePt3_15,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));
   ui->curvePt4_15->setValue(g_model.curves9[6][3]);connect(ui->curvePt4_15,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));
   ui->curvePt5_15->setValue(g_model.curves9[6][4]);connect(ui->curvePt5_15,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));
   ui->curvePt6_15->setValue(g_model.curves9[6][5]);connect(ui->curvePt6_15,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));
   ui->curvePt7_15->setValue(g_model.curves9[6][6]);connect(ui->curvePt7_15,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));
   ui->curvePt8_15->setValue(g_model.curves9[6][7]);connect(ui->curvePt8_15,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));
   ui->curvePt9_15->setValue(g_model.curves9[6][8]);connect(ui->curvePt9_15,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));

   ui->curvePt1_16->setValue(g_model.curves9[7][0]);connect(ui->curvePt1_16,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));
   ui->curvePt2_16->setValue(g_model.curves9[7][1]);connect(ui->curvePt2_16,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));
   ui->curvePt3_16->setValue(g_model.curves9[7][2]);connect(ui->curvePt3_16,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));
   ui->curvePt4_16->setValue(g_model.curves9[7][3]);connect(ui->curvePt4_16,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));
   ui->curvePt5_16->setValue(g_model.curves9[7][4]);connect(ui->curvePt5_16,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));
   ui->curvePt6_16->setValue(g_model.curves9[7][5]);connect(ui->curvePt6_16,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));
   ui->curvePt7_16->setValue(g_model.curves9[7][6]);connect(ui->curvePt7_16,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));
   ui->curvePt8_16->setValue(g_model.curves9[7][7]);connect(ui->curvePt8_16,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));
   ui->curvePt9_16->setValue(g_model.curves9[7][8]);connect(ui->curvePt9_16,SIGNAL(editingFinished()),this,SLOT(curvePointEdited()));

}


void ModelEdit::limitEdited()
{
    g_model.limitData[0].offset = ui->offsetDSB_1->value()*10;
    g_model.limitData[1].offset = ui->offsetDSB_2->value()*10;
    g_model.limitData[2].offset = ui->offsetDSB_3->value()*10;
    g_model.limitData[3].offset = ui->offsetDSB_4->value()*10;
    g_model.limitData[4].offset = ui->offsetDSB_5->value()*10;
    g_model.limitData[5].offset = ui->offsetDSB_6->value()*10;
    g_model.limitData[6].offset = ui->offsetDSB_7->value()*10;
    g_model.limitData[7].offset = ui->offsetDSB_8->value()*10;
    g_model.limitData[8].offset = ui->offsetDSB_9->value()*10;
    g_model.limitData[9].offset = ui->offsetDSB_10->value()*10;
    g_model.limitData[10].offset = ui->offsetDSB_11->value()*10;
    g_model.limitData[11].offset = ui->offsetDSB_12->value()*10;
    g_model.limitData[12].offset = ui->offsetDSB_13->value()*10;
    g_model.limitData[13].offset = ui->offsetDSB_14->value()*10;
    g_model.limitData[14].offset = ui->offsetDSB_15->value()*10;
    g_model.limitData[15].offset = ui->offsetDSB_16->value()*10;

    g_model.limitData[0].min = ui->minSB_1->value()+100;
    g_model.limitData[1].min = ui->minSB_2->value()+100;
    g_model.limitData[2].min = ui->minSB_3->value()+100;
    g_model.limitData[3].min = ui->minSB_4->value()+100;
    g_model.limitData[4].min = ui->minSB_5->value()+100;
    g_model.limitData[5].min = ui->minSB_6->value()+100;
    g_model.limitData[6].min = ui->minSB_7->value()+100;
    g_model.limitData[7].min = ui->minSB_8->value()+100;
    g_model.limitData[8].min = ui->minSB_9->value()+100;
    g_model.limitData[9].min = ui->minSB_10->value()+100;
    g_model.limitData[10].min = ui->minSB_11->value()+100;
    g_model.limitData[11].min = ui->minSB_12->value()+100;
    g_model.limitData[12].min = ui->minSB_13->value()+100;
    g_model.limitData[13].min = ui->minSB_14->value()+100;
    g_model.limitData[14].min = ui->minSB_15->value()+100;
    g_model.limitData[15].min = ui->minSB_16->value()+100;

    g_model.limitData[0].max = ui->maxSB_1->value()-100;
    g_model.limitData[1].max = ui->maxSB_2->value()-100;
    g_model.limitData[2].max = ui->maxSB_3->value()-100;
    g_model.limitData[3].max = ui->maxSB_4->value()-100;
    g_model.limitData[4].max = ui->maxSB_5->value()-100;
    g_model.limitData[5].max = ui->maxSB_6->value()-100;
    g_model.limitData[6].max = ui->maxSB_7->value()-100;
    g_model.limitData[7].max = ui->maxSB_8->value()-100;
    g_model.limitData[8].max = ui->maxSB_9->value()-100;
    g_model.limitData[9].max = ui->maxSB_10->value()-100;
    g_model.limitData[10].max = ui->maxSB_11->value()-100;
    g_model.limitData[11].max = ui->maxSB_12->value()-100;
    g_model.limitData[12].max = ui->maxSB_13->value()-100;
    g_model.limitData[13].max = ui->maxSB_14->value()-100;
    g_model.limitData[14].max = ui->maxSB_15->value()-100;
    g_model.limitData[15].max = ui->maxSB_16->value()-100;

    g_model.limitData[0].revert = ui->chInvCB_1->currentIndex();
    g_model.limitData[1].revert = ui->chInvCB_2->currentIndex();
    g_model.limitData[2].revert = ui->chInvCB_3->currentIndex();
    g_model.limitData[3].revert = ui->chInvCB_4->currentIndex();
    g_model.limitData[4].revert = ui->chInvCB_5->currentIndex();
    g_model.limitData[5].revert = ui->chInvCB_6->currentIndex();
    g_model.limitData[6].revert = ui->chInvCB_7->currentIndex();
    g_model.limitData[7].revert = ui->chInvCB_8->currentIndex();
    g_model.limitData[8].revert = ui->chInvCB_9->currentIndex();
    g_model.limitData[9].revert = ui->chInvCB_10->currentIndex();
    g_model.limitData[10].revert = ui->chInvCB_11->currentIndex();
    g_model.limitData[11].revert = ui->chInvCB_12->currentIndex();
    g_model.limitData[12].revert = ui->chInvCB_13->currentIndex();
    g_model.limitData[13].revert = ui->chInvCB_14->currentIndex();
    g_model.limitData[14].revert = ui->chInvCB_15->currentIndex();
    g_model.limitData[15].revert = ui->chInvCB_16->currentIndex();

    updateSettings();
}

void ModelEdit::curvePointEdited()
{            
    g_model.curves5[0][0] = ui->curvePt1_1->value();
    g_model.curves5[0][1] = ui->curvePt2_1->value();
    g_model.curves5[0][2] = ui->curvePt3_1->value();
    g_model.curves5[0][3] = ui->curvePt4_1->value();
    g_model.curves5[0][4] = ui->curvePt5_1->value();

    g_model.curves5[1][0] = ui->curvePt1_2->value();
    g_model.curves5[1][1] = ui->curvePt2_2->value();
    g_model.curves5[1][2] = ui->curvePt3_2->value();
    g_model.curves5[1][3] = ui->curvePt4_2->value();
    g_model.curves5[1][4] = ui->curvePt5_2->value();

    g_model.curves5[2][0] = ui->curvePt1_3->value();
    g_model.curves5[2][1] = ui->curvePt2_3->value();
    g_model.curves5[2][2] = ui->curvePt3_3->value();
    g_model.curves5[2][3] = ui->curvePt4_3->value();
    g_model.curves5[2][4] = ui->curvePt5_3->value();

    g_model.curves5[3][0] = ui->curvePt1_4->value();
    g_model.curves5[3][1] = ui->curvePt2_4->value();
    g_model.curves5[3][2] = ui->curvePt3_4->value();
    g_model.curves5[3][3] = ui->curvePt4_4->value();
    g_model.curves5[3][4] = ui->curvePt5_4->value();

    g_model.curves5[4][0] = ui->curvePt1_5->value();
    g_model.curves5[4][1] = ui->curvePt2_5->value();
    g_model.curves5[4][2] = ui->curvePt3_5->value();
    g_model.curves5[4][3] = ui->curvePt4_5->value();
    g_model.curves5[4][4] = ui->curvePt5_5->value();

    g_model.curves5[5][0] = ui->curvePt1_6->value();
    g_model.curves5[5][1] = ui->curvePt2_6->value();
    g_model.curves5[5][2] = ui->curvePt3_6->value();
    g_model.curves5[5][3] = ui->curvePt4_6->value();
    g_model.curves5[5][4] = ui->curvePt5_6->value();

    g_model.curves5[6][0] = ui->curvePt1_7->value();
    g_model.curves5[6][1] = ui->curvePt2_7->value();
    g_model.curves5[6][2] = ui->curvePt3_7->value();
    g_model.curves5[6][3] = ui->curvePt4_7->value();
    g_model.curves5[6][4] = ui->curvePt5_7->value();

    g_model.curves5[7][0] = ui->curvePt1_8->value();
    g_model.curves5[7][1] = ui->curvePt2_8->value();
    g_model.curves5[7][2] = ui->curvePt3_8->value();
    g_model.curves5[7][3] = ui->curvePt4_8->value();
    g_model.curves5[7][4] = ui->curvePt5_8->value();


    g_model.curves9[0][0] = ui->curvePt1_9->value();
    g_model.curves9[0][1] = ui->curvePt2_9->value();
    g_model.curves9[0][2] = ui->curvePt3_9->value();
    g_model.curves9[0][3] = ui->curvePt4_9->value();
    g_model.curves9[0][4] = ui->curvePt5_9->value();
    g_model.curves9[0][5] = ui->curvePt6_9->value();
    g_model.curves9[0][6] = ui->curvePt7_9->value();
    g_model.curves9[0][7] = ui->curvePt8_9->value();
    g_model.curves9[0][8] = ui->curvePt9_9->value();

    g_model.curves9[1][0] = ui->curvePt1_10->value();
    g_model.curves9[1][1] = ui->curvePt2_10->value();
    g_model.curves9[1][2] = ui->curvePt3_10->value();
    g_model.curves9[1][3] = ui->curvePt4_10->value();
    g_model.curves9[1][4] = ui->curvePt5_10->value();
    g_model.curves9[1][5] = ui->curvePt6_10->value();
    g_model.curves9[1][6] = ui->curvePt7_10->value();
    g_model.curves9[1][7] = ui->curvePt8_10->value();
    g_model.curves9[1][8] = ui->curvePt9_10->value();

    g_model.curves9[2][0] = ui->curvePt1_11->value();
    g_model.curves9[2][1] = ui->curvePt2_11->value();
    g_model.curves9[2][2] = ui->curvePt3_11->value();
    g_model.curves9[2][3] = ui->curvePt4_11->value();
    g_model.curves9[2][4] = ui->curvePt5_11->value();
    g_model.curves9[2][5] = ui->curvePt6_11->value();
    g_model.curves9[2][6] = ui->curvePt7_11->value();
    g_model.curves9[2][7] = ui->curvePt8_11->value();
    g_model.curves9[2][8] = ui->curvePt9_11->value();

    g_model.curves9[3][0] = ui->curvePt1_12->value();
    g_model.curves9[3][1] = ui->curvePt2_12->value();
    g_model.curves9[3][2] = ui->curvePt3_12->value();
    g_model.curves9[3][3] = ui->curvePt4_12->value();
    g_model.curves9[3][4] = ui->curvePt5_12->value();
    g_model.curves9[3][5] = ui->curvePt6_12->value();
    g_model.curves9[3][6] = ui->curvePt7_12->value();
    g_model.curves9[3][7] = ui->curvePt8_12->value();
    g_model.curves9[3][8] = ui->curvePt9_12->value();

    g_model.curves9[4][0] = ui->curvePt1_13->value();
    g_model.curves9[4][1] = ui->curvePt2_13->value();
    g_model.curves9[4][2] = ui->curvePt3_13->value();
    g_model.curves9[4][3] = ui->curvePt4_13->value();
    g_model.curves9[4][4] = ui->curvePt5_13->value();
    g_model.curves9[4][5] = ui->curvePt6_13->value();
    g_model.curves9[4][6] = ui->curvePt7_13->value();
    g_model.curves9[4][7] = ui->curvePt8_13->value();
    g_model.curves9[4][8] = ui->curvePt9_13->value();

    g_model.curves9[5][0] = ui->curvePt1_14->value();
    g_model.curves9[5][1] = ui->curvePt2_14->value();
    g_model.curves9[5][2] = ui->curvePt3_14->value();
    g_model.curves9[5][3] = ui->curvePt4_14->value();
    g_model.curves9[5][4] = ui->curvePt5_14->value();
    g_model.curves9[5][5] = ui->curvePt6_14->value();
    g_model.curves9[5][6] = ui->curvePt7_14->value();
    g_model.curves9[5][7] = ui->curvePt8_14->value();
    g_model.curves9[5][8] = ui->curvePt9_14->value();

    g_model.curves9[6][0] = ui->curvePt1_15->value();
    g_model.curves9[6][1] = ui->curvePt2_15->value();
    g_model.curves9[6][2] = ui->curvePt3_15->value();
    g_model.curves9[6][3] = ui->curvePt4_15->value();
    g_model.curves9[6][4] = ui->curvePt5_15->value();
    g_model.curves9[6][5] = ui->curvePt6_15->value();
    g_model.curves9[6][6] = ui->curvePt7_15->value();
    g_model.curves9[6][7] = ui->curvePt8_15->value();
    g_model.curves9[6][8] = ui->curvePt9_15->value();

    g_model.curves9[7][0] = ui->curvePt1_16->value();
    g_model.curves9[7][1] = ui->curvePt2_16->value();
    g_model.curves9[7][2] = ui->curvePt3_16->value();
    g_model.curves9[7][3] = ui->curvePt4_16->value();
    g_model.curves9[7][4] = ui->curvePt5_16->value();
    g_model.curves9[7][5] = ui->curvePt6_16->value();
    g_model.curves9[7][6] = ui->curvePt7_16->value();
    g_model.curves9[7][7] = ui->curvePt8_16->value();
    g_model.curves9[7][8] = ui->curvePt9_16->value();

    updateSettings();
}

void ModelEdit::tabSwitches()
{
    populateSourceCB(ui->cswitchCB_1,g_eeGeneral.stickMode,g_model.customSw[0].input);
    populateSourceCB(ui->cswitchCB_2,g_eeGeneral.stickMode,g_model.customSw[2].input);
    populateSourceCB(ui->cswitchCB_3,g_eeGeneral.stickMode,g_model.customSw[2].input);
    populateSourceCB(ui->cswitchCB_4,g_eeGeneral.stickMode,g_model.customSw[3].input);
    populateSourceCB(ui->cswitchCB_5,g_eeGeneral.stickMode,g_model.customSw[4].input);
    populateSourceCB(ui->cswitchCB_6,g_eeGeneral.stickMode,g_model.customSw[5].input);

    ui->cswitchOfs_1->setValue(g_model.customSw[0].offset);
    ui->cswitchOfs_2->setValue(g_model.customSw[1].offset);
    ui->cswitchOfs_3->setValue(g_model.customSw[2].offset);
    ui->cswitchOfs_4->setValue(g_model.customSw[3].offset);
    ui->cswitchOfs_5->setValue(g_model.customSw[4].offset);
    ui->cswitchOfs_6->setValue(g_model.customSw[5].offset);

    ui->cswitchFunc_1->setCurrentIndex(g_model.customSw[0].func);
    ui->cswitchFunc_2->setCurrentIndex(g_model.customSw[1].func);
    ui->cswitchFunc_3->setCurrentIndex(g_model.customSw[2].func);
    ui->cswitchFunc_4->setCurrentIndex(g_model.customSw[3].func);
    ui->cswitchFunc_5->setCurrentIndex(g_model.customSw[4].func);
    ui->cswitchFunc_6->setCurrentIndex(g_model.customSw[5].func);

    connect(ui->cswitchCB_1,SIGNAL(currentIndexChanged(int)),this,SLOT(switchesEdited()));
    connect(ui->cswitchCB_2,SIGNAL(currentIndexChanged(int)),this,SLOT(switchesEdited()));
    connect(ui->cswitchCB_3,SIGNAL(currentIndexChanged(int)),this,SLOT(switchesEdited()));
    connect(ui->cswitchCB_4,SIGNAL(currentIndexChanged(int)),this,SLOT(switchesEdited()));
    connect(ui->cswitchCB_5,SIGNAL(currentIndexChanged(int)),this,SLOT(switchesEdited()));
    connect(ui->cswitchCB_6,SIGNAL(currentIndexChanged(int)),this,SLOT(switchesEdited()));

    connect(ui->cswitchOfs_1,SIGNAL(editingFinished()),this,SLOT(switchesEdited()));
    connect(ui->cswitchOfs_2,SIGNAL(editingFinished()),this,SLOT(switchesEdited()));
    connect(ui->cswitchOfs_3,SIGNAL(editingFinished()),this,SLOT(switchesEdited()));
    connect(ui->cswitchOfs_4,SIGNAL(editingFinished()),this,SLOT(switchesEdited()));
    connect(ui->cswitchOfs_5,SIGNAL(editingFinished()),this,SLOT(switchesEdited()));
    connect(ui->cswitchOfs_6,SIGNAL(editingFinished()),this,SLOT(switchesEdited()));

    connect(ui->cswitchFunc_1,SIGNAL(currentIndexChanged(int)),this,SLOT(switchesEdited()));
    connect(ui->cswitchFunc_2,SIGNAL(currentIndexChanged(int)),this,SLOT(switchesEdited()));
    connect(ui->cswitchFunc_3,SIGNAL(currentIndexChanged(int)),this,SLOT(switchesEdited()));
    connect(ui->cswitchFunc_4,SIGNAL(currentIndexChanged(int)),this,SLOT(switchesEdited()));
    connect(ui->cswitchFunc_5,SIGNAL(currentIndexChanged(int)),this,SLOT(switchesEdited()));
    connect(ui->cswitchFunc_6,SIGNAL(currentIndexChanged(int)),this,SLOT(switchesEdited()));
}

void ModelEdit::switchesEdited()
{
    g_model.customSw[0].input = ui->cswitchCB_1->currentIndex();
    g_model.customSw[1].input = ui->cswitchCB_2->currentIndex();
    g_model.customSw[2].input = ui->cswitchCB_3->currentIndex();
    g_model.customSw[3].input = ui->cswitchCB_4->currentIndex();
    g_model.customSw[4].input = ui->cswitchCB_5->currentIndex();
    g_model.customSw[5].input = ui->cswitchCB_6->currentIndex();

    g_model.customSw[0].offset = ui->cswitchOfs_1->value();
    g_model.customSw[1].offset = ui->cswitchOfs_2->value();
    g_model.customSw[2].offset = ui->cswitchOfs_3->value();
    g_model.customSw[3].offset = ui->cswitchOfs_4->value();
    g_model.customSw[4].offset = ui->cswitchOfs_5->value();
    g_model.customSw[5].offset = ui->cswitchOfs_6->value();

    g_model.customSw[0].func = ui->cswitchFunc_1->currentIndex();
    g_model.customSw[1].func = ui->cswitchFunc_2->currentIndex();
    g_model.customSw[2].func = ui->cswitchFunc_3->currentIndex();
    g_model.customSw[3].func = ui->cswitchFunc_4->currentIndex();
    g_model.customSw[4].func = ui->cswitchFunc_5->currentIndex();
    g_model.customSw[5].func = ui->cswitchFunc_6->currentIndex();

    updateSettings();
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



