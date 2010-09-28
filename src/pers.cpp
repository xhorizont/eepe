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

#include "pers.h"
#include "file.h"

#define FILE_TYP_GENERAL 1
#define FILE_TYP_MODEL   2


EEPFILE::EEPFILE()
{
    //EFile *theFile = new EFile();
    fileChanged = false;
    memset(&g_eeGeneral,0,sizeof(g_eeGeneral));
    memset(&g_model,0,sizeof(g_model));


    generalDefault();
    putGeneralSettings(&g_eeGeneral);

}

bool EEPFILE::Changed()
{
    return fileChanged;
}

void EEPFILE::loadFile(void* buf)
{
    theFile.load(buf);
}


void EEPFILE::saveFile(void* buf)
{
    theFile.save(buf);
}

void EEPFILE::setChanged(bool v)
{
    fileChanged = v;
}

void EEPFILE::generalDefault()
{
  memset(&g_eeGeneral,0,sizeof(g_eeGeneral));
  g_eeGeneral.myVers   =  GENERAL_MYVER;
  g_eeGeneral.currModel=  0;
  g_eeGeneral.contrast = 25;
  g_eeGeneral.vBatWarn = 90;
  g_eeGeneral.stickMode=  1;
  for (int i = 0; i < 7; ++i) {
    g_eeGeneral.calibMid[i]     = 0x200;
    g_eeGeneral.calibSpanNeg[i] = 0x180;
    g_eeGeneral.calibSpanPos[i] = 0x180;
  }
  int16_t sum=0;
  for(int i=0; i<12;i++) sum+=g_eeGeneral.calibMid[i];
  g_eeGeneral.chkSum = sum;
}

bool EEPFILE::eeLoadGeneral()
{
  bool ret = getGeneralSettings(&g_eeGeneral);

  if (!ret) return false;

  uint16_t sum=0;
  if(g_eeGeneral.myVers==GENERAL_MYVER){
    for(int i=0; i<12;i++) sum+=g_eeGeneral.calibMid[i];
    return g_eeGeneral.chkSum == sum;
  }
  return false;
}

void EEPFILE::modelDefault(uint8_t id)
{
  memset(&g_model, 0, sizeof(g_model));
  //strcpy_P(g_model.name,PSTR("MODEL     "));
  g_model.name[5]='0'+(id+1)/10;
  g_model.name[6]='0'+(id+1)%10;
  g_model.mdVers = MDVERS;

  for(uint8_t i= 0; i<4; i++){
    g_model.mixData[i].destCh = i+1;
    g_model.mixData[i].srcRaw = i+1;
    g_model.mixData[i].weight = 100;
  }
}

void EEPFILE::DeleteModel(uint8_t id)
{
    theFile.rm(id);
}


void EEPFILE::eeLoadModelName(uint8_t id,char*buf,uint8_t len)
{
  if(id<MAX_MODELS)
  {
    //eeprom_read_block(buf,(void*)modelEeOfs(id),sizeof(g_model.name));
    theFile.openRd(FILE_MODEL(id));
    memset(buf,' ',len);
    *buf='0'+(id+1)/10; buf++;
    *buf='0'+(id+1)%10; buf++;
    *buf=':';           buf++;buf++;
    uint16_t res = theFile.readRlc((uint8_t*)buf,sizeof(g_model.name));
    if(res == sizeof(g_model.name) )
    {
      uint16_t sz=theFile.size();
      buf+=len-4;
      while(sz){ --buf; *buf='0'+sz%10; sz/=10;}
    }
  }
}

void EEPFILE::curmodelName(char* buf)
{
    memset(buf,0,sizeof(g_model.name)+1);
    memcpy(buf,&g_model.name,sizeof(g_model.name));
}

bool EEPFILE::eeLoadModel(uint8_t id)
{
  return getModel(&g_model, id);
}



bool EEPFILE::getModel(ModelData* model, uint8_t id)
{
    uint16_t sz = 0;

    if(id<MAX_MODELS)
    {
      theFile.openRd(FILE_MODEL(id));
      sz = theFile.readRlc((uint8_t*)model, sizeof(g_model));
    }

    return (sz == sizeof(ModelData));
}

bool EEPFILE::putModel(ModelData* model, uint8_t id)
{
    uint16_t sz = 0;

    if(id<MAX_MODELS)
    {
        sz = theFile.writeRlc(FILE_TMP, FILE_TYP_MODEL, (uint8_t*)model, sizeof(ModelData));
        if(sz == sizeof(ModelData))
            theFile.swap(FILE_MODEL(id),FILE_TMP);
    }

    return (sz == sizeof(ModelData));
}

bool EEPFILE::getGeneralSettings(EEGeneral* setData)
{
    theFile.openRd(FILE_GENERAL);
    uint8_t sz = theFile.readRlc((uint8_t*)setData, sizeof(EEGeneral));

    return (sz == sizeof(EEGeneral));
}

bool EEPFILE::putGeneralSettings(EEGeneral* setData)
{
    uint16_t sz = 0;

    sz = theFile.writeRlc(FILE_TMP, FILE_TYP_GENERAL, (uint8_t*)setData, sizeof(EEGeneral));
    if(sz == sizeof(EEGeneral)) theFile.swap(FILE_GENERAL,FILE_TMP);

    return (sz == sizeof(EEGeneral));
}
