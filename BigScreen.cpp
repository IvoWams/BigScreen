#include <BigScreen.h>
#include <global/Globals.h>
#include <fstream.h>

#include <gl/gl.h>
#include <sdl/sdl.h>
#include <sdl/sdl_image.h>

/*

Bigscreen Player
This version is: 1.0
Date: 30-6-2006

Some info:
This player was created for use at Drome langaming events, to present information on bigscreens
in a stylish and animated fashion. Development for this particular program started in april 2004.
It has currently undergone 15 beta versions. This is version 1.0 and probably the last one, since
coming the upcoming Drome event (13) is the last one.

It tries to connect to a server as configured in the config.bsml file. BSML stands for BigsSreen
Markup Language (yeah I know), which is pretty much HTML or SGL or whatever markup language. The
files downloaded from the server are all either BSML files, or media files (images and fonts). All
in all it pretty much resembles an internet browser.

The player can't do anything else than play, administration of the contents has to be done by third
party. For example, an intranet for remote editing of the pages. The pages are downloaded through HTTP
so pages can be generated dynamically by a webserver and some smart programming in ASP or PHP or whatever
floats your boat.

The following pages are required on the server :
	- a Cue file
	- All the page files as mentioned in the cue file
	
The cue file is basicly the playlist file, and it looks like this:
	
	<bsml>
		<page file="..." duration=x.y>
		<page ...>
		<page ...>
	</bsml>
	
The player parses this and queue's pages in order as mentioned in the cue file.

The more interesting stuff happens in page files. There are several design tags possible, the design
can then be canceled by closing tags. Just like normal HTML. However, there are a few things that's not
possible in HTML, and there are a LOT of things BSML can't do.

Since the possibilities of BSML are pretty limited I'll summarize them here :
	
	<bsml>

		<font face="..." style="..." color="..." size=x>
			This text has special font properties!
		</font>
		
		<img src="..." left=x.x top=y.y width=w.w height=h.h>
			Images don't need a closing tag.
			
		<margin left=x.x top=y.y right=r.r>
			The following text is made sure to fit between the margins.
			A bottom margin is useless.
		</margin>
		
		<fade delay=


	...
*/

BigScreen::BigScreen(){
	engine = Globals::getGlobals()->engine;
	engine->addListener(this);	
}

BigScreen::~BigScreen(){}

Image*	BigScreen::loadImage(const string& filename){
	ImageData* imagedata = ImageDataHandler::getImageDataHandler()->getImageData(filename);
	imagedata->dib = IMG_Load(filename.data());
	if(imagedata->dib){
		// If image data was successfully loaded, copy variables
		imagedata->width = imagedata->dib->w;
		imagedata->height = imagedata->dib->h;
		imagedata->bpp = imagedata->dib->format->BytesPerPixel;
		imagedata->available = true;	// prevent it from being loaded a second time
	} else {
		imagedata->error = true;
		// Cry
	}

	// Uh yeah return Image prop, dont forget to clean after use
	return new Image(filename, 0,0); // engine->getRenderer()->getWidth()-105, /* engine->getRenderer()->getHeight() */ 5);
	
}

void	BigScreen::onStart(){
	engine->getMouseHandler()->showMouse(false);
//	engine->getMouseHandler()->resetBoundary();
	
	// Interpreter and parser for config
	BSML2Config* bsml2config	= new BSML2Config();
	Parser* parser				= new Parser(bsml2config);
	
	// I/o to config file
	ifstream 	fin("config.bsml", ios::in);
	if(fin){
		string	 	result;

		// Read all text until \0 (eof?)
		getline(fin, result, '\0');

		// Parse config
		parser->parseBSML(result);

		// Set config obj
		config = bsml2config->getConfig();
		
		// Create HTTP engine (which runs in its own thread)
		httpthread = new HTTPThread(config->server_ip, config->server_port);
		
		// Create preloader engine (which also runs in its own thread)
		preloaderthread = new PreloaderThread(httpthread->http, config);
		
		// The main text player engine
		pageplayer = new PagePlayer(config);
		
		// The graffiti engine (the scroller at the bottom) IF it is enabled
		if(config->graffiti_enabled==1)graffiti = new Graffiti(config);
		
	} else {
		
		// Can't run the bigscreen engine if there's no config file.
		engine->quit();
		return;
	}
	
	fin.close();
	
	// Adjust our renderer window.
	engine->getRenderer()->changeResolution(config->video_left, config->video_top, config->video_width, config->video_height, config->video_bpp, config->video_fullscreen);

	// Grab a font for generic use.
    system_font = engine->getFontHandler()->getFont("data/FEASFBRG.TTF", 25);

	// Default background color
	glClearColor(0.0, 0.0, 0.0, 1);
//	glClearColor(0.3, 0.5, 0.7, 1);

	// Bigscreen UI elements

	label_status = new Label(0,
							 engine->getRenderer()->getHeight()-6,
							 385, 25,
							 system_font, ALIGN_LEFT);
							 
	label_status->setColor(1,1,1,1);
	label_status->setTextSize(1.0);
//	label_status->setText("");
	
	label_size = new Label(engine->getRenderer()->getWidth()-400,
							 engine->getRenderer()->getHeight()-6,
							 385, 23,
							 system_font, ALIGN_RIGHT);
							 
	label_size->setColor(0.7,0.9,1.0,0.7);
	label_size->setTextSize(1.0);
	label_status->setText("");

	progressbar = new ProgressBar(engine->getRenderer()->getWidth()-95,
								  /* engine->getRenderer()->getHeight() */ 89,
								  80, 5);

	progressbar->setMin(0);
	progressbar->setMax(100);
	progressbar->setProgress(0);
	
	engine->getInterfaceHandler()->addObject(progressbar);
//	engine->getInterfaceHandler()->addObject(label_status);
	engine->getInterfaceHandler()->addObject(label_size);

	
	// We're going to have to listen to timers
	engine->getTimerHandler()->addListener(this);

	// Because we want to update the UI once in a while
	timer_uiupdate = new Timer(0.1);
	engine->getTimerHandler()->addTimer(timer_uiupdate);
	
	// Add the DROME TV logo
	img_dromelogo = loadImage("data/drometelevision.png");
	img_dromelogo->setPosition(engine->getRenderer()->getWidth()-105, 5, 0);
	img_dromelogo->width = 100;
	img_dromelogo->height = 80;
	img_dromelogo->setScale(1,1,1);
	img_dromelogo->setRotation(0,0,0);
	img_dromelogo->setColor(1,1,1,1);
	img_dromelogo->order = 9;
	
	// Load the 'no connection' icon
	img_connection = loadImage("data/connection.png");
	img_connection->setPosition(engine->getRenderer()->getWidth()-50, 100, 0);
	img_connection->width = 40;
	img_connection->height = 40;
	img_connection->setScale(1,1,1);
	img_connection->setRotation(0,0,0);
	img_connection->setColor(1,1,1,1);
	img_connection->order = 9;
	img_connection->setBlinkStart(0);
	img_connection->setBlinkStop(10000000);
	img_connection->setBlinkDuration(0.2);
	img_connection->setBlinkOffset(0.8);
	img_connection->visible = false;

	// Load the 'updating' icon
	img_update = loadImage("data/update.png");
	img_update->setPosition(engine->getRenderer()->getWidth()-50, 100, 0);
	img_update->width = 40;
	img_update->height = 40;
	img_update->setScale(1,1,1);
	img_update->setRotation(0,0,0);
	img_update->setColor(1,1,1,1);
	img_update->order = 9;
	img_update->setBlinkStart(0);
	img_update->setBlinkStop(10000000);
	img_update->setBlinkDuration(0.2);
	img_update->setBlinkOffset(0.8);
	img_update->visible = true;
	
	engine->addObject(img_dromelogo);
	engine->addObject(img_connection);
	engine->addObject(img_update);
	
	engine->nextFrame();
	
	// Start an early update, so media is loaded before we begin
	preloaderthread->update();

	// Since the update is threaded, sleep a bit to make sure the correct flags are set
	Sleep(100);

	label_status->setText("Trying to connect");		
	
	// While the thread is updating	and there hasn't been an error
	while(preloaderthread->preloader->updating && httpthread->http->last_error == ""){
		
		// Catch the current download
		HTTPRequest*	current_download = httpthread->http->current_download;

		// If there actually IS a download
		if(current_download){
			
			// Change status label to reflect current download
//			label_status->setText("%s", current_download->remote_file.data());
//				if(current_download->downloaded<current_download->size){
//					label_size->setText("%d / %d", current_download->downloaded, current_download->size);
//				}else{
//					label_size->setText("%d", current_download->downloaded);
//				}

			// Do stuff with the progressbar to look real fancy
			progressbar->setMax(current_download->size);
			progressbar->setProgress(current_download->downloaded);
		}
		
		// Make sure there's time to actually get to render the scene
		engine->nextFrame();
		
		// Sleep a bit to give the preloader room to work
		Sleep(100);
	}

	// We're done pre-updating, no 'updating' icon please
	img_connection->visible = false;
	
	label_status->setText("");
	label_size->setText("");

	// Next update trigger handled by a timer
	timer_cacherefresh = new Timer(config->cue_update);
	engine->getTimerHandler()->addTimer(timer_cacherefresh);
}

void	BigScreen::onStop(){
	// From here on entire app comes down, so let windows clean it all up
}

void	BigScreen::onFrame(){
		// The UI update is triggered by a timer, however the progressbar wants to be smooth, so it goes here
		progressbar->setProgress(pageplayer->getTimerNextPage()->getDuration()-pageplayer->getTimerNextPage()->getCooldown());
}

void	BigScreen::onTime(Timer* timer){

	// Trigger cache update
	if(timer == timer_cacherefresh)preloaderthread->update();

	// Trigger UI update
	if(timer == timer_uiupdate){

		// On connection error, show connection icon
		img_connection->visible = (httpthread->http->last_error!="");
		
		// When in update mode, show update icon
		img_update->visible = (preloaderthread->preloader->updating && !img_connection->visible);

		progressbar->setMax(pageplayer->getTimerNextPage()->getDuration());

//		label_size->setText("%d/%d", pageplayer->getCueManager()->getCursor(), pageplayer->getCueManager()->getCueList()->getPageDataList()->size());

		// Draw a clock
		time_t t;
		time(&t);
		strftime(buffer_time, 35, "%H:%M", localtime(&t));
		label_size->setText("%s", buffer_time);
		
		if(httpthread->http->last_error!=""){
			label_status->setText("%s", httpthread->http->last_error.data());
		} else {
			if(preloaderthread->preloader->updating){
				label_status->setText("Updating...");
			}else{
				label_status->setText("");	// (int)timer_cacherefresh->getCooldown()
			}
		}
	}
}

/*
void	BigScreen::onKeyDown(int keycode){
}
*/
