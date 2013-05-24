#ifndef SERIALDIALOG_H
#define SERIALDIALOG_H
#include "qextserialport.h"
#include <QDialog>

namespace Ui {
    class serialDialog;
}

class serialDialog : public QDialog
{
    Q_OBJECT

public:
    explicit serialDialog(QWidget *parent = 0);
    ~serialDialog();

private:
    Ui::serialDialog *ui;
		int waitForAckNak( int mS ) ;
		int sendOneFile( QString fname ) ;

//    QString fileToSend ;
    
		QextSerialPort *port ;

private slots:
    void on_cancelButton_clicked();
		void on_FileEdit_editingFinished() ;
		void on_browseButton_clicked() ;
		void on_sendButton_clicked() ;
};

#endif // SERIAL1DIALOG_H
