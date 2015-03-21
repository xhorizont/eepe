#include "preferencesdialog.h"
#include "ui_preferencesdialog.h"
#include "stamp-eepe.h"
#include "mainwindow.h"
#include <QtGui>

#ifndef SKY
extern void populateDownloads( QComboBox *b ) ;
#endif

preferencesDialog::preferencesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::preferencesDialog)
{
    ui->setupUi(this);

    populateLocale();
    initSettings();

    connect(this,SIGNAL(accepted()),this,SLOT(write_values()));
}

preferencesDialog::~preferencesDialog()
{
    delete ui;
}

void preferencesDialog::write_values()
{
#ifdef SKY
    QSettings settings("er9x-eePskye", "eePskye");
    settings.setValue("startup_check_ersky9x", ui->startupCheck_er9x->isChecked());
    settings.setValue("startup_check_eepskye", ui->startupCheck_eepe->isChecked());
#else
    QSettings settings("er9x-eePe", "eePe");
    settings.setValue("startup_check_er9x", ui->startupCheck_er9x->isChecked());
    settings.setValue("startup_check_eepe", ui->startupCheck_eepe->isChecked());
    settings.setValue("processor", ui->ProcessorCB->currentIndex());
#endif
		settings.setValue("locale", ui->locale_QB->itemData(ui->locale_QB->currentIndex()));
    settings.setValue("default_channel_order", ui->channelorderCB->currentIndex());
    settings.setValue("default_mode", ui->stickmodeCB->currentIndex());
    settings.setValue("show_splash", ui->showSplash->isChecked());
    settings.setValue("download-version", ui->downloadVerCB->currentIndex());
    settings.setValue("default_EE_version", ui->DefaultVersionCB->currentIndex());
}


void preferencesDialog::initSettings()
{
#ifdef SKY
    int dnloadVersion ;
		QSettings settings("er9x-eePskye", "eePskye");
#else
    QSettings settings("er9x-eePe", "eePe");
		populateDownloads( ui->downloadVerCB ) ;
#endif    

    int i=ui->locale_QB->findData(settings.value("locale",QLocale::system().name()).toString());
    if(i<0) i=0;
    ui->locale_QB->setCurrentIndex(i);

    ui->channelorderCB->setCurrentIndex(settings.value("default_channel_order", 0).toInt());
    ui->stickmodeCB->setCurrentIndex(settings.value("default_mode", 1).toInt());
    ui->downloadVerCB->setCurrentIndex(settings.value("download-version", 0).toInt());
    ui->ProcessorCB->setCurrentIndex(settings.value("processor", 0).toInt());

    ui->startupCheck_er9x->setChecked(settings.value("startup_check_er9x", true).toBool());
    ui->startupCheck_eepe->setChecked(settings.value("startup_check_eepe", true).toBool());

    ui->showSplash->setChecked(settings.value("show_splash", true).toBool());

    currentER9Xrev = settings.value("currentER9Xrev", 1).toInt();
    ui->DefaultVersionCB->setCurrentIndex(settings.value("default_EE_version", 0).toInt());

    ui->er9x_ver_label->setText(QString("r%1").arg(currentER9Xrev));
}

void preferencesDialog::populateLocale()
{
    ui->locale_QB->clear();
    ui->locale_QB->addItem("English (default)", "");


    QStringList strl = QApplication::arguments();
    if(!strl.count()) return;

    QString path = ".";
#ifdef Q_OS_WIN32
    if(strl.count()) path = QFileInfo(strl[0]).canonicalPath() + "/lang";
#else
    if(strl.count()) path = QFileInfo(strl[0]).canonicalPath() + "/eepefiles";
#endif


    QDir directory = QDir(path);
    QStringList files = directory.entryList(QStringList("eepe_*.qm"), QDir::Files | QDir::NoSymLinks);

    foreach(QString file, files)
    {
        QLocale loc(file.mid(5,2));
        ui->locale_QB->addItem(QLocale::languageToString(loc.language()), loc.name());
    }


    //ui->locale_QB->addItems(files);


}


#ifdef SKY
void preferencesDialog::on_downloadVerCB_currentIndexChanged(int index)
{
	QSettings settings("er9x-eePskye", "eePskye");
	if ( index == 0 )
	{
    currentER9Xrev = settings.value("currentERSKY9Xrev", 1).toInt();
		ui->label_CurrentVersion->setText( "Current Version - ersky9x" ) ;
	}
	else
	{
    currentER9Xrev = settings.value("currentERSKY9XRrev", 1).toInt();
		ui->label_CurrentVersion->setText( "Current Version - ersky9xr" ) ;
	}
  ui->er9x_ver_label->setText(QString("r%1").arg(currentER9Xrev));
  settings.setValue("download-version", ui->downloadVerCB->currentIndex());
}
#endif

void preferencesDialog::on_er9x_dnld_2_clicked()
{
    MainWindow * mw = (MainWindow *)this->parent();

		write_values() ;		// In case changed
    mw->checkForUpdates(true);
}

void preferencesDialog::on_er9x_dnld_clicked()
{
    MainWindow * mw = (MainWindow *)this->parent();

		write_values() ;		// In case changed
    mw->downloadLatester9x();
}

