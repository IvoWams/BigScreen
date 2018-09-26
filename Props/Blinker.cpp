#include "Blinker.h"
#include <global/Globals.h>

Blinker::Blinker(){
    blink_start = 0;
    blink_stop = 0;
    blink_duration = 0;
    blink_offset = 0;	
}

Blinker::~Blinker(){
}

bool	Blinker::isBlinked(){
	double duration = Globals::getGlobals()->engine->getDuration();
	if(duration > blink_start && duration < blink_stop){
		double blink_progress = duration - blink_start;
		while(blink_progress > (blink_duration + blink_offset))blink_progress -= (blink_duration + blink_offset);
		return (blink_progress < blink_duration);
	}
	return false;
}
