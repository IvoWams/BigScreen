using namespace std;

#ifndef PAGEPLAYER_H
#define PAGEPLAYER_H

#include <vector>
#include <main/MainEngine.h>
#include <main/MainListener.h>
#include <timer/TimerListener.h>
#include <player/CueManager.h>
#include <page/PageBuilder.h>
#include <page/Page.h>

#include <config/Config.h>

class PagePlayer :	public	TimerListener,
					public	MainListener {
	private:
		MainEngine*						engine;
		vector<RootObject*>*			objects;		// Keep eye on currently playing objects
		CueManager*						cue_manager;	// Provides info for new pages
		Timer*							timer_nextpage;	// time to wait before grabbing new page
		PageBuilder*					pagebuilder;	// Creates page objects from bsml files
				
		int								update(int);	// Processes x objects from the page

//		double							zero_time;
		int								counter;
		
		Config*							config;
		
		ofstream						fout;
		
	public:
		PagePlayer(Config*);
		~PagePlayer();
		
		Timer*					getTimerNextPage(){ return timer_nextpage; }
		
		// Debug out
		CueManager*				getCueManager(){ return cue_manager; }
		vector<RootObject*>*	getObjects(){ return objects; }

		// Config
		Config*				getConfig(){ return config; }
	
		// MainListener
		void				onStart();
		void				onFrame();
		void				onStop();
		
		// TimerListener:
		void				onTime(Timer*);
		
//		string*				getStatus(){ return &status; }
};

#endif
