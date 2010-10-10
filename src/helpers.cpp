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


#define MODI_STR  "RUD ELE THR AIL RUD THR ELE AIL AIL ELE THR RUD AIL THR ELE RUD "
#define SRCP_STR  "P1  P2  P3  MAX FULLPPM1PPM2PPM3PPM4PPM5PPM6PPM7PPM8CH1 CH2 CH3 CH4 CH5 CH6 CH7 CH8 CH9 CH10CH11CH12CH13CH14CH15CH16CH17CH18CH19CH20CH21CH22CH23CH24CH25CH26CH27CH28CH29CH30"

QString getSourceStr(int stickMode=1, int idx=0)
{
    if(!idx)
        return "----";
    else if(idx>=1 && idx<=4)
    {
        QString modi = MODI_STR;
        return modi.mid((idx-1)*4+stickMode*16,4);
    }
    else
    {
        QString str = SRCP_STR;
        return str.mid((idx-5)*4,4);
    }

    return "";
}

void populateSourceCB(QComboBox *b, int stickMode, int value)
{
    QString modi = MODI_STR;
    QString str = SRCP_STR;
    b->clear();
    b->addItem("----");
    for(int i=0; i<4; i++)  b->addItem(modi.mid(i*4+stickMode*16,4));
    for(int i=0; i<29; i++) b->addItem(str.mid(i*4,4));
    b->setCurrentIndex(value);
    b->setMaxVisibleItems(10);
}



