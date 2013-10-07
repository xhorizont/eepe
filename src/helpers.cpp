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
	"Tim1",	// 4
	"Tim2",
	"Alt ",
	"Galt",
	"Gspd", // 8
	"T1= ",
	"T2= ",
	"RPM ",
	"FUEL", // 12
	"Mah1",
	"Mah2",
	"Cvlt",
	"Batt", // 16
	"Amps",
	"Mah ",
	"Ctot",
	"FasV",	// 20
	"AccX",
	"AccY",
	"AccZ",
	"Vspd", // 24
	"Gvr1",
	"Gvr2",
	"Gvr3",
	"Gvr4", // 28
	"Gvr5",
	"Gvr6",
	"Gvr7",
	"Fwat"  // 32
} ;
#define NUM_TELEM_ITEMS	33

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
	"P3 ", //12
	"C1 ",
	"C2 ",
	"C3 ",
	"C4 ",
	"C5 ",
	"C6 ",
	"C7 ",
	"C8 ",
	"C9 ",
	"C10",
	"C11",
	"C12",
	"C13",
	"C14",
	"C15",
	"C16",
	"C17",
	"C18",
	"C19",
	"C20",
	"C21",
	"C22",
	"C23",
	"C24"
} ;

QString AnaVolumeItems[] = {
	"---",
	"P1 ",
	"P2 ",
	"P3 ",
	"GV4",
	"GV5",
	"GV6",
	"GV7"
} ;

void populateAnaVolumeCB( QComboBox *b, int value )
{
  b->clear();
  for(int i=0; i<8; i++)
	{
    b->addItem(AnaVolumeItems[i]);
	}
  b->setCurrentIndex(value);
  b->setMaxVisibleItems(8);
}

void populateGvarCB(QComboBox *b, int value)
{
    b->clear();
#ifdef SKY
    for(int i=0; i<=36; i++)
#else
    for(int i=0; i<=28; i++)
#endif
        b->addItem(GvarItems[i]);
    b->setCurrentIndex(value);
    b->setMaxVisibleItems(13);
}

int numericSpinGvarValue( QSpinBox *sb, QComboBox *cb, QCheckBox *ck, int value, int defvar )
{
	if ( ( value < -125 ) || ( value > 125) )
	{
		// Was a GVAR
		if ( ck->checkState() )
		{ // stil is
			value = cb->currentIndex() ;
			value += 126 ;
			if ( value > 127 )
			{
        value -= 256 ;
			}
		}
		else
		{
			// Now isn't
			value = defvar ;		// Default value
			sb->setValue( value ) ;
			sb->setVisible( true ) ;
			cb->setVisible( false ) ;
		}
	}
	else
	{ // Not a GVAR
		if ( ck->checkState() )
		{ // Now is a GVAR
			value = 126 ;
			cb->setCurrentIndex( 0 ) ;
			cb->setVisible( true ) ;
			sb->setVisible( false ) ;
		}
		else
		{ // Still isn't a GVAR
			value = sb->value() ;
		}
	}
	return value ;		 
}

void populateSpinGVarCB( QSpinBox *sb, QComboBox *cb, QCheckBox *ck, int value, int min, int max )
{
  cb->clear() ;
  for (int i=1; i<=5; i++)
	{
    cb->addItem(QObject::tr("GV%1").arg(i));
  }
	sb->setMinimum( min ) ;
	sb->setMaximum( max ) ;

	if ( ( value < -125 ) || ( value > 125) )
	{
		// A GVAR
		if ( value < 0 )
		{
			value += 128+2 ;			
		}
		else
		{
			value -= 126 ;
		}
		// value is now 0-4 for GVAR 1-5
		ck->setChecked( true ) ;
		cb->setCurrentIndex(value) ;
		cb->setVisible( true ) ;
		sb->setVisible( false ) ;
	}
	else
	{
		ck->setChecked( false ) ;
		sb->setValue( value ) ;
		sb->setVisible( true ) ;
		cb->setVisible( false ) ;
	}
}



void populateNumericGVarCB( QComboBox *b, int value, int min, int max)
{
  b->clear();
  
	if ( ( value < -125 ) || ( value > 125) )
	{
		// A GVAR
		if ( value < 0 )
		{
			value += 128+2 ;			
		}
		else
		{
			value -= 126 ;
		}
		// value is now 0-4 for GVAR 1-5
		value += max+1 ;		
	}
	value -= min ;

	for (int i=min; i<=max; i++)
	{
    b->addItem(QString::number(i, 10), i);
//    if (value == i)
//      b->setCurrentIndex(b->count()-1);
  }
  for (int i=1; i<=5; i++)
	{
//    int16_t gval = (int16_t)(10000+i);
    b->addItem(QObject::tr("GV%1").arg(i));
//    if (value == gval)
//      b->setCurrentIndex(b->count()-1);
  }
   b->setCurrentIndex(value) ;
}

int numericGvarValue( QComboBox *b, int min, int max )
{
	int value ;

	value = b->currentIndex() ;
	value += min ;
	if ( value > max )
	{
		// A GVAR
		value -= (max+1) ;	// value is now 0-4 for GVAR 1-5 (126,127,-128,-127,-126
    if ( value <= 2 )
		{
			value += 126 ;
		}
		else
		{
			value -= 128+2 ;
		}
	}
	return value ;	
}

int16_t m_to_ft( int16_t metres )
{
	int16_t result ;

  // m to ft *105/32
	result = metres * 3 ;
	metres >>= 2 ;
	result += metres ;
	metres >>= 2 ;
  return result + (metres >> 1 );
}


//	"A1= ",
//	"A2= ",
//	"RSSI",
//	"TSSI",
#define TIMER1		4
#define TIMER2		5
#define FR_ALT_BARO 6
#define FR_GPS_ALT 7
//	"Galt",
//	"Gspd", // 8
//	"T1= ",
//	"T2= ",
//	"RPM ",
//	"FUEL", // 12
//	"Mah1",
//	"Mah2",
//	"Cvlt",
//	"Batt", // 16
//	"Amps",
//	"Mah ",
//	"Ctot",
//	"FasV",	// 20
//	"AccX",
//	"AccY",
//	"AccZ",
#define FR_VSPD	24
//	"Gvr1",
//	"Gvr2",
//	"Gvr3",
//	"Gvr4", // 28
//	"Gvr5",
//	"Gvr6",
//	"Gvr7",
#define FR_WATT	32


// This routine converts an 8 bit value for custom switch use
#ifdef SKY
int16_t convertTelemConstant( int8_t index, int8_t value, SKYModelData *model )
#else
int16_t convertTelemConstant( int8_t index, int8_t value, ModelData *model )
#endif
{
  int16_t result;

	result = value + 125 ;
  switch (index)
	{
    case TIMER1 :	// Timer1
    case TIMER2 :	// Timer2
      result *= 10 ;
    break;
		case FR_ALT_BARO:
    case FR_GPS_ALT:
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
			result *= 10 ;		// Allow for decimal place
      if ( model->FrSkyImperial )
      {
        // m to ft *105/32
        result = m_to_ft( result ) ;
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
    case 32:
      result *= 8 ;
    break;
		case FR_VSPD :
			result = value * 10 ;
		break ;
  }
  return result;
}

#define PREC1		1
#define PREC2		2

#ifdef SKY
void stringTelemetryChannel( char *string, int8_t index, int16_t val, SKYModelData *model )
#else
void stringTelemetryChannel( char *string, int8_t index, int16_t val, ModelData *model )
#endif
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
#ifdef SKY
        SKYFrSkyChannelData *fd ;
#else
			  FrSkyChannelData *fd ;
#endif

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
		case FR_WATT :
      unit = 'w' ;
		break ;
		case FR_VSPD :
			att |= PREC1 ;
			val /= 10 ;
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



void populateTelItemsCB(QComboBox *b, int start, int value)
{
    b->clear();
		if ( start )
		{
			start = 1 ;			
		}
    for(int i=start; i<=NUM_TELEM_ITEMS; i++)
        b->addItem(TelemItems[i]);
    b->setCurrentIndex(value);
    b->setMaxVisibleItems(30);
}


void populateAlarmCB(QComboBox *b, int value=0)
{
    b->clear();
    for(int i=0; i<=15; i++)
        b->addItem(AudioAlarms[i]);
    b->setCurrentIndex(value);
    b->setMaxVisibleItems(16);
}


#ifdef SKY
QString getSWName(int val)
#else
QString getSWName(int val, int extra )
#endif
{
	int limit = MAX_DRSWITCH ;
#ifndef SKY
	if ( extra )
	{
		limit += EXTRA_CSW ;
	}
#endif
    if(!val) return "---";
    if(val==limit) return "ON";
    if(val==-limit) return "OFF";

    return QString(val<0 ? "!" : "") + QString(SWITCHES_STR).mid((abs(val)-1)*3,3);
}

#ifdef SKY
void populateSwitchCB(QComboBox *b, int value=0)
#else
void populateSwitchCB(QComboBox *b, int value, int modelType)
#endif
{
    b->clear();
	int limit = MAX_DRSWITCH ;
#ifndef SKY
  if ( modelType )
	{
    limit += EXTRA_CSW ;
	}
#endif
    for(int i=-limit; i<=limit; i++)
#ifdef SKY
        b->addItem(getSWName(i));
#else
        b->addItem(getSWName(i,modelType));
#endif
    b->setCurrentIndex(value+limit);
    b->setMaxVisibleItems(10);
}

void populateTrainerSwitchCB(QComboBox *b, int value=0)
{
    b->clear();
    for(int i=-15; i<=15; i++)
#ifdef SKY
        b->addItem(getSWName(i));
#else
        b->addItem(getSWName(i,0));
#endif
    b->setCurrentIndex(value+15);
    b->setMaxVisibleItems(10);
}


#ifdef SKY
void populateSwitchShortCB(QComboBox *b, int value)
#else
void populateSwitchShortCB(QComboBox *b, int value, int modelType)
#endif
{
    b->clear();
	int limit = MAX_DRSWITCH-1 ;
#ifndef SKY
  if ( modelType )
	{
    limit += EXTRA_CSW ;
	}
#endif
    for(int i = -limit; i<=limit; i++)
#ifdef SKY
        b->addItem(getSWName(i));
#else
        b->addItem(getSWName(i,modelType));
#endif
    b->setCurrentIndex(value+limit);
    b->setMaxVisibleItems(10);
}

void populatePhasetrim(QComboBox *b, int which, int value)
{	// which i s0 for FP1, 1 for FP2 etc.
	char name[4] ;
	name[0] = 'F' ;
	name[1] = 'M' ;
	name[3] = 0 ;
	
	b->clear();
#ifdef SKY
#else
		value += TRIM_EXTENDED_MAX+1 ;
#endif
	if ( value > TRIM_EXTENDED_MAX )
	{
		value -= TRIM_EXTENDED_MAX ;
	}
	else
	{
		value = 0 ;
	}
 	b->addItem( "Own trim" ) ;
#ifdef SKY
 	for( int i= 0 ; i < 7 ; i += 1 )
#else
 	for( int i= 0 ; i < 5 ; i += 1 )
#endif
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
#ifdef SKY
		if ( *existing > TRIM_EXTENDED_MAX )
		{
			*existing = 0 ;			
		}
#else
		if ( *existing >= 0 )
		{
			*existing = -(TRIM_EXTENDED_MAX+1) ;
		}
#endif
		return -1 ;
	}
#ifdef SKY
	*existing = TRIM_EXTENDED_MAX + index ;
#else
	*existing = TRIM_EXTENDED_MAX + index - (TRIM_EXTENDED_MAX+1) ;
#endif
	return index ;
}

void populateSwitchxAndCB(QComboBox *b, int value=0)
{
	char name[6] ;
	name[0] = '!' ;
	name[1] = 'C' ;
	name[2] = 'S' ;
	name[4] = 0 ;
	
	b->clear();
  
  b->addItem("!TRN");
	for(int i='I' ; i>= 'A' ; i -= 1 )
	{
		name[3] = i ;
     b->addItem(name);
	}
  for(int i= '9' ; i>='1'; i -= 1 )
	{
		name[3] = i ;
    b->addItem(name);
	}

 	for(int i=-8 ; i<=8; i += 1)
#ifdef SKY
    b->addItem(getSWName(i));
#else
    b->addItem(getSWName(i,0));
#endif

	name[0] = 'C' ;
	name[1] = 'S' ;
	name[3] = 0 ;
  for(int i='1'; i<= '9'; i++)
	{
		name[2] = i ;
    b->addItem(name);
	}
  for(int i='A' ; i<= 'I' ; i++)
	{
		name[2] = i ;
    b->addItem(name);
	}
  b->addItem("TRN");
  b->setCurrentIndex(value+27);
  b->setMaxVisibleItems(10);
}



void populateSwitchAndCB(QComboBox *b, int value=0)
{
	char name[6] ;
	name[0] = '!' ;
	name[1] = 'C' ;
	name[2] = 'S' ;
	name[4] = 0 ;
	int maxsw ;

#ifdef SKY
	maxsw = '9' ;
#else
	maxsw = '7' ;
#endif
	
	b->clear();
  
#ifdef SKY
  b->addItem("!TRN");
	for(int i='O' ; i>= 'A' ; i -= 1 )
	{
		name[3] = i ;
     b->addItem(name);
	}
  for(int i= maxsw ; i>='1'; i -= 1 )
	{
		name[3] = i ;
    b->addItem(name);
	}
#endif
#ifdef SKY
 	for(int i=-8 ; i<=8; i += 1)
#else 	
	for(int i=0 ; i<=8; i += 1)
#endif
#ifdef SKY
    b->addItem(getSWName(i));
#else
    b->addItem(getSWName(i,0));
#endif

	name[0] = 'C' ;
	name[1] = 'S' ;
	name[3] = 0 ;
  for(int i='1'; i<= maxsw; i++)
	{
		name[2] = i ;
    b->addItem(name);
	}
#ifdef SKY
  for(int i='A' ; i<= 'O' ; i++)
	{
		name[2] = i ;
    b->addItem(name);
	}
  b->addItem("TRN");
#endif
  b->setCurrentIndex(value);
  b->setMaxVisibleItems(10);
}
//void populateSwitchAndCB(QComboBox *b, int value=0)
//{
//	char name[4] ;
//	name[0] = 'C' ;
//	name[1] = 'S' ;
//	name[3] = 0 ;
//    b->clear();
//    for(int i=0 ; i<=8; i++)
//        b->addItem(getSWName(i));

//    for(int i=1; i<=7; i++)
//		{
//			name[2] = i + '0' ;
//      b->addItem(name);
//		}
//    b->setCurrentIndex(value);
//    b->setMaxVisibleItems(10);
//}

#ifdef SKY
void populateSafetySwitchCB(QComboBox *b, int type, int value )
#else
void populateSafetySwitchCB(QComboBox *b, int type, int value, int extra )
#endif
{
	int offset = MAX_DRSWITCH ;
	int start = -MAX_DRSWITCH ;
	int last = MAX_DRSWITCH ;
	if ( type == VOICE_SWITCH )
	{
		offset = 0 ;
		start = 0 ;		
		last = MAX_DRSWITCH - 1 ;
#ifndef SKY
		if ( extra )
		{
			last += EXTRA_CSW ;
		}
#endif
	}
    b->clear();
    for(int i= start ; i<=last; i++)
#ifdef SKY
        b->addItem(getSWName(i),0);
#else
        b->addItem(getSWName(i,extra));
#endif
    b->setCurrentIndex(value+ offset ) ;
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

#ifdef SKY
void populateTmrBSwitchCB(QComboBox *b, int value)
#else
void populateTmrBSwitchCB(QComboBox *b, int value, int extra )
#endif
{
	int i ;
    b->clear();
	int limit = MAX_DRSWITCH-1 ;
#ifndef SKY
	if ( extra )
	{
		limit += EXTRA_CSW ;
	}
#endif
    for( i= -limit; i<= limit; i++)
#ifdef SKY
        b->addItem(getSWName(i));
#else
        b->addItem(getSWName(i,extra));
#endif
#ifdef SKY    
		for( i=1 ; i<MAX_DRSWITCH; i++)
        b->addItem('m'+getSWName(i),0);
#endif    
		b->setCurrentIndex(value+limit);
    b->setMaxVisibleItems(10);
}

void populateCurvesCB(QComboBox *b, int value)
{
    QString str = CURV_STR;
    b->clear();
    for(int i=(str.length()/3)-1; i >= 7 ; i -= 1)
		{
			b->addItem(str.mid(i*3,3).replace("c","!Curve "));
		}
    for(int i=0; i<(str.length()/3); i++)  b->addItem(str.mid(i*3,3).replace("c","Curve "));
#ifdef SKY    
		b->setCurrentIndex(value+24);
#else
    b->setCurrentIndex(value+16);
#endif
    b->setMaxVisibleItems(10);
}


#ifdef SKY
void populateTimerSwitchCB(QComboBox *b, int value=0 )
#else
void populateTimerSwitchCB(QComboBox *b, int value=0, int modelType=0)
#endif
{
    b->clear();
#ifdef SKY    
    for(int i=0 ; i<TMR_NUM_OPTION; i++)
        b->addItem(getTimerMode(i));
    b->setCurrentIndex(value);
#else
		int num_options = TMR_NUM_OPTION ;
    if ( modelType )
		{
			num_options += EXTRA_CSW * 2 ;
		}
    for(int i=-num_options; i<=num_options; i++)
        b->addItem(getTimerMode(i,modelType));
    b->setCurrentIndex(value+num_options);
#endif
    b->setMaxVisibleItems(10);
}

#ifdef SKY
QString getTimerMode(int tm)
#else
QString getTimerMode(int tm, int modelVersion )
#endif
{

#ifdef SKY    
    QString str = CURV_STR;
    QString stt = "OFFABSTHsTH%";
#else
    QString str = SWITCHES_STR;
    QString stt = "OFFABSRUsRU%ELsEL%THsTH%ALsAL%P1 P1%P2 P2%P3 P3%";
#endif

    QString s;
#ifdef SKY    
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
#else
    if(abs(tm)<TMR_VAROFS)
    {
        s = stt.mid(abs(tm)*3,3);
        if(tm<-1) s.prepend("!");
        return s;
    }

		int max_drswitch = MAX_DRSWITCH ;
		if ( modelVersion >= 2 )
		{
			max_drswitch += EXTRA_CSW ;			
		}

    if(abs(tm)<(TMR_VAROFS+max_drswitch-1))
    {
        s = str.mid((abs(tm)-TMR_VAROFS)*3,3);
        if(tm<0) s.prepend("!");
        return s;
    }


    s = "m" + str.mid((abs(tm)-(TMR_VAROFS+max_drswitch-1))*3,3);
    if(tm<0) s.prepend("!");
    return s;
#endif

}




#define MODI_STR  "RUD ELE THR AIL RUD THR ELE AIL AIL ELE THR RUD AIL THR ELE RUD "
#ifdef SKY    
#define SRCP_STR  "P1  P2  P3  HALFFULLCYC1CYC2CYC3PPM1PPM2PPM3PPM4PPM5PPM6PPM7PPM8CH1 CH2 CH3 CH4 CH5 CH6 CH7 CH8 CH9 CH10CH11CH12CH13CH14CH15CH16CH17CH18CH19CH20CH21CH22CH23CH243POSGV1 GV2 GV3 GV4 GV5 "
#else
#define SRCP_STR  "P1  P2  P3  HALFFULLCYC1CYC2CYC3PPM1PPM2PPM3PPM4PPM5PPM6PPM7PPM8CH1 CH2 CH3 CH4 CH5 CH6 CH7 CH8 CH9 CH10CH11CH12CH13CH14CH15CH163POSGV1 GV2 GV3 GV4 GV5 "
#endif

QString getSourceStr(int stickMode, int idx, int modelVersion )
{
    if(!idx)
        return "----";
    else if(idx>=1 && idx<=4)
    {
        QString modi = MODI_STR;
				if ( modelVersion >= 2 )
				{
        	return modi.mid( (idx-1) * 4, 4 ) ;
				}
        return modi.mid((idx-1)*4+stickMode*16,4);
    }
    else
    {
        QString str = SRCP_STR;
        return str.mid((idx-5)*4,4);
    }

    return "";
}

void populateSourceCB(QComboBox *b, int stickMode, int telem, int value, int modelVersion)
{
    b->clear();

		if ( modelVersion >= 2 )
		{
			stickMode = 0 ;
		}

#ifdef SKY    
    for(int i=0; i<45; i++) b->addItem(getSourceStr(stickMode,i));
#else
    for(int i=0; i<37; i++) b->addItem(getSourceStr(stickMode,i));
#endif
		if ( telem )
		{
    	for(int i=1; i<=NUM_TELEM_ITEMS; i++)
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

bool getSplashBIN(QString fileName, uchar * b, QWidget *parent)
{
    quint8 temp[BIN_FILE_SIZE] = {0};

    QFile file(fileName);

    if(!file.exists())
    {
        QMessageBox::critical(parent, QObject::tr("Error"),QObject::tr("Unable to find file %1!").arg(fileName));
        return 0;
    }

    if (!file.open(QIODevice::ReadOnly )) {  //reading file
        QMessageBox::critical(parent, QObject::tr("Error"),
                              QObject::tr("Error opening file %1:\n%2.")
                              .arg(fileName)
                              .arg(file.errorString()));
        return 0;
    }

    memset(temp,0,BIN_FILE_SIZE) ;

    long result = file.read((char*)&temp,file.size()) ;
    file.close();

    if (result!=file.size())
    {
      QMessageBox::critical(parent, QObject::tr("Error"),
                             QObject::tr("Error reading file %1:%2. %3 %4")
                             .arg(fileName)
                             .arg(file.errorString())
                             .arg(result)
                             .arg(file.size())										 
													 );

      return false;
    }

    QByteArray rawData = QByteArray::fromRawData((const char *)&temp, BIN_FILE_SIZE);
    QString mark;
    mark.clear();
    mark.append("SPS");
    mark.append('\0');
    int pos = rawData.indexOf(mark);

    if(pos<0)
        return false;

    memcpy(b, (const uchar *)&temp[pos + 7], SPLASH_SIZE);
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
    appendTextElement(qdoc, &gd, "Owner", QString::fromLatin1(tgen->ownerName,sizeof(tgen->ownerName)).trimmed());
    appendCDATAElement(qdoc, &gd, "Data", (const char *)tgen,sizeof(EEGeneral));
    return gd;

}


#ifdef SKY
QDomElement getModelDataXML(QDomDocument * qdoc, SKYModelData * tmod, int modelNum, int mdver)
#else
QDomElement getModelDataXML(QDomDocument * qdoc, ModelData * tmod, int modelNum, int mdver)
#endif
{
    QDomElement md = qdoc->createElement("MODEL_DATA");
    md.setAttribute("number", modelNum);

    appendNumberElement(qdoc, &md, "Version", mdver, true); // have to write value here
    appendTextElement(qdoc, &md, "Name", QString::fromLatin1(tmod->name,sizeof(tmod->name)).trimmed());
#ifdef SKY
    appendCDATAElement(qdoc, &md, "Data", (const char *)tmod,sizeof(SKYModelData));
#else
    appendCDATAElement(qdoc, &md, "Data", (const char *)tmod,sizeof(ModelData));
#endif
    return md;
}


bool loadGeneralDataXML(QDomDocument * qdoc, EEGeneral * tgen)
{
  memset( tgen, 0, sizeof( *tgen) ) ;
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
            QByteArray ba = QByteArray::fromBase64(ds.toLatin1());
            int size = ba.length() ;
            const char * data = ba.data();
            memcpy(tgen, data, size );
            break;
        }
        n = n.nextSibling();
    }

    //check version?

    return true;
}

#ifdef SKY
bool loadModelDataXML(QDomDocument * qdoc, SKYModelData * tmod, int modelNum)
#else
bool loadModelDataXML(QDomDocument * qdoc, ModelData * tmod, int modelNum)
#endif
{
  memset( tmod, 0, sizeof( *tmod) ) ;
	
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
            QByteArray ba = QByteArray::fromBase64(ds.toLatin1());
            int size = ba.length() ;
            const char * data = ba.data();
#ifdef SKY
            memcpy(tmod, data, size) ;
#else
            memcpy(tmod, data, size) ;
#endif
            break;
        }
        n = n.nextSibling();
    }

    //check version?

    return true;
}

//#ifdef SKY
//uint8_t CONVERT_MODE( uint8_t x )
//#else
uint8_t CONVERT_MODE( uint8_t x, int modelVersion, int stickMode )
//#endif
{
	if ( modelVersion >= 2 )
	{
		return x ;
	}
  return (((x)<=4) ? modn12x3[stickMode][((x)-1)] : (x)) ;
}


