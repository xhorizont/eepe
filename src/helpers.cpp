#include <QtGui>
#include "pers.h"
#include "helpers.h"



void populateSwitchCB(QComboBox *b, int value=0)
{
    QString str = SWITCHES_STR;

    b->clear();
    for(int i=-MAX_DRSWITCH; i<=MAX_DRSWITCH; i++)
    {
        switch (i)
        {
            case (0):
                    b->addItem("---");
                break;
            case (MAX_DRSWITCH):
                    b->addItem("ON");
                break;
            case (-MAX_DRSWITCH):
                    b->addItem("OFF");
                break;
            default:
                    if(i>0)
                        b->addItem(str.mid((i-1)*3,3));
                    else
                        b->addItem("!" + str.mid((-i-1)*3,3));
                break;
        }
    }
    b->setCurrentIndex(value+MAX_DRSWITCH);
    b->setMaxVisibleItems(10);
}

void populateTimerSwitchCB(QComboBox *b, int value=0)
{

    QString str = SWITCHES_STR;
    QString stt = "OFFABSRUsRU%ELsEL%THsTH%ALsAL%P1 P1%P2 P2%P3 P3%";

    b->clear();
    for(int i=-TMR_NUM_OPTION; i<=TMR_NUM_OPTION; i++)
    {
        QString s;
        if(i<0) s+="!";

        if(abs(i)<TMR_VAROFS) s += stt.mid(abs(i)*3,3);
        else if((abs(i)-TMR_VAROFS)<(MAX_DRSWITCH-1)) s += str.mid((abs(i)-TMR_VAROFS)*3,3);
        else s += "m" + str.mid((abs(i)-TMR_VAROFS-MAX_DRSWITCH+1)*3,3);

        b->addItem(s);
    }
    b->setCurrentIndex(value+TMR_NUM_OPTION);
    b->setMaxVisibleItems(10);
}
