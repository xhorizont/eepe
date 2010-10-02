#ifndef HELPERS_H
#define HELPERS_H

#include <QtGui>
#include "pers.h"

#define TMR_NUM_OPTION  (TMR_VAROFS+2*MAX_DRSWITCH-3)


void populateSwitchCB(QComboBox *b, int value);
void populateTimerSwitchCB(QComboBox *b, int value);


#endif // HELPERS_H
