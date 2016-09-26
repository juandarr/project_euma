/* 
 * //////////////////////////////////////////////////
 * //making sense of the Parallax PIR sensor's output
 * //////////////////////////////////////////////////
 *
 * Switches a LED according to the state of the sensors output pin.
 * Determines the beginning and end of continuous motion sequences.
 *
 * @author: Kristian Gohlke / krigoo (_) gmail (_) com / http://krx.at
 * @date:   3. September 2006 
 *
 * kr1 (cleft) 2006 
 * released under a creative commons "Attribution-NonCommercial-ShareAlike 2.0" license
 * http://creativecommons.org/licenses/by-nc-sa/2.0/de/
 *
 *
 * The Parallax PIR Sensor is an easy to use digital infrared motion sensor module. 
 * (http://www.parallax.com/detail.asp?product_id=555-28027)
 *
 * The sensor's output pin goes to HIGH if motion is present.
 * However, even if motion is present it goes to LOW from time to time, 
 * which might give the impression no motion is present. 
 * This program deals with this issue by ignoring LOW-phases shorter than a given time, 
 * assuming continuous motion is present during these phases.
 *  
 */

#include <Adafruit_NeoPixel.h>

/////////////////////////////
//VARS
//the time we give the sensor to calibrate (10-60 secs according to the datasheet)
int calibrationTime = 30;



//the time when the sensor outputs a low impulse
long unsigned int lowIn;         

//the amount of milliseconds the sensor has to be low 
//before we assume all motion has stopped
long unsigned int pause = 5000;  

boolean lockLow = true;
boolean takeLowTime;  

int pirPin = 12;    //the digital pin connected to the PIR sensor's output
#define PIN 8 //Neopixel pin

Adafruit_NeoPixel strip = Adafruit_NeoPixel(12, PIN, NEO_GRB + NEO_KHZ800);

int shift = 1;

int permutations[7][3] = {{0,0,1}, {0,1,0}, {0,1,1}, {1,0,0},{1,0,1}, {1,1,0}, {1,1,1}};
int cont = 0;
/////////////////////////////
//SETUP
void setup(){
	
  strip.begin();
  strip.show();
  strip.setBrightness(255);
  
  pinMode(pirPin, INPUT);
  digitalWrite(pirPin, LOW);

  //give the sensor some time to calibrate
  
    for(int i = 0; i < calibrationTime; i++){
      //Serial.print(".");
      delay(1000);
      }
    //Serial.println(" done");
    //Serial.println("SENSOR ACTIVE");
    delay(50);
  }

void color()
{
  for(int i=0; i< strip.numPixels(); i++)
  {
    strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) +shift) & 255));
  }
  strip.show();
}

void fixedColor(int x, int y, int z)
{
  
  for(int i=0; i< strip.numPixels(); i++)
  {
    strip.setPixelColor(i, x , y , z );
  }
  strip.show();
  
}

////////////////////////////
//LOOP
void loop(){

     if(digitalRead(pirPin) == HIGH){
       
       
  	   
       if(lockLow){  
       	
       	fixedColor(255*permutations[cont][0], 255*permutations[cont][1], 255*permutations[cont][2]);
	    cont++;
  	    if (cont>6) cont=0;
         //makes sure we wait for a transition to LOW before any further output is made:
         lockLow = false;            
         //Serial.println("---");
         //Serial.print("motion detected at ");
         //Serial.print(millis()/1000);
         //Serial.println(" sec"); 
         delay(50);
         
         }         
         takeLowTime = true;
       }

     if(digitalRead(pirPin) == LOW){  
       
       	
       //color();
       //shift++;
       //if (shift==250) shift=1;
	   
	   
       if(takeLowTime){
       	
        lowIn = millis();          //save the time of the transition from high to LOW
        takeLowTime = false;       //make sure this is only done at the start of a LOW phase
        
        }
       //if the sensor is low for more than the given pause, 
       //we assume that no more motion is going to happen
       if(!lockLow && millis() - lowIn > pause){  
           //makes sure this block of code is only executed again after 
           //a new motion sequence has been detected
           lockLow = true;                        
           //Serial.print("motion ended at ");      //output
           //Serial.print((millis() - pause)/1000);
           //Serial.println(" sec");
           fixedColor(0,0,0);
           delay(50);
           
           
           }
       }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}
