/*
 * 
 * https://github.com/hydronics2/COVID-19-Airflow-Sensor-AFH55M12
 * 
 * PCB accepts dual footprint feather ESP32 and Adafruit ItsyBitsy M0
 * 
 * 
 * //4/15/20 - added SLM output
 * //4/21/20 - updated with curve fit calibration from Patrick


*/

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h> // https://github.com/adafruit/Adafruit_Sensor
#include <Adafruit_BME280.h> //https://github.com/adafruit/Adafruit_BME280_Library

#include <Adafruit_GFX.h>  // https://github.com/adafruit/Adafruit-GFX-Library
#include <Adafruit_SSD1306.h>  // https://github.com/adafruit/Adafruit_SSD1306
#include <avr/dtostrf.h> //for converting from an integer to a string for the display
Adafruit_SSD1306 display(128, 64);  // Create display


#include <Fonts/FreeMonoBold12pt7b.h>  // Add a custom font
#include <Fonts/FreeMonoBoldOblique12pt7b.h>
#include <Fonts/FreeMono9pt7b.h>  // Add a custom font


#define atmosphericPressure 101625 //need a number (Pascals) here to evaluate BME280... may need a BME280 sensor on the top of the board to get atomospheric

Adafruit_BME280 bme1; // I2C pressure sensor under the display
Adafruit_BME280 bme2; // I2C pressure sensor on the bottom of the board

//pinouts for Feather esp32
//const int errorLED = 21; //feather esp32
//const int userLED = 22; //feather esp32
//const int buzzer = 23; //feather esp32
//const int encoder0PinA = 19;  //feather esp32
//const int encoder0PinB = 20;  //feather esp32
//const int encoderButton = 25; //feather esp32
//const int userButton = 24; //feather esp32

const int errorLED = 5; //itsybitsy M0
const int userLED = 7; //itsbitsy M0 
const int buzzer = 9; //itsybitsy M0
const int encoder0PinA = 0;  //itsybitsy M0
const int encoder0PinB = 1;  //itsybitsy M0
const int encoderButton = 11; //itsybitsy M0
const int userButton = 10; //itsybitsy M0

const int flowSensorPin = A0; 
int flowSensorValue = 0;   

const int sizeBreath = 500;
int arrayBreath[sizeBreath];
int incrementBreath = 0;

//these arrays capture the rolling average (non-breath) sensor value baseline
const int sizeRollingSmall = 10;
int arrayAverageSmall[sizeRollingSmall];
int incrementRollingSmall = 0;

const int sizeRolling = 50;
byte arrayAverage[sizeRolling];
int incrementRolling = 0;

boolean startBreathFlag = false;
boolean breathFlag = false;
int flowSensorThreshold = 1824; //temporary number
int averageSensorValue = 0;
long lastTimeSense;
int senseInterval = 20;

int flowSensorCalibration = 162; //actually number is 0.0162 converts delta ADC into SLM
double timePeriod;

int pressureReading = 0;
float pressureReading2 = 0;

void setup()  // Start of setup
{        
  Serial.begin(115200);
  analogReadResolution(12); //required for SAMD21

  pinMode(errorLED, OUTPUT);
  digitalWrite(errorLED, LOW);
  pinMode(userLED, OUTPUT);
  digitalWrite(userLED, LOW);
  pinMode(buzzer, OUTPUT);
  
  timePeriod = (double)senseInterval/1000; //time period in seconds
  Serial.print("sensing time period(seconds)= ");
  Serial.println(timePeriod);
  Serial.print("calibration factor = ");
  Serial.println(flowSensorCalibration/10000);
  
  delay(100);  // This delay is needed to let the display to initialize
}  


void loop()  // Start of loop
{
  long now = millis();

  if(now - lastTimeSense > senseInterval)
  {
    flowSensorValue = analogRead(flowSensorPin);
    //Serial.println(flowSensorValue);
    lastTimeSense = now;
    if(flowSensorValue > flowSensorThreshold)
    {
      if(startBreathFlag == false)
      {
        startBreathFlag = true;
        
        incrementBreath = 0;
      }else
      {
        //int SLM = (flowSensorValue - averageSensorValue)*flowSensorCalibration; //converts SLM 
        
        float SLM = constrain(17.816955 - .029497326 * flowSensorValue + 1.231863E-5 * sq(flowSensorValue),0,100);
        
        Serial.print("time: ");
        Serial.print(now);
        Serial.print(" ms, SLM: ");
        //Serial.println((float)SLM/10000); //converts flow sensor calibration back to correct magnitude for printing
        Serial.println(SLM);
        digitalWrite(userLED, HIGH);     
        if(incrementBreath == 20) 
        {
          breathFlag = true;
          digitalWrite(errorLED, HIGH);
        }
        arrayBreath[incrementBreath] = flowSensorValue;
        incrementBreath++;
        if(incrementBreath == sizeBreath){
          incrementBreath = 0;      
        }
      }
    }else 
    {
      startBreathFlag = false;
      breathFlag = false;
      digitalWrite(userLED, LOW);
      digitalWrite(errorLED, LOW);
      analogWrite(buzzer, 0);
      arrayAverageSmall[incrementRollingSmall] = flowSensorValue; //keep track of the average no-flow sensor value
      incrementRollingSmall++;
      if(incrementRollingSmall == sizeRollingSmall){               
        incrementRollingSmall = 0;
        int flowSensorSmallAverage = 0;
        for(int i = 0; i< sizeRollingSmall; i++)
        {
          flowSensorSmallAverage = flowSensorSmallAverage + arrayAverageSmall[i];
        }
        flowSensorSmallAverage = flowSensorSmallAverage/sizeRollingSmall;
        arrayAverage[incrementRolling] = flowSensorSmallAverage/10; //make it small enough to fit in a byte array.
        incrementRolling++;
        if(incrementRolling == sizeRolling){
          printAverageSensorValue();
          incrementRolling = 0;      
        }      
      }
    }
  }
}


void printAverageSensorValue()
{
  averageSensorValue = 0;
  for(int i=0; i<sizeRolling; i++)
  {
    averageSensorValue = averageSensorValue + arrayAverage[i];    
  }
  averageSensorValue = averageSensorValue/(sizeRolling);
  averageSensorValue = averageSensorValue*10; // change back into correct magnitude after packaging as a byte
  Serial.println();
  Serial.print("average sensor value: ");
  Serial.println(averageSensorValue);
  Serial.println();
  flowSensorThreshold = averageSensorValue + 200;
}
