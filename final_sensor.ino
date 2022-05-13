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
#include <SPI.h>
#include<SD.h>
#include <ErriezDHT22.h>

// Connect DTH22 DAT pin to Arduino DIGITAL pin
#if defined(ARDUINO_ARCH_AVR) || defined(ARDUINO_SAM_DUE)
#define DHT22_PIN      4
#else
#error "May work, but not tested on this target"
#endif

// Create DHT22 sensor object
DHT22 dht22 = DHT22(DHT22_PIN);

// Function prototypes

#include <SoftwareSerial.h>


AltSoftSerial BTserial(8,9);

// https://www.pjrc.com/teensy/td_libs_AltSoftSerial.html

File myFile;

DS3231 clock;
RTCDateTime dt;
TFMini tfmini;
SoftwareSerial SerialTFMini(3, 2);          //The only value that matters here is the first one, 2, Rx
int data=0;
unsigned long SLEEPCOUNTMillis;
int c;
boolean NL = true;
int16_t temperature;
int16_t humidity;
int a=0;
int y=0;
  int m=0;
  int d=0;
  int h=0;
  int mi=0;
  int s=0;
  unsigned long time0=0;
  unsigned long time1=0;
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
  clock.setDateTime(2022, 4, 4, 0, 0, 0);
  BTserial.begin(38400);  
  Serial.println("BTserial started at 9600");
  SD.begin(10);
  // Initialize serial port
    Serial.begin(9600);
    while (!Serial) {
        ;
    }
    Serial.println(F("DHT22 temperature and humidity sensor example\n"));

    // Initialize DHT22
    dht22.begin();
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
    
      
  if (BTserial.available() > 0) { // Checks whether data is comming from the serial port
    data = BTserial.read(); // Reads the data from the serial port
  }

   
    Serial.print(data);
    
    y=dt.year;
    m=dt.month;
    d=dt.day;
    h=dt.hour;
    mi=dt.minute;
    s=dt.second;

    if(dt.second == 30)
    {LowPower.powerDown(SLEEP_4S, ADC_OFF, BOD_OFF);}
    
    if((dt.hour == 0) && (dt.minute == 1)&&(dt.second == 0))
    { if (dht22.available()) {       
        temperature = dht22.readTemperature();
        humidity = dht22.readHumidity();       
    }
      
      myFile=SD.open("slide.txt",FILE_WRITE);
      myFile.print("Date:");
      myFile.print(y);   myFile.print("-");
      myFile.print(m);  myFile.print("-");
      myFile.print(d);    myFile.print("\n ");
      myFile.print("Time:");
      myFile.print(h);   myFile.print(":");
      myFile.print(mi); myFile.print("\n ");
      myFile.print(F("Temperature: "));
      myFile.print(temperature / 10);
      myFile.print(F("."));
      myFile.print(temperature % 10);
      myFile.print("\n ");
      myFile.print(F("Humidity: "));
      myFile.print(humidity / 10);
      myFile.print(F("."));
      myFile.print(humidity % 10);
      myFile.print(F(" %"));
      myFile.print("\n ");
      myFile.print(data); myFile.print("-"); myFile.print("slide#2");
      myFile.print("\n ");
      myFile.print(a); myFile.print("-"); myFile.print("slide#1");
      myFile.print("\n ");
      myFile.print("\n ");
      myFile.close();
      delay(1000);
      }
      if((dt.hour == 0) && (dt.minute == 2)&&(dt.second == 30)) 
      
      {
      if (dht22.available()) {
        temperature = dht22.readTemperature();
        humidity = dht22.readHumidity();
    }
      
      myFile=SD.open("slide.txt",FILE_WRITE);
      myFile.print("Date:");
      myFile.print(y);   myFile.print("-");
      myFile.print(m);  myFile.print("-");
      myFile.print(d);    myFile.print("\n ");
      myFile.print("Time:");
      myFile.print(h);   myFile.print(":");
      myFile.print(mi); myFile.print("\n ");
      myFile.print(F("Temperature: "));
      myFile.print(temperature / 10);
      myFile.print(F("."));
      myFile.print(temperature % 10);
      myFile.print("\n ");
      myFile.print(F("Humidity: "));
      myFile.print(humidity / 10);
      myFile.print(F("."));
      myFile.print(humidity % 10);
      myFile.print(F(" %"));
      myFile.print("\n ");
      myFile.print(data); myFile.print("-"); myFile.print("slide#2");
      myFile.print("\n ");
      myFile.print(a); myFile.print("-"); myFile.print("slide#1");
      myFile.print("\n ");
      myFile.print("\n ");
      myFile.close();
      delay(1000);
      }
      if((dt.hour == 0) && (dt.minute == 3)&&(dt.second == 30)) 
      
     {
      if (dht22.available()) {
        temperature = dht22.readTemperature();
        humidity = dht22.readHumidity();  
    }
      
      myFile=SD.open("slide.txt",FILE_WRITE);
      myFile.print("Date:");
      myFile.print(y);   myFile.print("-");
      myFile.print(m);  myFile.print("-");
      myFile.print(d);    myFile.print("\n ");
      myFile.print("Time:");
      myFile.print(h);   myFile.print(":");
      myFile.print(mi); myFile.print("\n ");
      myFile.print(F("Temperature: "));
      myFile.print(temperature / 10);
      myFile.print(F("."));
      myFile.print(temperature % 10);
      myFile.print("\n ");
      myFile.print(F("Humidity: "));
      myFile.print(humidity / 10);
      myFile.print(F("."));
      myFile.print(humidity % 10);
      myFile.print(F(" %"));
      myFile.print("\n ");
      myFile.print(data); myFile.print("-"); myFile.print("slide#2");
      myFile.print("\n ");
      myFile.print(a); myFile.print("-"); myFile.print("slide#1");
      myFile.print("\n ");
      myFile.print("\n ");
      myFile.close();
      delay(1000);
      
      }
    distance1= distance;
  }}
  delay(200);
      }
