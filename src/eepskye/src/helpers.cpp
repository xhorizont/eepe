#include <QtGui>
#include "pers.h"
#include "helpers.h"

QString AudioAlarms[] = {
	"Warn1",
	"Warn2",
	"Cheap",
	"Ring",
	"SciFi",
	"Robot",
	"Chirp",
	"Tada",
	"Crickt",
	"Siren",
	"AlmClk",
	"Ratata",
	"Tick",
	"Haptc1",
	"Haptc2",
	"Haptc3"
} ;

QString TelemItems[] = {
	"----",
	"A1= ",
	"A2= ",
	"RSSI",
	"TSSI",
	"Tim1",
	"Tim2",
	"Alt ",
	"Galt",
	"Gspd",
	"T1= ",
	"T2= ",
	"RPM ",
	"FUEL",
	"Mah1",
	"Mah2",
	"Cvlt",
	"Batt",
	"Amps",
	"Mah ",
	"Ctot",
	"FasV",
	"AccX",
	"AccY",
	"AccZ",
	"Vspd",
	"Gvr1",
	"Gvr2",
	"Gvr3",
	"Gvr4",
	"Gvr5"
} ;

QString GvarItems[] = {
	"---", // 0
	"Rtm",
	"Etm",
	"Ttm",
	"Atm",
	"REN",	// 5
	"RUD",
	"ELE",
	"THR",
	"AIL", // 9
	"P1 ",
	"P2 ",
	"P3 "  //12
} ;


void populateGvarCB(QComboBox *b, int value)
{
    b->clear();
    for(int i=0; i<=12; i++)
        b->addItem(GvarItems[i]);
    b->setCurrentIndex(value);
    b->setMaxVisibleItems(13);
}



// This routine converts an 8 bit value for custom switch use
int16_t convertTelemConstant( int8_t index, int8_t value)
{
  int16_t result;

	result = value + 125 ;
  switch (index)
	{
    case 4 :	// Timer1
    case 5 :	// Timer2
      result *= 10 ;
    break;
		case 6:		// Alt
    case 7:		// Gpa Alt
			if ( result > 63 )
			{
      	result *= 2 ;
      	result -= 64 ;
			}
			if ( result > 192 )
			{
      	result *= 2 ;
      	result -= 192 ;
			}
			if ( result > 448 )
			{
      	result *= 2 ;
      	result -= 488 ;
			}
    break;
    case 11:	// RPM
      result *= 100;
    break;
    case 9:   // Temp1
    case 10:	// temp2
      result -= 30;
    break;
    case 13:	// Mah1
    case 14:	// Mah2
    case 18:	// Mah
      result *= 50;
    break;

		case 15:	// Cell volts
      result *= 2;
		break ;
		case 19 :	// Cells total
		case 20 :	// FAS100 volts
      result *= 2;
		break ;
  }
  return result;
}

#define PREC1		1
#define PREC2		2

void stringTelemetryChannel( char *string, int8_t index, int16_t val, SKYModelData *model )
{
	uint8_t unit = ' ' ;
	uint8_t displayed = 0 ;
	uint8_t att = 0 ;

  switch (index)
	{
    case 4 :
    case 5 :
			{	
				int16_t rem ;

        rem = val % 60 ;
				val /= 60 ;
				sprintf( string, "%d:%02d", val, rem ) ;
//      putsTime(x-FW, y, val, att, att) ;
        displayed = 1 ;
//    	unit = channel + 2 + '1';
			}
		break ;
    
		case 0:
    case 1:
    	{
    	  uint32_t value = val ;
    	  uint8_t times2 ;
        SKYFrSkyChannelData *fd ;

  			fd = &model->frsky.channels[index] ;
    	  value = val ;
    		if (fd->type == 2/*V*/)
    		{
    		    times2 = 1 ;
    		}
    		else
    		{
    		    times2 = 0 ;
    		}
				uint16_t ratio ;
	
  			ratio = fd->ratio ;
  			if ( times2 )
  			{
  			    ratio <<= 1 ;
  			}
  			value *= ratio ;
				if ( fd->type == 3/*A*/)
  			{
  			    value /= 100 ;
  			    att = PREC1 ;
  			}
  			else if ( ratio < 100 )
  			{
  			    value *= 2 ;
  			    value /= 51 ;  // Same as *10 /255 but without overflow
  			    att = PREC2 ;
  			}
  			else
  			{
  			    value /= 255 ;
  			}

    	  if ( (fd->type == 0/*v*/) || (fd->type == 2/*v*/) )
    	  {
 			    att = PREC1 ;
					unit = 'v' ;
    	  }
    	  else
    	  {
			    if (fd->type == 3/*A*/)
					{
						unit = 'A' ;
					}
    	  }
				val = value ;
    	}
    break ;

    case 9:
    case 10:
			unit = 'C' ;
  		if ( model->FrSkyImperial )
  		{
  		  val += 18 ;
  		  val *= 115 ;
  		  val >>= 6 ;
  		  unit = 'F' ;
  		}
    break;
    
		case 6:
      unit = 'm' ;
			if (model->FrSkyUsrProto == 1)  // WS How High
			{
      	if ( model->FrSkyImperial )
        	unit = 'f' ;
				break ;
			}
    case 7:
      unit = 'm' ;
      if ( model->FrSkyImperial )
      {
        // m to ft *105/32
        val *= 105 ;
				val /= 32 ;
        unit = 'f' ;
      }
    break;
		
		case 17 :
			att |= PREC1 ;
      unit = 'A' ;
		break ;

		case 15:
			att |= PREC2 ;
      unit = 'v' ;
		break ;
		case 19 :
		case 20 :
			att |= PREC1 ;
      unit = 'v' ;
		break ;
		case 16:
			att |= PREC1 ;
      unit = 'v' ;
		break ;
    default:
    break;
  }
	if ( !displayed )
	{
		int16_t rem ;

		switch( att )
		{
			case PREC1 :
				rem = val % 10 ;
				val /= 10 ;
				sprintf( string, "%d.%01d %c", val, rem, unit ) ;
			break ;
			case PREC2 :
				rem = val % 100 ;
				val /= 100 ;
				sprintf( string, "%d.%02d %c", val, rem, unit ) ;
			break ;
			default :
				sprintf( string, "%d %c", val, unit ) ;
			break ;
		}
	}
}



void populateTelItemsCB(QComboBox *b, int value=0)
{
    b->clear();
    for(int i=1; i<=30; i++)
        b->addItem(TelemItems[i]);
    b->setCurrentIndex(value);
    b->setMaxVisibleItems(26);
}


void populateAlarmCB(QComboBox *b, int value=0)
{
    b->clear();
    for(int i=0; i<=15; i++)
        b->addItem(AudioAlarms[i]);
    b->setCurrentIndex(value);
    b->setMaxVisibleItems(16);
}

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

void populateSwitchShortCB(QComboBox *b, int value)
{
    b->clear();
    for(int i=-MAX_DRSWITCH+1; i<MAX_DRSWITCH; i++)
        b->addItem(getSWName(i));
    b->setCurrentIndex(value+MAX_DRSWITCH-1);
    b->setMaxVisibleItems(10);
}

void populatePhasetrim(QComboBox *b, int which, int value)
{	// which i s0 for FP1, 1 for FP2 etc.
	char name[4] ;
	name[0] = 'F' ;
	name[1] = 'P' ;
	name[3] = 0 ;
	
	b->clear();
	if ( value > TRIM_EXTENDED_MAX )
	{
		value -= TRIM_EXTENDED_MAX ;
	}
	else
	{
		value = 0 ;
	}
 	b->addItem( "Own trim" ) ;
 	for( int i= 0 ; i < 7 ; i += 1 )
	{
		if ( i == which )
		{
			continue ;			
		}
		name[2] = i + '0' ;
   	b->addItem( name ) ;
	}
	b->setCurrentIndex(value);
	b->setMaxVisibleItems( 7 ) ;
}

int decodePhaseTrim( int16_t *existing, int which, int index )
{
	if ( index == 0 )
	{
		if ( *existing > TRIM_EXTENDED_MAX )
		{
			*existing = 0 ;			
		}
		return -1 ;
	}
	*existing = TRIM_EXTENDED_MAX + index ;
	return index ;
}

void populateSwitchAndCB(QComboBox *b, int value=0)
{
	char name[4] ;
	name[0] = 'C' ;
	name[1] = 'S' ;
	name[3] = 0 ;
    b->clear();
    for(int i=0 ; i<=8; i++)
        b->addItem(getSWName(i));

    for(int i='0'; i<='9'; i++)
		{
			name[2] = i ;
      b->addItem(name);
		}
    for(int i='A' ; i<= 'O' ; i++)
		{
			name[2] = i ;
      b->addItem(name);
		}
    b->setCurrentIndex(value);
    b->setMaxVisibleItems(10);
}

void populateSafetySwitchCB(QComboBox *b, int type, int value=0)
{
    b->clear();
    for(int i=-MAX_DRSWITCH; i<=MAX_DRSWITCH; i++)
        b->addItem(getSWName(i));
    b->setCurrentIndex(value+MAX_DRSWITCH);
    b->setMaxVisibleItems(10);
		if (type == 2 )
		{
      	b->addItem(" 8 seconds");
      	b->addItem("12 seconds");
      	b->addItem("16 seconds");
		}
}


QString SafetyType[] = {"S","A","V"};
QString VoiceType[] = {"ON", "OFF", "BOTH", "15Secs", "30Secs", "60Secs", "Varibl"} ;

void populateSafetyVoiceTypeCB(QComboBox *b, int type, int value=0)
{
    b->clear();
		if ( type == 0 )
		{
    	for(int i= 0 ; i<=2; i++)
        b->addItem(SafetyType[i]);
    	b->setCurrentIndex(value);
    	b->setMaxVisibleItems(3);
		}
		else
		{
    	for(int i= 0 ; i<=6; i++)
        b->addItem(VoiceType[i]);
    	b->setCurrentIndex(value);
    	b->setMaxVisibleItems(7);
		}
}

void populateTmrBSwitchCB(QComboBox *b, int value=0)
{
	int i ;
    b->clear();

    for( i=-(MAX_DRSWITCH-1); i<MAX_DRSWITCH; i++)
        b->addItem(getSWName(i));
    for( i=1 ; i<MAX_DRSWITCH; i++)
        b->addItem('m'+getSWName(i));
    b->setCurrentIndex(value+MAX_DRSWITCH-1);
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
    for(int i=0 ; i<TMR_NUM_OPTION; i++)
        b->addItem(getTimerMode(i));
    b->setCurrentIndex(value);
    b->setMaxVisibleItems(10);
}

QString getTimerMode(int tm)
{

    QString str = CURV_STR;
    QString stt = "OFFABSTHsTH%";

    QString s;
    if(tm<TMR_VAROFS)
    {
        s = stt.mid(abs(tm)*3,3);
//        if(tm<-1) s.prepend("!");
        return s;
    }
		tm -= TMR_VAROFS ;

		if ( tm < 9 )
		{
       s = str.mid(tm*3+21,2) ;
		}
		else
		{
       s = str.mid(tm*3+21,3) ;
		}
     return s +'%' ;


//    s = "m" + str.mid((abs(tm)-(TMR_VAROFS+MAX_DRSWITCH-1))*3,3);
//    if(tm<0) s.prepend("!");
//    return s;
}




#define MODI_STR  "RUD ELE THR AIL RUD THR ELE AIL AIL ELE THR RUD AIL THR ELE RUD "
#define SRCP_STR  "P1  P2  P3  HALFFULLCYC1CYC2CYC3PPM1PPM2PPM3PPM4PPM5PPM6PPM7PPM8CH1 CH2 CH3 CH4 CH5 CH6 CH7 CH8 CH9 CH10CH11CH12CH13CH14CH15CH16CH17CH18CH19CH20CH21CH22CH23CH243POSGV1 GV2 GV3 GV4 GV5 "

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

void populateSourceCB(QComboBox *b, int stickMode, int telem, int value)
{
    b->clear();
    for(int i=0; i<45; i++) b->addItem(getSourceStr(stickMode,i));
		if ( telem )
		{
    	for(int i=1; i<=24; i++)
    	    b->addItem(TelemItems[i]);
		}
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


bool getSplashHEX(QString fileName, uchar * b, QWidget *parent)
{
    quint8 temp[HEX_FILE_SIZE] = {0};

    if(!loadiHEX(parent, fileName, (quint8*)&temp, HEX_FILE_SIZE, ""))
        return false;

    QByteArray rawData = QByteArray::fromRawData((const char *)&temp, HEX_FILE_SIZE);
    QString mark;
    mark.clear();
    mark.append(SPLASH_MARKER);
    mark.append('\0');
    int pos = rawData.indexOf(mark);

    if(pos<0)
        return false;

    memcpy(b, (const uchar *)&temp[pos + SPLASH_OFFSET], SPLASH_SIZE);
    return true;
}

bool putSplashHEX(QString fileName, uchar * b, QWidget *parent)
{
    quint8 temp[HEX_FILE_SIZE] = {0};
    int fileSize = loadiHEX(parent, fileName, (quint8*)&temp, HEX_FILE_SIZE, "");

    if(!fileSize)
        return false;

    QByteArray rawData = QByteArray::fromRawData((const char *)&temp, HEX_FILE_SIZE);
    QString mark;
    mark.clear();
    mark.append(SPLASH_MARKER);
    mark.append('\0');
    int pos = rawData.indexOf(QString(mark));

    if(pos<0)
        return false;

    memcpy((uchar *)&temp[pos + SPLASH_OFFSET], b, SPLASH_SIZE);

    if(!saveiHEX(parent, fileName, (quint8*)&temp, fileSize, "", 0))
        return false;

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

void appendNumberElement(QDomDocument * qdoc, QDomElement * pe,QString name, int value, bool forceZeroWrite)
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



QDomElement getModelDataXML(QDomDocument * qdoc, SKYModelData * tmod, int modelNum, int mdver)
{
    QDomElement md = qdoc->createElement("MODEL_DATA");
    md.setAttribute("number", modelNum);

    appendNumberElement(qdoc, &md, "Version", mdver, true); // have to write value here
    appendTextElement(qdoc, &md, "Name", QString::fromAscii(tmod->name,sizeof(tmod->name)).trimmed());
    appendCDATAElement(qdoc, &md, "Data", (const char *)tmod,sizeof(SKYModelData));
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
            QString ds = n.toCDATASection().data();
            QByteArray ba = QByteArray::fromBase64(ds.toAscii());
            const char * data = ba.data();
            memcpy(tgen, data, sizeof(EEGeneral));
            break;
        }
        n = n.nextSibling();
    }

    //check version?

    return true;
}

bool loadModelDataXML(QDomDocument * qdoc, SKYModelData * tmod, int modelNum)
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
            QString ds = n.toCDATASection().data();
            QByteArray ba = QByteArray::fromBase64(ds.toAscii());
            const char * data = ba.data();
            memcpy(tmod, data, sizeof(SKYModelData));
            break;
        }
        n = n.nextSibling();
    }

    //check version?

    return true;
}

