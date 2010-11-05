#ifndef MODELEDIT_H
#define MODELEDIT_H

#include <QDialog>
#include <QtGui>
#include "pers.h"
#include "mixerslist.h"

namespace Ui {
    class ModelEdit;
}

class ModelEdit : public QDialog
{
    Q_OBJECT

public:
    explicit ModelEdit(EEPFILE *eFile, uint8_t id, QWidget *parent = 0);
    ~ModelEdit();


private:
    Ui::ModelEdit *ui;
    EEPFILE *eeFile;

    MixersList *MixerlistWidget;

    EEGeneral g_eeGeneral;
    ModelData g_model;
    int       id_model;

    bool switchEditLock;

    QSpinBox  * cswitchOffset[NUM_CSW];
    QComboBox * cswitchSource1[NUM_CSW];
    QComboBox * cswitchSource2[NUM_CSW];

    void updateSettings();
    void tabModelEditSetup();
    void tabExpo();
    void tabMixes();
    void tabLimits();
    void tabCurves();
    void tabSwitches();
    void tabTrims();
    void updateTabCurves();
    void setSwitchWidgetVisibility(int i);

    void launchSimulation();
    void resizeEvent(QResizeEvent *event  = 0);

    void drawCurve();
    int currentCurve;

    QSpinBox *getNodeSB(int i);

    MixData* gm_addMix(uint8_t dch);
    void gm_deleteMix(int index);
    void gm_openMix(int index);
    void mixersDeleteList(QList<int> list);
    QList<int> createListFromSelected();

signals:
    void modelValuesChanged();


private slots:
    void on_resetCurve_1_clicked();
    void on_resetCurve_2_clicked();
    void on_resetCurve_3_clicked();
    void on_resetCurve_4_clicked();
    void on_resetCurve_5_clicked();
    void on_resetCurve_6_clicked();
    void on_resetCurve_7_clicked();
    void on_resetCurve_8_clicked();
    void on_resetCurve_9_clicked();
    void on_resetCurve_10_clicked();
    void on_resetCurve_11_clicked();
    void on_resetCurve_12_clicked();
    void on_resetCurve_13_clicked();
    void on_resetCurve_14_clicked();
    void on_resetCurve_15_clicked();
    void on_resetCurve_16_clicked();


    void on_pushButton_clicked();
    void mixersDelete(bool ask=true);
    void mixersCut();
    void mixersCopy();
    void mixersPaste();
    void mixersDuplicate();
    void mixerOpen();
    void mixerAdd();


    void MixerlistWidget_customContextMenuRequested(QPoint pos);
    void MixerlistWidget_doubleClicked(QModelIndex index);
    void MixerlistWidget_mimeDropped( int index, const QMimeData * data, Qt::DropAction action );

    void on_curveEdit_1_clicked();
    void on_curveEdit_2_clicked();
    void on_curveEdit_3_clicked();
    void on_curveEdit_4_clicked();
    void on_curveEdit_5_clicked();
    void on_curveEdit_6_clicked();
    void on_curveEdit_7_clicked();
    void on_curveEdit_8_clicked();
    void on_curveEdit_9_clicked();
    void on_curveEdit_10_clicked();
    void on_curveEdit_11_clicked();
    void on_curveEdit_12_clicked();
    void on_curveEdit_13_clicked();
    void on_curveEdit_14_clicked();
    void on_curveEdit_15_clicked();
    void on_curveEdit_16_clicked();

    void curvePointEdited();
    void limitEdited();
    void switchesEdited();
    void expoEdited();
    void mixesEdited();

    void on_spinBox_S1_valueChanged(int value);
    void on_spinBox_S2_valueChanged(int value);
    void on_spinBox_S3_valueChanged(int value);
    void on_spinBox_S4_valueChanged(int value);

    void on_bcRUDChkB_toggled(bool checked);
    void on_bcELEChkB_toggled(bool checked);
    void on_bcTHRChkB_toggled(bool checked);
    void on_bcAILChkB_toggled(bool checked);
    void on_bcP1ChkB_toggled(bool checked);
    void on_bcP2ChkB_toggled(bool checked);
    void on_bcP3ChkB_toggled(bool checked);

    void on_thrExpoChkB_toggled(bool checked);
    void on_thrTrimChkB_toggled(bool checked);
    void on_ppmDelaySB_editingFinished();
    void on_numChannelsSB_editingFinished();
    void on_timerValTE_editingFinished();
    void on_protocolCB_currentIndexChanged(int index);
    void on_pulsePolCB_currentIndexChanged(int index);
    void on_trimSWCB_currentIndexChanged(int index);
    void on_trimIncCB_currentIndexChanged(int index);
    void on_timerDirCB_currentIndexChanged(int index);
    void on_timerModeCB_currentIndexChanged(int index);
    void on_modelNameLE_editingFinished();
    void on_modelNameLE_textEdited(QString txt);
    void on_tabWidget_currentChanged(int index);
};

#endif // MODELEDIT_H
