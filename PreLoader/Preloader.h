// PreLoader

// Its jobs is to find in cue and page files, media that has to be available
// for the different classes (the cue file, page files, graffiti file, fonts and images)
// If a file does not exist on the client, it uses http to initiate download
// However, preloader does not guarantee the availability of a file (for example, if it
// just does not exist on the server), and it is important that these errors are handled outside
// of the preloader
// A file that is being downloaded is saved into a .part file. When it is ready, any older versions
// of the file (without the .part bit) is removed and the .part file is renamed. During this period
// the file may be used by an other class. This should be tested on undefined functionality

using namespace std;

#ifndef PRELOADER_H
#define PRELOADER_H

#include <string>
#include <bsml/Parser.h>
#include <config/Config.h>
#include <main/MainEngine.h>
#include <preloader/BSML2Preloader.h>
#include <fstream>
#include <props/Image.h>
#include <global/Globals.h>
#include <fstream>
#include <sdl/sdl.h>
#include <sdl/sdl_image.h>
#include <stdio.h>
#include <http/HTTP.h>
#include <image/ImageDataHandler.h>

class BSML2Preloader;

class Preloader {
	private:	
		MainEngine*		engine;
				
		Config*			config;
		Parser*			parser;
		BSML2Preloader* pl;
		
		// Keeping track of our requests
		vector<HTTPRequest*>	requests;
		
		// Debug out
		ofstream		fout;
		
		// Interface to the http thread
		HTTP*			http;
		
		// Set up a file for BSML2Preloader parsing
		bool			parseFile(const string&);
		
		// Set up a file to run through IMG_Load
		void			initImage(const string&);
		
	public:
		Preloader(HTTP* http, Config* config);
		~Preloader();
	
		// Methods used by the interpreter
		void	loadPage(const string&);
		void	loadFont(const string&);
		void	loadImage(const string&);

		// Errors
		string	status;
		
		// Status report
		bool	updating;
		
		// Running loop
		void	update();
};

#endif
