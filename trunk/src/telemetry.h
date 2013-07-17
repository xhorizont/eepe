#ifndef TELEMETRY_H
#define TELEMETRY_H
#include "qextserialport.h"
#include <QDialog>

namespace Ui {
    class telemetryDialog ;
}

class telemetryDialog : public QDialog
{
    Q_OBJECT

public:
    explicit telemetryDialog(QWidget *parent = 0);
    ~telemetryDialog();

private:
    Ui::telemetryDialog *ui;
    QTimer *timer ;
//		int waitForAckNak( int mS ) ;
//    int waitForCan( int mS ) ;
//		int sendOneFile( QString fname ) ;

//    QString fileToSend ;
    
		QextSerialPort *port ;
		int sending ;

private slots:
    void on_exitButton_clicked();
    void on_startButton_clicked();
		void on_WSdial_valueChanged( int value ) ;
    void timerEvent() ;
		void setupTimer() ;
//		void on_FileEdit_editingFinished() ;
//		void on_browseButton_clicked() ;
//		void on_sendButton_clicked() ;
};

#endif // TELEMETRY_H
