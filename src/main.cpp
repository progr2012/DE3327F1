/*

  Clock.ino
  
  Demonstrates a clock-like display with a very huge font.
  Also shows usage of u8x8_u8toa().

  Universal 8bit Graphics Library (https://github.com/olikraus/u8g2/)

  Copyright (c) 2016, olikraus@gmail.com
  All rights reserved.

  Redistribution and use in source and binary forms, with or without modification, 
  are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice, this list 
    of conditions and the following disclaimer.
    
  * Redistributions in binary form must reproduce the above copyright notice, this 
    list of conditions and the following disclaimer in the documentation and/or other 
    materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND 
  CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
  NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.  

*/

#include <Arduino.h>


#define NALADKA 1
#define RUCHNOY 2
#define PAVTOMAT 4
#define AVTOMAT 8
#define TOLKATELI 9
#define SQ1 16
#define TEST 15
//------------------------------------------------------
#define U1_YA1 0 //СМЫКАНИЕ
#define U1_YA2 1 //РАЗМЫКАНИЕ
#define U1_YA3 2 //ПОДВОД
#define U1_YA4 3 //ОТВОД
//регулировка скорости
#define U1_YA8 4 //
#define U1_YA9 5
#define U1_YA13 7
#define U1_YA28 6
//                    U2 74HC595
//-------------------------------------------------------
#define U2_YA5 0 //НАБОР
#define U2_YA6 1 //ВПРЫСК
#define U2_YA10 2
#define U2_YA11 3
#define U2_YA12 4
#define U2_YA14 5

//                    U3 74HC595
//-------------------------------------------------------
#define U3_YA31 0
#define U3_YA32 1
#define U3_YA33 2
#define U3_YA34 3
#define U3_YA35 4
#define U3_YA36 5
#define U3_YA7 6
#define U3_YA16 7

//--------------------   КНОПКИ   ----------------------
#define SMYK 1
#define RZMYK 2
#define OTV 4
#define PODV 8
#define VPRISK 16
#define VPRISK_II 17
#define NABOR 32
#define NABOR_YA12 33

#define FORM 128

//==================    БВК   ===========================
#define BVK1 1  // stop smykanie
#define BVK2 2  // stop razmykanie
#define BVK3 4  // stop podvod
#define BVK4 8  // stop otvod
#define BVK5 16 // stop vprisk I st
#define BVK6 32 // stop vprisk II st
#define BVK7 64 // stop nabor
#define BVK8 128 // ïåğåõîä ñ ñìûêàíèÿ è ğàçìûêàíèÿ

#define speed_test 0

uint8_t speed[]={0,
              (1<<U1_YA9)|(1<<U1_YA28),                         //1
              (1<<U1_YA9),                                      //2            
              (1<<U1_YA13)|(1<<U1_YA28),                        //3
              (1<<U1_YA13),                                     //4
              (1<<U1_YA9)|(1<<U1_YA13)|(1<<U1_YA28),            //5
              (1<<U1_YA9)|(1<<U1_YA13),                         //6
              (1<<U1_YA8)|(1<<U1_YA28),                         //7
              (1<<U1_YA8),                                      //8
              (1<<U1_YA8)|(1<<U1_YA9)|(1<<U1_YA28),             //9
              (1<<U1_YA8)|(1<<U1_YA9),                          //10
              (1<<U1_YA8)|(1<<U1_YA13)|(1<<U1_YA28),            //11
              (1<<U1_YA8)|(1<<U1_YA13),                         //12
              (1<<U1_YA8)|(1<<U1_YA9)|(1<<U1_YA13)|(1<<U1_YA28),//13
              (1<<U1_YA8)|(1<<U1_YA9)|(1<<U1_YA13)              //14
              };

uint8_t pressure[]={0,
                 (1<<U3_YA31),
                 (1<<U3_YA32),
                 (1<<U3_YA31)|(1<<U3_YA32),
                 (1<<U3_YA33),
                 (1<<U3_YA31)|(1<<U3_YA33),
                 (1<<U3_YA32)|(1<<U3_YA33),
                 (1<<U3_YA31)|(1<<U3_YA32)|(1<<U3_YA33),
                 (1<<U3_YA34),
                 (1<<U3_YA31)|(1<<U3_YA34),
                 (1<<U3_YA32)|(1<<U3_YA34),
                 (1<<U3_YA31)|(1<<U3_YA32)|(1<<U3_YA34),
                 (1<<U3_YA33)|(1<<U3_YA34),
                 (1<<U3_YA31)|(1<<U3_YA33)|(1<<U3_YA34),
                 (1<<U3_YA32)|(1<<U3_YA33)|(1<<U3_YA34),
                 (1<<U3_YA31)|(1<<U3_YA32)|(1<<U3_YA33)|(1<<U3_YA34),
                 (1<<U3_YA35),
                 (1<<U3_YA31)|(1<<U3_YA35),
                 (1<<U3_YA32)|(1<<U3_YA35),
                 (1<<U3_YA31)|(1<<U3_YA32)|(1<<U3_YA35),
                 (1<<U3_YA33)|(1<<U3_YA35),
                 (1<<U3_YA31)|(1<<U3_YA33)|(1<<U3_YA35),
                 (1<<U3_YA32)|(1<<U3_YA33)|(1<<U3_YA35),
                 (1<<U3_YA31)|(1<<U3_YA32)|(1<<U3_YA33)|(1<<U3_YA35),
                 (1<<U3_YA34)|(1<<U3_YA35),
                 (1<<U3_YA31)|(1<<U3_YA34)|(1<<U3_YA35),
                 (1<<U3_YA32)|(1<<U3_YA34)|(1<<U3_YA35),
                 (1<<U3_YA31)|(1<<U3_YA32)|(1<<U3_YA34)|(1<<U3_YA35),
                 (1<<U3_YA33)|(1<<U3_YA34)|(1<<U3_YA35),
                 (1<<U3_YA31)|(1<<U3_YA33)|(1<<U3_YA34)|(1<<U3_YA35),
                 (1<<U3_YA32)|(1<<U3_YA33)|(1<<U3_YA34)|(1<<U3_YA35),
                 (1<<U3_YA31)|(1<<U3_YA32)|(1<<U3_YA33)|(1<<U3_YA34)|(1<<U3_YA35),
                 (1<<U3_YA31)|(1<<U3_YA36),
                 (1<<U3_YA32)|(1<<U3_YA36),
                 (1<<U3_YA31)|(1<<U3_YA32),(1<<U3_YA36),
                 (1<<U3_YA33)|(1<<U3_YA36),
                 (1<<U3_YA31)|(1<<U3_YA33)|(1<<U3_YA36),
                 (1<<U3_YA32)|(1<<U3_YA33)|(1<<U3_YA36),
                 (1<<U3_YA31)|(1<<U3_YA32)|(1<<U3_YA33)|(1<<U3_YA36),
                 (1<<U3_YA34)|(1<<U3_YA36)
                };


#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

#include <ESP32Encoder.h>

QueueHandle_t queue_encoder;
QueueHandle_t queue_bvk;

SemaphoreHandle_t Serial_mutex = NULL;

bool exit_item = false;
bool test_operation = false;


#include "MCP23008.h"
#include "Wire.h"
uint8_t adress_i2c[3]={0x20,0x21,0x22};
MCP23008 MCP[3]={MCP23008(adress_i2c[0]),MCP23008(adress_i2c[1]),MCP23008(adress_i2c[2])};


#include <GyverHC595.h>
GyverHC595<3, HC_PINS,1000> GK(13, 12, 14); // программный SPI (bit bang)  CS-12 DT-13 CLK-14  SH-14  DS-12  ST-13


// ловим степ с накликиванием
#define BTN_PIN 4				// кнопка подключена сюда (BTN_PIN --- КНОПКА --- GND)
#include "GyverButton.h"
GButton butt1(BTN_PIN);


/*
  U8g2lib Example Overview:
    Frame Buffer Examples: clearBuffer/sendBuffer. Fast, but may not work with all Arduino boards because of RAM consumption
    Page Buffer Examples: firstPage/nextPage. Less RAM usage, should work with all Arduino boards.
    U8x8 Text Only Example: No RAM usage, direct communication with display controller. No graphics, 8x8 Text only.
    
  This is a page buffer example.    
*/


U8G2_ST7565_ERC12864_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 5, /* rs=*/ 19, /* rse=*/ 15); //cs-5;rse-15;rs-19;scl-18;si-23;vdd-+3.3v;vss-GND;A-3.3V;K-GND

struct Item{
  const char *name;
  uint8_t value;
};

struct Setting_item
{
    Item item[10];
    uint8_t count;
    const char *name;

};
struct Item_encoder
{
    uint8_t position;
    uint8_t count;
    uint8_t item;
    uint8_t select_item_menu;
    uint8_t value_bvk[3];
    uint8_t state;  ///0-настройки, 1-тест БВК
};

struct Item_bvk
{
  uint8_t value[3];
};

uint8_t cnt_tolkatel=0;
uint8_t cnt_nabor=0;
#define MAX_Setting_DE3327 11

Setting_item Setting_de3327[MAX_Setting_DE3327];

//ESP32Encoder encoder(true, enc_cb);
ESP32Encoder encoder;


////***************** ТАЙМЕРЫ  *******************************
 //#define NUM_TIMERS 5

 /* An array to hold handles to the created timers. */
 //TimerHandle_t xTimers[ NUM_TIMERS ];
 TimerHandle_t Timer_M_CYCL,Timer_dozat,Timer_ochlad,Timer_tolkatel,Timer_nabor;

 


void TPA_action(uint8_t action,uint8_t regim){
  switch(action){
    case SMYK: //СМЫКАНИЕ
      GK.writeByte(2,(1<<Setting_de3327[0].item[2].value)|speed[Setting_de3327[0].item[0].value]);
      GK.update();
    break;
    case RZMYK: //РАЗМЫКАНИЕ
          GK.writeByte(2,(1<<Setting_de3327[1].item[2].value)|speed[Setting_de3327[1].item[0].value]);
          GK.update();
    break;
    case PODV: //ПОДВОД
      GK.writeByte(2,(1<<Setting_de3327[2].item[2].value)|speed[Setting_de3327[2].item[0].value]);
      GK.update();
    break;
    case OTV: //ОТВОД
      GK.writeByte(2,(1<<Setting_de3327[3].item[2].value)|speed[Setting_de3327[3].item[0].value]);
      GK.update();
    break;
    case VPRISK: //ВПРЫСК
      switch (regim)
      {
      case NALADKA:
        GK.clearAll();
        GK.buffer[0] = Setting_de3327[4].item[1].value;
        GK.writeByte(2,speed[Setting_de3327[4].item[0].value]);
        GK.writeByte(1,(1<<Setting_de3327[4].item[2].value));
        //GK.writeByte(0,pressure[Setting_de3327[4].item[1].value]);
        GK.writeByte(0,Setting_de3327[4].item[1].value);
        GK.update();
      break;
      case RUCHNOY:        ///это смыкание                     //это подвод печки                 //это скорость
        GK.clearAll();
        GK.buffer[0] = Setting_de3327[4].item[1].value;
        GK.writeByte(2,(0<<Setting_de3327[0].item[2].value)|(0<<Setting_de3327[3].item[2].value)|speed[Setting_de3327[4].item[0].value]);
        GK.writeByte(1,(1<<Setting_de3327[4].item[2].value));/// это впрыск
        GK.writeByte(0,Setting_de3327[4].item[1].value);
        //GK.writeByte(0,pressure[Setting_de3327[4].item[1].value]);
        GK.update();
      break;
      case PAVTOMAT:        ///это смыкание                     //это подвод печки                 //это скорость
        GK.clearAll();
        GK.buffer[0] = Setting_de3327[4].item[1].value;
        GK.writeByte(2,(1<<Setting_de3327[0].item[2].value)|(1<<Setting_de3327[3].item[2].value)|speed[Setting_de3327[4].item[0].value]);
        GK.writeByte(1,(1<<Setting_de3327[4].item[2].value));/// это впрыск
        GK.writeByte(0,Setting_de3327[4].item[1].value);
        //GK.writeByte(0,pressure[Setting_de3327[4].item[1].value]);
        GK.update();
      break;
      }
    break;
    case VPRISK_II:
        GK.clearAll();
        GK.buffer[0] = Setting_de3327[4].item[1].value;
        GK.writeByte(2,(1<<Setting_de3327[0].item[2].value)|(1<<Setting_de3327[3].item[2].value)|speed[Setting_de3327[5].item[0].value]);
        GK.writeByte(1,(1<<Setting_de3327[4].item[2].value));/// это впрыск
        GK.writeByte(0,Setting_de3327[5].item[1].value);
        //GK.writeByte(0,pressure[Setting_de3327[5].item[1].value]);
        GK.update();
    break;
    case NABOR: //ВПРЫСК
      GK.clearAll();
      GK.buffer[0] = Setting_de3327[6].item[1].value;
      GK.writeByte(2,speed[Setting_de3327[6].item[0].value]);
      GK.writeByte(1,(1<<Setting_de3327[6].item[2].value)|(0<<U2_YA12));
      //GK.writeByte(1,(1<<Setting_de3327[6].item[2].value)|(1<<U2_YA12));
      //GK.writeByte(0,pressure[Setting_de3327[6].item[1].value]);
      //GK.writeByte(0,Setting_de3327[6].item[1].value);
      GK.update();
      ///// сюда нужно дописать включение таймера охлаждения, после окончания нужно раскрыть перссформу
    break;
    case NABOR_YA12: //ВПРЫСК
      GK.clearAll();
      GK.buffer[0] = Setting_de3327[6].item[1].value;
      GK.writeByte(2,speed[Setting_de3327[6].item[0].value]);
      GK.writeByte(1,(1<<Setting_de3327[6].item[2].value)|(1<<U2_YA12));
      //GK.writeByte(0,Setting_de3327[6].item[1].value);
      GK.update();
      ///// сюда нужно дописать включение таймера охлаждения, после окончания нужно раскрыть перссформу
    break;
  }
}

void vTimerCallback_M_CYCL( TimerHandle_t xTimer )
 {
  //GK.writeByte(2,(1<<Setting_de3327[0].item[2].value)|speed[Setting_de3327[0].item[0].value]);// это смыкание
  //GK.update();
  TPA_action(SMYK,RUCHNOY);
 }

 void vTimerCallback_dozat( TimerHandle_t xTimer )
 {
  //GK.writeByte(2,(1<<Setting_de3327[0].item[2].value)|speed[Setting_de3327[0].item[0].value]);// это 
  //GK.update();
  
  //TPA_action(NABOR,RUCHNOY);/// это набор
  xTimerStart(Timer_nabor,0); /// это набор
 }

 void vTimerCallback_ochlad( TimerHandle_t xTimer )
 {
  //GK.writeByte(2,(1<<Setting_de3327[0].item[2].value)|speed[Setting_de3327[0].item[0].value]);// это смыкание
  //GK.update();
  TPA_action(RZMYK,RUCHNOY);// это размыкание
 }

 void vTimerCallback_tolkatel( TimerHandle_t xTimer )
 {
  //GK.writeByte(2,(1<<Setting_de3327[0].item[2].value)|speed[Setting_de3327[0].item[0].value]);// это смыкание
  //GK.update();
  cnt_tolkatel++;
  TPA_action(RZMYK,RUCHNOY);// это размыкание
 }
 void vTimerCallback_nabor( TimerHandle_t xTimer )
 {
   cnt_nabor++;
   if(cnt_nabor<=Setting_de3327[7].item[0].value){
     TPA_action(NABOR_YA12,RUCHNOY);// это набор с YA12
     digitalWrite(2,true);
   }else{
     TPA_action(NABOR,RUCHNOY);// это набор без YA12
     digitalWrite(2,false);
   }
  
 }


void vTaskEncoder( void * pvParameters )
{
  uint8_t pin,dir;
  Item_encoder item_encoder;
  item_encoder.position = 0;
  item_encoder.item = 0;
  item_encoder.select_item_menu= 0;
  item_encoder.count= 0;
  uint8_t old_cnt_encoder=0;

  butt1.setDebounce(80);        // настройка антидребезга (по умолчанию 80 мс)
  butt1.setTimeout(1000);        // настройка таймаута на удержание (по умолчанию 500 мс)
  butt1.setClickTimeout(300);   // настройка таймаута между кликами (по умолчанию 300 мс)     


  //ESP32Encoder::useInternalWeakPullResistors=DOWN;
	// Enable the weak pull up resistors
	ESP32Encoder::useInternalWeakPullResistors=UP;

	// use pin 19 and 18 for the first encoder
	encoder.attachSingleEdge(16, 17);
  encoder.clearCount();
  encoder.setFilter(1023); //1023
  if(xSemaphoreTake(Serial_mutex, portMAX_DELAY) == pdTRUE){
    Serial.println("Start task encoder \n");
    Serial.printf("Остаток %d",11%3);
    }
  xSemaphoreGive(Serial_mutex);

  
  for( ;; )
  {   
    
      if(old_cnt_encoder!=encoder.getCount()){
        old_cnt_encoder= encoder.getCount();
        item_encoder.count = old_cnt_encoder;
        item_encoder.state=0;
      //Serial.printf("SEND Position %d Item %d Select_item %d Count %d \n",item_encoder.position,item_encoder.item,item_encoder.select_item_menu,item_encoder.count);
      xQueueSend(queue_encoder, &item_encoder, portMAX_DELAY);
      }else
      {
        vTaskDelay(50);
      }
      
      
    // Serial.printf("Encoder %d \n",encoder.getCount());
        butt1.tick();
        //vTaskDelay(300);
      if(butt1.isClick()){
          switch(exit_item){
            case false:
            {
              if(test_operation==false){
                item_encoder.position++;
                item_encoder.select_item_menu = item_encoder.count;
              }
              
               
              if(item_encoder.position==1&&!test_operation){ //Если мы заходим в подменю
                item_encoder.item = item_encoder.count;
                item_encoder.count=0;
                encoder.clearCount();
              }
              
              if(item_encoder.position==2){
                encoder.setCount(Setting_de3327[item_encoder.item].item[item_encoder.count].value);
                item_encoder.count = Setting_de3327[item_encoder.item].item[item_encoder.count].value;
              }
             
              
              //Serial.printf("SEND Position %d Item %d Select_item %d Count %d \n",item_encoder.position,item_encoder.item,item_encoder.select_item_menu,item_encoder.count);
              xQueueSend(queue_encoder, &item_encoder, portMAX_DELAY);
              //Serial.printf("Position %d Item %d Select_item %d Count %d \n",item_encoder.position,item_encoder.item,item_encoder.item.item[item_encoder.select_item_menu].name,item_encoder.count);
              //encoder.clearCount();
               if(test_operation==true){
                test_operation=false;
                Serial.println("Test operation!!!");
              }
            break;
            };
            case true:{
              exit_item = false;
              test_operation = false;
              item_encoder.position--;
              if(item_encoder.position==1){
                item_encoder.count = item_encoder.select_item_menu;
                encoder.setCount(item_encoder.count);
                }
              if(item_encoder.position==0){encoder.setCount(item_encoder.item);}
              if(item_encoder.position==255){item_encoder.position=0;}
              //Serial.printf("SEND Position %d Item %d Select_item %d Count %d \n",item_encoder.position,item_encoder.item,item_encoder.select_item_menu,item_encoder.count);
              xQueueSend(queue_encoder, &item_encoder, portMAX_DELAY);
              //Serial.printf("Position %d Item %s Select_item %d Count %d \n",item_encoder.position,item_encoder.item.name,item_encoder.select_item_menu,item_encoder.count);
              //
              break;
            }
          }
        }
      //vTaskDelay(200/portTICK_PERIOD_MS);
  }    
  
}
  
void Setting_write(uint8_t sett,uint8_t sel_item,uint8_t value){
    uint8_t x;
    u8g2.clearBuffer();
    u8g2.drawUTF8(25,12,Setting_de3327[sett].name);
    u8g2.drawHLine(0, 14, u8g2.getDisplayWidth());

      for(x=0;x<Setting_de3327[sett].count;x++){
        if(x==sel_item){
          u8g2.drawButtonUTF8(100,28+(14*x),U8G2_BTN_INV, 60, 20,  2, u8x8_u8toa(value,3));
          Setting_de3327[sett].item[sel_item].value = value;
          exit_item = true;
        }else
        {
          u8g2.drawUTF8(100,28+(14*(x)),u8x8_u8toa(Setting_de3327[sett].item[x].value,3));
        }
        u8g2.drawUTF8(0, 28+(14*x), Setting_de3327[sett].item[x].name);
      }
      
      if(sel_item==Setting_de3327[sett].count){
        u8g2.drawButtonUTF8(0, 28+(14*Setting_de3327[sett].count), U8G2_BTN_INV, u8g2.getDisplayWidth(),  5,  2, "Назад" );
        exit_item = true;
      }else
      {
        u8g2.drawUTF8(0,28+(14*(Setting_de3327[sett].count)),"Назад");
      }
      u8g2.sendBuffer();
     
  }


void lcd_menu_item(uint8_t sett,uint8_t sel_item){
    u8g2.clearBuffer();
    u8g2.drawUTF8(25,12,Setting_de3327[sett].name);
    u8g2.drawHLine(0, 14, u8g2.getDisplayWidth());
    if(sel_item<3){
      for(uint8_t x=0;x<3;x++){
        if(x==sel_item%3){
          u8g2.drawButtonUTF8(0,28+(14*x),U8G2_BTN_INV, 10, 20,  2, Setting_de3327[sett].item[x].name);
          if(x==Setting_de3327[sett].count-1){exit_item = true;}
          //if(x==Setting_de3327[sett].count-2){test_operation = true;}
        }else
        {
          u8g2.drawUTF8(0,28+(14*x),Setting_de3327[sett].item[x].name);
        }
        u8g2.drawUTF8(100, 28+(14*x), u8x8_u8toa(Setting_de3327[sett].item[x].value,3));
      }
    }else{
      for(uint8_t x=0;x<Setting_de3327[sett].count-3;x++){
        if(x==sel_item%3){
          u8g2.drawButtonUTF8(0,28+(14*x),U8G2_BTN_INV, 10, 20,  2, Setting_de3327[sett].item[x+3].name);
          if(x+3==Setting_de3327[sett].count-1){exit_item = true;}
          //if(x+3==Setting_de3327[sett].count-2){test_operation = true;}
        }else
        {
          u8g2.drawUTF8(0,28+(14*x),Setting_de3327[sett].item[x+3].name);
        }
        u8g2.drawUTF8(100, 28+(14*x), u8x8_u8toa(Setting_de3327[sett].item[x+3].value,3));
      }
      }
      u8g2.sendBuffer();
}

void lcd_menu(uint8_t sel_item){
  u8g2.drawUTF8(25,12,"Настройки");
  u8g2.drawHLine(0, 14, u8g2.getDisplayWidth());
  Item_encoder item_encoder;
  if((sel_item>=0)&&(sel_item<3)){
      for(uint8_t x=0;x<3;x++){
        if(x==sel_item%3){
          u8g2.drawButtonUTF8(0,28+(14*x),U8G2_BTN_INV, u8g2.getDisplayWidth(), 20,  2, Setting_de3327[x].name);
        }else
        {
          u8g2.drawUTF8(0,28+(14*x),Setting_de3327[x].name);
        }
      }
    }
  if((sel_item>2)&&(sel_item<6)){
    for(uint8_t x=0;x<3;x++){
      if(x==sel_item%3){
        u8g2.drawButtonUTF8(0,28+(14*x),U8G2_BTN_INV, u8g2.getDisplayWidth(), 20,  2, Setting_de3327[x+3].name);
        }else{u8g2.drawUTF8(0,28+(14*x),Setting_de3327[x+3].name);}
      }
    }
  if((sel_item>5)&&(sel_item<9)){
    for(uint8_t x=0;x<3;x++){
      if(x==sel_item%3){
        u8g2.drawButtonUTF8(0,28+(14*x),U8G2_BTN_INV, u8g2.getDisplayWidth(), 20,  2, Setting_de3327[x+6].name);
        }else{u8g2.drawUTF8(0,28+(14*x),Setting_de3327[x+6].name);}
      }
  }
  if((sel_item>8)&&(sel_item<12)){
    for(uint8_t x=0;x<(MAX_Setting_DE3327%3);x++){
      if(x==sel_item%3){
        u8g2.drawButtonUTF8(0,28+(14*x),U8G2_BTN_INV, u8g2.getDisplayWidth(), 20,  2, Setting_de3327[x+9].name);
        }else{u8g2.drawUTF8(0,28+(14*x),Setting_de3327[x+9].name);}
      }
  }
      u8g2.sendBuffer();
}

void Main_DE3327(){
    u8g2.firstPage();
      do{
             
          
      }while(u8g2.nextPage());
}

void vTaskLed( void * pvParameters )
{
  pinMode(2,OUTPUT);
  for(;;){
    vTaskDelay(500);
    //digitalWrite(2,true);
    //vTaskDelay(500);
    //digitalWrite(2,false);
  }
}
void vTaskBVK( void * pvParameters ) // считывание из MCP23008
{
  //uint8_t adress[]={0x40,0x42,0x44};//-------------------адреса MCP23008--------------------------
  //Item_bvk bvk;
  Item_encoder item_encoder;
  uint8_t bvk[3];
  uint8_t BVK1_7;
  uint8_t deist;
   Wire.begin();
  for(uint8_t x=0;x<3;x++){
    MCP[x].begin();
    MCP[x].pinMode8(0xff);
  }
  pinMode(12,OUTPUT);
  pinMode(13,OUTPUT);
  pinMode(14,OUTPUT);
  
  for(;;){

   
    //Serial.printf("MCP %d \n",MCP.read8());
    /*
    for(uint8_t x=0;x<3;x++){
      item_encoder.value_bvk[x] = MCP[x].read8();
    }
    item_encoder.state = 1;
    xQueueSend(queue_encoder, &item_encoder, portMAX_DELAY);
    */
   //GK.write(Setting_de3327[0].item[2].value,1);
  BVK1_7=MCP[2].read8()^0xff;
  switch (MCP[0].read8()^0xff)///ВЫБОР РЕЖИМА РАБОТЫ ТЕРМОПЛАСТА
  {
  case NALADKA:
        switch (MCP[1].read8()^0xff)
          {
          case SMYK: //СМЫКАНИЕ
            TPA_action(SMYK,NALADKA);
          break;
          case RZMYK: //РАЗМЫКАНИЕ
            TPA_action(RZMYK,NALADKA);
          break;
          case PODV: //ПОДВОL
            TPA_action(PODV,NALADKA);
          break;
          case OTV: //ОТВОД
            TPA_action(OTV,NALADKA);
          break;
          case VPRISK: //ВПРЫСК
            TPA_action(VPRISK,NALADKA);
          break;
          case NABOR: //НАБОР
            TPA_action(NABOR,NALADKA);
          break;
          
          default:
            GK.clearAll();
            xTimerStop(Timer_nabor,0);
            //GK.writeByte(2,0);
            GK.update();
          break;
          }
    break;
  case RUCHNOY:
      switch (MCP[1].read8()^0xff)
        {
          case SMYK: //СМЫКАНИЕ
            //GK.writeByte(2,(1<<Setting_de3327[0].item[2].value)|speed[Setting_de3327[0].item[0].value]);
            //GK.update();
            TPA_action(SMYK,RUCHNOY);
            deist = SMYK; /// смыкание
            break;
          case RZMYK: //РАЗМЫКАНИЕ
            //GK.writeByte(2,(1<<Setting_de3327[1].item[2].value)|speed[Setting_de3327[1].item[0].value]);
            //GK.update();
            TPA_action(RZMYK,RUCHNOY);
            deist = RZMYK; /// размыкание
            break;
          case PODV: //ПОДВОД
            //GK.writeByte(2,(1<<Setting_de3327[2].item[2].value)|speed[Setting_de3327[2].item[0].value]);
            //GK.update();
            TPA_action(PODV,RUCHNOY);
            deist = PODV; /// подвод
            break;
          case OTV: //ОТВОД
            //GK.writeByte(2,(1<<Setting_de3327[3].item[2].value)|speed[Setting_de3327[3].item[0].value]);
            //GK.update();
            TPA_action(OTV,RUCHNOY);
            deist = OTV; /// отвод
            break;
          case VPRISK: //ВПРЫСК
            //GK.writeByte(2,speed[Setting_de3327[4].item[0].value]);
            //GK.writeByte(1,(1<<Setting_de3327[4].item[2].value));
            //GK.writeByte(0,pressure[Setting_de3327[4].item[2].value]);
            //GK.update();
            TPA_action(VPRISK,RUCHNOY);
            deist = VPRISK; /// впрыск
            break;
          case NABOR: //НАБОР
            //GK.writeByte(2,speed[Setting_de3327[6].item[0].value]);
            //GK.writeByte(1,(1<<Setting_de3327[6].item[2].value));
            //GK.writeByte(0,pressure[Setting_de3327[6].item[2].value]);
            //GK.update();
            TPA_action(NABOR,RUCHNOY);
            xTimerStart(Timer_nabor,0);
            deist = NABOR; /// впрыск
            break;
//*************************************** Ожидаем завершение операции в РУЧНОМ режиме  ********************************
          default:/// после нажатия на одну из кнопок ждем когда включиться один из БВК
            switch(deist){
              case SMYK: //если происходит операция смыкания
                if(BVK1_7&BVK1){
                  GK.clearAll();
                  GK.update();
                  deist = 0;
                }
              break;
              case RZMYK: //если происходит окончание операции размыкания
                if(BVK1_7&BVK2){
                  if(cnt_tolkatel<Setting_de3327[10].item[0].value){
                    TPA_action(SMYK,RUCHNOY);
                    xTimerChangePeriod(Timer_tolkatel,Setting_de3327[10].item[1].value*100,100);
                    xTimerStart(Timer_tolkatel,0);
                    deist = RZMYK;
                  }else{
                    cnt_tolkatel=0;
                    GK.clearAll();
                    GK.update();
                    deist = 0;
                  }
                  
                }
              break;
              case PODV: //если происходит операция подвод печки
                if(BVK1_7&BVK3){
                  GK.clearAll();
                  GK.update();
                  deist = 0;
                }
              break;
              case OTV: //если происходит операция отвод печки
                if(BVK1_7&BVK4){
                  GK.clearAll();
                  GK.update();
                  deist = 0;
                }
              break;
              case VPRISK: //если происходит операция впрыска
                if(BVK1_7&BVK6){
                  GK.clearAll();
                  GK.update();
                  deist = 0;
                }
              break;
              case NABOR: //если происходит операция впрыска
                if(BVK1_7&BVK7){
                  GK.clearAll();
                  GK.update();
                  xTimerStop(Timer_nabor,0);
                  deist = 0;
                }
              break;
            }
            break;
          }
    break;
    case PAVTOMAT:
        switch (MCP[1].read8()^0xff)
        {
          case SMYK: //СМЫКАНИЕ
            //GK.writeByte(2,(1<<Setting_de3327[0].item[2].value)|speed[Setting_de3327[0].item[0].value]);
            //GK.update();
            TPA_action(SMYK,RUCHNOY);
            deist = SMYK; /// смыкание
          break;
//******************* САМОЕ МЯСКО!!!!************************************************************
          default:
            switch(deist){
              case SMYK: //если происходит операция смыкания
                if(BVK1_7&BVK1){
                  //GK.writeByte(2,(1<<Setting_de3327[1].item[2].value)|speed[Setting_de3327[1].item[0].value]); // это размыкание
                  GK.clearAll();
                  GK.update();
                  TPA_action(VPRISK,PAVTOMAT);
                  deist = VPRISK;
                }
              break;
              case RZMYK: //если происходит операция размыкания
                if(BVK1_7&BVK2){
                  if(cnt_tolkatel<Setting_de3327[10].item[0].value){
                    TPA_action(SMYK,RUCHNOY);
                    xTimerChangePeriod(Timer_tolkatel,Setting_de3327[10].item[1].value*100,100);
                    xTimerStart(Timer_tolkatel,0);
                    deist = RZMYK;
                  }else{
                    cnt_tolkatel=0;
                    GK.clearAll();
                    GK.update();
                    deist = 0;
                  }
                  
                }
              break;
              case VPRISK:
                if(BVK1_7&BVK5){
                 // GK.clearAll();
                 // GK.update();
                  TPA_action(VPRISK_II,RUCHNOY);
                  xTimerChangePeriod(Timer_dozat,Setting_de3327[9].item[0].value*100,100);
                  xTimerStart(Timer_dozat,0);
                  deist = NABOR;
                }
              break;
              case NABOR:
                if(BVK1_7&BVK7){
                  GK.clearAll();
                  GK.update();
                  xTimerStop(Timer_nabor,0);
                  xTimerChangePeriod(Timer_ochlad,Setting_de3327[8].item[0].value*100,100);
                  xTimerStart(Timer_ochlad,0);
                  //TPA_action(RZMYK,RUCHNOY);
                  deist = RZMYK;
                }
              break;
            }
          break;
        }
    break;
    case AVTOMAT:
        switch (MCP[1].read8()^0xff)
        {
          case SMYK: //СМЫКАНИЕ
            //GK.writeByte(2,(1<<Setting_de3327[0].item[2].value)|speed[Setting_de3327[0].item[0].value]);
            //GK.update();
            TPA_action(SMYK,RUCHNOY);
            deist = SMYK; /// смыкание
          break;
//******************* САМОЕ МЯСКО!!!!************************************************************
          default:
            switch(deist){
              case SMYK: //если происходит операция смыкания
                if(BVK1_7&BVK1){
                  //GK.writeByte(2,(1<<Setting_de3327[1].item[2].value)|speed[Setting_de3327[1].item[0].value]); // это размыкание
                  GK.clearAll();
                  GK.update();
                  TPA_action(VPRISK,RUCHNOY);
                  deist = VPRISK;
                }
              break;
              case RZMYK: //если происходит окончание операции размыкания
                if(BVK1_7&BVK2){
                  if(cnt_tolkatel<Setting_de3327[10].item[0].value){
                    TPA_action(SMYK,RUCHNOY);
                    xTimerChangePeriod(Timer_tolkatel,Setting_de3327[10].item[1].value*100,100);
                    xTimerStart(Timer_tolkatel,0);
                    deist = RZMYK;
                  }else{
                    cnt_tolkatel=0;
                    GK.clearAll();
                    GK.update();
                    xTimerChangePeriod(Timer_M_CYCL,Setting_de3327[9].item[1].value*100,100);
                    xTimerStart(Timer_M_CYCL,0);
                    deist = SMYK;
                  }
                  
                }
              break;
              case VPRISK:
                if(BVK1_7&BVK5){
                 // GK.clearAll();
                 // GK.update();
                  TPA_action(VPRISK_II,RUCHNOY);
                  xTimerChangePeriod(Timer_dozat,Setting_de3327[9].item[0].value*100,100);
                  xTimerStart(Timer_dozat,0);
                  deist = NABOR;
                }
              break;
              case NABOR:
                if(BVK1_7&BVK7){
                  GK.clearAll();
                  GK.update();
                  xTimerStop(Timer_nabor,0);
                  xTimerChangePeriod(Timer_ochlad,Setting_de3327[8].item[0].value*100,100);
                  xTimerStart(Timer_ochlad,0);
                  //TPA_action(RZMYK,RUCHNOY);
                  deist = RZMYK;
                }
              break;
            }
          break;
        }
    break;
    default:
      GK.clearAll();
      GK.update();
    break;
}
vTaskDelay(50);
}
}


void vTaskDisplay( void * pvParameters )
{
  uint8_t m,h;
  Item_encoder item_encoder;
  //Item_bvk bvk;
  u8g2.begin();
  u8g2.setContrast(5);
  u8g2.setFont(u8g2_font_unifont_t_cyrillic);
  uint8_t y=0;
  
  u8g2.firstPage();
  do{
  lcd_menu(1);
  }while(u8g2.nextPage());
  
  if(xSemaphoreTake(Serial_mutex, portMAX_DELAY) == pdTRUE){Serial.println("Start task display \n");}
  xSemaphoreGive(Serial_mutex);


  for( ;; )
  {
    
    if(xQueueReceive(queue_encoder, &item_encoder,portMAX_DELAY)){
    Serial.printf("Position %d Item %d Select_item %d Count %d \n",item_encoder.position,item_encoder.item,item_encoder.select_item_menu,item_encoder.count);
   
      u8g2.firstPage();
      do{
          
          //lcd_menu_item(y,0);
          if(item_encoder.state==1){
            u8g2.drawUTF8(25,12,"БВК");
          u8g2.drawHLine(0, 14, u8g2.getDisplayWidth());
          u8g2.drawUTF8(0,28+(14*0),"MCP 0x40");
          u8g2.drawUTF8(100,28+(14*0),u8x8_u8toa(item_encoder.value_bvk[0],3));
          u8g2.drawUTF8(0,28+(14*1),"MCP 0x42");
          u8g2.drawUTF8(100,28+(14*1),u8x8_u8toa(item_encoder.value_bvk[1],3));
          u8g2.drawUTF8(0,28+(14*2),"MCP 0x44");
          u8g2.drawUTF8(100,28+(14*2),u8x8_u8toa(item_encoder.value_bvk[2],3));
          u8g2.sendBuffer();
          }
          if(item_encoder.state==0){
            switch(item_encoder.position){
            case 0:
            {
              if(item_encoder.count>MAX_Setting_DE3327-1){
                item_encoder.count=0;
                encoder.clearCount();
                }
              //lcd_Menu(item_encoder.count);
              lcd_menu(item_encoder.count);
              break;
            }
            case 1:
            {
              if(item_encoder.count>Setting_de3327[item_encoder.item].count-1){
                item_encoder.count=0;
                encoder.clearCount();
                }
              lcd_menu_item(item_encoder.item,item_encoder.count);
              break;
            }
            case 2:
            {
              Setting_write(item_encoder.item,item_encoder.select_item_menu,item_encoder.count);
              break;
            }
          }
          }
          
          
      }while(u8g2.nextPage());
    }
      //y++; 
      //vTaskDelay(1000/portTICK_PERIOD_MS);
}
}

void setup(void) {
  Serial_mutex = xSemaphoreCreateMutex(); // Create the mutex
  Serial.begin(9600);
  queue_encoder = xQueueCreate(10, sizeof(Item_encoder));
  queue_bvk = xQueueCreate(10, sizeof(Item_bvk));
  SPI.begin(18,19,23,5);

  Timer_M_CYCL = xTimerCreate(
            "M_CYCL", /* name */
            pdMS_TO_TICKS(1000), /* period/time ms */
            pdFALSE, /* auto reload */
            (void*)0, /* timer ID */
            vTimerCallback_M_CYCL/* callback */
            );
  Timer_dozat = xTimerCreate(
            "DOZAT", /* name */
            pdMS_TO_TICKS(1000), /* period/time ms */
            pdFALSE, /* auto reload */
            (void*)0, /* timer ID */
            vTimerCallback_dozat/* callback */
            );
  Timer_ochlad = xTimerCreate(
            "OCHLAD", /* name */
            pdMS_TO_TICKS(1000), /* period/time ms */
            pdFALSE, /* auto reload */
            (void*)0, /* timer ID */
            vTimerCallback_ochlad/* callback */
            );
  Timer_tolkatel = xTimerCreate(
            "Tolkatel", /* name */
            pdMS_TO_TICKS(1000), /* period/time ms */
            pdFALSE, /* auto reload */
            (void*)0, /* timer ID */
            vTimerCallback_tolkatel/* callback */
            );
  Timer_nabor = xTimerCreate(
            "Nabor", /* name */
            pdMS_TO_TICKS(5), /* period/time ms */
            pdTRUE, /* auto reload */
            (void*)0, /* timer ID */
            vTimerCallback_nabor/* callback */
            );

  Setting_de3327[0].name = "Смыкание";
  Setting_de3327[0].item[0].name= "Скорость";
  Setting_de3327[0].item[0].value= 1;
  Setting_de3327[0].item[1].name= "Давление";
  Setting_de3327[0].item[1].value= 4;
  Setting_de3327[0].item[2].name= "Гидроклапан";
  Setting_de3327[0].item[2].value= 0;
  Setting_de3327[0].item[3].name= "БВК";
  Setting_de3327[0].item[3].value= 3;
  Setting_de3327[0].item[4].name= "Тест";
  Setting_de3327[0].item[4].value= true;
  Setting_de3327[0].item[5].name= "Назад";
  Setting_de3327[0].item[5].value= true;
  Setting_de3327[0].count = 6;

  Setting_de3327[1].name = "Размыкание";
  Setting_de3327[1].item[0].name= "Скорость";
  Setting_de3327[1].item[0].value= 1;
  Setting_de3327[1].item[1].name= "Давление";
  Setting_de3327[1].item[1].value= 5;
  Setting_de3327[1].item[2].name= "Гидроклапан";
  Setting_de3327[1].item[2].value= 1;
  Setting_de3327[1].item[3].name= "БВК";
  Setting_de3327[1].item[3].value= 3;
  Setting_de3327[1].item[4].name= "Тест";
  Setting_de3327[1].item[4].value= true;
  Setting_de3327[1].item[5].name= "Назад";
  Setting_de3327[1].item[5].value= true;
  Setting_de3327[1].count = 6;

  Setting_de3327[2].name = "Подвод";
  Setting_de3327[2].item[0].name= "Скорость";
  Setting_de3327[2].item[0].value= 1;
  Setting_de3327[2].item[1].name= "Давление";
  Setting_de3327[2].item[1].value= 0;
  Setting_de3327[2].item[2].name= "Гидроклапан";
  Setting_de3327[2].item[2].value= 2;
  Setting_de3327[2].item[3].name= "БВК";
  Setting_de3327[2].item[3].value= 3;
  Setting_de3327[2].item[4].name= "Тест";
  Setting_de3327[2].item[4].value= true;
  Setting_de3327[2].item[5].name= "Назад";
  Setting_de3327[2].item[5].value= true;
  Setting_de3327[2].count= 6;

  Setting_de3327[3].name = "Отвод";
  Setting_de3327[3].item[0].name= "Скорость";
  Setting_de3327[3].item[0].value= 1;
  Setting_de3327[3].item[1].name= "Давление";
  Setting_de3327[3].item[1].value= 0;
  Setting_de3327[3].item[2].name= "Гидроклапан";
  Setting_de3327[3].item[2].value= 3;
  Setting_de3327[3].item[3].name= "БВК";
  Setting_de3327[3].item[3].value= 3;
  Setting_de3327[3].item[4].name= "Тест";
  Setting_de3327[3].item[4].value= true;
  Setting_de3327[3].item[5].name= "Назад";
  Setting_de3327[3].item[5].value= true;
  Setting_de3327[3].count= 6;

  Setting_de3327[4].name = "Впрыск 1 ст";
  Setting_de3327[4].item[0].name= "Скорость";
  Setting_de3327[4].item[0].value= 4;
  Setting_de3327[4].item[1].name= "Давление";
  Setting_de3327[4].item[1].value= 4;
  Setting_de3327[4].item[2].name= "Гидроклапан";
  Setting_de3327[4].item[2].value= 1;
  Setting_de3327[4].item[3].name= "БВК";
  Setting_de3327[4].item[3].value= 3;
  Setting_de3327[4].item[4].name= "Тест";
  Setting_de3327[4].item[4].value= true;
  Setting_de3327[4].item[5].name= "Назад";
  Setting_de3327[4].item[5].value= true;
  Setting_de3327[4].count= 6;

  Setting_de3327[5].name = "Впрыск 2 ст";
  Setting_de3327[5].item[0].name= "Скорость";
  Setting_de3327[5].item[0].value= 2;
  Setting_de3327[5].item[1].name= "Давление";
  Setting_de3327[5].item[1].value= 4;
  Setting_de3327[5].item[2].name= "Гидроклапан";
  Setting_de3327[5].item[2].value= 1;
  Setting_de3327[5].item[3].name= "БВК";
  Setting_de3327[5].item[3].value= 3;
  Setting_de3327[5].item[4].name= "Тест";
  Setting_de3327[5].item[4].value= true;
  Setting_de3327[5].item[5].name= "Назад";
  Setting_de3327[5].item[5].value= true;
  Setting_de3327[5].count= 6;

  Setting_de3327[6].name = "Набор";
  Setting_de3327[6].item[0].name= "Скорость";
  Setting_de3327[6].item[0].value= 3;
  Setting_de3327[6].item[1].name= "Давление";
  Setting_de3327[6].item[1].value= 4;
  Setting_de3327[6].item[2].name= "Гидроклапан";
  Setting_de3327[6].item[2].value= 0;
  Setting_de3327[6].item[3].name= "БВК";
  Setting_de3327[6].item[3].value= 3;
  Setting_de3327[6].item[4].name= "Тест";
  Setting_de3327[6].item[4].value= true;
  Setting_de3327[6].item[5].name= "Назад";
  Setting_de3327[6].item[5].value= true;
  Setting_de3327[6].count= 6;

  Setting_de3327[7].name = "Подпор";
  Setting_de3327[7].item[0].name= "Скорость";
  Setting_de3327[7].item[0].value= 125;
  Setting_de3327[7].item[1].name= "Давление";
  Setting_de3327[7].item[1].value= 7;
  Setting_de3327[7].item[2].name= "Гидроклапан";
  Setting_de3327[7].item[2].value= 1;
  Setting_de3327[7].item[3].name= "БВК";
  Setting_de3327[7].item[3].value= 0;
  Setting_de3327[7].item[4].name= "Тест";
  Setting_de3327[7].item[4].value= true;
  Setting_de3327[7].item[5].name= "Назад";
  Setting_de3327[7].item[5].value= true;
  Setting_de3327[7].count= 6;

  Setting_de3327[8].name = "Охлаждение";
  Setting_de3327[8].item[0].name= "Время";
  Setting_de3327[8].item[0].value= 70;
  Setting_de3327[8].item[1].name= "Тест";
  Setting_de3327[8].item[1].value= true;
  Setting_de3327[8].item[2].name= "Назад";
  Setting_de3327[8].item[2].value= true;
  Setting_de3327[8].count= 3;

  Setting_de3327[9].name = "Таймеры";
  Setting_de3327[9].item[0].name= "Дожатие";
  Setting_de3327[9].item[0].value= 30;
  Setting_de3327[9].item[1].name= "М-у циклами";
  Setting_de3327[9].item[1].value= 10;
  Setting_de3327[9].item[2].name= "Тест";
  Setting_de3327[9].item[2].value= true;
  Setting_de3327[9].item[3].name= "Назад";
  Setting_de3327[9].item[3].value= true;
  Setting_de3327[9].count= 4;

  Setting_de3327[10].name = "Толкатели";
  Setting_de3327[10].item[0].name= "Кол-во";
  Setting_de3327[10].item[0].value= 2;
  Setting_de3327[10].item[1].name= "Время";
  Setting_de3327[10].item[1].value= 3;
  Setting_de3327[10].item[2].name= "Назад";
  Setting_de3327[10].item[2].value= true;
  Setting_de3327[10].count= 3;

  //Setting_de3327[1].name = "Назад";
  //Setting_de3327[10].item[0].name= "Назад";
  //Setting_de3327[10].item[0].value= true;

  
   
  //pinMode(2,OUTPUT);
   
  //u8g2.enableUTF8Print();
  xTaskCreate( vTaskBVK, "BVK", 2048, NULL, 0, NULL );
  xTaskCreate( vTaskEncoder, "ENCODER", 2048, NULL, 2, NULL );
  xTaskCreate( vTaskDisplay, "DISPLAY", 2048, NULL, 1, NULL );
  xTaskCreate( vTaskLed, "LED", 1024, NULL, 3, NULL );
}



void loop(void) {
  
  
  
}

