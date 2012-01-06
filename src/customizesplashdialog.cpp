#include "customizesplashdialog.h"
#include "ui_customizesplashdialog.h"

#include <QtGui>
#include "helpers.h"


#define SPLASH_MARKER "Splash"
#define SPLASH_SIZE (128*64)
#define SPLASH_OFFSET (6+1+3) // "Splash" + zero + 3 header bytes
#define HEX_FILE_SIZE (1024*256)



customizeSplashDialog::customizeSplashDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::customizeSplashDialog)
{
    ui->setupUi(this);
}

customizeSplashDialog::~customizeSplashDialog()
{
    delete ui;
}

void customizeSplashDialog::on_pushButton_clicked()
{
    QString fileName;
    QSettings settings("er9x-eePe", "eePe");
    quint8 temp[HEX_FILE_SIZE] = {0};

    fileName = QFileDialog::getOpenFileName(this,tr("Open"),settings.value("lastDir").toString(),tr("HEX files (*.hex);;"));

    if(!loadiHEX(this, fileName, (quint8*)&temp, HEX_FILE_SIZE, ""))
    {
        QMessageBox::critical(this, tr("Error"),
                              tr("Error reading file"));
        return;
    }

    QByteArray rawData = QByteArray::fromRawData((const char *)&temp, HEX_FILE_SIZE);
    int pos = rawData.indexOf(QString(SPLASH_MARKER));

    QByteArray data = rawData.mid(pos + SPLASH_OFFSET,SPLASH_SIZE);


}
