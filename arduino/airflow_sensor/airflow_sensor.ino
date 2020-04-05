/*
 * Tested with an Adafruit Feather ESP32 
 * 
 * 
 * Libraries needed:
 * https://github.com/adafruit/Adafruit_SSD1306
 * https://github.com/adafruit/Adafruit-GFX-Library
*/

#include <Adafruit_GFX.h>  // Include core graphics library for the display
#include <Adafruit_SSD1306.h>  // Include Adafruit_SSD1306 library to drive the display

Adafruit_SSD1306 display(128, 64);  // Create display


#include <Fonts/FreeMonoBold12pt7b.h>  // Add a custom font
#include <Fonts/FreeMonoBoldOblique12pt7b.h>
#include <Fonts/FreeMono9pt7b.h>  // Add a custom font



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
long lastTimeSense;
int senseInterval = 20;

double flowSensorCalibration = 0.4;
double timePeriod;


void setup()  // Start of setup
{        
  Serial.begin(115200);
  timePeriod = (double)senseInterval/1000; //time period in seconds
  Serial.print("sensing time period(seconds)= ");
  Serial.println(timePeriod);
  Serial.print("calibration factor = ");
  Serial.println(flowSensorCalibration);
  
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
        Serial.println(flowSensorValue);
        if(incrementBreath == 20) 
        {
          breathFlag = true;
          displayBreath();
        }
        arrayBreath[incrementBreath] = flowSensorValue;
        incrementBreath++;
        if(incrementBreath == sizeBreath){
          incrementBreath = 0;      
        }
      }
    }else 
    {
      if(breathFlag)
      {        
        calculateBreath();
      }
      startBreathFlag = false;
      breathFlag = false;

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
        //Serial.println(flowSensorSmallAverage);
        arrayAverage[incrementRolling] = flowSensorSmallAverage/10; //make it small enough to be a byte.
        //Serial.println(arrayAverage[incrementRolling]);
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
  long averageSensorValue = 0;
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
  float totalBreath = 0;  
  for(int i=0; i<incrementBreath; i++)
  {
    float change = (arrayBreath[i] - averageSensorValue)*flowSensorCalibration*timePeriod;
    totalBreath = totalBreath + change;
  }
  
  Serial.print("increment Breath: ");
  Serial.println(incrementBreath);
  Serial.print("volume: ");
  Serial.println(totalBreath);
  updateDisplay(totalBreath);
}  


void updateDisplay(int flowValue)
{
  // Convert flowValue into a string, so we can change the text alignment to the right:
  // It can be also used to add or remove decimal numbers.
  char string[10];  // Create a character array of 10 characters
  // Convert float to a string:
  dtostrf(flowValue, 4, 0, string);  // (<variable>,<amount of digits we are going to use>,<amount of decimal digits>,<string name>)

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

  // Print variable with right alignment:
  display.setCursor(48, 57);  // (x,y)
  //display.println(string);  // Text or value to print
  display.setCursor(83, 57);  // (x,y)
  //display.println(string);  // Text or value to print

  display.display();  // Print everything we set previously
}

void displayBreath()
{

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

  // Print variable with right alignment:
  display.setCursor(50,40);  // (x,y)
  display.println("breath");  // Text or value to print
  display.setCursor(50,60);  // (x,y)
  display.println("triggered");  // Text or value to print

  display.display();  // Print everything we set previously
}

void printAverageSensorValue()
{
  long averageSensorValue = 0;
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
