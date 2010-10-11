#include "burnconfigdialog.h"
#include "ui_burnconfigdialog.h"
#include <QtGui>

burnConfigDialog::burnConfigDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::burnConfigDialog)
{
    ui->setupUi(this);

    QSettings settings("er9x-eePe", "eePe");
    QString avrdudeLoc = settings.value("avrdude_location", QString("avrdude")).toString();
    QString programmer = settings.value("programmer", QString("usbasp")).toString();

    ui->avrdude_location->setText(avrdudeLoc);
    int idx = ui->avrdude_programmer->findText(programmer);
    if(idx>=0) ui->avrdude_programmer->setCurrentIndex(idx);

    //avrdude -c usbasp -p m64 -U flash:r:"backupflash.bin:r
    ui->avrdude_line->setText(getAVRLine("efile.hex"));

}

burnConfigDialog::~burnConfigDialog()
{
    delete ui;
}

QString burnConfigDialog::getAVRLine(const QString &fileName, int memType, int opr)
{
    QSettings settings("er9x-eePe", "eePe");
    QString avrdudeLoc = settings.value("avrdude_location", QString("avrdude")).toString();
    QString programmer = settings.value("programmer", QString("usbasp")).toString();

    QString str = avrdudeLoc + " -c " + programmer + " -p m64 -U "; //MEM:OPR:FILE:FTYPE"
    if(memType==MEM_TYPE_EEPROM) str += "eeprom:";
    if(memType==MEM_TYPE_FLASH)  str += "flash:";

    if(opr==OPR_TYPE_READ) str += "r:";
    if(opr==OPR_TYPE_READ) str += "w:";

    str += "\"" + fileName + "\":";
    if(QFileInfo(fileName).suffix().toUpper()=="HEX") str += "i";
    if(QFileInfo(fileName).suffix().toUpper()=="BIN") str += "r";

    return str;
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

void burnConfigDialog::on_avrdude_programmer_currentIndexChanged(QString text)
{
    QSettings settings("er9x-eePe", "eePe");
    settings.setValue("programmer", text);
    ui->avrdude_line->setText(getAVRLine("efile.hex"));
}

void burnConfigDialog::on_avrdude_location_editingFinished()
{
    QSettings settings("er9x-eePe", "eePe");
    settings.setValue("avrdude_location", ui->avrdude_location->text());
    ui->avrdude_line->setText(getAVRLine("efile.hex"));
}

void burnConfigDialog::on_pushButton_clicked()
{
    QSettings settings("er9x-eePe", "eePe");
    QString avrdudeLoc = settings.value("avrdude_location", QString("avrdude")).toString();
    QString fileName = QFileDialog::getOpenFileName(this, tr("Select Location"),avrdudeLoc);

    if(!fileName.isEmpty())
    {
        ui->avrdude_location->setText(fileName);
        settings.setValue("avrdude_location", fileName);
        ui->avrdude_line->setText(getAVRLine("efile.hex"));

    }
}
