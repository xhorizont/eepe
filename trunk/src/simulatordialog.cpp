#include "simulatordialog.h"
#include "ui_simulatordialog.h"
#include "node.h"
#include <QtGui>
#include <inttypes.h>
#include "pers.h"
#include "helpers.h"

#define GBALL_SIZE  20
#define GVARS	1

#define RESX    1024
#define RESXu   1024u
#define RESXul  1024ul
#define RESXl   1024l
#define RESKul  100ul
#define RESX_PLUS_TRIM (RESX+128)

#define IS_THROTTLE(x)  (((2-(g_eeGeneral.stickMode&1)) == x) && (x<4))
#define GET_DR_STATE(x) (!getSwitch(g_model.expoData[x].drSw1,0) ?   \
    DR_HIGH :                                  \
    !getSwitch(g_model.expoData[x].drSw2,0)?   \
    DR_MID : DR_LOW);

extern int GlobalModified ;
extern EEGeneral Sim_g ;
extern int GeneralDataValid ;
extern ModelData Sim_m ;
extern int ModelDataValid ;

simulatorDialog::simulatorDialog( QWidget *parent) :
    QDialog(parent),
    ui(new Ui::simulatorDialog)
{
    ui->setupUi(this);

		current_limits = 2 ;
    beepVal = 0;
    beepShow = 0;

    bpanaCenter = 0;
    g_tmr10ms = 0;
		one_sec_precount = 0 ;

    memset(&chanOut,0,sizeof(chanOut));
    memset(&calibratedStick,0,sizeof(calibratedStick));
    memset(&g_ppmIns,0,sizeof(g_ppmIns));
    memset(&ex_chans,0,sizeof(ex_chans));
    memset(&trim,0,sizeof(trim));

    memset(&sDelay,0,sizeof(sDelay));
    memset(&act,0,sizeof(act));

    memset(&anas,0,sizeof(anas));
    memset(&chans,0,sizeof(chans));

    memset(&swOn,0,sizeof(swOn));

    trimptr[0] = &trim[0] ;
    trimptr[1] = &trim[1] ;
    trimptr[2] = &trim[2] ;
    trimptr[3] = &trim[3] ;

    setupSticks();
		timer = 0 ;
//    setupTimer();
}

simulatorDialog::~simulatorDialog()
{
    delete ui;
}

void simulatorDialog::closeEvent ( )
{
    timer->stop();
    delete timer;
		timer = 0 ;
}

void simulatorDialog::setupTimer()
{
  if (timer == 0)
  {
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(timerEvent()));
  }
  getValues();
  perOut(true);
  timer->start(10);
}

void simulatorDialog::timerEvent()
{
		uint8_t i ;
    g_tmr10ms++;

		if ( GlobalModified )
		{
			if ( GeneralDataValid )
			{
	    	memcpy(&g_eeGeneral,&Sim_g,sizeof(EEGeneral));
				GeneralDataValid = 0 ;
			}
			if ( ModelDataValid )
			{
  	  	memcpy(&g_model,&Sim_m,sizeof(ModelData));
				ModelDataValid = 0 ;
			}
    	
			char buf[sizeof(g_model.name)+1];
    	memcpy(&buf,&g_model.name,sizeof(g_model.name));
    	buf[sizeof(g_model.name)] = 0;
    	modelName = tr("Simulating ") + QString(buf);
    	setWindowTitle(modelName);

    	if(g_eeGeneral.stickMode & 1)
    	{
    		  nodeLeft->setCenteringY(false);   //mode 1,3 -> THR on left
    		  ui->holdLeftY->setChecked(true);
    	}
    	else
    	{
    		  nodeRight->setCenteringY(false);   //mode 1,3 -> THR on right
    		  ui->holdRightY->setChecked(true);
    	}

			CurrentPhase = getFlightPhase() ;

    	ui->trimHLeft->setValue( g_model.trim[(g_eeGeneral.stickMode>2)   ? 3 : 0]);  // mode=(0 || 1) -> rud trim else -> ail trim
    	ui->trimVLeft->setValue( g_model.trim[(g_eeGeneral.stickMode & 1) ? 1 : 2]);  // mode=(0 || 2) -> thr trim else -> ele trim
    	ui->trimVRight->setValue(g_model.trim[(g_eeGeneral.stickMode & 1) ? 2 : 1]);  // mode=(0 || 2) -> ele trim else -> thr trim
    	ui->trimHRight->setValue(g_model.trim[(g_eeGeneral.stickMode>2)   ? 0 : 3]);  // mode=(0 || 1) -> ail trim else -> rud trim
			GlobalModified = 0 ;
		}

    getValues();

    perOut();

    setValues();
    centerSticks();

    timerTick();
    //    if(s_timerState != TMR_OFF)
    setWindowTitle(modelName + QString(" - Timer: (%3, %4) %1:%2")
                   .arg(abs(-s_timerVal)/60, 2, 10, QChar('0'))
                   .arg(abs(-s_timerVal)%60, 2, 10, QChar('0'))
                   .arg(getTimerMode(g_model.tmrMode, g_model.modelVersion))
                   .arg(g_model.tmrDir ? "Count Up" : "Count Down"));

    if(beepVal)
    {
        beepVal = 0;
        QApplication::beep();
    }


#define CBEEP_ON  "QLabel { background-color: #FF364E }"
#define CBEEP_OFF "QLabel { }"

    ui->label_beep->setStyleSheet(beepShow ? CBEEP_ON : CBEEP_OFF);
    if(beepShow) beepShow--;


		if ( ++one_sec_precount >= 10 )
		{
			one_sec_precount -= 10 ;
			// 0.1 second has elapsed			
			for ( i = 0 ; i < NUM_CSW ; i += 1 )
			{
    		CSwData &cs = g_model.customSw[i];
    		uint8_t cstate = CS_STATE(cs.func);

    		if(cstate == CS_TIMER)
				{
					int16_t y ;
					y = CsTimer[i] ;
					if ( y == 0 )
					{
						int8_t z ;
						z = cs.v1 ;
						if ( z >= 0 )
						{
							z = -z-1 ;
							y = z * 10 ;					
						}
						else
						{
							y = z ;
						}
					}
					else if ( y < 0 )
					{
						if ( ++y == 0 )
						{
							int8_t z ;
							z = cs.v2 ;
							if ( z >= 0 )
							{
								y = z * 10 ;
							}
							else
							{
								y = -z-1 ;
							}
						}
					}
					else  // if ( CsTimer[i] > 0 )
					{
						y -= 1 ;
					}
					if ( cs.andsw )
					{
						int8_t x ;
						x = cs.andsw ;
						if ( x > 8 )
						{
							x += 1 ;
						}
	      	  if (getSwitch( x, 0, 0) == 0 )
					  {
							y = -1 ;
						}	
					}
					CsTimer[i] = y ;
				}
			}
		}

		if ( ee_type )
		{
			
			for ( i = NUM_CSW ; i < NUM_CSW+EXTRA_CSW ; i += 1 )
			{
    		CxSwData *cs = &g_model.xcustomSw[i-NUM_CSW];
    	
				uint8_t cstate = CS_STATE(cs->func);

    		if(cstate == CS_TIMER)
				{
					int16_t y ;
					y = CsTimer[i] ;
					if ( y == 0 )
					{
						int8_t z ;
						z = cs->v1 ;
						if ( z >= 0 )
						{
							z = -z-1 ;
							y = z * 10 ;					
						}
						else
						{
							y = z ;
						}
					}
					else if ( y < 0 )
					{
						if ( ++y == 0 )
						{
							int8_t z ;
							z = cs->v2 ;
							if ( z >= 0 )
							{
								z += 1 ;
								y = z * 10 - 1  ;
							}
							else
							{
								y = -z-1 ;
							}
						}
					}
					else  // if ( CsTimer[i] > 0 )
					{
						y -= 1 ;
					}
					if ( cs->andsw )
					{
						int8_t x ;
						x = cs->andsw ;
						if ( x > 8 )
						{
							x += 1 ;
						}
						if ( x < -8 )
						{
							x -= 1 ;
						}
						if ( x > 9+NUM_CSW+EXTRA_CSW )
						{
							x = 9 ;			// Tag TRN on the end, keep EEPROM values
						}
						if ( x < -(9+NUM_CSW+EXTRA_CSW) )
						{
							x = -9 ;			// Tag TRN on the end, keep EEPROM values
						}
	  	      if (getSwitch( x, 0, 0) == 0 )
					  {
							y = -1 ;
						}	
					}
					CsTimer[i] = y ;
				}
			}
		} 

}

void simulatorDialog::centerSticks()
{
    if(ui->leftStick->scene()) nodeLeft->stepToCenter();
    if(ui->rightStick->scene()) nodeRight->stepToCenter();
}

void simulatorDialog::setType( uint8_t type )
{
	ee_type = type ;
}

void simulatorDialog::loadParams(const EEGeneral gg, const ModelData gm)
{
    memcpy(&g_eeGeneral,&gg,sizeof(EEGeneral));
    memcpy(&g_model,&gm,sizeof(ModelData));

    char buf[sizeof(g_model.name)+1];
    memcpy(&buf,&g_model.name,sizeof(g_model.name));
    buf[sizeof(g_model.name)] = 0;
    modelName = tr("Simulating ") + QString(buf);
    setWindowTitle(modelName);

    if(g_eeGeneral.stickMode & 1)
    {
        nodeLeft->setCenteringY(false);   //mode 1,3 -> THR on left
        ui->holdLeftY->setChecked(true);
    }
    else
    {
        nodeRight->setCenteringY(false);   //mode 1,3 -> THR on right
        ui->holdRightY->setChecked(true);
    }

		CurrentPhase = getFlightPhase() ;

    ui->trimHLeft->setValue( g_model.trim[(g_eeGeneral.stickMode>2)   ? 3 : 0]);  // mode=(0 || 1) -> rud trim else -> ail trim
    ui->trimVLeft->setValue( g_model.trim[(g_eeGeneral.stickMode & 1) ? 1 : 2]);  // mode=(0 || 2) -> thr trim else -> ele trim
    ui->trimVRight->setValue(g_model.trim[(g_eeGeneral.stickMode & 1) ? 2 : 1]);  // mode=(0 || 2) -> ele trim else -> thr trim
    ui->trimHRight->setValue(g_model.trim[(g_eeGeneral.stickMode>2)   ? 0 : 3]);  // mode=(0 || 1) -> ail trim else -> rud trim

    beepVal = 0;
    beepShow = 0;
    bpanaCenter = 0;
    g_tmr10ms = 0;

    s_timeCumTot = 0;
    s_timeCumAbs = 0;
    s_timeCumSw = 0;
    s_timeCumThr = 0;
    s_timeCum16ThrP = 0;
    s_timerState = 0;
    beepAgain = 0;
    g_LightOffCounter = 0;
    s_timerVal = 0;
    s_time = 0;
    s_cnt = 0;
    s_sum = 0;
    sw_toggled = 0;

    setupTimer();
		GlobalModified = 0 ;
}


uint32_t simulatorDialog::getFlightPhase()
{
	uint32_t i ;
  for ( i = 0 ; i < MAX_PHASES ; i += 1 )
	{
    PhaseData *phase = &g_model.phaseData[i];
    if ( phase->swtch && getSwitch( phase->swtch, 0 ) )
		{
      return i + 1 ;
    }
  }
  return 0 ;
}

int16_t simulatorDialog::getRawTrimValue( uint8_t phase, uint8_t idx )
{
	if ( phase )
	{
		return g_model.phaseData[phase-1].trim[idx] + TRIM_EXTENDED_MAX + 1 ;
	}	
	else
	{
		return *trimptr[idx] ;
	}
}

uint32_t simulatorDialog::getTrimFlightPhase( uint8_t phase, uint8_t idx )
{
  for ( uint32_t i=0 ; i<MAX_PHASES ; i += 1 )
	{
    if (phase == 0) return 0;
    int16_t trim = getRawTrimValue( phase, idx ) ;
    if ( trim <= TRIM_EXTENDED_MAX )
		{
			return phase ;
		}
    uint32_t result = trim-TRIM_EXTENDED_MAX-1 ;
    if (result >= phase)
		{
			result += 1 ;
		}
    phase = result;
  }
  return 0;
}


int16_t simulatorDialog::getTrimValue( uint8_t phase, uint8_t idx )
{
  return getRawTrimValue( getTrimFlightPhase( phase, idx ), idx ) ;
}


void simulatorDialog::setTrimValue(uint8_t phase, uint8_t idx, int16_t trim)
{
	if ( phase )
	{
		phase = getTrimFlightPhase( phase, idx ) ;
	}
	if ( phase )
	{
  	g_model.phaseData[phase-1].trim[idx] = trim - ( TRIM_EXTENDED_MAX + 1 ) ;
	}
	else
	{
    if(trim < -125 || trim > 125)
		{
			trim = ( trim > 0 ) ? 125 : -125 ;
		}	
   	*trimptr[idx] = trim ;
	}
}

uint32_t simulatorDialog::adjustMode( uint32_t x )
{
	switch (g_eeGeneral.stickMode )
	{
		case 1 :
			if ( x == 2 )
			{
				x = 1 ;
			}
			else if ( x == 1 )
			{
				x = 2 ;
			}
		break ;
		case 2 :
			if ( x == 3 )
			{
				x = 0 ;
			}
			else if ( x == 0 )
			{
				x = 3 ;
			}
		break ;
		case 3 :
			x = 3 - x ;
		break ;
	}
	return x ;
}

const uint8_t stickScramble[] =
{
    0, 1, 2, 3,
    0, 2, 1, 3,
    3, 1, 2, 0,
    3, 2, 1, 0 } ;

void simulatorDialog::getValues()
{
		int8_t trims[4] ;

  calibratedStick[0] = 1024*nodeLeft->getX(); //RUD
  calibratedStick[1] = -1024*nodeLeft->getY(); //ELE
  calibratedStick[2] = -1024*nodeRight->getY(); //THR
  calibratedStick[3] = 1024*nodeRight->getX(); //AIL
  if ( g_model.modelVersion >= 2 )
	{
		uint8_t stickIndex = g_eeGeneral.stickMode*4 ;
		
//    calibratedStick[stickScramble[stickIndex+0]] = 1024*nodeLeft->getX(); //RUD
//    calibratedStick[stickScramble[stickIndex+1]] = -1024*nodeLeft->getY(); //ELE
//    calibratedStick[stickScramble[stickIndex+2]] = -1024*nodeRight->getY(); //THR
//    calibratedStick[stickScramble[stickIndex+3]] = 1024*nodeRight->getX(); //AIL
    
		uint8_t index ;
		index =g_eeGeneral.crosstrim ? 3 : 0 ;
		index =  stickScramble[stickIndex+index] ;
		trims[index] = ui->trimHLeft->value();
		index =g_eeGeneral.crosstrim ? 2 : 1 ;
		index =  stickScramble[stickIndex+index] ;
		trims[index] = ui->trimVLeft->value();
		index =g_eeGeneral.crosstrim ? 1 : 2 ;
		index =  stickScramble[stickIndex+index] ;
		trims[index] = ui->trimVRight->value();
		index =g_eeGeneral.crosstrim ? 0 : 3 ;
		index =  stickScramble[stickIndex+index] ;
		trims[index] = ui->trimHRight->value();
	}
	else
	{
//    calibratedStick[0] = 1024*nodeLeft->getX(); //RUD
//    calibratedStick[1] = -1024*nodeLeft->getY(); //ELE
//    calibratedStick[2] = -1024*nodeRight->getY(); //THR
//    calibratedStick[3] = 1024*nodeRight->getX(); //AIL
    trims[g_eeGeneral.crosstrim ? 3 : 0] = ui->trimHLeft->value();
    trims[g_eeGeneral.crosstrim ? 2 : 1] = ui->trimVLeft->value();
    trims[g_eeGeneral.crosstrim ? 1 : 2] = ui->trimVRight->value();
    trims[g_eeGeneral.crosstrim ? 0 : 3] = ui->trimHRight->value();
	}
		uint32_t phase ;
		
		phase = getTrimFlightPhase( CurrentPhase, 0 ) ;
    setTrimValue( phase, 0, trims[0] ) ;
		phase = getTrimFlightPhase( CurrentPhase, 1 ) ;
    setTrimValue( phase, 1, trims[1] ) ;
		phase = getTrimFlightPhase( CurrentPhase, 2 ) ;
    setTrimValue( phase, 2, trims[2] ) ;
		phase = getTrimFlightPhase( CurrentPhase, 3 ) ;
    setTrimValue( phase, 3, trims[3] ) ;

    calibratedStick[4] = ui->dialP_1->value();
    calibratedStick[5] = ui->dialP_2->value();
    calibratedStick[6] = ui->dialP_3->value();

    if(g_eeGeneral.throttleReversed)
    {
        calibratedStick[THR_STICK] *= -1;
        if( !g_model.thrTrim)
        {
          *trimptr[THR_STICK] *= -1;
        }
    }
	for( uint8_t i = 0 ; i < MAX_GVARS ; i += 1 )
	{
		int x ;
		// ToDo, test for trim inputs here
		if ( g_model.gvars[i].gvsource )
		{
			if ( g_model.gvars[i].gvsource <= 4 )
			{
				uint32_t y ;
				y = g_model.gvars[i].gvsource - 1 ;

				y = adjustMode( y ) ;
				
//				uint32_t phaseNo = getTrimFlightPhase( CurrentPhase, y ) ;
				g_model.gvars[i].gvar = getTrimValue( CurrentPhase, y ) ;
				 
			}
			else if ( g_model.gvars[i].gvsource == 5 )	// REN
			{
				//g_model.gvars[i].gvar = RotaryControl ;
			}
			else if ( g_model.gvars[i].gvsource <= 9 )	// Stick
			{
        x = calibratedStick[ g_model.gvars[i].gvsource-5 - 1 ] / 8 ;
				if ( x < -125 )
				{
					x = -125 ;					
				}
				if ( x > 125 )
				{
					x = 125 ;					
				}
        g_model.gvars[i].gvar = x ;
			}
			else if ( g_model.gvars[i].gvsource <= 12 )	// Pot
			{
				uint32_t y ;
				y = g_model.gvars[i].gvsource - 6 ;

				y = adjustMode( y ) ;
				
				x = calibratedStick[ y ] / 8 ;
				if ( x < -125 )
				{
					x = -125 ;					
				}
				if ( x > 125 )
				{
					x = 125 ;					
				}
        g_model.gvars[i].gvar = x ;
			}
			else if ( g_model.gvars[i].gvsource <= 36 )	// Chans
			{
				x = ex_chans[g_model.gvars[i].gvsource-13] / 10 ;
				if ( x < -125 )
				{
					x = -125 ;					
				}
				if ( x > 125 )
				{
					x = 125 ;					
				}
        g_model.gvars[i].gvar = x ;
			}
		}
	}

}

int simulatorDialog::chVal(int val)
{
	if ( current_limits == 1 )
	{
    return qMin(1280, qMax(-1280, val));
	}
	else
	{
    return qMin(1024, qMax(-1024, val));
	}
}

void simulatorDialog::setValues()
{
  if ( current_limits != g_model.extendedLimits )
	{
		int limit ;
    current_limits = g_model.extendedLimits ;
		if ( current_limits )
		{
      limit = 1280 ;
		}
		else
		{
      limit = 1024 ;
		}
    ui->chnout_1->setMinimum( - limit ) ;
    ui->chnout_1->setMaximum( limit ) ;
    ui->chnout_2->setMinimum( - limit ) ;
    ui->chnout_2->setMaximum( limit ) ;
    ui->chnout_3->setMinimum( - limit ) ;
    ui->chnout_3->setMaximum( limit ) ;
    ui->chnout_4->setMinimum( - limit ) ;
    ui->chnout_4->setMaximum( limit ) ;
    ui->chnout_5->setMinimum( - limit ) ;
    ui->chnout_5->setMaximum( limit ) ;
    ui->chnout_6->setMinimum( - limit ) ;
    ui->chnout_6->setMaximum( limit ) ;
    ui->chnout_7->setMinimum( - limit ) ;
    ui->chnout_7->setMaximum( limit ) ;
    ui->chnout_8->setMinimum( - limit ) ;
    ui->chnout_8->setMaximum( limit ) ;
    ui->chnout_9->setMinimum( - limit ) ;
    ui->chnout_9->setMaximum( limit ) ;
    ui->chnout_10->setMinimum( - limit ) ;
    ui->chnout_10->setMaximum( limit ) ;
    ui->chnout_11->setMinimum( - limit ) ;
    ui->chnout_11->setMaximum( limit ) ;
    ui->chnout_12->setMinimum( - limit ) ;
    ui->chnout_12->setMaximum( limit ) ;
    ui->chnout_13->setMinimum( - limit ) ;
    ui->chnout_13->setMaximum( limit ) ;
    ui->chnout_14->setMinimum( - limit ) ;
    ui->chnout_14->setMaximum( limit ) ;
    ui->chnout_15->setMinimum( - limit ) ;
    ui->chnout_15->setMaximum( limit ) ;
    ui->chnout_16->setMinimum( - limit ) ;
    ui->chnout_16->setMaximum( limit ) ;
		
	}
    ui->chnout_1->setValue(chVal(chanOut[0]));
    ui->chnout_2->setValue(chVal(chanOut[1]));
    ui->chnout_3->setValue(chVal(chanOut[2]));
    ui->chnout_4->setValue(chVal(chanOut[3]));
    ui->chnout_5->setValue(chVal(chanOut[4]));
    ui->chnout_6->setValue(chVal(chanOut[5]));
    ui->chnout_7->setValue(chVal(chanOut[6]));
    ui->chnout_8->setValue(chVal(chanOut[7]));
    ui->chnout_9->setValue(chVal(chanOut[8]));
    ui->chnout_10->setValue(chVal(chanOut[9]));
    ui->chnout_11->setValue(chVal(chanOut[10]));
    ui->chnout_12->setValue(chVal(chanOut[11]));
    ui->chnout_13->setValue(chVal(chanOut[12]));
    ui->chnout_14->setValue(chVal(chanOut[13]));
    ui->chnout_15->setValue(chVal(chanOut[14]));
    ui->chnout_16->setValue(chVal(chanOut[15]));

    ui->chnoutV_1->setText(QString("%1").arg((qreal)chanOut[0]*100/1024, 0, 'f', 1));
    ui->chnoutV_2->setText(QString("%1").arg((qreal)chanOut[1]*100/1024, 0, 'f', 1));
    ui->chnoutV_3->setText(QString("%1").arg((qreal)chanOut[2]*100/1024, 0, 'f', 1));
    ui->chnoutV_4->setText(QString("%1").arg((qreal)chanOut[3]*100/1024, 0, 'f', 1));
    ui->chnoutV_5->setText(QString("%1").arg((qreal)chanOut[4]*100/1024, 0, 'f', 1));
    ui->chnoutV_6->setText(QString("%1").arg((qreal)chanOut[5]*100/1024, 0, 'f', 1));
    ui->chnoutV_7->setText(QString("%1").arg((qreal)chanOut[6]*100/1024, 0, 'f', 1));
    ui->chnoutV_8->setText(QString("%1").arg((qreal)chanOut[7]*100/1024, 0, 'f', 1));
    ui->chnoutV_9->setText(QString("%1").arg((qreal)chanOut[8]*100/1024, 0, 'f', 1));
    ui->chnoutV_10->setText(QString("%1").arg((qreal)chanOut[9]*100/1024, 0, 'f', 1));
    ui->chnoutV_11->setText(QString("%1").arg((qreal)chanOut[10]*100/1024, 0, 'f', 1));
    ui->chnoutV_12->setText(QString("%1").arg((qreal)chanOut[11]*100/1024, 0, 'f', 1));
    ui->chnoutV_13->setText(QString("%1").arg((qreal)chanOut[12]*100/1024, 0, 'f', 1));
    ui->chnoutV_14->setText(QString("%1").arg((qreal)chanOut[13]*100/1024, 0, 'f', 1));
    ui->chnoutV_15->setText(QString("%1").arg((qreal)chanOut[14]*100/1024, 0, 'f', 1));
    ui->chnoutV_16->setText(QString("%1").arg((qreal)chanOut[15]*100/1024, 0, 'f', 1));

    ui->leftXPerc->setText(QString("X %1\%").arg((qreal)nodeLeft->getX()*100, 2, 'f', 0));
    ui->leftYPerc->setText(QString("Y %1\%").arg((qreal)nodeLeft->getY()*-100, 2, 'f', 0));

    ui->rightXPerc->setText(QString("X %1\%").arg((qreal)nodeRight->getX()*100, 2, 'f', 0));
    ui->rightYPerc->setText(QString("Y %1\%").arg((qreal)nodeRight->getY()*-100, 2, 'f', 0));

#define CSWITCH_ON  "QLabel { background-color: #4CC417 }"
#define CSWITCH_OFF "QLabel { }"

    ui->labelCSW_1->setStyleSheet(getSwitch(DSW_SW1,0)   ? CSWITCH_ON : CSWITCH_OFF);
    ui->labelCSW_2->setStyleSheet(getSwitch(DSW_SW2,0)   ? CSWITCH_ON : CSWITCH_OFF);
    ui->labelCSW_3->setStyleSheet(getSwitch(DSW_SW3,0)   ? CSWITCH_ON : CSWITCH_OFF);
    ui->labelCSW_4->setStyleSheet(getSwitch(DSW_SW4,0)   ? CSWITCH_ON : CSWITCH_OFF);
    ui->labelCSW_5->setStyleSheet(getSwitch(DSW_SW5,0)   ? CSWITCH_ON : CSWITCH_OFF);
    ui->labelCSW_6->setStyleSheet(getSwitch(DSW_SW6,0)   ? CSWITCH_ON : CSWITCH_OFF);
    ui->labelCSW_7->setStyleSheet(getSwitch(DSW_SW7,0)   ? CSWITCH_ON : CSWITCH_OFF);
    ui->labelCSW_8->setStyleSheet(getSwitch(DSW_SW8,0)   ? CSWITCH_ON : CSWITCH_OFF);
    ui->labelCSW_9->setStyleSheet(getSwitch(DSW_SW9,0)   ? CSWITCH_ON : CSWITCH_OFF);
    ui->labelCSW_10->setStyleSheet(getSwitch(DSW_SWA,0)  ? CSWITCH_ON : CSWITCH_OFF);
    ui->labelCSW_11->setStyleSheet(getSwitch(DSW_SWB,0)  ? CSWITCH_ON : CSWITCH_OFF);
    ui->labelCSW_12->setStyleSheet(getSwitch(DSW_SWC,0)  ? CSWITCH_ON : CSWITCH_OFF);

#define CRED  "QSlider::handle:horizontal:disabled { background: #CC0000;border: 1px solid #aaa;border-radius: 4px; }"
#define CBLUE "QSlider::handle:horizontal:disabled { background: #0000CC;border: 1px solid #aaa;border-radius: 4px; }"
	int onoff[16] ;
	int i ;
	for ( i = 0 ; i < 16 ; i += 1 )
	{
		onoff[i] = 0 ;
    if ( ( g_model.protocol == PROTO_PPM ) || ( g_model.protocol == PROTO_PPM16 ) )
		{
			if ( i >= g_model.ppmStart )
			{
				if ( i < g_model.ppmStart + g_model.ppmNCH*2+8 )
				{
					onoff[i] = 1 ;				
				}
			}
		}
		else if ( g_model.protocol == PROTO_PXX )
		{
			if ( i >= g_model.ppmStart )
			{
				if ( i < g_model.ppmStart + 8 )
				{
					onoff[i] = 1 ;				
				}
			}
		}
	}
  ui->chnout_1->setStyleSheet( onoff[0] ? CRED : CBLUE ) ;
  ui->chnout_2->setStyleSheet( onoff[1] ? CRED : CBLUE ) ;
  ui->chnout_3->setStyleSheet( onoff[2] ? CRED : CBLUE ) ;
  ui->chnout_4->setStyleSheet( onoff[3] ? CRED : CBLUE ) ;
  ui->chnout_5->setStyleSheet( onoff[4] ? CRED : CBLUE ) ;
  ui->chnout_6->setStyleSheet( onoff[5] ? CRED : CBLUE ) ;
  ui->chnout_7->setStyleSheet( onoff[6] ? CRED : CBLUE ) ;
  ui->chnout_8->setStyleSheet( onoff[7] ? CRED : CBLUE ) ;
  ui->chnout_9->setStyleSheet( onoff[8] ? CRED : CBLUE ) ;
  ui->chnout_10->setStyleSheet( onoff[9] ? CRED : CBLUE ) ;
  ui->chnout_11->setStyleSheet( onoff[10] ? CRED : CBLUE ) ;
  ui->chnout_12->setStyleSheet( onoff[11] ? CRED : CBLUE ) ;
  ui->chnout_13->setStyleSheet( onoff[12] ? CRED : CBLUE ) ;
  ui->chnout_14->setStyleSheet( onoff[13] ? CRED : CBLUE ) ;
  ui->chnout_15->setStyleSheet( onoff[14] ? CRED : CBLUE ) ;
  ui->chnout_16->setStyleSheet( onoff[14] ? CRED : CBLUE ) ;
}

void simulatorDialog::beepWarn1()
{
    beepVal = 1;
    beepShow = 20;
}

void simulatorDialog::beepWarn2()
{
    beepVal = 1;
    beepShow = 20;
}

void simulatorDialog::beepWarn()
{
    beepVal = 1;
    beepShow = 20;
}

void simulatorDialog::setupSticks()
{
    QGraphicsScene *leftScene = new QGraphicsScene(ui->leftStick);
    leftScene->setItemIndexMethod(QGraphicsScene::NoIndex);
    ui->leftStick->setScene(leftScene);

    // ui->leftStick->scene()->addLine(0,10,20,30);

    QGraphicsScene *rightScene = new QGraphicsScene(ui->rightStick);
    rightScene->setItemIndexMethod(QGraphicsScene::NoIndex);
    ui->rightStick->setScene(rightScene);

    // ui->rightStick->scene()->addLine(0,10,20,30);

    nodeLeft = new Node();
    nodeLeft->setPos(-GBALL_SIZE/2,-GBALL_SIZE/2);
    nodeLeft->setBallSize(GBALL_SIZE);
    leftScene->addItem(nodeLeft);

    nodeRight = new Node();
    nodeRight->setPos(-GBALL_SIZE/2,-GBALL_SIZE/2);
    nodeRight->setBallSize(GBALL_SIZE);
    rightScene->addItem(nodeRight);
}

void simulatorDialog::resizeEvent(QResizeEvent *event)
{

    if(ui->leftStick->scene())
    {
        QRect qr = ui->leftStick->contentsRect();
        qreal w  = (qreal)qr.width()  - GBALL_SIZE;
        qreal h  = (qreal)qr.height() - GBALL_SIZE;
        qreal cx = (qreal)qr.width()/2;
        qreal cy = (qreal)qr.height()/2;
        ui->leftStick->scene()->setSceneRect(-cx,-cy,w,h);

        QPointF p = nodeLeft->pos();
        p.setX(qMin(cx, qMax(p.x(), -cx)));
        p.setY(qMin(cy, qMax(p.y(), -cy)));
        nodeLeft->setPos(p);
    }

    if(ui->rightStick->scene())
    {
        QRect qr = ui->rightStick->contentsRect();
        qreal w  = (qreal)qr.width()  - GBALL_SIZE;
        qreal h  = (qreal)qr.height() - GBALL_SIZE;
        qreal cx = (qreal)qr.width()/2;
        qreal cy = (qreal)qr.height()/2;
        ui->rightStick->scene()->setSceneRect(-cx,-cy,w,h);

        QPointF p = nodeRight->pos();
        p.setX(qMin(cx, qMax(p.x(), -cx)));
        p.setY(qMin(cy, qMax(p.y(), -cy)));
        nodeRight->setPos(p);
    }
    QDialog::resizeEvent(event);
}


inline qint16 calc100toRESX(qint8 x)
{
    return (qint16)x*10 + x/4 - x/64;
}

inline qint16 calc1000toRESX(qint16 x)
{
    return x + x/32 - x/128 + x/512;
}


bool simulatorDialog::keyState(EnumKeys key)
{
    switch (key)
    {
    case (SW_ThrCt):   return ui->switchTHR->isChecked(); break;
    case (SW_RuddDR):  return ui->switchRUD->isChecked(); break;
    case (SW_ElevDR):  return ui->switchELE->isChecked(); break;
    case (SW_ID0):     return ui->switchID0->isChecked(); break;
    case (SW_ID1):     return ui->switchID1->isChecked(); break;
    case (SW_ID2):     return ui->switchID2->isChecked(); break;
    case (SW_AileDR):  return ui->switchAIL->isChecked(); break;
    case (SW_Gear):    return ui->switchGEA->isChecked(); break;
    case (SW_Trainer): return ui->switchTRN->isDown(); break;
    default:
        return false;
        break;
    }
}

qint16 simulatorDialog::getValue(qint8 i)
{
    if(i<PPM_BASE) return calibratedStick[i];//-512..512
    else if(i<CHOUT_BASE) return g_ppmIns[i-PPM_BASE];// - g_eeGeneral.ppmInCalib[i-PPM_BASE];
    else return ex_chans[i-CHOUT_BASE];
    return 0;
}

bool Last_switch[NUM_CSW+EXTRA_CSW] ;

bool simulatorDialog::getSwitch(int swtch, bool nc, qint8 level)
{
    bool ret_value ;
    uint8_t cs_index ;
    
		if(level>5) return false; //prevent recursive loop going too deep

		if ( swtch == 0 )
		{
			return  nc ;
		}

		if ( ee_type )
		{
    	switch(swtch)
			{
    	case  MAX_DRSWITCH+EXTRA_CSW: return  true;
    	case -MAX_DRSWITCH-EXTRA_CSW: return  false;
    	}
		}
		else
		{
    	switch(swtch)
			{
    	case  MAX_DRSWITCH: return  true;
    	case -MAX_DRSWITCH: return  false;
    	}
		}

    uint8_t dir = swtch>0;
		
		if(abs(swtch)<(PHY_SWITCH))
		{
        if(!dir) return ! keyState((EnumKeys)(SW_BASE-swtch-1));
        return            keyState((EnumKeys)(SW_BASE+swtch-1));
    }

    //custom switch, Issue 78
    //use putsChnRaw
    //input -> 1..4 -> sticks,  5..8 pots
    //MAX,FULL - disregard
    //ppm
    cs_index = abs(swtch)-(PHY_SWITCH);

		if ( ee_type )
		{
			if ( cs_index >= NUM_CSW )
			{
				CxSwData &cs = g_model.xcustomSw[cs_index-NUM_CSW];

    		if(!cs.func) return false;
		
    		if ( level>4 )
    		{
    		  ret_value = Last_switch[cs_index] ;
    		  return swtch>0 ? ret_value : !ret_value ;
    		}

    		int8_t a = cs.v1;
    		int8_t b = cs.v2;
    		int16_t x = 0;
    		int16_t y = 0;

    		// init values only if needed
    		uint8_t s = CS_STATE(cs.func);
    		if(s == CS_VOFS)
    		{
    		    x = getValue(cs.v1-1);
    		    y = calc100toRESX(cs.v2);
    		}
    		else if(s == CS_VCOMP)
    		{
    		    x = getValue(cs.v1-1);
    		    y = getValue(cs.v2-1);
    		}

    		switch (cs.func) {
    		case (CS_VPOS):
    		    ret_value = (x>y);
    		    break;
    		case (CS_VNEG):
    		    ret_value = (x<y) ;
    		    break;
    		case (CS_APOS):
    		    ret_value = (abs(x)>y) ;
    		    break;
    		case (CS_ANEG):
    		    ret_value = (abs(x)<y) ;
    		    break;

    		case (CS_AND):
    		    ret_value = (getSwitch(a,0,level+1) && getSwitch(b,0,level+1));
    		    break;
    		case (CS_OR):
    		    ret_value = (getSwitch(a,0,level+1) || getSwitch(b,0,level+1));
    		    break;
    		case (CS_XOR):
    		    ret_value = (getSwitch(a,0,level+1) ^ getSwitch(b,0,level+1));
    		    break;

    		case (CS_EQUAL):
    		    ret_value = (x==y);
    		    break;
    		case (CS_NEQUAL):
    		    ret_value = (x!=y);
    		    break;
    		case (CS_GREATER):
    		    ret_value = (x>y);
    		    break;
    		case (CS_LESS):
    		    ret_value = (x<y);
    		    break;
    		case (CS_EGREATER):
    		    ret_value = (x>=y);
    		    break;
    		case (CS_ELESS):
    		    ret_value = (x<=y);
    		    break;
    		case (CS_TIME):
    		    ret_value = CsTimer[cs_index] >= 0 ;
    		    break;
    		default:
    		    return false;
    		    break;
    		}
				if ( ret_value )
				{
					if ( cs.andsw )
					{
						int8_t x ;
						x = cs.andsw ;
						if ( x > 8 )
						{
							x += 1 ;
						}
						if ( x < -8 )
						{
							x -= 1 ;
						}
						if ( x > 9+NUM_CSW )
						{
							x = 9 ;			// Tag TRN on the end, keep EEPROM values
						}
						if ( x < -(9+NUM_CSW) )
						{
							x = -9 ;			// Tag TRN on the end, keep EEPROM values
						}
    		    ret_value = getSwitch( x, 0, level+1) ;
					}
				}
				Last_switch[cs_index] = ret_value ;
				return swtch>0 ? ret_value : !ret_value ;
			}
		}

		CSwData &cs = g_model.customSw[cs_index];
    if(!cs.func) return false;
		
    if ( level>4 )
    {
      ret_value = Last_switch[cs_index] ;
      return swtch>0 ? ret_value : !ret_value ;
    }

    int8_t a = cs.v1;
    int8_t b = cs.v2;
    int16_t x = 0;
    int16_t y = 0;

    // init values only if needed
    uint8_t s = CS_STATE(cs.func);
    if(s == CS_VOFS)
    {
        x = getValue(cs.v1-1);
        y = calc100toRESX(cs.v2);
    }
    else if(s == CS_VCOMP)
    {
        x = getValue(cs.v1-1);
        y = getValue(cs.v2-1);
    }

    switch (cs.func) {
    case (CS_VPOS):
        ret_value = (x>y);
        break;
    case (CS_VNEG):
        ret_value = (x<y) ;
        break;
    case (CS_APOS):
        ret_value = (abs(x)>y) ;
        break;
    case (CS_ANEG):
        ret_value = (abs(x)<y) ;
        break;

    case (CS_AND):
        ret_value = (getSwitch(a,0,level+1) && getSwitch(b,0,level+1));
        break;
    case (CS_OR):
        ret_value = (getSwitch(a,0,level+1) || getSwitch(b,0,level+1));
        break;
    case (CS_XOR):
        ret_value = (getSwitch(a,0,level+1) ^ getSwitch(b,0,level+1));
        break;

    case (CS_EQUAL):
        ret_value = (x==y);
        break;
    case (CS_NEQUAL):
        ret_value = (x!=y);
        break;
    case (CS_GREATER):
        ret_value = (x>y);
        break;
    case (CS_LESS):
        ret_value = (x<y);
        break;
    case (CS_EGREATER):
        ret_value = (x>=y);
        break;
    case (CS_ELESS):
        ret_value = (x<=y);
        break;
    case (CS_TIME):
        ret_value = CsTimer[cs_index] >= 0 ;
        break;
    default:
        return false;
        break;
    }
		if ( ret_value )
		{
			if ( cs.andsw )
			{
				int8_t x ;
				x = cs.andsw ;
				if ( x > 8 )
				{
					x += 1 ;
				}
        ret_value = getSwitch( x, 0, level+1) ;
			}
		}
		Last_switch[cs_index] = ret_value ;
		return swtch>0 ? ret_value : !ret_value ;
}


uint16_t expou(uint16_t x, uint16_t k)
{
    // k*x*x*x + (1-k)*x
    return ((unsigned long)x*x*x/0x10000*k/(RESXul*RESXul/0x10000) + (RESKul-k)*x+RESKul/2)/RESKul;
}
// expo-funktion:
// ---------------
// kmplot
// f(x,k)=exp(ln(x)*k/10) ;P[0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20]
// f(x,k)=x*x*x*k/10 + x*(1-k/10) ;P[0,1,2,3,4,5,6,7,8,9,10]
// f(x,k)=x*x*k/10 + x*(1-k/10) ;P[0,1,2,3,4,5,6,7,8,9,10]
// f(x,k)=1+(x-1)*(x-1)*(x-1)*k/10 + (x-1)*(1-k/10) ;P[0,1,2,3,4,5,6,7,8,9,10]

int16_t expo(int16_t x, int16_t k)
{
    if(k == 0) return x;
    int16_t   y;
    bool    neg =  x < 0;
    if(neg)   x = -x;
    if(k<0){
        y = RESXu-expou(RESXu-x,-k);
    }else{
        y = expou(x,k);
    }
    return neg? -y:y;
}

uint16_t isqrt32(uint32_t n)
{
    uint16_t c = 0x8000;
    uint16_t g = 0x8000;

    for(;;) {
        if((uint32_t)g*g > n)
            g ^= c;
        c >>= 1;
        if(c == 0)
            return g;
        g |= c;
    }
}

int16_t simulatorDialog::intpol(int16_t x, uint8_t idx) // -100, -75, -50, -25, 0 ,25 ,50, 75, 100
{
#define D9 (RESX * 2 / 8)
#define D5 (RESX * 2 / 4)
    bool    cv9 = idx >= MAX_CURVE5;
    int8_t *crv = cv9 ? g_model.curves9[idx-MAX_CURVE5] : g_model.curves5[idx];
    int16_t erg;

    x+=RESXu;
    if(x < 0) {
        erg = (int16_t)crv[0] * (RESX/4);
    } else if(x >= (RESX*2)) {
        erg = (int16_t)crv[(cv9 ? 8 : 4)] * (RESX/4);
    } else {
        int16_t a,dx;
        if(cv9){
            a   = (uint16_t)x / D9;
            dx  =((uint16_t)x % D9) * 2;
        } else {
            a   = (uint16_t)x / D5;
            dx  = (uint16_t)x % D5;
        }
        erg  = (int16_t)crv[a]*((D5-dx)/2) + (int16_t)crv[a+1]*(dx/2);
    }
    return erg / 25; // 100*D5/RESX;
}

void simulatorDialog::timerTick()
{
    int16_t val = 0;
    if((abs(g_model.tmrMode)>1) && (abs(g_model.tmrMode)<TMR_VAROFS)) {
        val = calibratedStick[CONVERT_MODE(abs(g_model.tmrMode)/2,g_model.modelVersion,g_eeGeneral.stickMode)-1];
        val = (g_model.tmrMode<0 ? RESX-val : val+RESX ) / (RESX/16);  // only used for %
    }

    int8_t tm = g_model.tmrMode;
  	int8_t tmb ;
		uint8_t switch_b ;
		uint8_t max_drswitch ;
		max_drswitch = ( ee_type ) ? MAX_DRSWITCH+EXTRA_CSW : MAX_DRSWITCH ;
    tmb = g_model.tmrModeB ;

    if(abs(tm)>=(TMR_VAROFS+max_drswitch-1)){ //toggeled switch//abs(g_model.tmrMode)<(10+MAX_DRSWITCH-1)
        static uint8_t lastSwPos;
        if(!(sw_toggled | s_sum | s_cnt | s_time | lastSwPos)) lastSwPos = tm < 0;  // if initializing then init the lastSwPos
        uint8_t swPos = getSwitch(tm>0 ? tm-(TMR_VAROFS+max_drswitch-1-1) : tm+(TMR_VAROFS+MAX_DRSWITCH-1-1) ,0);
        if(swPos && !lastSwPos)  sw_toggled = !sw_toggled;  //if switcdh is flipped first time -> change counter state
        lastSwPos = swPos;
    }

   	switch_b = tmb ? getSwitch( tmb ,0) : 1 ; //normal switch
		
		if ( switch_b == 0 )
		{
			val = 0 ;		// Stop TH%
		}

    s_time++;
    if(s_time<100) return; //1 sec
    s_time = 0;
    if(abs(tm)<TMR_VAROFS) sw_toggled = false; // not switch - sw timer off
    else if(abs(tm)<(TMR_VAROFS+max_drswitch-1)) sw_toggled = getSwitch((tm>0 ? tm-(TMR_VAROFS-1) : tm+(TMR_VAROFS-1)) ,0); //normal switch

    s_timeCumTot               += 1;
    s_timeCumAbs               += 1;
    if(val) s_timeCumThr       += 1;
    if(abs(g_model.tmrMode)==TMRMODE_ABS) sw_toggled = true ;
    
		if(sw_toggled && switch_b) s_timeCumSw += 1;
    s_timeCum16ThrP            += val/2;

    s_timerVal = g_model.tmrVal;
    uint8_t tmrM = abs(g_model.tmrMode);
    if(tmrM==TMRMODE_NONE) s_timerState = TMR_OFF;
    else if(tmrM==TMRMODE_ABS)
		{
			if ( tmb == 0 ) s_timerVal -= s_timeCumAbs ;
    	else s_timerVal -= s_timeCumSw ; //switch
    }
		else if(tmrM<TMR_VAROFS) s_timerVal -= (tmrM&1) ? s_timeCum16ThrP/16 : s_timeCumThr;// stick% : stick
    else s_timerVal -= s_timeCumSw; //switch

    switch(s_timerState)
    {
    case TMR_OFF:
        if(g_model.tmrMode != TMRMODE_NONE) s_timerState=TMR_RUNNING;
        break;
    case TMR_RUNNING:
        if(s_timerVal<=0 && g_model.tmrVal) s_timerState=TMR_BEEPING;
        break;
    case TMR_BEEPING:
        if(s_timerVal <= -MAX_ALERT_TIME)   s_timerState=TMR_STOPPED;
        if(g_model.tmrVal == 0)             s_timerState=TMR_RUNNING;
        break;
    case TMR_STOPPED:
        break;
    }

    static int16_t last_tmr;

    if(last_tmr != s_timerVal)  //beep only if seconds advance
    {
        if(s_timerState==TMR_RUNNING)
        {
            if(g_eeGeneral.preBeep && g_model.tmrVal) // beep when 30, 15, 10, 5,4,3,2,1 seconds remaining
            {
                if(s_timerVal==30) {beepAgain=2; beepWarn2();} //beep three times
                if(s_timerVal==20) {beepAgain=1; beepWarn2();} //beep two times
                if(s_timerVal==10)  beepWarn2();
                if(s_timerVal<= 3)  beepWarn2();

                if(g_eeGeneral.flashBeep && (s_timerVal==30 || s_timerVal==20 || s_timerVal==10 || s_timerVal<=3))
                    g_LightOffCounter = FLASH_DURATION;
            }

            if(g_eeGeneral.minuteBeep && (((g_model.tmrDir ? g_model.tmrVal-s_timerVal : s_timerVal)%60)==0)) //short beep every minute
            {
                beepWarn2();
                if(g_eeGeneral.flashBeep) g_LightOffCounter = FLASH_DURATION;
            }
        }
        else if(s_timerState==TMR_BEEPING)
        {
            beepWarn();
            if(g_eeGeneral.flashBeep) g_LightOffCounter = FLASH_DURATION;
        }
    }
    last_tmr = s_timerVal;
    if(g_model.tmrDir) s_timerVal = g_model.tmrVal-s_timerVal; //if counting backwards - display backwards



}

// GVARS helpers

int8_t simulatorDialog::REG100_100(int8_t x)
{
	return REG( x, -100, 100 ) ;
}

int8_t simulatorDialog::REG(int8_t x, int8_t min, int8_t max)
{
  int8_t result = x;
  if (x >= 126 || x <= -126) {
    x = (uint8_t)x - 126;
    result = g_model.gvars[x].gvar ;
    if (result < min) {
      g_model.gvars[x].gvar = result = min;
//      eeDirty( EE_MODEL | EE_TRIM ) ;
    }
    if (result > max) {
      g_model.gvars[x].gvar = result = max;
//      eeDirty( EE_MODEL | EE_TRIM ) ;
    }
  }
  return result;
}


void simulatorDialog::perOut(bool init)
{
    int16_t trimA[4];
    uint8_t  anaCenter = 0;
    uint16_t d = 0;

		CurrentPhase = getFlightPhase() ;

    uint8_t ele_stick, ail_stick ;
	  if ( g_model.modelVersion >= 2 )
		{
			ele_stick = 1 ; //ELE_STICK ;
  	  ail_stick = 3 ; //AIL_STICK ;
		}
		else
		{
			ele_stick = ELE_STICK ;
  	  ail_stick = AIL_STICK ;
		}
    //===========Swash Ring================
    if(g_model.swashRingValue)
    {
        uint32_t v = (calibratedStick[ele_stick]*calibratedStick[ele_stick] +
                      calibratedStick[ail_stick]*calibratedStick[ail_stick]);
        uint32_t q = RESX*g_model.swashRingValue/100;
        q *= q;
        if(v>q)
            d = isqrt32(v);
    }
    //===========Swash Ring================


    for(uint8_t i=0;i<7;i++){        // calc Sticks

        //Normalization  [0..2048] ->   [-1024..1024]

        int16_t v = calibratedStick[i];
        //    v -= g_eeGeneral.calibMid[i];
        //    v  =  v * (int32_t)RESX /  (max((int16_t)100,(v>0 ?
        //                                     g_eeGeneral.calibSpanPos[i] :
        //                                     g_eeGeneral.calibSpanNeg[i])));
        //    if(v <= -RESX) v = -RESX;
        //    if(v >=  RESX) v =  RESX;
        //    calibratedStick[i] = v; //for show in expo

        if(!(v/16)) anaCenter |= 1<<(CONVERT_MODE((i+1),g_model.modelVersion,g_eeGeneral.stickMode)-1);

        //===========Swash Ring================
        if(d && (i==ele_stick || i==ail_stick))
            v = (int32_t)v*g_model.swashRingValue*RESX/(d*100);
        //===========Swash Ring================


				uint8_t index = i ;
				if ( g_model.modelVersion >= 2 )
				{
					if ( i < 4 )
					{
						uint8_t stickIndex = g_eeGeneral.stickMode*4 ;
  	        index = stickScramble[stickIndex+i] ;
					}
				}
        if(i<4)
				{ //only do this for sticks
            uint8_t expoDrOn = GET_DR_STATE(index);
            uint8_t stkDir = v>0 ? DR_RIGHT : DR_LEFT;
			  
				

            if(IS_THROTTLE(index) && g_model.thrExpo){
#if GVARS
                v  = 2*expo((v+RESX)/2,REG100_100(g_model.expoData[index].expo[expoDrOn][DR_EXPO][DR_RIGHT]));
#else
                v  = 2*expo((v+RESX)/2,g_model.expoData[index].expo[expoDrOn][DR_EXPO][DR_RIGHT]);
#endif                    
                stkDir = DR_RIGHT;
            }
            else
#if GVARS
                v  = expo(v,REG100_100(g_model.expoData[index].expo[expoDrOn][DR_EXPO][stkDir]));
#else
                v  = expo(v,g_model.expoData[index].expo[expoDrOn][DR_EXPO][stkDir]);
#endif                    

#if GVARS
            int32_t x = (int32_t)v * (REG(g_model.expoData[index].expo[expoDrOn][DR_WEIGHT][stkDir]+100, 0, 100))/100;
#else
            int32_t x = (int32_t)v * (g_model.expoData[index].expo[expoDrOn][DR_WEIGHT][stkDir]+100)/100;
#endif                    
            v = (int16_t)x;
            if (IS_THROTTLE(index) && g_model.thrExpo) v -= RESX;

				  if ( g_model.modelVersion >= 2 )
					{
          	trimA[i] = getTrimValue( CurrentPhase, i )*2 ;
					}	
					else
					{
            //do trim -> throttle trim if applicable
            int32_t vv = 2*RESX;
            if(IS_THROTTLE(i) && g_model.thrTrim)
						{
							int8_t ttrim ;
							ttrim = getTrimValue( CurrentPhase, i ) ;
//							ttrim = *trimptr[i] ;
							if(g_eeGeneral.throttleReversed)
							{
								ttrim = -ttrim ;
							}
							vv = ((int32_t)ttrim+125)*(RESX-v)/(2*RESX);
						}

            //trim
            trimA[i] = (vv==2*RESX) ? getTrimValue( CurrentPhase, i )*2 : (int16_t)vv*2; //    if throttle trim -> trim low end
//            trimA[i] = (vv==2*RESX) ? *trimptr[i]*2 : (int16_t)vv*2; //    if throttle trim -> trim low end
					}
        }
				if ( g_model.modelVersion >= 2 )
				{
       		anas[index] = v; //set values for mixer
				}
				else
				{
       		anas[i] = v; //set values for mixer
				}
    }
	  if ( g_model.modelVersion >= 2 )
		{
      if(g_model.thrTrim)
			{
				int8_t ttrim ;
				ttrim = getTrimValue( CurrentPhase, 2 ) ;
				if(g_eeGeneral.throttleReversed)
				{
					ttrim = -ttrim ;
				}
       	trimA[2] = ((int32_t)ttrim+125)*(RESX-anas[2])/(RESX) ;
			}
		}

    //===========BEEP CENTER================
    anaCenter &= g_model.beepANACenter;
    if(((bpanaCenter ^ anaCenter) & anaCenter)) beepWarn1();
    bpanaCenter = anaCenter;


    calibratedStick[MIX_MAX-1]=calibratedStick[MIX_FULL-1]=1024;
    anas[MIX_MAX-1]  = RESX;     // MAX
    anas[MIX_FULL-1] = RESX;     // FULL
		anas[MIX_3POS-1] = keyState(SW_ID0) ? -1024 : (keyState(SW_ID1) ? 0 : 1024) ;


    for(uint8_t i=0;i<NUM_PPM;i++)    anas[i+PPM_BASE]   = g_ppmIns[i];// - g_eeGeneral.ppmInCalib[i]; //add ppm channels
    for(uint8_t i=0;i<NUM_CHNOUT;i++) anas[i+CHOUT_BASE] = chans[i]; //other mixes previous outputs
#if GVARS
        for(uint8_t i=0;i<MAX_GVARS;i++) anas[i+MIX_3POS] = g_model.gvars[i].gvar * 8 ;
#endif


    //===========Swash Mix================
#define REZ_SWASH_X(x)  ((x) - (x)/8 - (x)/128 - (x)/512)   //  1024*sin(60) ~= 886
#define REZ_SWASH_Y(x)  ((x))   //  1024 => 1024

    if(g_model.swashType)
    {
        int16_t vp = anas[ele_stick]+trimA[ele_stick];
        int16_t vr = anas[ail_stick]+trimA[ail_stick];
        int16_t vc = 0;
        if(g_model.swashCollectiveSource)
            vc = anas[g_model.swashCollectiveSource-1];

        if(g_model.swashInvertELE) vp = -vp;
        if(g_model.swashInvertAIL) vr = -vr;
        if(g_model.swashInvertCOL) vc = -vc;

        switch (g_model.swashType)
        {
        case (SWASH_TYPE_120):
            //          vp = REZ_SWASH_Y(vp);
            //          vr = REZ_SWASH_X(vr);
            anas[MIX_CYC1-1] = vc - vp;
            anas[MIX_CYC2-1] = vc + vp/2 + vr;
            anas[MIX_CYC3-1] = vc + vp/2 - vr;
            break;
        case (SWASH_TYPE_120X):
            //          vp = REZ_SWASH_X(vp);
            //          vr = REZ_SWASH_Y(vr);
            anas[MIX_CYC1-1] = vc - vr;
            anas[MIX_CYC2-1] = vc + vr/2 + vp;
            anas[MIX_CYC3-1] = vc + vr/2 - vp;
            break;
        case (SWASH_TYPE_140):
            //          vp = REZ_SWASH_Y(vp);
            //          vr = REZ_SWASH_Y(vr);
            anas[MIX_CYC1-1] = vc - vp;
            anas[MIX_CYC2-1] = vc + vp + vr;
            anas[MIX_CYC3-1] = vc + vp - vr;
            break;
        case (SWASH_TYPE_90):
            //          vp = REZ_SWASH_Y(vp);
            //          vr = REZ_SWASH_Y(vr);
            anas[MIX_CYC1-1] = vc - vp;
            anas[MIX_CYC2-1] = vc + vr;
            anas[MIX_CYC3-1] = vc - vr;
            break;
        default:
            break;
        }

        calibratedStick[MIX_CYC1-1]=anas[MIX_CYC1-1];
        calibratedStick[MIX_CYC2-1]=anas[MIX_CYC2-1];
        calibratedStick[MIX_CYC3-1]=anas[MIX_CYC3-1];
    }

    memset(chans,0,sizeof(chans));        // All outputs to 0

    uint8_t mixWarning = 0;
    //========== MIXER LOOP ===============

    // Set the trim pointers back to the master set
    trimptr[0] = &trim[0] ;
    trimptr[1] = &trim[1] ;
    trimptr[2] = &trim[2] ;
    trimptr[3] = &trim[3] ;
        
    if( (g_eeGeneral.throttleReversed) && (!g_model.thrTrim))
    {
        *trimptr[THR_STICK] *= -1;
    }
		{
			int8_t trims[4] ;
			int i ;
			int idx ;
	
      for ( i = 0 ;  i <= 3 ; i += 1 )
			{
				idx = i ;
				if ( g_eeGeneral.crosstrim )
				{
					idx = 3 - idx ;			
				}
        trims[i] = getTrimValue( CurrentPhase, idx ) ;
			}
		
			
      if ( g_model.modelVersion >= 2 )
			{
				uint8_t stickIndex = g_eeGeneral.stickMode*4 ;
		
				uint8_t index ;
				index =g_eeGeneral.crosstrim ? 3 : 0 ;
				index =  stickScramble[stickIndex+index] ;
				ui->trimHLeft->setValue( trims[index]);  // mode=(0 || 1) -> rud trim else -> ail trim
				index =g_eeGeneral.crosstrim ? 2 : 1 ;
				index =  stickScramble[stickIndex+index] ;
    		ui->trimVLeft->setValue( trims[index]);  // mode=(0 || 2) -> thr trim else -> ele trim
				index =g_eeGeneral.crosstrim ? 1 : 2 ;
				index =  stickScramble[stickIndex+index] ;
    		ui->trimVRight->setValue(trims[index]);  // mode=(0 || 2) -> ele trim else -> thr trim
				index =g_eeGeneral.crosstrim ? 0 : 3 ;
				index =  stickScramble[stickIndex+index] ;
    		ui->trimHRight->setValue(trims[index]);  // mode=(0 || 1) -> ail trim else -> rud trim
			}
			else
			{
				ui->trimHLeft->setValue( trims[0]);  // mode=(0 || 1) -> rud trim else -> ail trim
    		ui->trimVLeft->setValue( trims[1]);  // mode=(0 || 2) -> thr trim else -> ele trim
    		ui->trimVRight->setValue(trims[2]);  // mode=(0 || 2) -> ele trim else -> thr trim
    		ui->trimHRight->setValue(trims[3]);  // mode=(0 || 1) -> ail trim else -> rud trim
      }
		
		}
		if( (g_eeGeneral.throttleReversed) && (!g_model.thrTrim))
    {
        *trimptr[THR_STICK] *= -1;
    }

    for(uint8_t i=0;i<MAX_MIXERS;i++){
        MixData &md = g_model.mixData[i];
#if GVARS
        int8_t mixweight = REG100_100( md.weight) ;
#endif

        if((md.destCh==0) || (md.destCh>NUM_CHNOUT)) break;

        //Notice 0 = NC switch means not used -> always on line
        int16_t v  = 0;
        uint8_t swTog;

#define DEL_MULT 256

        //swOn[i]=false;
        if(!getSwitch(md.swtch,1)){ // switch on?  if no switch selected => on
            swTog = swOn[i];
            swOn[i] = false;
            if(md.srcRaw!=MIX_FULL && md.srcRaw!=MIX_MAX) continue;// if not MAX or FULL - next loop
            if(md.mltpx==MLTPX_REP) continue; // if switch is off and REPLACE then off
            v = md.srcRaw==MIX_FULL ? -RESX : 0; // switch is off => FULL=-RESX
        }
        else {
            swTog = !swOn[i];
            swOn[i] = true;
            uint8_t k = md.srcRaw-1;
            v = anas[k]; //Switch is on. MAX=FULL=512 or value.
            if(k>=CHOUT_BASE && (k<i)) v = chans[k];
            if(md.mixWarn) mixWarning |= 1<<(md.mixWarn-1); // Mix warning
            if ( md.enableFmTrim )
            {
                if ( md.srcRaw <= 4 )
                {
                    trimptr[md.srcRaw-1] = &md.sOffset ;		// Use the value stored here for the trim
                    if( (g_eeGeneral.throttleReversed) && (!g_model.thrTrim))
                    {
                      *trimptr[THR_STICK] *= -1;
                    }
										ui->trimHLeft->setValue( getTrimValue( CurrentPhase, 0 ));  // mode=(0 || 1) -> rud trim else -> ail trim
    								ui->trimVLeft->setValue( getTrimValue( CurrentPhase, 1 ));  // mode=(0 || 2) -> thr trim else -> ele trim
    								ui->trimVRight->setValue(getTrimValue( CurrentPhase, 2 ));  // mode=(0 || 2) -> ele trim else -> thr trim
    								ui->trimHRight->setValue(getTrimValue( CurrentPhase, 3 ));  // mode=(0 || 1) -> ail trim else -> rud trim
                    if( (g_eeGeneral.throttleReversed) && (!g_model.thrTrim))
                    {
                      *trimptr[THR_STICK] *= -1;
                    }
                }
            }
        }

        //========== INPUT OFFSET ===============
        if ( ( md.enableFmTrim == 0 ) && ( md.lateOffset == 0 ) )
        {
#if GVARS
            if(md.sOffset) v += calc100toRESX( REG( md.sOffset, -125, 125 )	) ;
#else
            if(md.sOffset) v += calc100toRESX(md.sOffset);
#endif
        }

        //========== DELAY and PAUSE ===============
        if (md.speedUp || md.speedDown || md.delayUp || md.delayDown)  // there are delay values
        {
            if(init)
            {
                act[i]=(int32_t)v*DEL_MULT;
                swTog = false;
            }
            int32_t tact = act[i] ;
            int16_t diff = v-tact/DEL_MULT;

            if(swTog) {
                //need to know which "v" will give "anas".
                //curves(v)*weight/100 -> anas
                // v * weight / 100 = anas => anas*100/weight = v
                if(md.mltpx==MLTPX_REP)
                {
                    tact = (int32_t)anas[md.destCh-1+CHOUT_BASE]*DEL_MULT * 100 ;
//                    Output.act[i] *=100;
#if GVARS
                    if(mixweight) tact /= mixweight ;
#else
                    if(md.weight) tact /= md.weight;
#endif
                }
                diff = v-act[i]/DEL_MULT;
                if(diff) sDelay[i] = (diff<0 ? md.delayUp :  md.delayDown) * 100;
            }

            if(sDelay[i]){ // perform delay
              if (--sDelay[i] != 0)
              { // At end of delay, use new V and diff
                v = act[i]/DEL_MULT;    // Stay in old position until delay over
                diff = 0;
              }
            }

            if(diff && (md.speedUp || md.speedDown)){
                //rate = steps/sec => 32*1024/100*md.speedUp/Down
                //act[i] += diff>0 ? (32768)/((int16_t)100*md.speedUp) : -(32768)/((int16_t)100*md.speedDown);
                //-100..100 => 32768 ->  100*83886/256 = 32768,   For MAX we divide by 2 since it's asymmetrical

                int32_t rate = (int32_t)DEL_MULT*2048*100;
#if GVARS
                if(mixweight) rate /= abs(mixweight);
#else
                if(md.weight) rate /= abs(md.weight);
#endif
                act[i] = (diff>0) ? ((md.speedUp>0)   ? act[i]+(rate)/((int16_t)100*md.speedUp)   :  (int32_t)v*DEL_MULT) :
                                    ((md.speedDown>0) ? act[i]-(rate)/((int16_t)100*md.speedDown) :  (int32_t)v*DEL_MULT) ;


                if(((diff>0) && (v<(act[i]/DEL_MULT))) || ((diff<0) && (v>(act[i]/DEL_MULT)))) act[i]=(int32_t)v*DEL_MULT; //deal with overflow
                v = act[i]/DEL_MULT;
            }
            else if (diff)
            {
              act[i]=(int32_t)v*DEL_MULT;
            }
        }


        //========== CURVES ===============
        if ( md.differential )
				{
      		//========== DIFFERENTIAL =========
          int16_t curveParam = REG( md.curve, -100, 100 ) ;
      		if (curveParam > 0 && v < 0)
      		  v = ((int32_t)v * (100 - curveParam)) / 100;
      		else if (curveParam < 0 && v > 0)
      		  v = ((int32_t)v * (100 + curveParam)) / 100;
				}
				else
				{
        	switch(md.curve){
        	case 0:
        	    break;
        	case 1:
        	    if(md.srcRaw == MIX_FULL) //FUL
        	    {
        	        if( v<0 ) v=-RESX;   //x|x>0
        	        else      v=-RESX+2*v;
        	    }else{
        	        if( v<0 ) v=0;   //x|x>0
        	    }
        	    break;
        	case 2:
        	    if(md.srcRaw == MIX_FULL) //FUL
        	    {
        	        if( v>0 ) v=RESX;   //x|x<0
        	        else      v=RESX+2*v;
        	    }else{
        	        if( v>0 ) v=0;   //x|x<0
        	    }
        	    break;
        	case 3:       // x|abs(x)
        	    v = abs(v);
        	    break;
        	case 4:       //f|f>0
        	    v = v>0 ? RESX : 0;
        	    break;
        	case 5:       //f|f<0
        	    v = v<0 ? -RESX : 0;
        	    break;
        	case 6:       //f|abs(f)
        	    v = v>0 ? RESX : -RESX;
        	    break;
        	default: //c1..c16
						{
        	    int8_t idx = md.curve ;
							if ( idx < 0 )
							{
								v = -v ;
								idx = 6 - idx ;								
							}
        	   	v = intpol(v, idx - 7);
						}
        	}
				}

        //========== TRIM ===============
        if((md.carryTrim==0) && (md.srcRaw>0) && (md.srcRaw<=4)) v += trimA[md.srcRaw-1];  //  0 = Trim ON  =  Default

        //========== MULTIPLEX ===============
#if GVARS
        int32_t dv = (int32_t)v*mixweight ;
#else
        int32_t dv = (int32_t)v*md.weight;
#endif
        
        //========== lateOffset ===============
        if ( ( md.enableFmTrim == 0 ) && ( md.lateOffset ) )
        {
#if GVARS
            if(md.sOffset) dv += calc100toRESX( REG( md.sOffset, -125, 125 )	) * 100  ;
#else
            if(md.sOffset) dv += calc100toRESX(md.sOffset) * 100 ;
#endif
        }
				switch(md.mltpx){
        case MLTPX_REP:
            chans[md.destCh-1] = dv;
            break;
        case MLTPX_MUL:
            chans[md.destCh-1] *= dv/100l;
            chans[md.destCh-1] /= RESXl;
            break;
        default:  // MLTPX_ADD
            chans[md.destCh-1] += dv; //Mixer output add up to the line (dv + (dv>0 ? 100/2 : -100/2))/(100);
            break;
        }
    }


    //========== MIXER WARNING ===============
    //1= 00,08
    //2= 24,32,40
    //3= 56,64,72,80
    if(mixWarning & 1) if(((g_tmr10ms&0xFF)==  0)) beepWarn1();
    if(mixWarning & 2) if(((g_tmr10ms&0xFF)== 64) || ((g_tmr10ms&0xFF)== 72)) beepWarn1();
    if(mixWarning & 4) if(((g_tmr10ms&0xFF)==128) || ((g_tmr10ms&0xFF)==136) || ((g_tmr10ms&0xFF)==144)) beepWarn1();


    //========== LIMITS ===============
    for(uint8_t i=0;i<NUM_CHNOUT;i++){
        // chans[i] holds data from mixer.   chans[i] = v*weight => 1024*100
        // later we multiply by the limit (up to 100) and then we need to normalize
        // at the end chans[i] = chans[i]/100 =>  -1024..1024
        // interpolate value with min/max so we get smooth motion from center to stop
        // this limits based on v original values and min=-1024, max=1024  RESX=1024


        int32_t q = chans[i];// + (int32_t)g_model.limitData[i].offset*100; // offset before limit

        chans[i] /= 100; // chans back to -1024..1024
        ex_chans[i] = chans[i]; //for getswitch

        int16_t ofs = g_model.limitData[i].offset;
				int16_t xofs = ofs ;
				if ( xofs > g_model.sub_trim_limit )
				{
					xofs = g_model.sub_trim_limit ;
				}
				else if ( xofs < -g_model.sub_trim_limit )
				{
					xofs = -g_model.sub_trim_limit ;
				}
        int16_t lim_p = 10*(g_model.limitData[i].max+100) + xofs ;
        int16_t lim_n = 10*(g_model.limitData[i].min-100) + xofs ; //multiply by 10 to get same range as ofs (-1000..1000)
				if ( lim_p > 1250 )
				{
					lim_p = 1250 ;
				}
				if ( lim_n < -1250 )
				{
					lim_n = -1250 ;
				}
        if(ofs>lim_p) ofs = lim_p;
        if(ofs<lim_n) ofs = lim_n;

        if(q)
				{
					int16_t temp = (q<0) ? ((int16_t)ofs-lim_n) : ((int16_t)lim_p-ofs) ;
          q = ( q * temp ) / 100000 ; //div by 100000 -> output = -1024..1024
				}
				
				int16_t result ;
				result = calc1000toRESX(ofs);
  			result += q ; // we convert value to a 16bit value
        
				lim_p = calc1000toRESX(lim_p);
        lim_n = calc1000toRESX(lim_n);
        if(result>lim_p) result = lim_p;
        if(result<lim_n) result = lim_n;

        if(g_model.limitData[i].revert) result = -result ;// finally do the reverse.

				{
					uint8_t numSafety = 16 - g_model.numVoice ;
					if ( i < numSafety )
					{
        		if(g_model.safetySw[i].opt.ss.swtch)  //if safety sw available for channel check and replace val if needed
						{
							if ( ( g_model.safetySw[i].opt.ss.mode != 1 ) && ( g_model.safetySw[i].opt.ss.mode != 2 ) )	// And not used as an alarm
							{
        		    if(getSwitch(g_model.safetySw[i].opt.ss.swtch,0)) result = calc100toRESX(g_model.safetySw[i].opt.ss.val) ;
							}
						}
					}
				}
        //cli();
        chanOut[i] = result ; //copy consistent word to int-level
        //sei();
    }
}


void simulatorDialog::on_holdLeftX_clicked(bool checked)
{
    nodeLeft->setCenteringX(!checked);
}

void simulatorDialog::on_holdLeftY_clicked(bool checked)
{
    nodeLeft->setCenteringY(!checked);
}

void simulatorDialog::on_holdRightX_clicked(bool checked)
{
    nodeRight->setCenteringX(!checked);
}

void simulatorDialog::on_holdRightY_clicked(bool checked)
{
    nodeRight->setCenteringY(!checked);
}


void simulatorDialog::on_FixLeftX_clicked(bool checked)
{
    nodeLeft->setFixedX(checked);
}

void simulatorDialog::on_FixLeftY_clicked(bool checked)
{
    nodeLeft->setFixedY(checked);
}

void simulatorDialog::on_FixRightX_clicked(bool checked)
{
    nodeRight->setFixedX(checked);
}

void simulatorDialog::on_FixRightY_clicked(bool checked)
{
    nodeRight->setFixedY(checked);
}

