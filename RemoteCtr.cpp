
#include "RemoteCtr.h"

RemoteCtr::RemoteCtr(int recPin){
	RECV_PIN = recPin;
  init();
}



void RemoteCtr::init(){
	irrecv = new IRrecv(RECV_PIN);
	irrecv->enableIRIn();
}

bool RemoteCtr::isIdle(){
  return irrecv->isIdle();
}

byte RemoteCtr::decode(){
	byte retVal = 0;

	if (irrecv->decode(&results)) {
		
		if (results.decode_type == SONY) {
			switch (results.value) {
			case REMOTE_LEFT:
				retVal = COMMAND_LEFT;
				break;
			case REMOTE_RIGHT:
				retVal = COMMAND_RIGHT;
				break;
			case REMOTE_UP:
				retVal = COMMAND_UP;
				break;
			case REMOTE_DOWN:
				retVal = COMMAND_DOWN;
				break;
			case REMOTE_MODE:
				retVal = COMMAND_MODE;
				break;
			}
			//irrecv.enableIRIn();
		}
		delay(100);
		irrecv->resume(); // Receive the next value
	}
	return retVal;
}
