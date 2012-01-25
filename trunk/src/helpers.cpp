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

void appendCDATAElement(QDomDocument * qdoc, QDomElement * pe,QString name, const char * data, int size)
{
        QDomElement e = qdoc->createElement(name);
        QDomCDATASection t = qdoc->createCDATASection(name);
        t.setData(QByteArray(data, size).toBase64());
        e.appendChild(t);
        pe->appendChild(e);
}

QDomElement getGeneralDataXML(QDomDocument * qdoc, EEGeneral * tgen)
{
    QDomElement gd = qdoc->createElement("GENERAL_DATA");

    appendNumberElement(qdoc, &gd, "Version", tgen->myVers, true); // have to write value here
    appendTextElement(qdoc, &gd, "Owner", QString::fromAscii(tgen->ownerName,sizeof(tgen->ownerName)).trimmed());
    appendCDATAElement(qdoc, &gd, "Data", (const char *)tgen,sizeof(EEGeneral));
    return gd;

}



QDomElement getModelDataXML(QDomDocument * qdoc, ModelData * tmod, int modelNum)
{
    QDomElement md = qdoc->createElement("MODEL_DATA");
    md.setAttribute("number", modelNum);

    appendNumberElement(qdoc, &md, "Version", tmod->mdVers, true); // have to write value here
    appendTextElement(qdoc, &md, "Name", QString::fromAscii(tmod->name,sizeof(tmod->name)).trimmed());
    appendCDATAElement(qdoc, &md, "Data", (const char *)tmod,sizeof(ModelData));
    return md;
}




bool loadGeneralDataXML(QDomDocument * qdoc, EEGeneral * tgen)
{
    //look for "GENERAL_DATA" tag
    QDomElement gde = qdoc->elementsByTagName("GENERAL_DATA").at(0).toElement();

    if(gde.isNull()) // couldn't find
        return false;

    //load cdata into tgen
    QDomNode n = gde.elementsByTagName("Data").at(0).firstChild();// get all children in Data
    while (!n.isNull())
    {
        if (n.isCDATASection())
        {
            const char * data = QByteArray::fromBase64(n.toCDATASection().data().toAscii()).data();
            memcpy(tgen, data, sizeof(EEGeneral));
            break;
        }
        n = n.nextSibling();
    }

    //check version?

    return true;
}

bool loadModelDataXML(QDomDocument * qdoc, ModelData * tmod, int modelNum)
{
    //look for MODEL_DATA with modelNum attribute.
    //if modelNum = -1 then just pick the first one
    QDomNodeList ndl = qdoc->elementsByTagName("MODEL_DATA");

    //cycle through nodes to find correct model number
    QDomNode k = ndl.at(0);
    if(modelNum>=0) //if we should look for SPECIFIC model cycle through models
    {
        while(!k.isNull())
        {
            int a = k.toElement().attribute("number").toInt();
            if(a==modelNum)
                break;
            k = k.nextSibling();
        }
    }

    if(k.isNull()) // couldn't find
        return false;

    //load cdata into tgen
    QDomNode n = k.toElement().elementsByTagName("Data").at(0).firstChild();// get all children in Data
    while (!n.isNull())
    {
        if (n.isCDATASection())
        {
            const char * data = QByteArray::fromBase64(n.toCDATASection().data().toAscii()).data();
            memcpy(tmod, data, sizeof(ModelData));
            break;
        }
        n = n.nextSibling();
    }

    //check version?

    return true;
}

