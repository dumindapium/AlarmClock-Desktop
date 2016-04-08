//
//
//
//LCD
//      SCK  - Pin 8
//      MOSI - Pin 7
//      DC   - Pin 6
//      RST  - Pin 9
//      CS   - Pin 5

#include "ClockScreen.h"


extern uint8_t SmallFont[];
extern uint8_t BigNumbers[];


String printDigits(byte digits);



ClockScreen::ClockScreen() {
  ds1306 = new DS1306(10);
  lcd5110 = new LCD5110(8, 7, 6, 9, 5);  //(5, 6, 7, 8, 9);


  pinMode(LCD_LED,OUTPUT);

  ds1306->init();
  delay(200);
  ds1306->setSeconds(0);  //sec20
  //ds1306->setMinutes(22);  //min 30
  //ds1306->setHours(true,false,1); //hour 4
  //ds1306->setDate(01);
  //ds1306->setMonth(01);
  ds1306->setYear(16);
  ds1306->setALHours(true,false,6);
  ds1306->setALMinutes(30);
  ds1306->setALSeconds(0);
  
  
  lcd5110->InitLCD();
  delay(1000);

  currentMode = VIEW_CLOCK;

}

void ClockScreen::ShowCurrentView() {
//  if(((byte)timeInfo[0]>=8 && ((char)timeInfo[3])=='A') || ((byte)timeInfo[0]<=5 && ((char)timeInfo[3])=='P')){
//    analogWrite(LCD_LED,0);
//  }else{analogWrite(LCD_LED,220);}


  //analogWrite(LCD_LED,0);
  
  switch (currentMode)
  {
    case SET_TIME: {
        showSetClockTime();
      } break;
    case SET_DATE: {
        showSetClockDate();
      } break;
    case SET_ALARM: {
        showSetAlarm();
      } break;
    case VIEW_CLOCK: {
        showViewClock();
      } break;
  }
}

void ClockScreen::DispatchCommand(byte command) {
  switch (command) {
    case ClockScreen::SCOMMAND_LEFT: {
        //TIME
        if (currentMode == SET_TIME && currentSelect == SET_CLOCK_MM) {
          currentSelect = SET_CLOCK_HH;
        }

        //DATE
        if (currentMode == SET_DATE && currentSelect == SET_CLOCK_DATE) {
          currentSelect = SET_CLOCK_MONTH;
        }
        else if (currentMode == SET_DATE && currentSelect == SET_CLOCK_MONTH) {
          currentSelect = SET_CLOCK_YEAR;
        }

        //ALARM
        if (currentMode == SET_ALARM && currentSelect == SET_ALARM_DAY) {
          currentSelect = SET_ALARM_MM;
        }
        else if (currentMode == SET_ALARM && currentSelect == SET_ALARM_MM) {
          currentSelect = SET_ALARM_HH;
        }
      } break;
    case ClockScreen::SCOMMAND_RIGHT: {
        //TIME
        if (currentMode == SET_TIME && currentSelect == SET_NONE) {
          currentSelect = SET_CLOCK_HH;
        }
        else if (currentMode == SET_TIME && currentSelect == SET_CLOCK_HH) {
          currentSelect = SET_CLOCK_MM;
        }

        //DATE
        if (currentMode == SET_DATE && currentSelect == SET_NONE) {
          currentSelect = SET_CLOCK_YEAR;
        }
        else if (currentMode == SET_DATE && currentSelect == SET_CLOCK_YEAR) {
          currentSelect = SET_CLOCK_MONTH;
        }
        else if (currentMode == SET_DATE && currentSelect == SET_CLOCK_MONTH) {
          currentSelect = SET_CLOCK_DATE;
        }

        //ALARM
        if (currentMode == SET_ALARM && currentSelect == SET_NONE) {
          currentSelect = SET_ALARM_HH;
        }
        else if (currentMode == SET_ALARM && currentSelect == SET_ALARM_HH) {
          currentSelect = SET_ALARM_MM;
        }
        else if (currentMode == SET_ALARM && currentSelect == SET_ALARM_MM) {
          currentSelect = SET_ALARM_DAY;
        }
      } break;
    case ClockScreen::SCOMMAND_UP: {
        ChangeValue(1);
      } break;
    case ClockScreen::SCOMMAND_DOWN: {
        ChangeValue(0);
      } break;
    case ClockScreen::SCOMMAND_MODE: {
        if (currentMode == VIEW_CLOCK) {
          currentMode = SET_TIME;
        }
        else if (currentMode == SET_TIME) {
          if(isChanged){
            ds1306->setHours(true,((byte)timeInfo[0]>12),((byte)timeInfo[0]>12)?((byte)timeInfo[0]-12):((byte)timeInfo[0]));
            ds1306->setMinutes((byte)timeInfo[1]);
            //seconds ignored
          }
          currentMode = SET_DATE;
        }
        else if (currentMode == SET_DATE) {
          if(isChanged){
            ds1306->setDate((byte)dateInfo[2]);
            ds1306->setMonth((byte)dateInfo[1]);
            ds1306->setYear((byte)dateInfo[0]);
          }
          currentMode = SET_ALARM;
        }
        else if (currentMode == SET_ALARM) {
          if(isChanged){
            ds1306->setALDay((byte)alarmInfo[0]);
            ds1306->setALHours(true,((byte)alarmInfo[1]>12),((byte)alarmInfo[1]>12)?((byte)alarmInfo[1]-12):((byte)alarmInfo[1]));
            ds1306->setALMinutes((byte)alarmInfo[2]);
            ds1306->enableAlarm(true);
          }
          currentMode = VIEW_CLOCK;
        }
        currentSelect = SET_NONE;
        LoadCurrentValues();
      } break;

//      case ClockScreen::SCOMMAND_MUTE:{
//        StopAlarm();
//      } break;
  }
}

void ClockScreen::StopAlarm(){
  ds1306->setALDay(alarmInfo[0]);
}



//----------------------------- Private methods --------------------------------

void ClockScreen::LoadCurrentValues() {
  switch (currentMode)
  {
    case SET_TIME: {
        ds1306->getTime(timeInfo);
      } break;
    case SET_DATE: {
        ds1306->getDate(dateInfo);
      } break;
    case SET_ALARM: {
        ds1306->getAlarm(alarmInfo);
      } break;
  }
}

void ClockScreen::showSetClockTime() {

  lcd5110->clrScr();
  lcd5110->setFont(SmallFont);
  lcd5110->print("Set time", 0, 0);
  //timeString = printDigits(timeInfo[0]) + "-" + printDigits(timeInfo[1]); // "06-41";
  lcd5110->setFont(BigNumbers);

  switch (currentSelect)
  {
    case SET_CLOCK_HH: {
        lcd5110->invertText(true);
        lcd5110->print(printDigits(timeInfo[0]), 0, 10);
        lcd5110->invertText(false);
        lcd5110->print("-" + printDigits(timeInfo[1]), 28, 10);
      }
      break;
    case SET_CLOCK_MM: {
        lcd5110->print(printDigits(timeInfo[0]) + "-"  , 0, 10);
        lcd5110->invertText(true);
        lcd5110->print(printDigits(timeInfo[1]), 42, 10);
      }
      break;
    case SET_NONE: {
        timeString = printDigits(timeInfo[0]) + "-" + printDigits(timeInfo[1]);
        lcd5110->print(timeString, 0, 10);
      }
      break;
  }
  lcd5110->update();
}

void ClockScreen::showSetClockDate() {

  lcd5110->clrScr();
  lcd5110->setFont(SmallFont);
  lcd5110->print("Set date", 0, 0);
  //dateString = "20" + printDigits(dateInfo[0]) + "-" + printDigits(dateInfo[1]) + "-" + printDigits(dateInfo[2]);
  switch (currentSelect)
  {
    case SET_CLOCK_YEAR: {
        lcd5110->invertText(true);
        lcd5110->print("20" + printDigits(dateInfo[0]), 0, 16);
        lcd5110->invertText(false);
        lcd5110->print("-" + printDigits(dateInfo[1]) + "-" + printDigits(dateInfo[2]), 24, 16);
      }
      break;
    case SET_CLOCK_MONTH: {
        lcd5110->print("20" + printDigits(dateInfo[0]) + "-", 0, 16);
        lcd5110->invertText(true);
        lcd5110->print(printDigits(dateInfo[1]) , 30, 16);
        lcd5110->invertText(false);
        lcd5110->print("-" + printDigits(dateInfo[2]), 42, 16);
      }
      break;
    case SET_CLOCK_DATE: {
        lcd5110->print("20" + printDigits(dateInfo[0]) + "-" + printDigits(dateInfo[1]) + "-" , 0, 16);
        lcd5110->invertText(true);
        lcd5110->print(printDigits(dateInfo[2]), 48, 16);
      }
      break;
    case SET_NONE: {
        dateString = "20" + printDigits(dateInfo[0]) + "-" + printDigits(dateInfo[1]) + "-" + printDigits(dateInfo[2]);
        lcd5110->print(dateString, 0, 16);
      }
      break;
  }
  lcd5110->update();
}

void ClockScreen::showSetAlarm() {

  lcd5110->clrScr();
  lcd5110->setFont(SmallFont);
  lcd5110->print("Set alarm", 0, 0);
  switch (currentSelect)
  {
    case SET_ALARM_HH: {
        lcd5110->print(String(alarmInfo[0]), 74, 10);
        lcd5110->setFont(BigNumbers);
        lcd5110->invertText(true);
        lcd5110->print(printDigits(alarmInfo[1]), 0, 10);
        lcd5110->invertText(false);
        lcd5110->print("-" + printDigits(alarmInfo[2]), 28, 10);
      }
      break;
    case SET_ALARM_MM: {
        lcd5110->print(String(alarmInfo[0]), 74, 10);
        lcd5110->setFont(BigNumbers);
        lcd5110->print(printDigits(alarmInfo[1]) + "-", 0, 10);
        lcd5110->invertText(true);
        lcd5110->print(printDigits(alarmInfo[2]), 42, 10);
      }
      break;
    case SET_ALARM_DAY: {
        lcd5110->invertText(true);
        lcd5110->print(String(alarmInfo[0]), 74, 10);
        lcd5110->setFont(BigNumbers);
        lcd5110->print(printDigits(alarmInfo[1]) + "-" + printDigits(alarmInfo[2]), 0, 10);
      }
      break;
    case SET_NONE: {
        lcd5110->print(String(alarmInfo[0]), 74, 10);
        lcd5110->setFont(BigNumbers);
        lcd5110->print(printDigits(alarmInfo[1]) + "-" + printDigits(alarmInfo[2]), 0, 10);
      }
      break;
  }
  lcd5110->update();
}

void ClockScreen::showViewClock() {

  if (millis() % 1000 > 500) {
    timeString = printDigits(timeInfo[0]) + "-" + printDigits(timeInfo[1]); // "06-41";

  }
  else {
    timeString = printDigits(timeInfo[0]) + "/" + printDigits(timeInfo[1]); // "06/41";

  }
  unsigned long currentMillis = millis();
  if ((currentMillis - prevMillis) >= 1000) {
    Serial.print("Time Read");
    prevMillis = millis();
    ds1306->getTime(timeInfo);
    ds1306->getDate(dateInfo);
    dateString = "20" + printDigits(dateInfo[0]) + "-" + printDigits(dateInfo[1]) + "-" + printDigits(dateInfo[2]);
    Serial.println(timeString);
  }
  lcd5110->clrScr();
  lcd5110->setFont(BigNumbers);
  lcd5110->print(timeString, 0, 0);
  lcd5110->setFont(SmallFont);
  lcd5110->print(String((char)timeInfo[3]), 74, 0);// p for pm
  lcd5110->print(dateString, CENTER, 32);
  lcd5110->update();
}

void ClockScreen::ChangeValue(bool inc) {
  if (inc) { // increase the selected value
    switch(currentSelect){
      case SET_CLOCK_HH:{ timeInfo[0] = ((byte)timeInfo[0] == 23 ? 0 : ((byte)timeInfo[0])+1); } break;
      case SET_CLOCK_MM:{ timeInfo[1] = ((byte)timeInfo[1] == 59 ? 0 : ((byte)timeInfo[1])+1);} break;
      case SET_CLOCK_DATE:{ dateInfo[2] = ((byte)dateInfo[2] == 31 ? 0 : ((byte)dateInfo[2])+1);} break;
      case SET_CLOCK_MONTH:{ dateInfo[1] = ((byte)dateInfo[1] == 12 ? 0 : ((byte)dateInfo[1])+1);} break;
      case SET_CLOCK_YEAR:{ dateInfo[0] = ((byte)dateInfo[0] == 99 ? 0 : ((byte)dateInfo[0])+1);} break;
      case SET_ALARM_HH:{ alarmInfo[1] = ((byte)alarmInfo[1] == 23 ? 0 : ((byte)alarmInfo[1])+1);} break;
      case SET_ALARM_MM:{ alarmInfo[2] = ((byte)alarmInfo[2] == 59 ? 0 : ((byte)alarmInfo[2])+1);} break;
      case SET_ALARM_DAY:{ alarmInfo[0] = ((byte)alarmInfo[0] == 7 ? 0 : ((byte)alarmInfo[0])+1);} break;
    }
    isChanged = (currentSelect!=SET_NONE)? true : false;
  } else { // decrease the selected value

  }
}




String printDigits(byte digits)
{
  String str;
  // utility function for digital clock display: prints preceding colon and leading 0
  if (digits < 10)
    str = "0" + String(digits);
  else
    str = String(digits);

  return str;
}
