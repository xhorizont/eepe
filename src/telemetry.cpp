#include "telemetry.h"
#include "ui_telemetryDialog.h"
#include "stamp-eepe.h"
#include "mainwindow.h"
#include <QtGui>
#include "qextserialenumerator.h"
#include <QtCore/QList>
#include <QString>
#include <inttypes.h>

telemetryDialog::telemetryDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::telemetryDialog)
{
  QString temp ;
  ui->setupUi(this);

	QList<QextPortInfo> ports = QextSerialEnumerator::getPorts() ;
	ui->TelPortCB->clear() ;
  foreach (QextPortInfo info, ports)
	{
  	ui->TelPortCB->addItem(info.portName) ;
	}
	port = NULL ;
	timer = NULL ;
	sending = 0 ;
	ui->startButton->setText("Start") ;
  ui->WSvalue->setText(tr("%1").arg(ui->WSdial->value())) ;

}

telemetryDialog::~telemetryDialog()
{
	if ( timer )
	{
    timer->stop() ;
    delete timer ;
		timer = NULL ;
	}
	if ( port )
	{
		if (port->isOpen())
		{
    	port->close();
		}
    delete port ;
		port = NULL ;
	}
  delete ui;
}

void telemetryDialog::on_exitButton_clicked()
{
	done(0) ;	
}

void telemetryDialog::on_startButton_clicked()
{
	QString portname ;
	
	if ( sending )
	{
		sending = 0 ;
		ui->startButton->setText("Start") ;
		if ( timer )
		{
  	  timer->stop() ;
  	  delete timer ;
			timer = NULL ;
		}
		if ( port )
		{
			if (port->isOpen())
			{
  	  	port->close();
			}
  	  delete port ;
			port = NULL ;
		}
	}
	else
	{
	  portname = ui->TelPortCB->currentText() ;
#ifdef Q_OS_UNIX
  	port = new QextSerialPort(portname, QextSerialPort::Polling) ;
#else
	  port = new QextSerialPort(portname, QextSerialPort::Polling) ;
#endif /*Q_OS_UNIX*/
	  port->setBaudRate(BAUD9600) ;
  	port->setFlowControl(FLOW_OFF) ;
	  port->setParity(PAR_NONE) ;
  	port->setDataBits(DATA_8) ;
	  port->setStopBits(STOP_1) ;
    //set timeouts to 500 ms
  	port->setTimeout(1000) ;
  	if (!port->open(QIODevice::ReadWrite | QIODevice::Unbuffered) )
  	{
  	  QMessageBox::critical(this, "eePe", tr("Com Port Unavailable"));
			if (port->isOpen())
			{
  	  	port->close();
			}
  	  delete port ;
			port = NULL ;
			return ;	// Failed
		}
		setupTimer() ;		
		sending = 1 ;
		ui->startButton->setText("Stop") ;
		
	}
}

void telemetryDialog::on_WSdial_valueChanged( int value )
{
	ui->WSvalue->setText(tr("%1").arg(value)) ;
}

void telemetryDialog::setupTimer()
{
  timer = new QTimer(this);
  connect(timer,SIGNAL(timeout()),this,SLOT(timerEvent())) ;
  timer->start(500) ;
}

void telemetryDialog::timerEvent()
{
  unsigned char data[2] ;
	data[0] = ui->WSdial->value() ;
	data[1] = ui->WSdial->value() >> 8 ;
	if ( port )
	{
  	port->write( QByteArray::fromRawData ( (char *)data, 2 ), 2 ) ;
	}
}


