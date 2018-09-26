using namespace std;

#ifndef BIGSCREEN_H
#define BIGSCREEN_H

#include <stdio.h> 
#include <main.h>
#include <main/MainEngine.h>
#include <main/MainListener.h>
#include <timer/TimerListener.h>
#include <timer/Timer.h>
#include <interface/Label.h>
#include <font/Font.h>
#include <props/Image.h>
#include <bsml/Parser.h>
#include <config/BSML2Config.h>
#include <config/Config.h>
#include <graffiti/Graffiti.h>
#include <player/PagePlayer.h>
#include <Interface/ProgressBar.h>
#include <http/HTTPThread.h>

// Stupid fars en zo
class MainEngine;

class BigScreen : public    MainListener,
				  public	TimerListener /*,
				  public	InputListener*/ {

	private:
		MainEngine*			engine;
		
		// Configuration data
		Config*				config;
		
		// Load at least one font for basic info output
		Font*				system_font;
		
		// Main aspects of the bigscreen engine, The Page Player (the chartile thingies), and the Scrolling Sign Marquee (grafitti)
		
		// HTTP Handling
		HTTPThread*			httpthread;

		// Preloading thread for parrallel downloading of page and images
		PreloaderThread*	preloaderthread;

		// Player of slideshow and animations
		PagePlayer*			pageplayer;
		
		bool				graffiti_enabled;
		Graffiti*			graffiti;

		// General purpose label
		Label*				label_status;
		Label*				label_size;
		
		// Status info update
		Timer*				timer_second;
		
		// Cue/Cache refresh
		Timer*				timer_cacherefresh;
		
		// Progressbar
		ProgressBar*		progressbar;
		
		// Drome logo
		Image*				img_dromelogo;
		Image*				img_connection;
		Image*				img_update;
		
		char				buffer_time[35];
		Timer*				timer_uiupdate;
		
		Image*				loadImage(const string&);
			
	public:
		BigScreen();
		~BigScreen();
		
		// MainListener:
		void	onStart();
		void	onStop();
		void	onFrame();
		
		// TimerListener
		void	onTime(Timer*);
		
		// InputListener
		// void	onKeyDown(int);
};

#endif
