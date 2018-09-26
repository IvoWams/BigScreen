using namespace std;

#ifndef TICKER_H
#define TICKER_H

// Ticker is a (abstract) container that lets its contents scroll from right to left
// Contents are chartiles

// Different types of tickers? (concretetickers)
// GraffitiTicker

// Parsing text for tickers?

#include <main/MainListener.h>
#include <main/MainEngine.h>
#include <timer/TimerListener.h>
#include <timer/Timer.h>
#include <vector>
#include <global/RootObject.h>
#include <string>

class Ticker {
	private:
		MainEngine*				engine;
		vector<RootObject*>*	line;
		
	public:
		Ticker();
		Ticker();
		
		void	setHeight(double _height){ height = _height; }
		void	setSpeed(double _speed){ speed = _speed; }
		
		vector<RootObject*>*	getLine();

		void					addObject(RootObject*);
};

#endif
