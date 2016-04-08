
// RemoteCtr.h

#ifndef _REMOTECTR_h
#define _REMOTECTR_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif


#define REMOTE_LEFT    0X2D0
#define REMOTE_RIGHT  0XCD0
#define REMOTE_UP   0X2F0
#define REMOTE_DOWN   0XAF0
#define REMOTE_MODE   0XA70
#define REMOTE_MUTE   0x210

#include <IRremote.h>

class RemoteCtr
{
  public:
    const byte COMMAND_LEFT = 1;
    const byte COMMAND_RIGHT = 2;
    const byte COMMAND_UP = 3;
    const byte COMMAND_DOWN = 4;
    const byte COMMAND_MODE = 5;
    const byte COMMAND_MUTE = 6;
  
    RemoteCtr(int recPin);
    
  
    void init();
    byte decode();
    bool isIdle();
  
  private:
    int RECV_PIN = 4;
    IRrecv* irrecv;
    decode_results results;
};

#endif
