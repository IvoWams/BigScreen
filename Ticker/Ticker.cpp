#include <Ticker.h>
#include <global/Globals.h>

Ticker::Ticker() : MainListener(){
	engine = Globals::getGlobals()->engine;
	
	tiles = new vector<CharTile*>;
}

Ticker::~Ticker(){
	delete tiles;
}

void	Ticker::onStart(){
}

void	Ticker::onFrame(){
}

void	Ticker::onStop(){
}

