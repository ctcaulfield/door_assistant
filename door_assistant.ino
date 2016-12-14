// Hardware: 
//Grove - Sound Sensor - A0/A1
//Grove LED Bar - 12C
//Button clicker A3/A4
//Buzzer D3/D4
/*macro definitions of the sound sensor and the LED*/
#define SOUND_SENSOR A0
#define NUM_OF_LEDS 10 // Number of LEDs in the LED bar
#define THRESHOLD_VALUE 200//The threshold to turn the led on 400.00*5/1024 = 1.95v

#include <Grove_LED_Bar.h>

Grove_LED_Bar LEDbar(A4, A5, 1); // Setting the data and clock pins used

int speakerPin = 3;
const int  buttonPin = A3;
// variables will change:
int buttonState = 0;


//setup tune
boolean playTune = false; //default
int length = 9;
char notes[]="acbdebaaf";
int beats[]={1,1,2,1,1,4,1,1,3};
int tempo = 300;


void setup()
{
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);
  LEDbar.begin();
}

void loop()
{
  // read the pushbutton input pin:
  buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH){
      if(playTune){
        playTune = false;
        Bean.setLed(0,255,0);
      }
      //if currently true
      else{
        playTune = true;
        Bean.setLed(139,0,0);
      }
   }
   //avoid bouncing
   delay(100);
   int sensorValue = analogRead(SOUND_SENSOR);
   //if sound is louder than threshold
   if(sensorValue > THRESHOLD_VALUE){
    //difference of values
    turnOnLED(sensorValue - THRESHOLD_VALUE);
    //user wants audio based on button click selection? red/green
    if (playTune) {
      goneTune();
    }
   }
   turnOffLED();
}


//turns on LED bar
void turnOnLED(int loudness)
{ 
  //loudness is in the 100's - divide by 100 to reach within 1-10 scale for bar level
  float barLength = loudness/100*(2);
  //starting point
  LEDbar.setLed(1,1);
  //span out based on frequency
  //setLed to .5 for half brightness
  for(int i=2;i<barLength;i++){
    LEDbar.setLed(i,.5);
  }
  //last bar will be fully brighten since its the end point
  LEDbar.setLed(barLength,1);
}

//remove all
void turnOffLED()
{
  for(int i=0;i<=10;i++){
    LEDbar.setLed(i,0);
  }
}

//im gone! Make noise!
void goneTune(){
    for(int i=0;i<length;i++){
      if(notes[i] == ' '){
        delay(beats[i]*tempo); //rest
      }
      else{
        playNote(notes[i],beats[i]*tempo);
      }
     //pause between notes
     delay(tempo/2);
    }  
}
//tone to play
void playTone(int tone, int duration){
  for(long i=0; i<duration * 1000L; i+=tone*2){
    digitalWrite(speakerPin,HIGH);
    delayMicroseconds(tone);
    digitalWrite(speakerPin, LOW);
    delayMicroseconds(tone);
  }
}
//set of notes to play
void playNote(char note, int duration){
  char names[] = {'c','d','e','f','g','a','b','b'};
  int tones[] = {1915,1700,1519,1432,1275,1136,1014,956};

  //play the tone for the corresponding notes
  for(int i=0; i<8; i++){
    if(names[i]  == note){
      playTone(tones[i],duration);
    }
  }
  
}

