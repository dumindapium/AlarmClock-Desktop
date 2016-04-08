// ClockScreen.h

#ifndef _CLOCKSCREEN_h
#define _CLOCKSCREEN_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <DS1306Lib.h>
#include <LCD5110_Graph.h>

#define SET_TIME 1
#define SET_DATE  2
#define SET_ALARM 3
#define SET_CLOCK_HH	4
#define SET_CLOCK_MM	5
#define SET_CLOCK_DATE	6
#define SET_CLOCK_MONTH	7
#define SET_CLOCK_YEAR	8
#define SET_ALARM_HH	9
#define SET_ALARM_MM	10
#define SET_ALARM_DAY  11
#define VIEW_CLOCK	12
#define SET_NONE  99

#define LCD_LED 3 //PWM

class ClockScreen
{
public:
	ClockScreen();
  void DispatchCommand(byte command);
	void ShowCurrentView();
  void StopAlarm();

private:
	DS1306* ds1306;
	LCD5110* lcd5110;
	byte currentMode;
  byte currentSelect;
 
	byte timeInfo[4];   //hh,mm,ss,'p'/'a'
	byte dateInfo[3];   //yy,mm,dd
	byte alarmInfo[3];  //day,hh,mm
	unsigned long prevMillis = 0;
	String timeString = "";
	String dateString = "";
  bool isChanged = false;

  enum commands{
    SCOMMAND_LEFT = 1,
    SCOMMAND_RIGHT = 2,
    SCOMMAND_UP = 3,
    SCOMMAND_DOWN = 4,
    SCOMMAND_MODE = 5,
    SCOMMAND_MUTE = 6
  };
  
	void showSetClockTime();
	void showSetClockDate();
	void showSetAlarm();
	void showViewClock();
  void ChangeValue(bool inc);
  void LoadCurrentValues();
};


#endif

