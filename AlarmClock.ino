        

//DS


//Remote

#define	RECIEVE_PIN  4
#define TONE_PIN  A4


#include "ClockScreen.h"
#include "RemoteCtr.h"


ClockScreen* screen;
RemoteCtr* remotectr;




volatile bool alarmState = false;
byte noteDurationsClock[] = {  4, 8, 4, 8, 4, 8, 4, 8 };
unsigned long previousAlarmMillis = 0;


void setup() {
  

  
	
	screen = new ClockScreen();
	remotectr = new RemoteCtr(RECIEVE_PIN);

  pinMode(TONE_PIN, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(2), alarmISR, FALLING); //digitalPin 2 -Atmeg Pin-4
}

void loop() {

	byte command = remotectr->decode();
	if (command != 0){
		//command recieved
    screen->DispatchCommand(command);
	}
  if(remotectr->isIdle()){
	  screen->ShowCurrentView();
  }

  // making alarm tone
  if(alarmState){
    if (millis() - previousAlarmMillis <= 15000) {
      for (int thisNote = 0; thisNote < 8; thisNote++) {
        int noteDuration = 1000 / noteDurationsClock[thisNote];
        digitalWrite(TONE_PIN,HIGH); delay(noteDuration);digitalWrite(TONE_PIN,LOW); delay(noteDuration * 1.30);
      }
    }
    else{
      alarmState=false;
      screen->StopAlarm();
    }
  }
}


void alarmISR(){
  alarmState = true;
  previousAlarmMillis = millis();
}



