/*
 * 
 * https://github.com/hydronics2/COVID-19-Airflow-Sensor-AFH55M12
 * 
 * PCB accepts dual footprint feather ESP32 and Adafruit ItsyBitsy M0
 * 
 * 
 * //4/15/20 - added SLM output
 * //4/21/20 - adjusted with Patrick's curve fit calbration equation.


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
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // Initialize display with the I2C address of 0x3C 
  display.clearDisplay();  // Clear the buffer
  display.setTextColor(WHITE);  // Set color of the text
  display.setRotation(0);  // Set orientation. Goes from 0, 1, 2 or 3
  display.setTextWrap(false);  // By default, long lines of text are set to automatically “wrap” back to the leftmost column.
                               // To override this behavior (so text will run off the right side of the display - useful for
                               // scrolling marquee effects), use setTextWrap(false). The normal wrapping behavior is restored
                               // with setTextWrap(true).
  display.dim(0);  //Set brightness (0 is maximun and 1 is a little dim)

  if (! bme1.begin(0x76, &Wire)) //this is the sensor under the display
  {
    Serial.println("Could not find a valid BME280 sensor, check wiring! for 0x76");
    while (1);
  }
//  if (! bme2.begin(0x77, &Wire)) {
//    Serial.println("Could not find a valid BME280 sensor, check wiring! for 0x77");
//    while (1);
//  }
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
          displayBreath();
          digitalWrite(errorLED, HIGH);
          analogWrite(buzzer, 5);
          pressureReading = bme1.readPressure(); //ok lets take a pressure reading now that there is flow
        }
        arrayBreath[incrementBreath] = flowSensorValue;
        incrementBreath++;
        if(incrementBreath == sizeBreath){
          incrementBreath = 0;      
        }
      }
    }else 
    {
      if(breathFlag) //a breath just finished... calculate the volume
      {        
        calculateBreath();
      }
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


void calculateBreath()
{
  pressureReading = pressureReading - atmosphericPressure; //subtract current pressure
  pressureReading2 = (float)pressureReading * 0.0101972; //convert to cm of water
  Serial.print("pressure reading: ");
  Serial.println(pressureReading2);

  
  long totalSensorValue = 0;
  for(int i=0; i<sizeRolling; i++)
  {
    totalSensorValue = totalSensorValue + arrayAverage[i];    
  }
  totalSensorValue = totalSensorValue/(sizeRolling);
  averageSensorValue = totalSensorValue*10; // change back into correct magnitude after packaging as a byte
  Serial.println();
  Serial.print("average sensor value: ");
  Serial.println(averageSensorValue);
  float totalBreath = 0;  
  for(int i=0; i<incrementBreath; i++) //evaluate all the flow readings we received over the breath
  {
    //long change = (arrayBreath[i] - averageSensorValue)*flowSensorCalibration*senseInterval*1000/60000; //converts SLM to mL
    float change = (constrain(17.816955-.029497326*arrayBreath[i]+1.231863E-5*sq((float)arrayBreath[i]),0,100))*senseInterval*1000/60000; //converts SLM to mL
    totalBreath = totalBreath + change;
  }
  Serial.print("increment Breath: "); //this is how many flow samples we evaluated
  Serial.println(incrementBreath);
  Serial.print("volume: "); //this is the total volume of all the flow samples
  //Serial.println((float)totalBreath/10000); //adjust for correct magnitude of calibration factor
  Serial.println(totalBreath);
  //float totalBreath2 = totalBreath/10000;
  updateDisplay(totalBreath);
}  


void updateDisplay(int flowValue)
{
  // Convert flowValue into a string, so we can change the text alignment to the right:
  // It can be also used to add or remove decimal numbers.
  char string[10];  // Create a character array of 10 characters
  char pressure[10];
  // Convert float to a string:
  dtostrf(flowValue, 4, 0, string);  // (<variable>,<amount of digits we are going to use>,<amount of decimal digits>,<string name>)
  
  //floatToString(pressure,pressureReading2,0,7,true);
  //dtostrf(pressureReading2, 4, 0, pressure);

  display.clearDisplay();  // Clear the display so we can refresh

  display.setFont(&FreeMonoBoldOblique12pt7b);  // Set a custom font
  display.setTextSize(0);  // Set text size. We are using a custom font so you should always use the text size of 0
  // Print text:
  display.setCursor(0, 20);  // (x,y)
  display.println("Vt");  // Text or value to print
  // Draw rectangle:
  display.drawRect(40, 0, 60, 27, WHITE);  // Draw rectangle (x,y,width,height,color)
                                           // It draws from the location to down-right   
  display.setFont(&FreeMonoBold12pt7b);  // Set a custom font  
  // Print variable with left alignment:
  display.setCursor(42, 20);  // (x,y)
  display.println(string);  // string for right alignment

  // Print text:
  display.setFont(&FreeMono9pt7b);  // Set a custom font
  display.setCursor(0, 40);  // (x,y)
  display.println("PEEP");  // Text or value to print
  
  // Print text:
  display.setFont(&FreeMono9pt7b);  // Set a custom font
  display.setCursor(0, 57);  // (x,y)
  display.println("/PIP");  // Text or value to print

  // Print mL
  display.setCursor(105, 20);  // (x,y)
  display.println("mL");

  // Print cm
  display.setCursor(105, 57);  // (x,y)
  display.println("cm");

  // Print variable with right alignment:
  display.setCursor(51, 57);  // (x,y)
  display.println(pressureReading2);  // Text or value to print
  display.setCursor(83, 57);  // (x,y)
  //display.println(pressure);  // Text or value to print

  display.display();  // Print everything we set previously
}

void displayBreath()
{

  display.clearDisplay();  // Clear the display so we can refresh
  // Print variable with right alignment:
  display.setCursor(50,40);  // (x,y)
  display.println("breath");  // Text or value to print
  display.setCursor(50,60);  // (x,y)
  display.println("triggered");  // Text or value to print
  display.display();  // Print everything we set previously
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
  Serial.println();
  Serial.print("average sensor value: ");
  Serial.println(averageSensorValue);

  flowSensorThreshold = averageSensorValue + 200;
}
