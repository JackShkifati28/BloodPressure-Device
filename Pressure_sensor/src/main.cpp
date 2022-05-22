// ECE-UY 3144/4144 Introduction to Embedded Systems Design
// Professor Matthew S. Campisi
// Final Project "The Pressure is On"
// Bartosz Bieńkowski, Juan Jose, Jack Shkifati

// YouTube link: https://youtu.be/qNyu2ac87pc

/*
         _   _ ________  ___ _____  ____________ _____ _____ _____ _   _______ _____ 
   ____ | | | |  _  |  \/  ||  ___| | ___ \ ___ \  ___/  ___/  ___| | | | ___ \  ___|
  / __ \| |_| | | | | .  . || |__   | |_/ / |_/ / |__ \ `--.\ `--.| | | | |_/ / |__  
 / / _` |  _  | | | | |\/| ||  __|  |  __/|    /|  __| `--. \`--. \ | | |    /|  __| 
| | (_| | | | \ \_/ / |  | || |___  | |   | |\ \| |___/\__/ /\__/ / |_| | |\ \| |___ 
 \ \__,_\_| |_/\___/\_|  |_/\____/  \_|   \_| \_\____/\____/\____/ \___/\_| \_\____/ 
  \____/                                                                             
                                                                                    
*/

// Note: we have a miscalibrated sensor that reports 18 mmHg when at atmospherpic pressure.
// Our code is written to account for this deviation.

#include <Arduino.h>
#include "Wire.h"
#include <Adafruit_CircuitPlayground.h>

#define Pressure 0x18
#define Output_Measurement 0xAA
#define exit 0x00
#define Pressure_Min 0
#define Pressure_Max 300
#define Output_Min 419430
#define Output_Max 3774873

uint8_t stage;
float readings[10];
uint8_t readingsIdx = 0;
unsigned short timeAtStage3 = 0;

float last20Pressures[20]; // Array used for deflation rate
int last20PressuresIdx = 0;
float currPressure, prevPressure, slope;

// final vales to be found
float systolic, diastolic, maxDifference;
float maxPeak = 0;
float strongestHeartbeat, heartRate;
bool systolicFound = false; // switch to true when first heartbeat detected
float pressureBottomBeat, pressureTopPeak;

// used to calculate the biggest heartbeat
bool goingUp = false;

// helper variables for calculating the heart rate
int heartBeatCounter;
int timeForHeartRate;

float readPressure(){
  uint32_t value = 0;
  Wire.beginTransmission(Pressure);

  Wire.write(Output_Measurement);
  Wire.write(exit);
  Wire.write(exit);

  Wire.endTransmission(false);
  delay(5);

  Wire.requestFrom(Pressure, 4, true);

  if (Wire.available())
  {
    int status = Wire.read(); // read status byte
    
    // verify status byte
    if(status & (1<<5)){
      Serial.println("device is busy");
      return -1;
    }
    else if (!(status & (1<<6))){
      Serial.println("device has no power");
      return -2;
    }
    else if (status & (1<<2)){
      Serial.println("bad memory");
      return -3;
    }
    else if(status & (1<<0)){
      Serial.println("saturated");
      return -4;
    }
    
    // read raw pressure data
    value |= ((uint32_t)Wire.read()<<16);
    value |= ((uint32_t)Wire.read()<<8);
    value |= (Wire.read()<<0);
    readings[readingsIdx] = value;

    if(value >= 0){
      float realPressure = (float)(value - Output_Min) * ((float)Pressure_Max - (float)Pressure_Min)/((float)Output_Max - (float)Output_Min) + Pressure_Min;
      readings[readingsIdx] = realPressure;
      readingsIdx++;
      if(readingsIdx >= 10) readingsIdx = 0;
      return realPressure;
    }
  }
  return (float)value;
}

// set the appropriate number of LEDs to yellow depending on current pressure to use while pumping
void pumpingBar(float pressurePumped){
  CircuitPlayground.clearPixels();
  CircuitPlayground.setPixelColor(4, 0xFFFF00);
  if (pressurePumped > 40){
    CircuitPlayground.setPixelColor(3,0xFFFF00);
    if (pressurePumped > 55){
      CircuitPlayground.setPixelColor(2,0xFFFF00);
      if (pressurePumped > 70){
        CircuitPlayground.setPixelColor(1,0xFFFF00);
        if (pressurePumped > 85){
          CircuitPlayground.setPixelColor(0,0xFFFF00);
          if (pressurePumped > 100){
            CircuitPlayground.setPixelColor(9,0xFFFF00);
            if (pressurePumped > 115){
              CircuitPlayground.setPixelColor(8,0xFFFF00);
              if (pressurePumped > 130){
                CircuitPlayground.setPixelColor(7,0xFFFF00);
                if (pressurePumped > 145){
                  CircuitPlayground.setPixelColor(6,0xFFFF00);
                  if (pressurePumped > 160) CircuitPlayground.setPixelColor(5,0xFFFF00);
                }
              }
            }
          }
        }
      }
    }
  }
}

// set the appropriate number of LEDs depending on current deflation rate, to let the user dynamically adjust the valve
void slopeBar(float decreaseRate){
  CircuitPlayground.clearPixels();
  CircuitPlayground.setPixelColor(4,0x0000FF);
  if (decreaseRate > 0.5){
    CircuitPlayground.setPixelColor(3,0x0000FF);
    if (decreaseRate > 1){
      CircuitPlayground.setPixelColor(2,0x007F7F);
      if (decreaseRate > 2){
        CircuitPlayground.setPixelColor(1,0x007F7F);
        if (decreaseRate > 3){
          CircuitPlayground.setPixelColor(0,0x00FF00);
          if (decreaseRate > 4){
            CircuitPlayground.setPixelColor(9,0x00FF00);
            if (decreaseRate > 5){
              CircuitPlayground.setPixelColor(8,0x7F7F00);
              if (decreaseRate > 6){
                CircuitPlayground.setPixelColor(7,0x7F7F00);
                if (decreaseRate > 6.5){
                  CircuitPlayground.setPixelColor(6,0xFF0000);
                  if (decreaseRate > 7) CircuitPlayground.setPixelColor(5,0xFF0000);
                }
              }
            }
          }
        }
      }
    }
  }
}

// Calculates the deflation rate and light up neo pixels accordingly
void deflationRate(float currPressure){
  float prevPressure = last20Pressures[(last20PressuresIdx+1)%20];
  slopeBar(abs(currPressure - prevPressure));
  last20Pressures[last20PressuresIdx] = currPressure;
  last20PressuresIdx = (last20PressuresIdx + 1) % 20;
}

void setup(){
  Serial.begin(9600);
  Wire.begin();
  CircuitPlayground.begin();
  stage = 1;
  CircuitPlayground.setPixelColor(4, 0xFFFF00);
}

void loop(){
  currPressure = readPressure(); // read pressure from the sensor
  delay(45); // additional 5 miliseconds for processing

  // PRE-PUMPING STAGE
  if((stage == 1) && (currPressure > 30)) stage = 2;

  // PUMPING STAGE
  else if((stage == 2)){
    pumpingBar(currPressure); // set LEDs to indicate progress
    if(currPressure > 170){ // required pressure reached
      for(int x = 0; x < 10; x++) CircuitPlayground.setPixelColor(x, 0xFFFFFF); // set the LEDs to white to let the user stop pumping
      stage = 3; // move to stage 3
    }
  }

  // PRE-DEFLATION STAGE
  else if(stage == 3){
    // prime the last20Pressures array for stage 4
    last20Pressures[last20PressuresIdx] = currPressure;
    last20PressuresIdx = (last20PressuresIdx + 1) % 20;
    
    // if two seconds in stage 3 have passed, prepare for stage 4 and move to it
    if(timeAtStage3 > 40){
      timeAtStage3 = 0; // make sure the timer is zeroed-out for the next user
      prevPressure = currPressure; // save current pressure to prevPressure
      heartBeatCounter = 0;
      timeForHeartRate = 0;
      maxDifference = 0;
      stage = 4; // move to stage 4
    }

    timeAtStage3++;
  }

  // DEFLATION AND MEASUREMENT STAGE
  else if(stage == 4){
    deflationRate(currPressure);
    float slope = (currPressure - prevPressure);
    
    //Before systolic is found, no need to worry about detecting heartbeats
    if(systolicFound){
      //This is the end of a heartbeat
      if(goingUp && (slope < 0)){
        pressureTopPeak = currPressure;
        if(((pressureTopPeak - pressureBottomBeat) > maxDifference) && (currPressure <= 70)){
          //Start accounting for biggest heartbeat below 70 mmHg, the measurement is unreliable if the data is captured earlier
          maxDifference = pressureTopPeak - pressureBottomBeat;
          maxPeak = currPressure;
        }
        else strongestHeartbeat = maxPeak;
        goingUp = false;
      }
      //This means that a new heartbeat was detected
      else if(!goingUp && (slope > 0)){
        pressureBottomBeat = currPressure;
        goingUp = true;
        heartBeatCounter++;
      }
      timeForHeartRate++;
    }

    //The program enters this if statement when the first heartbeat/systolic is found
    if((slope > 0) && (!systolicFound)){
      systolic = currPressure * 0.85; // coefficient determined experimentally
      systolicFound = true;
      goingUp = true;
      pressureBottomBeat = currPressure;
      heartBeatCounter++;
    }

    prevPressure = currPressure;

    // set all LEDs to pink to signal end of measurement, progress to data communication stage
    if(currPressure < 50){
      for(uint8_t i = 0; i < 10; i++) CircuitPlayground.setPixelColor(i, 0xFF00FF);
      stage = 5;
    }
  }

  // DATA COMMUNICATION STAGE
  else if(stage == 5){
    Serial.print("Your systolic is ");
    Serial.println(systolic);
    diastolic = systolic - strongestHeartbeat;
    Serial.print("Your diastolic is ");
    Serial.println(diastolic);
    float heartRate = heartBeatCounter / (timeForHeartRate*0.0009); //Multiply the time by 0.0009 because it's in milliseconds, compensated for processing delays
    Serial.print("Your heart rate is ");
    Serial.println(heartRate);
    delay(10000);

    // return to stage 1 for device reuse
    CircuitPlayground.clearPixels();
    CircuitPlayground.setPixelColor(4, 0xFFFF00);
    stage = 1;
  }
}