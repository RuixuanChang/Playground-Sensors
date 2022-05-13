#include <SoftwareSerial.h>
#include <AltSoftSerial.h>
#include <SPI.h>
#include <SD.h>
#include "TFMini.h"
//加长每次间隔的时间 RTC叫醒 RTC自动休息
#include <DS3232RTC.h>
#include <Wire.h>
#include <DS3231.h>
#include <LowPower.h>
DS3231 clock;
RTCDateTime dt;
TFMini tfmini;
SoftwareSerial SerialTFMini(3, 2);          //The only value that matters here is the first one, 2, Rx
AltSoftSerial BTserial(8,9);
unsigned long SLEEPCOUNTMillis;
int c;
boolean NL = true;
int a=0;
void getTFminiData(int* distance, int* strength)
{ 
  static char i = 0;
  char j = 0;
  int checksum = 0;
  static int rx[9];
  if (SerialTFMini.available())
  {
    rx[i] = SerialTFMini.read();
    if (rx[0] != 0x59)
    {
      i = 0;
    }
    else if (i == 1 && rx[1] != 0x59)
    {
      i = 0;
    }
    else if (i == 8)
    {
      for (j = 0; j < 8; j++)
      {
        checksum += rx[j];
      }
      if (rx[8] == (checksum % 256))
      {
        *distance = rx[2] + rx[3] * 256;
        *strength = rx[4] + rx[5] * 256;
      
      }
      i = 0;
    }
    else
    {
      i++;
    }
  }
}
 
 
void setup()
{
  Serial.begin(9600);       //Initialize hardware serial port (serial debug port)
  
  while (!Serial);            // wait for serial port to connect. Needed for native USB port only
  Serial.println ("Initializing...");
  SerialTFMini.begin(TFMINI_BAUDRATE);    //Initialize the data rate for the SoftwareSerial port
  tfmini.begin(&SerialTFMini);            //Initialize the TF Mini sensorSerial.begin(9600);
  
    Serial.begin(9600);
  
  // Initialize DS3231
  Serial.println("Initialize DS3231");;
  clock.begin();

  // Disarm alarms and clear alarms for this example, because alarms is battery backed.
  // Under normal conditions, the settings should be reset after power and restart microcontroller.
  clock.armAlarm1(false);
  clock.armAlarm2(false);
  clock.clearAlarm1();
  clock.clearAlarm2();
 
  // Manual (Year, Month, Day, Hour, Minute, Second)
  clock.setDateTime(2022, 4, 5, 14, 30, 0);
  BTserial.begin(38400);  
  Serial.println("BTserial started at 9600");
  
}
 
void loop()
{
  int distance = 0;
  int strength = 0;
  
  int distance1;
  dt = clock.getDateTime();
  getTFminiData(&distance, &strength);
  while (!distance)
  {
    getTFminiData(&distance, &strength);
    if(distance){
    
      
      
      if ((distance<=20) && (distance1>=20))
      {a=a+1;
       
       
      }
      
      Serial.print(a);
      BTserial.write(a);
      if((dt.hour == 0) && (dt.minute == 1)&&(dt.second == 0)) {
    
      BTserial.write(a);
      
      
      }
      
      if((dt.hour == 0) && (dt.minute == 2)&&(dt.second == 0)) {
    
      BTserial.write(a);
      
      }
      if((dt.hour == 0) && (dt.minute == 3)&&(dt.second == 0)) {
    
      BTserial.write(a);
      
      }
      
      distance1= distance;
    
 }}
  delay(200);
}
