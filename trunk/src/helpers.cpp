#include <QtGui>
#include "pers.h"
#include "helpers.h"


QString getSWName(int val)
{

    if(!val) return "---";
    if(val==MAX_DRSWITCH) return "ON";
    if(val==-MAX_DRSWITCH) return "OFF";

    return QString(val<0 ? "!" : "") + QString(SWITCHES_STR).mid((abs(val)-1)*3,3);
}

void populateSwitchCB(QComboBox *b, int value=0)
{
    b->clear();
    for(int i=-MAX_DRSWITCH; i<=MAX_DRSWITCH; i++)
        b->addItem(getSWName(i));
    b->setCurrentIndex(value+MAX_DRSWITCH);
    b->setMaxVisibleItems(10);
}

void populateCurvesCB(QComboBox *b, int value)
{
    QString str = CURV_STR;
    b->clear();
    for(int i=0; i<(str.length()/3); i++)  b->addItem(str.mid(i*3,3).replace("c","Curve "));
    b->setCurrentIndex(value);
    b->setMaxVisibleItems(10);
}


void populateTimerSwitchCB(QComboBox *b, int value=0)
{
    b->clear();
    for(int i=-TMR_NUM_OPTION; i<=TMR_NUM_OPTION; i++)
        b->addItem(getTimerMode(i));
    b->setCurrentIndex(value+TMR_NUM_OPTION);
    b->setMaxVisibleItems(10);
}

QString getTimerMode(int tm)
{

    QString str = SWITCHES_STR;
    QString stt = "OFFABSRUsRU%ELsEL%THsTH%ALsAL%P1 P1%P2 P2%P3 P3%";

    QString s;
    if(abs(tm)<TMR_VAROFS)
    {
        s = stt.mid(abs(tm)*3,3);
        if(tm<-1) s.prepend("!");
        return s;
    }

    if(abs(tm)<(TMR_VAROFS+MAX_DRSWITCH-1))
    {
        s = str.mid((abs(tm)-TMR_VAROFS)*3,3);
        if(tm<0) s.prepend("!");
        return s;
    }


    s = "m" + str.mid((abs(tm)-(TMR_VAROFS+MAX_DRSWITCH-1))*3,3);
    if(tm<0) s.prepend("!");
    return s;

}




#define MODI_STR  "RUD ELE THR AIL RUD THR ELE AIL AIL ELE THR RUD AIL THR ELE RUD "
#define SRCP_STR  "P1  P2  P3  MAX FULLCYC1CYC2CYC3PPM1PPM2PPM3PPM4PPM5PPM6PPM7PPM8CH1 CH2 CH3 CH4 CH5 CH6 CH7 CH8 CH9 CH10CH11CH12CH13CH14CH15CH16CH17CH18CH19CH20CH21CH22CH23CH24CH25CH26CH27CH28CH29CH30"

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
    b->clear();
    for(int i=0; i<37; i++) b->addItem(getSourceStr(stickMode,i));
    b->setCurrentIndex(value);
    b->setMaxVisibleItems(10);
}


QString getCSWFunc(int val)
{
    return QString(CSWITCH_STR).mid(val*CSW_LEN_FUNC,CSW_LEN_FUNC);
}


void populateCSWCB(QComboBox *b, int value)
{
    b->clear();
    for(int i=0; i<CSW_NUM_FUNC; i++) b->addItem(getCSWFunc(i));
    b->setCurrentIndex(value);
    b->setMaxVisibleItems(10);
}

int getValueFromLine(const QString &line, int pos, int len=2)
{
    bool ok;
    int hex = line.mid(pos,len).toInt(&ok, 16);
    return ok ? hex : -1;
}

QString iHEXLine(quint8 * data, quint16 addr, quint8 len)
{
    QString str = QString(":%1%2000").arg(len,2,16,QChar('0')).arg(addr,4,16,QChar('0')); //write start, bytecount (32), address and record type
    quint8 chkSum = 0;
    chkSum = -len; //-bytecount; recordtype is zero
    chkSum -= addr & 0xFF;
    chkSum -= addr >> 8;
    for(int j=0; j<len; j++)
    {
        str += QString("%1").arg(data[addr+j],2,16,QChar('0'));
        chkSum -= data[addr+j];
    }

    str += QString("%1").arg(chkSum,2,16,QChar('0'));
    return str.toUpper(); // output to file and lf;
}

int loadiHEX(QWidget *parent, QString fileName, quint8 * data, int datalen, QString header)
{
    //load from intel hex type file
    QFile file(fileName);
    int finalSize = 0;

    if(!file.exists())
    {
        QMessageBox::critical(parent, QObject::tr("Error"),QObject::tr("Unable to find file %1!").arg(fileName));
        return 0;
    }

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {  //reading HEX TEXT file
        QMessageBox::critical(parent, QObject::tr("Error"),
                              QObject::tr("Error opening file %1:\n%2.")
                              .arg(fileName)
                              .arg(file.errorString()));
        return 0;
    }

    memset(data,0,datalen);

    QTextStream in(&file);

    if(!header.isEmpty())
    {
        QString hline = in.readLine();

        if(hline!=header)
        {
            QMessageBox::critical(parent, QObject::tr("Error"),
                                  QObject::tr("Invalid EEPE File Format %1")
                                  .arg(fileName));
            file.close();
            return 0;
        }
    }

    while (!in.atEnd())
    {
        QString line = in.readLine();

        if(line.left(1)!=":") continue;

        int byteCount = getValueFromLine(line,1);
        int address = getValueFromLine(line,3,4);
        int recType = getValueFromLine(line,7);

        if(byteCount<0 || address<0 || recType<0)
        {
            QMessageBox::critical(parent, QObject::tr("Error"),QObject::tr("Error reading file %1!").arg(fileName));
            file.close();
            return 0;
        }

        QByteArray ba;
        ba.clear();

        quint8 chkSum = 0;
        chkSum -= byteCount;
        chkSum -= recType;
        chkSum -= address & 0xFF;
        chkSum -= address >> 8;
        for(int i=0; i<byteCount; i++)
        {
            quint8 v = getValueFromLine(line,(i*2)+9) & 0xFF;
            chkSum -= v;
            ba.append(v);
        }


        quint8 retV = getValueFromLine(line,(byteCount*2)+9) & 0xFF;
        if(chkSum!=retV) {
            QMessageBox::critical(parent, QObject::tr("Error"),QObject::tr("Checksum Error reading file %1!").arg(fileName));
            file.close();
            return 0;
        }

        if((recType == 0x00) && ((address+byteCount)<=datalen)) //data record - ba holds record
        {
            memcpy(&data[address],ba.data(),byteCount);
            finalSize += byteCount;
        }

    }

    file.close();
    return finalSize;
}

bool saveiHEX(QWidget *parent, QString fileName, quint8 * data, int datalen, QString header, int notesIndex)
{
    QFile file(fileName);


    //open file
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(parent, QObject::tr("Error"),
                             QObject::tr("Cannot write file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return false;
    }

    QTextStream out(&file);

    //write file header in Intel HEX format
    if(!header.isEmpty())
    {
        out << header << "\n";
    }

    int addr = 0;
    if(notesIndex>0)
        addr =0;

    while (addr<datalen)
    {
        int llen = 32;
        if((datalen-addr)<llen)
            llen = datalen-addr;

        out << iHEXLine(data, addr, llen) << "\n";

        addr += llen;
    }

    out << ":00000001FF";  // write EOF

    file.close();

    return true;
}

void appendTextElement(QDomDocument * qdoc, QDomElement * pe, QString name, QString value)
{
    QDomElement e = qdoc->createElement(name);
    QDomText t = qdoc->createTextNode(name);
    t.setNodeValue(value);
    e.appendChild(t);
    pe->appendChild(e);
}

void appendNumberElement(QDomDocument * qdoc, QDomElement * pe,QString name, int value, bool forceZeroWrite = false)
{
    if(value || forceZeroWrite)
    {
        QDomElement e = qdoc->createElement(name);
        QDomText t = qdoc->createTextNode(name);
        t.setNodeValue(QString("%1").arg(value));
        e.appendChild(t);
        pe->appendChild(e);
    }
}

QDomElement appendEmptyElement(QDomDocument * qdoc, QDomElement * pe,QString name)
{
    QDomElement e = qdoc->createElement(name);
    pe->appendChild(e);
    return e;
}

bool isArrayZero(quint8 * ar, int size)
{
    for(int i=0; i<size; i++)
    {
        if(ar[i])
            return false;
    }

    return true;
}

QDomElement getGeneralDataXML(QDomDocument * qdoc, EEGeneral * tgen)
{
    QDomElement gd = qdoc->createElement("GENERAL_DATA");

    //    uint8_t   myVers;
    appendNumberElement(qdoc, &gd, "myVers", tgen->myVers, true); // have to write value here

    //    int16_t   calibMid[7];
    if(!isArrayZero((quint8 *)&tgen->calibMid,sizeof(tgen->calibMid)))
    {
        QDomElement calibMid = appendEmptyElement(qdoc, &gd, "calibMid");
        for(int i=0; i<7; i++)
            appendNumberElement(qdoc, &calibMid, QString("%1").arg(i), tgen->calibMid[i]);
    }

    //    int16_t   calibSpanNeg[7];
    if(!isArrayZero((quint8 *)&tgen->calibSpanNeg,sizeof(tgen->calibSpanNeg)))
    {
        QDomElement calibSpanNeg = appendEmptyElement(qdoc, &gd, "calibSpanNeg");
        for(int i=0; i<7; i++)
            appendNumberElement(qdoc, &calibSpanNeg, QString("%1").arg(i), tgen->calibSpanNeg[i]);
    }

    //    int16_t   calibSpanPos[7];
    if(!isArrayZero((quint8 *)&tgen->calibSpanPos,sizeof(tgen->calibSpanPos)))
    {
        QDomElement calibSpanPos = appendEmptyElement(qdoc, &gd, "calibSpanPos");
        for(int i=0; i<7; i++)
            appendNumberElement(qdoc, &calibSpanPos, QString("%1").arg(i), tgen->calibSpanPos[i]);
    }

    //    uint16_t  chkSum;
    appendNumberElement(qdoc, &gd, "chkSum", tgen->chkSum);

    //    uint8_t   currModel; //0..15
    appendNumberElement(qdoc, &gd, "currModel", tgen->currModel);

    //    uint8_t   contrast;
    appendNumberElement(qdoc, &gd, "contrast", tgen->contrast);

    //    uint8_t   vBatWarn;
    appendNumberElement(qdoc, &gd, "vBatWarn", tgen->vBatWarn);

    //    int8_t    vBatCalib;
    appendNumberElement(qdoc, &gd, "vBatCalib", tgen->vBatCalib);

    //    int8_t    lightSw;
    appendNumberElement(qdoc, &gd, "lightSw", tgen->lightSw);

    //    TrainerData trainer;
    //    typedef struct t_TrainerMix {
    //        uint8_t srcChn:3; //0-7 = ch1-8
    //        int8_t  swtch:5;
    //        int8_t  studWeight:6;
    //        uint8_t mode:2;   //off,add-mode,subst-mode
    //    } __attribute__((packed)) TrainerMix; //

    //    typedef struct t_TrainerData {
    //        int16_t        calib[4];
    //        TrainerMix     mix[4];
    //    } __attribute__((packed)) TrainerData;
    if(!isArrayZero((quint8 *)&tgen->trainer,sizeof(tgen->trainer)))
    {
        QDomElement trainer = appendEmptyElement(qdoc, &gd, "trainer");

        if(!isArrayZero((quint8 *)&tgen->trainer.calib,sizeof(tgen->trainer.calib)))
        {
            QDomElement trainer_calib = appendEmptyElement(qdoc, &trainer, "calib");
            for(int i=0; i<4; i++)
                appendNumberElement(qdoc, &trainer_calib, QString("%1").arg(i), tgen->trainer.calib[i]);
        }

        if(!isArrayZero((quint8 *)&tgen->trainer.mix,sizeof(tgen->trainer.mix)))
        {
            QDomElement trainer_mix = appendEmptyElement(qdoc, &trainer, "calib");
            for(int i=0; i<4; i++)
            {
                if(!isArrayZero((quint8 *)&tgen->trainer.mix[i],sizeof(tgen->trainer.mix[i])))
                {
                    QDomElement mix = appendEmptyElement(qdoc, &trainer_mix, "mix");
                    mix.setAttribute("id",i);

                    appendNumberElement(qdoc, &mix, "srcChn", tgen->trainer.mix[i].srcChn);
                    appendNumberElement(qdoc, &mix, "swtch", tgen->trainer.mix[i].swtch);
                    appendNumberElement(qdoc, &mix, "studWeight", tgen->trainer.mix[i].studWeight);
                    appendNumberElement(qdoc, &mix, "mode", tgen->trainer.mix[i].mode);
                }
            }
        }
    }


    //    uint8_t   view;
    appendNumberElement(qdoc, &gd, "view", tgen->view);

    //    uint8_t   disableThrottleWarning:1;
    appendNumberElement(qdoc, &gd, "disableThrottleWarning", tgen->disableThrottleWarning);

    //    uint8_t   disableSwitchWarning:1;
    appendNumberElement(qdoc, &gd, "disableSwitchWarning", tgen->disableSwitchWarning);

    //    uint8_t   disableMemoryWarning:1;
    appendNumberElement(qdoc, &gd, "disableMemoryWarning", tgen->disableMemoryWarning);

    //    uint8_t   beeperVal:3;
    appendNumberElement(qdoc, &gd, "beeperVal", tgen->beeperVal);

    //    uint8_t   reserveWarning:1;
    appendNumberElement(qdoc, &gd, "reserveWarning", tgen->reserveWarning);

    //    uint8_t   disableAlarmWarning:1;
    appendNumberElement(qdoc, &gd, "disableAlarmWarning", tgen->disableAlarmWarning);

    //    uint8_t   stickMode;
    appendNumberElement(qdoc, &gd, "stickMode", tgen->stickMode);

    //    int8_t    inactivityTimer;
    appendNumberElement(qdoc, &gd, "inactivityTimer", tgen->inactivityTimer);

    //    uint8_t   throttleReversed:1;
    appendNumberElement(qdoc, &gd, "throttleReversed", tgen->throttleReversed);

    //    uint8_t   minuteBeep:1;
    appendNumberElement(qdoc, &gd, "minuteBeep", tgen->minuteBeep);

    //    uint8_t   preBeep:1;
    appendNumberElement(qdoc, &gd, "preBeep", tgen->preBeep);

    //    uint8_t   flashBeep:1;
    appendNumberElement(qdoc, &gd, "flashBeep", tgen->flashBeep);

    //    uint8_t   disableSplashScreen:1;
    appendNumberElement(qdoc, &gd, "disableSplashScreen", tgen->disableSplashScreen);

    //    uint8_t   disablePotScroll:1;
    appendNumberElement(qdoc, &gd, "disablePotScroll", tgen->disablePotScroll);

    //    uint8_t   disableBG:1;
    appendNumberElement(qdoc, &gd, "disableBG", tgen->disableBG);

    //    uint8_t   frskyinternalalarm:1;
    appendNumberElement(qdoc, &gd, "frskyinternalalarm", tgen->frskyinternalalarm);

    //    uint8_t   filterInput;
    appendNumberElement(qdoc, &gd, "filterInput", tgen->filterInput);

    //    uint8_t   lightAutoOff;
    appendNumberElement(qdoc, &gd, "lightAutoOff", tgen->lightAutoOff);

    //    uint8_t   templateSetup;  //RETA order according to chout_ar array
    appendNumberElement(qdoc, &gd, "templateSetup", tgen->templateSetup);

    //    int8_t    PPM_Multiplier;
    appendNumberElement(qdoc, &gd, "PPM_Multiplier", tgen->PPM_Multiplier);

    //    uint8_t   FRSkyYellow:4;
    appendNumberElement(qdoc, &gd, "FRSkyYellow", tgen->FRSkyYellow);

    //    uint8_t   FRSkyOrange:4;
    appendNumberElement(qdoc, &gd, "FRSkyOrange", tgen->FRSkyOrange);

    //    uint8_t   FRSkyRed:4;
    appendNumberElement(qdoc, &gd, "FRSkyRed", tgen->FRSkyRed);

    //    uint8_t   hideNameOnSplash:1;
    appendNumberElement(qdoc, &gd, "hideNameOnSplash", tgen->hideNameOnSplash);

    //    uint8_t   speakerPitch;
    appendNumberElement(qdoc, &gd, "speakerPitch", tgen->speakerPitch);

    //    uint8_t   hapticStrength;
    appendNumberElement(qdoc, &gd, "hapticStrength", tgen->hapticStrength);

    //    uint8_t   speakerMode;
    appendNumberElement(qdoc, &gd, "speakerMode", tgen->speakerMode);

    //    uint8_t   lightOnStickMove;
    appendNumberElement(qdoc, &gd, "lightOnStickMove", tgen->lightOnStickMove);

    //    char      ownerName[GENERAL_OWNER_NAME_LEN];
    appendTextElement(qdoc, &gd, "ownerName", QString::fromAscii(tgen->ownerName,sizeof(tgen->ownerName)).trimmed());

    //    uint8_t   switchWarningStates;
    appendNumberElement(qdoc, &gd, "switchWarningStates", tgen->switchWarningStates);

    return gd;
}

QDomElement getModelDataXML(QDomDocument * qdoc, ModelData * tmod, int modelNum)
{
    QDomElement md = qdoc->createElement("MODEL_DATA");
    md.setAttribute("number", modelNum);


    //      char      name[MODEL_NAME_LEN];             // 10 must be first for eeLoadModelName
    appendTextElement(qdoc, &md, "name", QString::fromAscii(tmod->name,sizeof(tmod->name)).trimmed());

    //      uint8_t   mdVers;
    appendNumberElement(qdoc, &md, "mdVers", tmod->mdVers, true);

    //      int8_t    tmrMode;              // timer trigger source -> off, abs, stk, stk%, sw/!sw, !m_sw/!m_sw
    appendNumberElement(qdoc, &md, "tmrMode", tmod->tmrMode);

    //      uint8_t   tmrDir:1;    //0=>Count Down, 1=>Count Up
    appendNumberElement(qdoc, &md, "tmrDir", tmod->tmrDir);

    //      uint8_t   traineron:1;  // 0 disable trainer, 1 allow trainer
    appendNumberElement(qdoc, &md, "traineron", tmod->traineron);

    //      uint8_t   t2throttle:1 ;  // Start timer2 using throttle
    appendNumberElement(qdoc, &md, "t2throttle", tmod->t2throttle);

    //      uint8_t   FrSkyUsrProto:2 ;  // Protocol in FrSky User Data, 0=FrSky Hub, 1=WS HowHigh
    appendNumberElement(qdoc, &md, "FrSkyUsrProto", tmod->FrSkyUsrProto);

    //      uint8_t   FrSkyImperial:1 ;  // Convert FrSky values to imperial units
    appendNumberElement(qdoc, &md, "FrSkyImperial", tmod->FrSkyImperial);

    //      uint8_t   FrSkyAltAlarm:2;
    appendNumberElement(qdoc, &md, "FrSkyAltAlarm", tmod->FrSkyAltAlarm);

    //      uint16_t  tmrVal;
    appendNumberElement(qdoc, &md, "tmrVal", tmod->tmrVal);

    //      uint8_t   protocol;
    appendNumberElement(qdoc, &md, "protocol", tmod->protocol);

    //      int8_t    ppmNCH;
    appendNumberElement(qdoc, &md, "ppmNCH", tmod->ppmNCH);

    //      uint8_t   thrTrim:4;            // Enable Throttle Trim
    appendNumberElement(qdoc, &md, "thrTrim", tmod->thrTrim);

    //      uint8_t   thrExpo:4;            // Enable Throttle Expo
    appendNumberElement(qdoc, &md, "thrExpo", tmod->thrExpo);

    //      int8_t    trimInc;              // Trim Increments
    appendNumberElement(qdoc, &md, "trimInc", tmod->trimInc);

    //      int8_t    ppmDelay;
    appendNumberElement(qdoc, &md, "ppmDelay", tmod->ppmDelay);

    //      int8_t    trimSw;
    appendNumberElement(qdoc, &md, "trimSw", tmod->trimSw);

    //      uint8_t   beepANACenter;        // 1<<0->A1.. 1<<6->A7
    appendNumberElement(qdoc, &md, "beepANACenter", tmod->beepANACenter);

    //      uint8_t   pulsePol:1;
    appendNumberElement(qdoc, &md, "pulsePol", tmod->pulsePol);

    //      uint8_t   extendedLimits:1;
    appendNumberElement(qdoc, &md, "extendedLimits", tmod->extendedLimits);

    //      uint8_t   swashInvertELE:1;
    appendNumberElement(qdoc, &md, "swashInvertELE", tmod->swashInvertELE);

    //      uint8_t   swashInvertAIL:1;
    appendNumberElement(qdoc, &md, "swashInvertAIL", tmod->swashInvertAIL);

    //      uint8_t   swashInvertCOL:1;
    appendNumberElement(qdoc, &md, "swashInvertCOL", tmod->swashInvertCOL);

    //      uint8_t   swashType:3;
    appendNumberElement(qdoc, &md, "swashType", tmod->swashType);

    //      uint8_t   swashCollectiveSource;
    appendNumberElement(qdoc, &md, "swashCollectiveSource", tmod->swashCollectiveSource);

    //      uint8_t   swashRingValue;
    appendNumberElement(qdoc, &md, "swashRingValue", tmod->swashRingValue);

    //      int8_t    ppmFrameLength;    //0=22.5  (10msec-30msec) 0.5msec increments
    appendNumberElement(qdoc, &md, "ppmFrameLength", tmod->ppmFrameLength);


    //      LimitData limitData[NUM_CHNOUT];
    if(!isArrayZero((quint8 *)&tmod->limitData,sizeof(tmod->limitData)))
    {
        QDomElement limitData = appendEmptyElement(qdoc, &md, "limitData");
        for(int i=0; i<NUM_CHNOUT; i++)
            if(!isArrayZero((quint8 *)&tmod->limitData[i],sizeof(tmod->limitData[i])))
            {
                QDomElement singleLimit = appendEmptyElement(qdoc, &limitData, QString("%1").arg(i));

                //        int8_t  min;
                appendNumberElement(qdoc, &singleLimit, "min", tmod->limitData[i].min);

                //        int8_t  max;
                appendNumberElement(qdoc, &singleLimit, "max", tmod->limitData[i].max);

                //        bool    revert;
                appendNumberElement(qdoc, &singleLimit, "revert", tmod->limitData[i].revert);

                //        int16_t  offset;
                appendNumberElement(qdoc, &singleLimit, "offset", tmod->limitData[i].offset);
            }
    }



    //      ExpoData  expoData[4];
    if(!isArrayZero((quint8 *)&tmod->expoData,sizeof(tmod->expoData)))
    {
        QDomElement expoData = appendEmptyElement(qdoc, &md, "expoData");
        for(int i=0; i<4; i++)
            if(!isArrayZero((quint8 *)&tmod->expoData[i],sizeof(tmod->expoData[i])))
            {
                QDomElement singleExpo = appendEmptyElement(qdoc, &expoData, QString("%1").arg(i));

                //        int8_t  expo[3][2][2];
                QDomElement expo = appendEmptyElement(qdoc, &singleExpo, "expo");
                if(!isArrayZero((quint8 *)&tmod->expoData[i],sizeof(tmod->expoData[i])))
                {
                    for(int a=0; a<3; a++)
                        if(!isArrayZero((quint8 *)&tmod->expoData[i].expo[a],sizeof(tmod->expoData[i].expo[a])))
                        {
                            QDomElement expo_a = appendEmptyElement(qdoc, &expo, QString("%1").arg(a));
                            for(int b=0; b<2; b++)
                                if(!isArrayZero((quint8 *)&tmod->expoData[i].expo[a][b],sizeof(tmod->expoData[i].expo[a][b])))
                                {
                                    QDomElement expo_b = appendEmptyElement(qdoc, &expo_a, QString("%1").arg(b));
                                    for(int c=0; c<2; c++)
                                        appendNumberElement(qdoc, &expo_b, QString("%1").arg(c), tmod->expoData[i].expo[a][b][c]);
                                }
                        }
                }

                //        int8_t  drSw1;
                appendNumberElement(qdoc, &singleExpo, "drSw1", tmod->expoData[i].drSw1);

                //        int8_t  drSw2;
                appendNumberElement(qdoc, &singleExpo, "drSw2", tmod->expoData[i].drSw2);

            }
    }


    //      MixData   mixData[MAX_MIXERS];
    if(!isArrayZero((quint8 *)&tmod->mixData,sizeof(tmod->mixData)))
    {
        QDomElement mixData = appendEmptyElement(qdoc, &md, "mixData");
        for(int i=0; i<MAX_MIXERS; i++)
            if(!isArrayZero((quint8 *)&tmod->mixData[i],sizeof(tmod->mixData[i])))
            {
                QDomElement singleMix = appendEmptyElement(qdoc, &mixData, QString("%1").arg(i));

                //        uint8_t destCh;            //        1..NUM_CHNOUT
                appendNumberElement(qdoc, &singleMix, "destCh", tmod->mixData[i].destCh);

                //        uint8_t srcRaw;            //
                appendNumberElement(qdoc, &singleMix, "srcRaw", tmod->mixData[i].srcRaw);

                //        int8_t  weight;
                appendNumberElement(qdoc, &singleMix, "weight", tmod->mixData[i].weight);

                //        int8_t  swtch;
                appendNumberElement(qdoc, &singleMix, "swtch", tmod->mixData[i].swtch);

                //        uint8_t curve;             //0=symmetrisch 1=no neg 2=no pos
                appendNumberElement(qdoc, &singleMix, "curve", tmod->mixData[i].curve);

                //        uint8_t delayUp:4;
                appendNumberElement(qdoc, &singleMix, "delayUp", tmod->mixData[i].delayUp);

                //        uint8_t delayDown:4;
                appendNumberElement(qdoc, &singleMix, "delayDown", tmod->mixData[i].delayDown);

                //        uint8_t speedUp:4;         // Servogeschwindigkeit aus Tabelle (10ms Cycle)
                appendNumberElement(qdoc, &singleMix, "speedUp", tmod->mixData[i].speedUp);

                //        uint8_t speedDown:4;       // 0 nichts
                appendNumberElement(qdoc, &singleMix, "speedDown", tmod->mixData[i].speedDown);

                //        uint8_t carryTrim:1;
                appendNumberElement(qdoc, &singleMix, "carryTrim", tmod->mixData[i].carryTrim);

                //        uint8_t mltpx:3;           // multiplex method 0=+ 1=* 2=replace
                appendNumberElement(qdoc, &singleMix, "mltpx", tmod->mixData[i].mltpx);

                //        uint8_t mixWarn:2;         // mixer warning
                appendNumberElement(qdoc, &singleMix, "mixWarn", tmod->mixData[i].mixWarn);

                //        uint8_t enableFmTrim:1;
                appendNumberElement(qdoc, &singleMix, "enableFmTrim", tmod->mixData[i].enableFmTrim);

                //        uint8_t mixres:1;
                appendNumberElement(qdoc, &singleMix, "mixres", tmod->mixData[i].mixres);

                //        int8_t  sOffset;
                appendNumberElement(qdoc, &singleMix, "sOffset", tmod->mixData[i].sOffset);
            }
    }



    //      int8_t    trim[4];
    if(!isArrayZero((quint8 *)&tmod->trim,sizeof(tmod->trim)))
    {
        QDomElement trim = appendEmptyElement(qdoc, &md, "trim");
        for(int i=0; i<4; i++)
            appendNumberElement(qdoc, &trim, QString("%1").arg(i), tmod->trim[i]);
    }

    //      int8_t    curves5[MAX_CURVE5][5];
    if(!isArrayZero((quint8 *)&tmod->curves5,sizeof(tmod->curves5)))
    {
        QDomElement curves5a = appendEmptyElement(qdoc, &md, "curves5");
        for(int a=0; a<MAX_CURVE5; a++)
            if(!isArrayZero((quint8 *)&tmod->curves5[a],sizeof(tmod->curves5[a])))
            {
                QDomElement curves5b = appendEmptyElement(qdoc, &curves5a, QString("%1").arg(a));
                for(int b=0; b<5; b++)
                    appendNumberElement(qdoc, &curves5b, QString("%1").arg(b), tmod->curves5[a][b]);
            }
    }

    //      int8_t    curves9[MAX_CURVE9][9];
    if(!isArrayZero((quint8 *)&tmod->curves9,sizeof(tmod->curves9)))
    {
        QDomElement curves9a = appendEmptyElement(qdoc, &md, "curves9");
        for(int a=0; a<MAX_CURVE9; a++)
            if(!isArrayZero((quint8 *)&tmod->curves9[a],sizeof(tmod->curves9[a])))
            {
                QDomElement curves9b = appendEmptyElement(qdoc, &curves9a, QString("%1").arg(a));
                for(int b=0; b<9; b++)
                    appendNumberElement(qdoc, &curves9b, QString("%1").arg(b), tmod->curves9[a][b]);
            }
    }



    //      CSwData   customSw[NUM_CSW];
    if(!isArrayZero((quint8 *)&tmod->customSw,sizeof(tmod->customSw)))
    {
        QDomElement customSw = appendEmptyElement(qdoc, &md, "customSw");
        for(int i=0; i<NUM_CSW; i++)
            if(!isArrayZero((quint8 *)&tmod->customSw[i],sizeof(tmod->customSw[i])))
            {
                QDomElement singleCSW = appendEmptyElement(qdoc, &customSw, QString("%1").arg(i));

                //        int8_t  v1; //input
                appendNumberElement(qdoc, &singleCSW, "v1", tmod->customSw[i].v1);

                //        int8_t  v2; //offset
                appendNumberElement(qdoc, &singleCSW, "v2", tmod->customSw[i].v2);

                //        uint8_t func;
                appendNumberElement(qdoc, &singleCSW, "func", tmod->customSw[i].func);
            }
    }



    //      SafetySwData  safetySw[NUM_CHNOUT];
    if(!isArrayZero((quint8 *)&tmod->safetySw,sizeof(tmod->safetySw)))
    {
        QDomElement safetySw = appendEmptyElement(qdoc, &md, "safetySw");
        for(int i=0; i<NUM_CHNOUT; i++)
            if(!isArrayZero((quint8 *)&tmod->safetySw[i],sizeof(tmod->safetySw[i])))
            {
                QDomElement singleSSW = appendEmptyElement(qdoc, &safetySw, QString("%1").arg(i));

                //        int8_t  swtch;
                appendNumberElement(qdoc, &singleSSW, "swtch", tmod->safetySw[i].swtch);

                //        int8_t  val;
                appendNumberElement(qdoc, &singleSSW, "val", tmod->safetySw[i].val);
            }
    }


    //      FrSkyData frsky;
    if(!isArrayZero((quint8 *)&tmod->frsky,sizeof(tmod->frsky)))
    {
        QDomElement frsky = appendEmptyElement(qdoc, &md, "frsky");
        QDomElement channels = appendEmptyElement(qdoc, &frsky, "channels");

        for(int i=0; i<2; i++)
            if(!isArrayZero((quint8 *)&tmod->frsky.channels[i],sizeof(tmod->frsky.channels[i])))
            {
                QDomElement singleFrsky = appendEmptyElement(qdoc, &channels, QString("%1").arg(i));

                //        uint8_t   ratio;                // 0.0 means not used, 0.1V steps EG. 6.6 Volts = 66. 25.1V = 251, etc.
                appendNumberElement(qdoc, &singleFrsky, "ratio", tmod->frsky.channels[i].ratio);

                //        uint8_t   alarms_value[2];      // 0.1V steps EG. 6.6 Volts = 66. 25.1V = 251, etc.
                if(!isArrayZero((quint8 *)&tmod->frsky.channels[i].alarms_value,sizeof(tmod->frsky.channels[i].alarms_value)))
                {
                    QDomElement alarms_value = appendEmptyElement(qdoc, &singleFrsky, "alarms_value");
                    for(int a=0; a<2; a++)
                        appendNumberElement(qdoc, &alarms_value, QString("%1").arg(a), tmod->frsky.channels[i].alarms_value[a]);
                }

                //        uint8_t   alarms_level:4;
                appendNumberElement(qdoc, &singleFrsky, "alarms_level", tmod->frsky.channels[i].alarms_level);

                //        uint8_t   alarms_greater:2;     // 0=LT(<), 1=GT(>)
                appendNumberElement(qdoc, &singleFrsky, "alarms_greater", tmod->frsky.channels[i].alarms_greater);

                //        uint8_t   type:2;               // future use: 0=volts, ...
                appendNumberElement(qdoc, &singleFrsky, "type", tmod->frsky.channels[i].type);

            }
    }


    return md;
}



bool loadGeneralDataXML(QDomDocument * qdoc, EEGeneral * tgen)
{
    return true;
}

bool loadModelDataXML(QDomDocument * qdoc, ModelData * tmod)
{
    return true;
}

