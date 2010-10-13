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
    return settings.value("temp_directory", QDir("./").absolutePath()).toString();
}

QString burnConfigDialog::getAVRDUDE()
{
    QSettings settings("er9x-eePe", "eePe");
    return settings.value("avrdude_location", QFileInfo("avrdude.exe").absoluteFilePath()).toString();
}

QStringList burnConfigDialog::getAVRArgs()
{
    QSettings settings("er9x-eePe", "eePe");
    QString str = settings.value("avr_arguments").toString();
    return str.split(" ", QString::SkipEmptyParts);
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
    ui->avrArgs->setText(getAVRArgs().join(" "));
    int idx = ui->avrdude_programmer->findText(getProgrammer());
    if(idx>=0) ui->avrdude_programmer->setCurrentIndex(idx);
}

void burnConfigDialog::putSettings()
{
    QSettings settings("er9x-eePe", "eePe");
    settings.setValue("avrdude_location", ui->avrdude_location->text());
    settings.setValue("temp_directory", ui->temp_location->text());
    settings.setValue("programmer", ui->avrdude_programmer->currentText());
    settings.setValue("avr_arguments", ui->avrArgs->text());
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

void burnConfigDialog::on_avrArgs_editingFinished()
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


void burnConfigDialog::listProgrammers()
{
    QStringList arguments;
    arguments << "-c?";

    avrOutputDialog ad(this, ui->avrdude_location->text(), arguments, "List available programmers", AVR_DIALOG_KEEP_OPEN);
    ad.exec();
}

void burnConfigDialog::on_pushButton_3_clicked()
{
    listProgrammers();
}



void burnConfigDialog::on_pushButton_4_clicked()
{
    QStringList arguments;
    arguments << "-?";

    avrOutputDialog ad(this, ui->avrdude_location->text(), arguments, "Show help", AVR_DIALOG_KEEP_OPEN);
    ad.exec();
}




void burnConfigDialog::on_resetFuses_clicked()
{
    //fuses
    //avrdude -c usbasp -p m64 -U lfuse:w:<0x0E>:m
    //avrdude -c usbasp -p m64 -U hfuse:w:<0x89>:m  0x81 for eeprom protection
    //avrdude -c usbasp -p m64 -U efuse:w:<0xFF>:m

    QMessageBox::StandardButton ret = QMessageBox::No;

    ret = QMessageBox::warning(this, tr("eePe"),
                               tr("<b><u>WARNING!</u></b><br>Writing fuses can mess up your radio.<br>Do this only if you are sure they are wrong!<br>Are you sure you want to continue?"),
                               QMessageBox::Yes | QMessageBox::No);
    if (ret == QMessageBox::Yes)
    {
        QString avrdudeLoc = ui->avrdude_location->text();
        QString programmer = ui->avrdude_programmer->currentText();
        QStringList args   = ui->avrArgs->text().split(" ", QString::SkipEmptyParts);

        QStringList str;
        str << "-U" << "lfuse:w:0x0E:m" << "-U" << "hfuse:w:0x89:m" << "-U" << "efuse:w:0xFF:m";
        //use hfuse = 0x81 to prevent eeprom being erased with every flashing

        QStringList arguments;
        arguments << "-c" << programmer << "-p" << "m64" << args << "-u" << str;

        avrOutputDialog ad(this, avrdudeLoc, arguments, "Reset Fuses",AVR_DIALOG_KEEP_OPEN);
        ad.exec();
    }

}

void burnConfigDialog::on_readFuses_clicked()
{
    QString avrdudeLoc = ui->avrdude_location->text();
    QString programmer = ui->avrdude_programmer->currentText();
    QStringList args   = ui->avrArgs->text().split(" ", QString::SkipEmptyParts);

    QStringList str;
    str << "-U" << "lfuse:r:-:i" << "-U" << "hfuse:r:-:i" << "-U" << "efuse:r:-:i";

    QStringList arguments;
    arguments << "-c" << programmer << "-p" << "m64" << args << str;

    avrOutputDialog ad(this, avrdudeLoc, arguments, "Read Fuses",AVR_DIALOG_KEEP_OPEN);
    ad.exec();
}
