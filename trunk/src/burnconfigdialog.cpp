#include "burnconfigdialog.h"
#include "ui_burnconfigdialog.h"
#include "avroutputdialog.h"
#include <QtGui>

burnConfigDialog::burnConfigDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::burnConfigDialog)
{
    ui->setupUi(this);
    ui->avrdude_programmer->model()->sort(0);

    getSettings();
    connect(this,SIGNAL(accepted()),this,SLOT(putSettings()));
}

burnConfigDialog::~burnConfigDialog()
{
    delete ui;
}

void burnConfigDialog::getSettings()
{

    QSettings settings("er9x-eePe", "eePe");
    avrTempDir = settings.value("temp_directory", QDir("./").absolutePath()).toString();
    avrLoc = settings.value("avrdude_location", QFileInfo("avrdude.exe").absoluteFilePath()).toString();
    QString str = settings.value("avr_arguments").toString();
    avrArgs = str.split(" ", QString::SkipEmptyParts);
    avrProgrammer =  settings.value("programmer", QString("usbasp")).toString();
    avrPort =  settings.value("avr_port", "").toString();

    ui->avrdude_location->setText(getAVRDUDE());
    ui->temp_location->setText(getTempDir());
    ui->avrArgs->setText(getAVRArgs().join(" "));

    int idx1 = ui->avrdude_programmer->findText(getProgrammer());
    int idx2 = ui->avrdude_port->findText(getPort());
    if(idx1>=0) ui->avrdude_programmer->setCurrentIndex(idx1);
    if(idx2>=0) ui->avrdude_port->setCurrentIndex(idx2);
}

void burnConfigDialog::putSettings()
{
//    avrTempDir = ui->temp_location->text();
//    avrLoc = ui->avrdude_location->text();
//    avrArgs = ui->avrArgs->text().split(" ", QString::SkipEmptyParts);
//    avrProgrammer = ui->avrdude_programmer->currentText();
//    avrPort = ui->avrdude_port->currentText();
//    avrEraseEEPROM = ui->eraseEEPROM_CB->isChecked();


    QSettings settings("er9x-eePe", "eePe");
    settings.setValue("avrdude_location", avrLoc);
    settings.setValue("temp_directory", avrTempDir);
    settings.setValue("programmer", avrProgrammer);
    settings.setValue("avr_port", avrPort);
    settings.setValue("avr_arguments", avrArgs.join(" "));
}

void burnConfigDialog::on_avrdude_programmer_currentIndexChanged(QString )
{
    avrProgrammer = ui->avrdude_programmer->currentText();
}

void burnConfigDialog::on_avrdude_location_editingFinished()
{
    avrLoc = ui->avrdude_location->text();
}

void burnConfigDialog::on_temp_location_editingFinished()
{
    avrTempDir = ui->temp_location->text();
}

void burnConfigDialog::on_avrArgs_editingFinished()
{
    avrArgs = ui->avrArgs->text().split(" ", QString::SkipEmptyParts);
}

void burnConfigDialog::on_avrdude_port_currentIndexChanged(QString )
{
    avrPort = ui->avrdude_port->currentText();
}

void burnConfigDialog::on_pushButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Select Location"),ui->avrdude_location->text());

    if(!fileName.isEmpty())
    {
        ui->avrdude_location->setText(fileName);
        avrLoc = fileName;
    }
}

void burnConfigDialog::on_pushButton_2_clicked()
{
    QString dirName = QFileDialog::getExistingDirectory(this, tr("Select Location"),ui->temp_location->text());

    if(!dirName.isEmpty())
    {
        ui->temp_location->setText(dirName);
        avrTempDir = dirName;
    }
}


void burnConfigDialog::listProgrammers()
{
    QStringList arguments;
    arguments << "-c?";

    avrOutputDialog *ad = new avrOutputDialog(this, ui->avrdude_location->text(), arguments, "List available programmers", AVR_DIALOG_KEEP_OPEN);
    ad->setWindowIcon(QIcon(":/images/list.png"));
    ad->show();
}

void burnConfigDialog::on_pushButton_3_clicked()
{
    listProgrammers();
}



void burnConfigDialog::on_pushButton_4_clicked()
{
    QStringList arguments;
    arguments << "-?";

    avrOutputDialog *ad = new avrOutputDialog(this, ui->avrdude_location->text(), arguments, "Show help", AVR_DIALOG_KEEP_OPEN);
    ad->setWindowIcon(QIcon(":/images/configure.png"));
    ad->show();
}


void burnConfigDialog::readFuses()
{
    QStringList args   = avrArgs;
    if(!avrPort.isEmpty()) args << "-P" << avrPort;

    QStringList str;
    str << "-U" << "lfuse:r:-:i" << "-U" << "hfuse:r:-:i" << "-U" << "efuse:r:-:i";

    QStringList arguments;
    arguments << "-c" << avrProgrammer << "-p" << "m64" << args << str;

    avrOutputDialog *ad = new avrOutputDialog(this, avrLoc, arguments, "Read Fuses",AVR_DIALOG_KEEP_OPEN);
    ad->setWindowIcon(QIcon(":/images/fuses.png"));
    ad->show();
}

void burnConfigDialog::restFuses(bool eeProtect)
{
    //fuses
    //avrdude -c usbasp -p m64 -U lfuse:w:<0x0E>:m
    //avrdude -c usbasp -p m64 -U hfuse:w:<0x89>:m  0x81 for eeprom protection
    //avrdude -c usbasp -p m64 -U efuse:w:<0xFF>:m

    QMessageBox::StandardButton ret = QMessageBox::No;

    ret = QMessageBox::warning(this, tr("eePe"),
                               tr("<b><u>WARNING!</u></b><br>This will reset the fuses to the factory settings.<br>Writing fuses can mess up your radio.<br>Do this only if you are sure they are wrong!<br>Are you sure you want to continue?"),
                               QMessageBox::Yes | QMessageBox::No);
    if (ret == QMessageBox::Yes)
    {
        QStringList args   = avrArgs;
        if(!avrPort.isEmpty()) args << "-P" << avrPort;

        QString erStr = eeProtect ? "hfuse:w:0x81:m" : "hfuse:w:0x89:m";
        QStringList str;
        str << "-U" << "lfuse:w:0x0E:m" << "-U" << erStr << "-U" << "efuse:w:0xFF:m";
        //use hfuse = 0x81 to prevent eeprom being erased with every flashing

        QStringList arguments;
        arguments << "-c" << avrProgrammer << "-p" << "m64" << args << "-u" << str;

        avrOutputDialog *ad = new avrOutputDialog(this, avrLoc, arguments, "Reset Fuses",AVR_DIALOG_KEEP_OPEN);
        ad->setWindowIcon(QIcon(":/images/fuses.png"));
        ad->show();
    }

}
