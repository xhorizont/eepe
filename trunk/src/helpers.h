#ifndef HELPERS_H
#define HELPERS_H

#include <QtGui>
#include <QtXml>
#include "pers.h"

#define TMR_NUM_OPTION  (TMR_VAROFS+2*MAX_DRSWITCH-3)
#define SPLASH_MARKER "Splash\0"
#define SPLASH_WIDTH (128)
#define SPLASH_HEIGHT (64)
#define SPLASH_SIZE (SPLASH_WIDTH*SPLASH_HEIGHT/8)
#define SPLASH_OFFSET (6+1+3) // "Splash" + zero + 3 header bytes
#define HEX_FILE_SIZE (1024*64)


void populateGvarCB(QComboBox *b, int value=0) ;
void populateSwitchCB(QComboBox *b, int value);
void populateSafetySwitchCB(QComboBox *b, int type, int value) ;
void populateSafetyVoiceTypeCB(QComboBox *b, int type, int value);
void populateTelItemsCB(QComboBox *b, int value) ;
void populateAlarmCB(QComboBox *b, int value);
void populateCurvesCB(QComboBox *b, int value);
void populateTimerSwitchCB(QComboBox *b, int value);
void populateSwitchAndCB(QComboBox *b, int value) ;
void populateTmrBSwitchCB(QComboBox *b, int value) ;
void populateSourceCB(QComboBox *b, int stickMode=1, int telem = 0, int value=0);
void populateCSWCB(QComboBox *b, int value);
int16_t convertTelemConstant( int8_t index, int8_t value) ;
QString getSourceStr(int stickMode, int idx);
QString getTimerMode(int tm);
QString getSWName(int val);
QString getCSWFunc(int val);

void stringTelemetryChannel( char *string, int8_t index, int16_t val, ModelData *model ) ;

int  loadiHEX(QWidget *parent, QString fileName, quint8 * data, int datalen, QString header);
bool saveiHEX(QWidget *parent, QString fileName, quint8 * data, int datalen, QString header, int notesIndex=0);

void appendTextElement(QDomDocument * qdoc, QDomElement * pe, QString name, QString value);
void appendNumberElement(QDomDocument * qdoc, QDomElement * pe,QString name, int value, bool forceZeroWrite = false);
void appendCDATAElement(QDomDocument * qdoc, QDomElement * pe,QString name, const char * data, int size);

QDomElement getGeneralDataXML(QDomDocument * qdoc, EEGeneral * tgen);   //parse out data to XML format
QDomElement getModelDataXML(QDomDocument * qdoc, ModelData * tmod, int modelNum, int mdver); //parse out data to XML format

bool loadGeneralDataXML(QDomDocument * qdoc, EEGeneral * tgen); // get data from XML
bool loadModelDataXML(QDomDocument * qdoc, ModelData * tmod, int modelNum = -1); // get data from XML

bool getSplashHEX(QString fileName, uchar * b, QWidget *parent = 0);
bool putSplashHEX(QString fileName, uchar * b, QWidget *parent = 0);

#endif // HELPERS_H
