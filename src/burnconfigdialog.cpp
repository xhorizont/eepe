#include "burnconfigdialog.h"
#include "ui_burnconfigdialog.h"
#include "avroutputdialog.h"
#include <QtGui>

burnConfigDialog::burnConfigDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::burnConfigDialog)
{
    ui->setupUi(this);
    getSettings();
}

burnConfigDialog::~burnConfigDialog()
{
    delete ui;
}

QString burnConfigDialog::getTempDir()
{
    QSettings settings("er9x-eePe", "eePe");
    return settings.value("temp_directory", QDir("avrdude.exe").absolutePath()).toString();
}

QString burnConfigDialog::getAVRDUDE()
{
    QSettings settings("er9x-eePe", "eePe");
    return settings.value("avrdude_location", QFileInfo("avrdude.exe").absoluteFilePath()).toString();
}

QString burnConfigDialog::getProgrammer()
{
    QSettings settings("er9x-eePe", "eePe");
    return settings.value("programmer", QString("usbasp")).toString();
}

void burnConfigDialog::getSettings()
{
    ui->avrdude_location->setText(getAVRDUDE());
    ui->temp_location->setText(getTempDir());
    int idx = ui->avrdude_programmer->findText(getProgrammer());
    if(idx>=0) ui->avrdude_programmer->setCurrentIndex(idx);
}

void burnConfigDialog::putSettings()
{
    QSettings settings("er9x-eePe", "eePe");
    settings.setValue("avrdude_location", ui->avrdude_location->text());
    settings.setValue("temp_directory", ui->temp_location->text());
    settings.setValue("programmer", ui->avrdude_programmer->currentText());
}


void burnConfigDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void burnConfigDialog::on_avrdude_programmer_currentIndexChanged(QString )
{
    putSettings();
}

void burnConfigDialog::on_avrdude_location_editingFinished()
{
    putSettings();
}

void burnConfigDialog::on_temp_location_editingFinished()
{
    putSettings();
}

void burnConfigDialog::on_pushButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Select Location"),ui->avrdude_location->text());

    if(!fileName.isEmpty())
    {
        ui->avrdude_location->setText(fileName);
        putSettings();
    }
}

void burnConfigDialog::on_pushButton_2_clicked()
{
    QString dirName = QFileDialog::getExistingDirectory(this, tr("Select Location"),ui->temp_location->text());

    if(!dirName.isEmpty())
    {
        ui->temp_location->setText(dirName);
        putSettings();
    }
}

void burnConfigDialog::on_pushButton_3_clicked()
{
    QStringList arguments;
    arguments << "-c" << "?";

    avrOutputDialog ad(this, ui->avrdude_location->text(), arguments);
    ad.exec();
}


