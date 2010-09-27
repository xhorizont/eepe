/*
 * Author - Erez Raviv <erezraviv@gmail.com>
 *
 * Based on th9x -> http://code.google.com/p/th9x/
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */


#ifndef pers_h
#define pers_h

#define VERS 1

#include <inttypes.h>
#include <string.h>



//#define eeprom_write_block eeWriteBlockCmp
// bs=16  128 blocks    verlust link:128  16files:16*8  128     sum 256
// bs=32   64 blocks    verlust link: 64  16files:16*16 256     sum 320
//
#  define EESIZE   2048
#  define BS       16
#  define RESV     64  //reserv for eeprom header with directory (eeFs)
#define FIRSTBLK (RESV/BS)
#define BLOCKS   (EESIZE/BS)
#define EEFS_VERS 4

#define MAX_MODELS 16
#define MAXFILES (1+MAX_MODELS+3)

struct DirEnt{
  uint8_t  startBlk;
  uint16_t size:12;
  uint16_t typ:4;
}__attribute__((packed));

struct EeFs{
  uint8_t  version;
  uint8_t  mySize;
  uint8_t  freeList;
  uint8_t  bs;
  DirEnt   files[MAXFILES];
}__attribute__((packed));










const uint8_t modn12x3[4][4]= {
  {1, 2, 3, 4},
  {1, 3, 2, 4},
  {4, 2, 3, 1},
  {4, 3, 2, 1} };



//convert from mode 1 to mode g_eeGeneral.stickMode
//NOTICE!  =>  1..4 -> 1..4
#define CONVERT_MODE(x) (((x)<=4) ? modn12x3[g_eeGeneral.stickMode][((x)-1)] : (x))

enum EnumKeys {
  KEY_MENU ,
  KEY_EXIT ,
  KEY_DOWN ,
  KEY_UP  ,
  KEY_RIGHT ,
  KEY_LEFT ,
  TRM_LH_DWN  ,
  TRM_LH_UP   ,
  TRM_LV_DWN  ,
  TRM_LV_UP   ,
  TRM_RV_DWN  ,
  TRM_RV_UP   ,
  TRM_RH_DWN  ,
  TRM_RH_UP   ,
  //SW_NC     ,
  //SW_ON     ,
  SW_ThrCt  ,
  SW_RuddDR ,
  SW_ElevDR ,
  SW_ID0    ,
  SW_ID1    ,
  SW_ID2    ,
  SW_AileDR ,
  SW_Gear   ,
  SW_Trainer
};

#define SWITCHES_STR "THR""RUD""ELE""ID0""ID1""ID2""AIL""GEA""TRN""SW1""SW2""SW3""SW4""SW5""SW6"
#define NUM_CSW  6 //number of custom switches

//#define SW_BASE      SW_NC
#define SW_BASE      SW_ThrCt
#define SW_BASE_DIAG SW_ThrCt
//#define SWITCHES_STR "  NC  ON THR RUD ELE ID0 ID1 ID2 AILGEARTRNR"
#define MAX_DRSWITCH (1+SW_Trainer-SW_ThrCt+1+NUM_CSW)



#define DSW_THR  1
#define DSW_RUD  2
#define DSW_ELE  3
#define DSW_ID0  4
#define DSW_ID1  5
#define DSW_ID2  6
#define DSW_AIL  7
#define DSW_GEA  8
#define DSW_TRN  9



#define NUM_KEYS TRM_RH_UP+1
#define TRM_BASE TRM_LH_DWN


#define TMRMODE_NONE     0
#define TMRMODE_ABS      1
#define TMRMODE_THR      2
#define TMRMODE_THR_REL  3
#define MAX_ALERT_TIME   60

#define PROTO_PPM        0
#define PROTO_SILV_A     1
#define PROTO_SILV_B     2
#define PROTO_SILV_C     3
#define PROTO_TRACER_CTP1009 4
#define PROT_MAX         4
#define PROT_STR "PPM   SILV_ASILV_BSILV_CTRAC09"
#define PROT_STR_LEN     6



#define GETADC_SING = 0
#define GETADC_OSMP = 1
#define GETADC_FILT = 2


/// liefert Dimension eines Arrays
#define DIM(arr) (sizeof((arr))/sizeof((arr)[0]))


#define EE_GENERAL 1
#define EE_MODEL   2
/// Markiert einen EEPROM-Bereich als dirty. der Bereich wird dann in
/// eeCheck ins EEPROM zurueckgeschrieben.
void eeCheck(uint8_t msk);
//void eeWriteGeneral();
void eeLoadModelName(uint8_t id,char*buf,uint8_t len);
void eeLoadModel(uint8_t id);
//void eeSaveModel(uint8_t id);
bool eeDuplicateModel(uint8_t id);
bool eeLoadGeneral();

#define NUM_PPM     8
//number of real outputchannels CH1-CH8
#define NUM_CHNOUT   16
///number of real input channels (1-9) plus virtual input channels X1-X4
#define NUM_XCHNRAW (NUM_CHNOUT+9+NUM_PPM) // NUMCH + P1P2P3+ AIL/RUD/ELE/THR + MAX/FULL
///number of real output channels (CH1-CH8) plus virtual output channels X1-X4
#define NUM_XCHNOUT (NUM_CHNOUT) //(NUM_CHNOUT)//+NUM_VIRT)



#define TMR_VAROFS  16

#define SUB_MODE_V     1
#define SUB_MODE_H     2
#define SUB_MODE_H_DBL 3
//uint8_t checkSubGen(uint8_t event,uint8_t num, uint8_t sub, uint8_t mode);




#include "file.h"
#include "myeeprom.h"


class EEPFILE
{

    EEGeneral g_eeGeneral;
    ModelData g_model;
    EFile theFile;
    bool fileChanged;




public:
    EEPFILE();

    bool Changed();
    void setChanged(bool v);
    void loadFile(void* buf);
    void saveFile(void* buf);


    void eeCheck(uint8_t msk);
    bool eeDuplicateModel(uint8_t id);
    void eeLoadModel(uint8_t id);
    void eeLoadModelName(uint8_t id,char*buf,uint8_t len);
    void modelDefault(uint8_t id);
    bool eeLoadGeneral();
    void generalDefault();

};




#endif //pers_h


