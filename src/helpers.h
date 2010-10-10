#ifndef HELPERS_H
#define HELPERS_H

#include <QtGui>
#include "pers.h"

#define TMR_NUM_OPTION  (TMR_VAROFS+2*MAX_DRSWITCH-3)


void populateSwitchCB(QComboBox *b, int value);
void populateTimerSwitchCB(QComboBox *b, int value);
void populateSourceCB(QComboBox *b, int stickMode=1, int value=0);
QString getSourceStr(int stickMode, int idx);


#endif // HELPERS_H
