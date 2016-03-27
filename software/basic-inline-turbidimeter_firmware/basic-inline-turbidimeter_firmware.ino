//#define NO_PORTD_PINCHANGES
//#define NO_PORTC_PINCHANGES
//#include <GSM.h>

#include <avr/interrupt.h>
#include <avr/power.h>
#include <avr/sleep.h>
#include <JeeLib.h>
#include <PinChangeInt.h>
#include <SPI.h>
#include <SD.h>
#include <Wire.h>

#define RUN_CHECK_GND A2
#define RUN_CHECK_VCC  9
#define SLEEP_INDICATOR_OFF_TIME 29000
#define SLEEP_INDICATOR_ON_TIME  1000
#define SLEEP_INDICATOR_LOG_ERROR_ON_TIME  1000
#define SLEEP_INDICATOR_LOG_ERROR_OFF_TIME  5000

#define BAUD_RATE 19200
#define SD_CS   10
#define LED1     5
#define LED2     6

#define READ_TM  200 // milleseconds between frequency calculations
#define DOWN_TIME  20000

#define TSL_OE   A1    // frequency signal from the sensor
#define TSL_S1    3    // S1 and S0 are pins on the TSL230R chip
#define TSL_S0    2
#define TSL_FREQ  4    // frequency signal from the sensor
#define TPIN     A0
#define VPIN     A3
//#define TSL_S3       // frequency signal from the sensor
//#define TSL_S2       // frequency signal from the sensor

#define DIV_R1 10000   // resistance for R1
#define DIV_R2  1000   // resistance for R2

#define SERIAL_DEBUG true
#define RTC_DEBUG false
#define DEBUG_CHECK_CARD_AT_STARTUP true

File myFile;
boolean logfile_error = false;
unsigned long timer, loop_timer, pulse_count;
int scale = 2;
byte byt;
long idx = 0;
float system_voltage = 0.0;
//Sd2Card card;
//SdVolume volume;
//SdFile root;

ISR(WDT_vect) { Sleepy::watchdogEvent(); }  // boilerplate for low-power waiting

void setup() { 
  pinMode(RUN_CHECK_GND, OUTPUT);
  digitalWrite(RUN_CHECK_GND, LOW);
  pinMode(RUN_CHECK_VCC, OUTPUT);
  digitalWrite(RUN_CHECK_VCC, LOW);

  pinMode(TPIN, INPUT);  
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(TSL_FREQ, INPUT); // light sensor
  pinMode(TSL_S0, OUTPUT);  // light sensor
  pinMode(TSL_S1, OUTPUT);  // light sensor
//  pinMode(TSL_S2, OUTPUT);  // light source
//  pinMode(TSL_S3, OUTPUT);  // light source
  pinMode(TSL_OE, OUTPUT);  // light source
//  digitalWrite(TSL_S2, HIGH);
//  digitalWrite(TSL_S3, HIGH);  
  digitalWrite(TSL_OE, LOW); 
  digitalWrite(TSL_S0, HIGH);
  digitalWrite(TSL_S1, HIGH);

  Wire.begin();  
  Serial.begin(BAUD_RATE);
  Serial.println("starting...");
  Serial.end();
  
  // clear /EOSC bit // Sometimes necessary to ensure that the clock  // keeps running on just battery power. Once set, it shouldn't need to be reset but it's a good  // idea to make sure.
  Wire.beginTransmission(0x68); // address DS3231
  Wire.write(0x0E); // select register
  Wire.write(0b00011100); // write register bitmap, bit 7 is /EOSC
  Wire.endTransmission();  
  if(RTC_DEBUG){ set_rtc_time(byte(00), byte(8), byte(15), byte(6), byte(17), byte(4), byte(15)); delay(100);}
  char out_ts[30];
  get_rtc_time(out_ts);
  //Serial.print("timestamp: ");
  //Serial.println(out_ts);
    
  pinMode(SD_CS, OUTPUT);
  //setup_sd();
  Serial.begin(BAUD_RATE);
  if (!SD.begin(SD_CS)) {Serial.println("initialization failed!");}
  else{Serial.println("initialization done.");}
  // re-open the file for reading:
  myFile = SD.open("log.txt");
  if (myFile){
    if(DEBUG_CHECK_CARD_AT_STARTUP) {
      Serial.println("log.txt:");
      while (myFile.available()) {
        //Serial.write(
        myFile.read();
      }
      myFile.close();
    }
  }else{
    Serial.println("error opening log.txt");
    logfile_error = true;
  }
  myFile.close();
  //PCintPort::attachInterrupt(TSL_FREQ, add_pulse, RISING);
}

void add_pulse() {pulse_count++;}

void loop(){
  loop_timer = millis();
  Serial.print("loop begins as...");
  Serial.println(millis());
  char rd_idx[15];
  char out_ts[30];
  char out_data[10];
  get_rtc_time(out_ts);
  sprintf (rd_idx, "%lu", idx);
  take_readings(6, false, false);
  float reading = take_readings(8, true, false);
  //char tmp[10];
  dtostrf(reading,3,3,out_data);
  myFile = SD.open("log.txt", FILE_WRITE);
  if (myFile) {
    float t = read_temperature();
    float v = getVoltageLevel();
    Serial.begin(BAUD_RATE);
    Serial.print("writing to logfile\t");
    Serial.print(t);
    Serial.print(",");
    Serial.print(v);
    Serial.print(",");
    Serial.print(millis());//out_ts);
    Serial.print(",");
    Serial.println(out_data);
    myFile.print(rd_idx);
    myFile.print(",");
    myFile.print(t);
    myFile.print(",");
    myFile.print(v);
    myFile.print(",");
    myFile.print(millis());//out_ts);
    myFile.print(",");
    myFile.println(out_data);
    myFile.close();
    logfile_error = false;
  } else {
    logfile_error = true;
  }
  idx++;

  Serial.println("preparing to sleep now...");
  delay(100);
  digitalWrite(TSL_OE, HIGH);
  
  //long quiet_time = DOWN_TIME - SLEEP_INDICATOR_OFF_TIME - SLEEP_INDICATOR_ON_TIME;
  //Serial.print("time elapsed...");
  //Serial.println(millis() - loop_timer);
  //Serial.print("msec to delay:");
  //Serial.println((DOWN_TIME - millis() + loop_timer) % (SLEEP_INDICATOR_OFF_TIME + SLEEP_INDICATOR_ON_TIME));
  //Serial.println((quiet_time - millis() + timer) % (SLEEP_INDICATOR_OFF_TIME + SLEEP_INDICATOR_ON_TIME));
  //replace with delay(quiet_time % (SLEEP_INDICATOR_OFF_TIME + SLEEP_INDICATOR_ON_TIME)) to bring (SLEEP_INDICATOR_OFF_TIME + SLEEP_INDICATOR_ON_TIME) to an even divisor of quiet_time
  //then replace the while loop with an (quiet_time / (SLEEP_INDICATOR_OFF_TIME + SLEEP_INDICATOR_ON_TIME)) for loop

  //Serial.end();
  boolean modder = true;
  while(millis() - loop_timer < DOWN_TIME){
    if(modder){
      Serial.print("sleep for ");
      Serial.println((DOWN_TIME - millis() + loop_timer) % (SLEEP_INDICATOR_OFF_TIME + SLEEP_INDICATOR_ON_TIME));
      delay(100);
      Sleepy::loseSomeTime((DOWN_TIME - millis() + loop_timer) % (SLEEP_INDICATOR_OFF_TIME + SLEEP_INDICATOR_ON_TIME));
      modder = false;
    }else{
      if(logfile_error){
        for(int i = 0; i < 3; i++){
          Sleepy::loseSomeTime(SLEEP_INDICATOR_LOG_ERROR_ON_TIME);
          digitalWrite(9, HIGH);
          Sleepy::loseSomeTime(SLEEP_INDICATOR_LOG_ERROR_OFF_TIME);
          digitalWrite(9, LOW);
        }
      }else{
        Serial.print("sleep for ");
        Serial.println(SLEEP_INDICATOR_OFF_TIME + SLEEP_INDICATOR_ON_TIME);
        delay(100);
        Sleepy::loseSomeTime(SLEEP_INDICATOR_OFF_TIME + SLEEP_INDICATOR_ON_TIME);
        //digitalWrite(9, HIGH);
        //Sleepy::loseSomeTime(SLEEP_INDICATOR_ON_TIME);
        //digitalWrite(9, LOW);
      }
    }
  }

  //Serial.begin(BAUD_RATE);
  delay(100);
  Serial.print("waking up now at...");
  Serial.println(millis());
  digitalWrite(TSL_OE, LOW);
}

/****************************************************************************************************/
float getVoltageLevel(){
  analogReference(INTERNAL);
  delay(10);
  float vpin = analogRead(VPIN); //drop the first reading
  delay(100);
  vpin = float(analogRead(VPIN));
  if(SERIAL_DEBUG){
    Serial.print("raw voltage divider value: ");
  }
  float divider_value = float(DIV_R2) / float(DIV_R1+DIV_R2);
  system_voltage = vpin/ 1023.0 * 1.1 / divider_value;  
  return system_voltage;
}

float read_temperature(){
  //with 10mV per degree C, and 1100mV for 1024 steps, scaling_factor = 10/(1100/1024) ~ 9.3
  //10.9mV/C, 1024 steps from
  analogReference(DEFAULT);
  delay(5);
  int t = analogRead(TPIN);
  analogReference(INTERNAL);
  delay(5);
  return float(t);
}

float take_readings(int rdgs, boolean printout, boolean dark_counts){
  PCintPort::attachInterrupt(TSL_FREQ, add_pulse, RISING);  delay(5);
  float rd = 0.0, high = 0.0, low = 1000000.0, sum = 0.0, avg = 0.0;
  for(int i = 0; i < rdgs; ++i){
    rd = 0.0;
    if(dark_counts){
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, LOW);
      delay(2);
      timer = millis();
      pulse_count = 0;
      while (timer + READ_TM > millis()){;}
      rd -= pulse_count / scale;
    }
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, HIGH);
    delay(2);
    timer = millis();
    pulse_count = 0;
    while (timer + READ_TM > millis()){;}
    rd += pulse_count / scale;
    if(rd > high){ high = rd;}
    if(rd < low){low = rd;}
    sum += rd;
    if(printout && SERIAL_DEBUG){
      Serial.print("reading: ");
      Serial.println(rd);
    }
  }
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  avg = 0.0;
  if(rdgs > 2){
    sum -= high + low;
    avg = sum / (rdgs - 2);
  }else{
    avg = sum / rdgs;
  }
  detachInterrupt(TSL_FREQ);
  delay(5);
  /*if(printout && SERIAL_DEBUG){
    Serial.print("average: ");
    Serial.println(avg);
    Serial.println();
    delay(100);
  }*/
  return avg;
}

void get_rtc_time(char *buffer){
  // send request to receive data starting at register 0
  Wire.beginTransmission(0x68); // 0x68 is DS3231 device address
  Wire.write((byte)0); // start at register 0
  Wire.endTransmission();
  Wire.requestFrom(0x68, 7); // request info
  while(Wire.available())  { 
    int seconds = Wire.read(); // get seconds
    int minutes = Wire.read(); // get minutes
    int hours = bcdToDec(Wire.read() & 0b111111);//Wire.read();   // get hours
    int dotw = bcdToDec(Wire.read());
    int dotm = bcdToDec(Wire.read());
    int mnth = bcdToDec(Wire.read());
    int yr = bcdToDec(Wire.read());
    //hours = //(((hours & 0b00100000)>>5)*20 + ((hours & 0b00010000)>>4)*10 + (hours & 0b00001111)); // convert BCD to decimal (assume 24 hour mode)
    seconds = (((seconds & 0b11110000)>>4)*10 + (seconds & 0b00001111)); // convert BCD to decimal
    minutes = (((minutes & 0b11110000)>>4)*10 + (minutes & 0b00001111)); // convert BCD to decimal
    yr += 2000;
    
    if(seconds >= 10 && minutes >= 10){sprintf(buffer, "%d/%d/%d %d:%d:%d", yr, mnth, dotm, hours, minutes, seconds);}
    if(seconds >= 10 && minutes < 10){sprintf(buffer, "%d/%d/%d %d:0%d:%d", yr, mnth, dotm, hours, minutes, seconds);}
    if(seconds < 10 && minutes < 10){sprintf(buffer, "%d/%d/%d %d:0%d:0%d", yr, mnth, dotm, hours, minutes, seconds);}
    if(seconds < 10 && minutes >= 10){sprintf(buffer, "%d/%d/%d %d:%d:0%d", yr, mnth, dotm, hours, minutes, seconds);}
    //s += 2000 + yr;    s += "/" + mnth;     s += "/" + dotm;    s += " " + hours;     s += ":" + minutes;     s += ":" + seconds;
  }
}

//set the time on the DS3231M
void set_rtc_time(byte seconds, byte minutes, byte hours, byte dotw, byte dotm, byte mnth, byte yr){  // sets time and date data to DS3231
  Wire.beginTransmission(0x68);
  Wire.write(0); // set next input to start at the seconds register
  Wire.write(decToBcd(seconds)); // set seconds
  Wire.write(decToBcd(minutes)); // set minutes
  Wire.write(decToBcd(hours)); // set hours
  Wire.write(decToBcd(dotm)); // set day of week (1=Sunday, 7=Saturday)
  Wire.write(decToBcd(dotm)); // set date (1 to 31)
  Wire.write(decToBcd(mnth)); // set month
  Wire.write(decToBcd(yr)); // set year (0 to 99)
  Wire.endTransmission(); 
}

// Convert binary coded decimal to normal decimal numbers
byte bcdToDec(byte val)  {return ( (val/16*10) + (val%16) );}

//opposite of bcdToDec
byte decToBcd(byte val){return( (val/10*16) + (val%10) );}
