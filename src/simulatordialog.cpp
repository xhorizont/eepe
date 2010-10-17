#include "simulatordialog.h"
#include "ui_simulatordialog.h"
#include "node.h"
#include <QtGui>
#include <inttypes.h>
#include "pers.h"

#define GBALL_SIZE  20

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



simulatorDialog::simulatorDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::simulatorDialog)
{
    ui->setupUi(this);

    beepVal = 0;
    beepShow = 0;

    setupSticks();
    setupTimer();
}

simulatorDialog::~simulatorDialog()
{
    delete ui;
}

void simulatorDialog::setupTimer()
{
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(timerEvent()));
    getValues();
    perOut(true);
    timer->start(10);

}

void simulatorDialog::timerEvent()
{
    g_tmr10ms++;
    getValues();
    perOut();
    setValues();
    centerSticks();

    if(beepVal)
    {
        beepVal = 0;
        QApplication::beep();
    }


#define CBEEP_ON  "QLabel { background-color: #FF364E }"
#define CBEEP_OFF "QLabel { }"

    ui->label_beep->setStyleSheet(beepShow ? CBEEP_ON : CBEEP_OFF);
    if(beepShow) beepShow--;
}

void simulatorDialog::centerSticks()
{
    if(ui->leftStick->scene()) nodeLeft->stepToCenter();
    if(ui->rightStick->scene()) nodeRight->stepToCenter();
}

void simulatorDialog::loadParams(EEGeneral *gg, ModelData *gm)
{
    memcpy(&g_eeGeneral,gg,sizeof(EEGeneral));
    memcpy(&g_model,gm,sizeof(ModelData));

    char buf[sizeof(g_model.name)+1];
    memcpy(&buf,&g_model.name,sizeof(g_model.name));
    buf[sizeof(g_model.name)] = 0;
    this->setWindowTitle("Simulating " + QString(buf));

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


    ui->trimHLeft->setValue(g_model.trim[0]);
    ui->trimVLeft->setValue(g_model.trim[1]);
    ui->trimVRight->setValue(g_model.trim[2]);
    ui->trimHRight->setValue(g_model.trim[3]);
}

void simulatorDialog::getValues()
{

    calibratedStick[0] = 1024*nodeLeft->getX(); //RUD
    calibratedStick[1] = -1024*nodeLeft->getY(); //ELE
    calibratedStick[2] = -1024*nodeRight->getY(); //THR
    calibratedStick[3] = 1024*nodeRight->getX(); //AIL

    trim[0] = ui->trimHLeft->value();
    trim[1] = ui->trimVLeft->value();
    trim[2] = ui->trimVRight->value();
    trim[3] = ui->trimHRight->value();

    calibratedStick[4] = ui->dialP_1->value();
    calibratedStick[5] = ui->dialP_2->value();
    calibratedStick[6] = ui->dialP_3->value();

    if(g_eeGeneral.throttleReversed)
    {
        if(g_eeGeneral.stickMode & 1)//mode 1,3 -> THR on left
        {
            calibratedStick[1] = 1024*nodeLeft->getY(); //ELE
            trim[1] = -ui->trimVLeft->value();
        }
        else //mode 1,3 -> THR on right
        {
            calibratedStick[2] = 1024*nodeRight->getY(); //THR
            trim[2] = -ui->trimVRight->value();
        }
    }

    if(g_model.thrTrim)
    {
        int i = 1+(g_eeGeneral.stickMode & 1);
        int j = trim[i];

        trim[i] = (g_eeGeneral.throttleReversed) ? ((qint32)j-125)*(RESX+calibratedStick[i])/(2*RESX) :
                                                   ((qint32)j+125)*(RESX-calibratedStick[i])/(2*RESX);
    }

}

inline int chVal(int val)
{
    return qMin(1024, qMax(-1024, val));
}

void simulatorDialog::setValues()
{
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

    ui->chnoutV_1->setText(tr("%1").arg((qreal)chanOut[0]*100/1024, 0, 'f', 1));
    ui->chnoutV_2->setText(tr("%1").arg((qreal)chanOut[1]*100/1024, 0, 'f', 1));
    ui->chnoutV_3->setText(tr("%1").arg((qreal)chanOut[2]*100/1024, 0, 'f', 1));
    ui->chnoutV_4->setText(tr("%1").arg((qreal)chanOut[3]*100/1024, 0, 'f', 1));
    ui->chnoutV_5->setText(tr("%1").arg((qreal)chanOut[4]*100/1024, 0, 'f', 1));
    ui->chnoutV_6->setText(tr("%1").arg((qreal)chanOut[5]*100/1024, 0, 'f', 1));
    ui->chnoutV_7->setText(tr("%1").arg((qreal)chanOut[6]*100/1024, 0, 'f', 1));
    ui->chnoutV_8->setText(tr("%1").arg((qreal)chanOut[7]*100/1024, 0, 'f', 1));
    ui->chnoutV_9->setText(tr("%1").arg((qreal)chanOut[8]*100/1024, 0, 'f', 1));
    ui->chnoutV_10->setText(tr("%1").arg((qreal)chanOut[9]*100/1024, 0, 'f', 1));
    ui->chnoutV_11->setText(tr("%1").arg((qreal)chanOut[10]*100/1024, 0, 'f', 1));
    ui->chnoutV_12->setText(tr("%1").arg((qreal)chanOut[11]*100/1024, 0, 'f', 1));
    ui->chnoutV_13->setText(tr("%1").arg((qreal)chanOut[12]*100/1024, 0, 'f', 1));
    ui->chnoutV_14->setText(tr("%1").arg((qreal)chanOut[13]*100/1024, 0, 'f', 1));
    ui->chnoutV_15->setText(tr("%1").arg((qreal)chanOut[14]*100/1024, 0, 'f', 1));
    ui->chnoutV_16->setText(tr("%1").arg((qreal)chanOut[15]*100/1024, 0, 'f', 1));

#define CSWITCH_ON  "QLabel { background-color: #4CC417 }"
#define CSWITCH_OFF "QLabel { }"

    ui->labelCSW1->setStyleSheet(getSwitch(DSW_CS1,0) ? CSWITCH_ON : CSWITCH_OFF);
    ui->labelCSW2->setStyleSheet(getSwitch(DSW_CS2,0) ? CSWITCH_ON : CSWITCH_OFF);
    ui->labelCSW3->setStyleSheet(getSwitch(DSW_CS3,0) ? CSWITCH_ON : CSWITCH_OFF);
    ui->labelCSW4->setStyleSheet(getSwitch(DSW_CS4,0) ? CSWITCH_ON : CSWITCH_OFF);
    ui->labelCSW5->setStyleSheet(getSwitch(DSW_CS5,0) ? CSWITCH_ON : CSWITCH_OFF);
    ui->labelCSW5->setStyleSheet(getSwitch(DSW_CS6,0) ? CSWITCH_ON : CSWITCH_OFF);
}

void simulatorDialog::beepWarn1()
{
    beepVal = 1;
    beepShow = 20;
}



void simulatorDialog::setupSticks()
{
    QGraphicsScene *leftScene = new QGraphicsScene(ui->leftStick);
    leftScene->setItemIndexMethod(QGraphicsScene::NoIndex);
    ui->leftStick->setScene(leftScene);

    QGraphicsScene *rightScene = new QGraphicsScene(ui->rightStick);
    rightScene->setItemIndexMethod(QGraphicsScene::NoIndex);
    ui->rightStick->setScene(rightScene);


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
  return (qint16)x*10 + x/4;
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

bool simulatorDialog::getSwitch(int swtch, bool nc)
{
    switch(swtch){
       case  0:            return  nc;
       case  MAX_DRSWITCH: return  true;
       case -MAX_DRSWITCH: return  false;
     }

     uint8_t dir = swtch>0;
     if(abs(swtch)<(MAX_DRSWITCH-NUM_CSW)) {
       if(!dir) return ! keyState((EnumKeys)(SW_BASE-swtch-1));
       return            keyState((EnumKeys)(SW_BASE+swtch-1));
     }

     //custom switch, Issue 78
     //use putsChnRaw
     //input -> 1..4 -> sticks,  5..8 pots
     //MAX,FULL - disregard
     //ppm
     CSwData &cs = g_model.customSw[abs(swtch)-(MAX_DRSWITCH-NUM_CSW)];
     int16_t  v = 0;
     uint8_t  i = cs.input-1;
     if(i<MIX_MAX) v = calibratedStick[i];//-512..512
     else if(i<=MIX_FULL) v = 1024; //FULL/MAX
     else if(i<MIX_FULL+NUM_PPM) v = g_ppmIns[i-MIX_FULL] - g_eeGeneral.ppmInCalib[i-MIX_FULL];
     else v = ex_chans[i-MIX_FULL-NUM_PPM];

     int16_t ofs = calc100toRESX(cs.offset); //coffset 100 -> 1024
     switch (cs.func) {
       case (CS_VPOS):   return swtch>0 ? (v>ofs) : !(v>ofs);
       case (CS_VNEG):   return swtch>0 ? (v<ofs) : !(v<ofs);
       case (CS_APOS):   return swtch>0 ? (abs(v)>ofs) : !(abs(v)>ofs);
       case (CS_ANEG):   return swtch>0 ? (abs(v)<ofs) : !(abs(v)<ofs);
       default:          return false;
     }
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

void simulatorDialog::perOut(bool init)
{
  static int16_t  anas [NUM_XCHNRAW];
  static int32_t  chans[NUM_CHNOUT];
  int16_t trimA[4];

  static uint8_t  bpanaCenter;

  uint8_t  anaCenter = 0;

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

    if(!(v/16)) anaCenter |= 1<<(CONVERT_MODE((i+1))-1);


    if(i<4) { //only do this for sticks
      uint8_t expoDrOn = GET_DR_STATE(i);
      uint8_t stkDir = v>0 ? DR_RIGHT : DR_LEFT;

      if(IS_THROTTLE(i) && g_model.thrExpo){
        v  = 2*expo((v+RESX)/2,g_model.expoData[i].expo[expoDrOn][DR_EXPO][DR_RIGHT]);
        stkDir = DR_RIGHT;
      }
      else
        v  = expo(v,g_model.expoData[i].expo[expoDrOn][DR_EXPO][stkDir]);

      int32_t x = (int32_t)v * (g_model.expoData[i].expo[expoDrOn][DR_WEIGHT][stkDir]+100)/100;
      v = (int16_t)x;
      if (IS_THROTTLE(i) && g_model.thrExpo) v -= RESX;

      //do trim -> throttle trim if applicable
      int32_t vv = 2*RESX;
      if(IS_THROTTLE(i) && g_model.thrTrim) vv = (g_eeGeneral.throttleReversed) ?
                                 ((int32_t)trim[i]-125)*(RESX+v)/(2*RESX) :
                                 ((int32_t)trim[i]+125)*(RESX-v)/(2*RESX);

      //trim
      trimA[i] = (vv==2*RESX) ? trim[i]*2 : (int16_t)vv*2; //    if throttle trim -> trim low end
    }
    anas[i] = v; //set values for mixer
  }

  //===========BEEP CENTER================
  anaCenter &= g_model.beepANACenter;
  if(((bpanaCenter ^ anaCenter) & anaCenter)) beepWarn1();
  bpanaCenter = anaCenter;


  anas[MIX_MAX-1]  = RESX;     // MAX
  anas[MIX_FULL-1] = RESX;     // FULL
  for(uint8_t i=PPM_BASE;i<CHOUT_BASE;i++)    anas[i] = g_ppmIns[i-PPM_BASE] - g_eeGeneral.ppmInCalib[i-PPM_BASE]; //add ppm channels
  for(uint8_t i=CHOUT_BASE;i<NUM_XCHNRAW;i++) anas[i] = chans[i-CHOUT_BASE]; //other mixes previous outputs

  memset(chans,0,sizeof(chans));        // All outputs to 0

   uint8_t mixWarning = 0;
    //========== MIXER LOOP ===============
    for(uint8_t i=0;i<MAX_MIXERS;i++){
      MixData &md = g_model.mixData[i];

      if((md.destCh==0) || (md.destCh>NUM_CHNOUT)) break;

      //Notice 0 = NC switch means not used -> always on line
      static uint8_t swOn[MAX_MIXERS];

      int16_t v  = 0;
      uint8_t swTog;

      //swOn[i]=false;
      if(!getSwitch(md.swtch,1)){ // switch on?  if no switch selected => on
        swTog = swOn[i];
        swOn[i] = false;
        if(md.srcRaw!=MIX_MAX && md.srcRaw!=MIX_FULL) continue;// if not MAX or FULL - next loop
        if(md.mltpx==MLTPX_REP) continue; // if switch is off and REPLACE then off
        v = (md.srcRaw == MIX_FULL ? -RESX : 0); // switch is off and it is either MAX=0 or FULL=-512

        //if(md.srcRaw==MIX_MAX) continue;
      }
      else {
        swTog = !swOn[i];
        swOn[i] = true;
        v = anas[md.srcRaw-1]; //Switch is on. MAX=FULL=512 or value.
        if(md.mixWarn) mixWarning |= 1<<(md.mixWarn-1); // Mix warning
      }

      //========== INPUT OFFSET ===============
      if(md.sOffset) v += calc100toRESX(md.sOffset);

      //========== DELAY and PAUSE ===============
      if (md.speedUp || md.speedDown || md.delayUp || md.delayDown)  // there are delay values
      {
        static int16_t sDelay[MAX_MIXERS];
        static int32_t act   [MAX_MIXERS];

#define DEL_MULT 256

        if(init) {
          act[i]=(int32_t)v*DEL_MULT;
          swTog = false;
        }
        int16_t diff = v-act[i]/DEL_MULT;

        if(swTog) {
            //need to know which "v" will give "anas".
            //curves(v)*weight/100 -> anas
            // v * weight / 100 = anas => anas*100/weight = v
          if(md.mltpx==MLTPX_REP)
          {
              act[i] = (int32_t)anas[md.destCh-1+CHOUT_BASE]*DEL_MULT;
              act[i] *=100;
              if(md.weight) act[i] /= md.weight;
          }
          diff = v-act[i]/DEL_MULT;
          if(diff) sDelay[i] = (diff<0 ? md.delayUp :  md.delayDown) * 100;
        }

        if(sDelay[i]){ // perform delay
            sDelay[i]--;
            v = act[i]/DEL_MULT;
            diff = 0;
        }

        if(diff && (md.speedUp || md.speedDown)){
          //rate = steps/sec => 32*1024/100*md.speedUp/Down
          //act[i] += diff>0 ? (32768)/((int16_t)100*md.speedUp) : -(32768)/((int16_t)100*md.speedDown);
          //-100..100 => 32768 ->  100*83886/256 = 32768,   For MAX we divide by 2 since it's asymmetrical

            int32_t rate = (int32_t)DEL_MULT*2048*100;
            if(md.weight) rate /= md.weight;
            act[i] = (diff>0) ? ((md.speedUp>0)   ? act[i]+(rate)/((int16_t)100*md.speedUp)   :  (int32_t)v*DEL_MULT) :
                     ((md.speedDown>0) ? act[i]-(rate)/((int16_t)100*md.speedDown) :  (int32_t)v*DEL_MULT) ;


          if(((diff>0) && (v<(act[i]/DEL_MULT))) || ((diff<0) && (v>(act[i]/DEL_MULT)))) act[i]=(int32_t)v*DEL_MULT; //deal with overflow
          v = act[i]/DEL_MULT;
        }
      }


      //========== CURVES ===============
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
          v = intpol(v, md.curve - 7);
      }

      //========== TRIM ===============
      if((md.carryTrim==0) && (md.srcRaw>0) && (md.srcRaw<=4)) v += trimA[md.srcRaw-1];  //  0 = Trim ON  =  Default

      //========== MULTIPLEX ===============
      int32_t dv = (int32_t)v*md.weight;
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

    int16_t v = 0;
    int16_t lim_p = g_model.limitData[i].max+100;
    int16_t lim_n = g_model.limitData[i].min-100;

    int32_t q = chans[i]; // offset before limit
    if(q) v = (q>0) ? q*lim_p/10000 : -q*lim_n/10000; //div by 10000 -> output = -1024..1024
    chans[i] /= 100; // chans back to -512..512
    ex_chans[i] = chans[i]; //for getswitch

    //impose hard limits
    lim_p = calc100toRESX(lim_p);
    lim_n = calc100toRESX(lim_n);
    if(v>lim_p) v = lim_p;
    if(v<lim_n) v = lim_n;// absolute limits - do not go over!

    v+=g_model.limitData[i].offset;      //offset after limit.
    if(g_model.limitData[i].revert) v=-v;// finally do the reverse.

    //cli();
    if(v>1024)  v =  1024;
    if(v<-1024) v = -1024; //limits set by ppm streamer
    chanOut[i] = v; //copy consistent word to int-level
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

