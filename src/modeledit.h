#ifndef MODELEDIT_H
#define MODELEDIT_H

#include <QDialog>
#include "pers.h"

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

    EEGeneral g_eeGeneral;
    ModelData g_model;
    int       id_model;

    void updateSettings();
    void tabModelEditSetup();
    void tabLimits();
    void tabTrims();

private slots:
    void on_chInvCB_1_currentIndexChanged(int index);
    void on_chInvCB_2_currentIndexChanged(int index);
    void on_chInvCB_3_currentIndexChanged(int index);
    void on_chInvCB_4_currentIndexChanged(int index);
    void on_chInvCB_5_currentIndexChanged(int index);
    void on_chInvCB_6_currentIndexChanged(int index);
    void on_chInvCB_7_currentIndexChanged(int index);
    void on_chInvCB_8_currentIndexChanged(int index);
    void on_chInvCB_9_currentIndexChanged(int index);
    void on_chInvCB_10_currentIndexChanged(int index);
    void on_chInvCB_11_currentIndexChanged(int index);
    void on_chInvCB_12_currentIndexChanged(int index);
    void on_chInvCB_13_currentIndexChanged(int index);
    void on_chInvCB_14_currentIndexChanged(int index);
    void on_chInvCB_15_currentIndexChanged(int index);
    void on_chInvCB_16_currentIndexChanged(int index);

    void on_maxSB_1_editingFinished();
    void on_maxSB_2_editingFinished();
    void on_maxSB_3_editingFinished();
    void on_maxSB_4_editingFinished();
    void on_maxSB_5_editingFinished();
    void on_maxSB_6_editingFinished();
    void on_maxSB_7_editingFinished();
    void on_maxSB_8_editingFinished();
    void on_maxSB_9_editingFinished();
    void on_maxSB_10_editingFinished();
    void on_maxSB_11_editingFinished();
    void on_maxSB_12_editingFinished();
    void on_maxSB_13_editingFinished();
    void on_maxSB_14_editingFinished();
    void on_maxSB_15_editingFinished();
    void on_maxSB_16_editingFinished();

    void on_minSB_1_editingFinished();
    void on_minSB_2_editingFinished();
    void on_minSB_3_editingFinished();
    void on_minSB_4_editingFinished();
    void on_minSB_5_editingFinished();
    void on_minSB_6_editingFinished();
    void on_minSB_7_editingFinished();
    void on_minSB_8_editingFinished();
    void on_minSB_9_editingFinished();
    void on_minSB_10_editingFinished();
    void on_minSB_11_editingFinished();
    void on_minSB_12_editingFinished();
    void on_minSB_13_editingFinished();
    void on_minSB_14_editingFinished();
    void on_minSB_15_editingFinished();
    void on_minSB_16_editingFinished();

    void on_offsetDSB_1_editingFinished();
    void on_offsetDSB_2_editingFinished();
    void on_offsetDSB_3_editingFinished();
    void on_offsetDSB_4_editingFinished();
    void on_offsetDSB_5_editingFinished();
    void on_offsetDSB_6_editingFinished();
    void on_offsetDSB_7_editingFinished();
    void on_offsetDSB_8_editingFinished();
    void on_offsetDSB_9_editingFinished();
    void on_offsetDSB_10_editingFinished();
    void on_offsetDSB_11_editingFinished();
    void on_offsetDSB_12_editingFinished();
    void on_offsetDSB_13_editingFinished();
    void on_offsetDSB_14_editingFinished();
    void on_offsetDSB_15_editingFinished();
    void on_offsetDSB_16_editingFinished();

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
