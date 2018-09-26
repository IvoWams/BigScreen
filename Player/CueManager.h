using namespace std;

#ifndef CUE_H
#define CUE_H

#include <fstream>
#include <vector>
#include <string>
#include <main/MainEngine.h>
#include <timer/TimerHandler.h>
#include <timer/TimerListener.h>
#include <timer/Timer.h>
#include <bsml/Parser.h>
#include <player/CueList.h>
#include <player/BSML2CueList.h>
#include <page/Page.h>
#include <config/Config.h>
#include <preloader/PreloaderThread.h>

class CueManager : public	TimerListener {
	
	private:
		MainEngine*		engine;
		string			cuefile;
		
		Config*			config;
		
		// Cue list of filenames
		CueList*		cuelist;

		// Cue cursor, aka play pointer
		int				cursor_cue;

		// Preloader for images and whatnot, runs parallel to the main thread
//		PreloaderThread*	t_preloader;				

		// Getting cue DATA
		string			cue_file;
		Parser*			parser;			// The parsing of the file, reuse is meh
		BSML2CueList*	bsml2cuelist;	// The interpretation of the BSML
		
		// Update interval of the cue file
		Timer*			timer_update;
		
		// Status info
		string			status;
		
		ofstream		fout;

	public:
		CueManager(Config*);
		~CueManager();

		
		// Main interface with pageplayer
		int				getCursor(){ return cursor_cue; }
		CueList*		getCueList(){ return cuelist; }
		PageData*		getNextPageData();
		const string&	getStatus(){ return status; }
		
		Timer*			getTimer(){ return timer_update; }
		
		// Cue file update time
		void			onTime(Timer*);
};

#endif
