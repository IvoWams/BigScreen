#include <player/PagePlayer.h>
#include <global/Globals.h>
#include <fstream>

PagePlayer::PagePlayer(Config*	_config){
	engine = Globals::getGlobals()->engine;
	
//	fout.open("PagePlayer.txt", ios::out);
//	fout << "Start\n";
//	fout.flush();
	
	// Add player here?? Or in our main??
	engine->addListener(this);

	objects = new vector<RootObject*>;
	config = _config;
	pagebuilder = new PageBuilder(config);
	
	counter = 0;
	
	
	// Debug config
//	current_page = new Page();
	
	// point builder to dir locs
//	pagebuilder->setRemoteImages(config->server_root + config->server_images);
//	pagebuilder->setRemoteMedia(config->server_root + config->server_media);
//	pagebuilder->setRemoteFonts(config->server_root + config->server_fonts);
//	pagebuilder->setLocalCache(config->client_cache);

}

PagePlayer::~PagePlayer(){
//	fout << "End\n";
//	fout.close();
	engine->getTimerHandler()->delListener(this);
	engine->getTimerHandler()->delTimer(timer_nextpage);
	delete timer_nextpage;
	delete pagebuilder;
}

void	PagePlayer::onStart(){
	// Construct cue
	cue_manager = new CueManager(config);

	timer_nextpage = new Timer(0.1);	// start the first page as soon as possible
	engine->getTimerHandler()->addTimer(timer_nextpage);
	engine->getTimerHandler()->addListener(this);
	
//	zero_time = engine->getDuration();
//	fout << " zerotime: " << zero_time << "\n";
//	fout.flush();
	// Zero time sets a base time start. Keyframes created a frame later
	// will then be animated accordingly (synchronized)

//	onTime(timer_nextpage);	
}

void	PagePlayer::onFrame(){

	// The more objects need to be updated (added), the bigger frameskip
	// So depending on the speed of the updater and the system it is running on
	// this should be adjusted accordingly. For now just update everything it has
	// (the biggest frameskips are from loading textures now anyway)
	
	update(pagebuilder->getCurrentPage()->pageobjectdatalist->size());
}

void	PagePlayer::onStop(){
	engine->getTimerHandler()->delListener(this);
	engine->getTimerHandler()->delTimer(timer_nextpage);
	delete timer_nextpage;
}

int		PagePlayer::update(int nr_of_objects){
	// Update takes x objects of the current playing page, add those to the engine
	// and adds the transitionary effects (that could be handled by external class)
	
	if(nr_of_objects<=0)return 0;

	// Keyframe handler expects a time of 'age'
	double t = engine->getDuration();
	double d = 1.5;	// Extra 'delay' so initial animations are outside of any possible frameskip
					// This value is increased after each object to give transitions a more 'wavey' effect
					
	int	updated = 0;
	
	for(int i=0;i<nr_of_objects;++i){
		d += 0.01;	// 'wave' effect for objects, in seconds
		
		// If no more in list, return updated up until now
		if(pagebuilder->getCurrentPage()->pageobjectdatalist->empty())return updated;
			
		// Retrieve first in line object info (queueing)
		PageObjectData* pageobjectdata = pagebuilder->getCurrentPage()->pageobjectdatalist->front();
		
		// Remove first from list
		pagebuilder->getCurrentPage()->pageobjectdatalist->erase(pagebuilder->getCurrentPage()->pageobjectdatalist->begin());
		
		// Add this object to engine
		engine->addObject(pageobjectdata->object);
		
		// Add to tracking list
		objects->push_back(pageobjectdata->object);
		
		KeyFrameHandler* keyframehandler = engine->getKeyFrameHandler();
		
		// Transition keyframing!
		
		// On all transitions > Alpha blending
		keyframehandler->setAlpha(pageobjectdata->object, t, 0);	
		keyframehandler->setAlpha(pageobjectdata->object, t + pageobjectdata->delay + d, 0);	
		keyframehandler->setAlpha(pageobjectdata->object, t + pageobjectdata->delay + d + pageobjectdata->fade_in, pageobjectdata->object->getColor()->A);
		keyframehandler->setAlpha(pageobjectdata->object, t + pageobjectdata->duration + d - pageobjectdata->fade_out, pageobjectdata->object->getColor()->A);
		keyframehandler->setAlpha(pageobjectdata->object, t + pageobjectdata->duration + d , 0);

		// *** SCALING ***

		// On transitions 1, ... > Scale in and out

		if(pageobjectdata->fade_in_type == 1){
			keyframehandler->setScale(pageobjectdata->object, t, 0, 0, 0);
			keyframehandler->setScale(pageobjectdata->object, t + pageobjectdata->delay + d, 0, 0, 0);	
			keyframehandler->setScale(pageobjectdata->object, t + pageobjectdata->delay + d + pageobjectdata->fade_in, 1, 1, 1);
		}
	
		if(pageobjectdata->fade_out_type == 1){
			keyframehandler->setScale(pageobjectdata->object, t + pageobjectdata->duration + d - pageobjectdata->fade_out, 1, 1, 1);
			keyframehandler->setScale(pageobjectdata->object, t + pageobjectdata->duration + d , 0, 0, 0);
		}
		
		// On transition 3, ... > Stretch in and out
		
		if(pageobjectdata->fade_in_type == 3){
			if(rand()>(0.5*RAND_MAX)){
				keyframehandler->setScale(pageobjectdata->object, t, 250, 0.01, 1);
				keyframehandler->setScale(pageobjectdata->object, t + pageobjectdata->delay + d + 0.2 * pageobjectdata->fade_in, 125, 0.02, 1);	
				keyframehandler->setScale(pageobjectdata->object, t + pageobjectdata->delay + d + 0.4 * pageobjectdata->fade_in, 75, 0.05, 1);	
				keyframehandler->setScale(pageobjectdata->object, t + pageobjectdata->delay + d + 0.6 * pageobjectdata->fade_in, 15, 0.1, 1);	
				keyframehandler->setScale(pageobjectdata->object, t + pageobjectdata->delay + d + 0.8 * pageobjectdata->fade_in, 5, 0.2, 1);	
				keyframehandler->setScale(pageobjectdata->object, t + pageobjectdata->delay + d + pageobjectdata->fade_in, 1, 1, 1);
			} else {
				keyframehandler->setScale(pageobjectdata->object, t, 0.01, 250, 1);
				keyframehandler->setScale(pageobjectdata->object, t + pageobjectdata->delay + d + 0.2 * pageobjectdata->fade_in, 0.02, 125, 1);	
				keyframehandler->setScale(pageobjectdata->object, t + pageobjectdata->delay + d + 0.4 * pageobjectdata->fade_in, 0.05, 75, 1);	
				keyframehandler->setScale(pageobjectdata->object, t + pageobjectdata->delay + d + 0.6 * pageobjectdata->fade_in, 0.1, 15, 1);	
				keyframehandler->setScale(pageobjectdata->object, t + pageobjectdata->delay + d + 0.8 * pageobjectdata->fade_in, 0.2, 5, 1);	
				keyframehandler->setScale(pageobjectdata->object, t + pageobjectdata->delay + d + pageobjectdata->fade_in, 1, 1, 1);
			}
		}
	
		if(pageobjectdata->fade_out_type == 3){
			keyframehandler->setScale(pageobjectdata->object, t + pageobjectdata->duration + d - pageobjectdata->fade_out, 1, 1, 1);
			keyframehandler->setScale(pageobjectdata->object, t + pageobjectdata->duration + d , 0, 0, 0);
		}

		// Transition 5 > Zoom effect
		
		if(pageobjectdata->fade_in_type == 5){
			keyframehandler->setScale(pageobjectdata->object, t, 0, 0, 1);
			keyframehandler->setScale(pageobjectdata->object, t + pageobjectdata->delay + d, 0.1, 0.1, 0);
			keyframehandler->setScale(pageobjectdata->object, t + pageobjectdata->delay + d + pageobjectdata->fade_in-0.1, 0.15, 0.15, 1);
			keyframehandler->setScale(pageobjectdata->object, t + pageobjectdata->delay + d + pageobjectdata->fade_in, 1, 1, 1);
		}
	
		if(pageobjectdata->fade_out_type == 5){
			keyframehandler->setScale(pageobjectdata->object, t + pageobjectdata->duration + d - pageobjectdata->fade_out, 1, 1, 1);
			keyframehandler->setScale(pageobjectdata->object, t + pageobjectdata->duration + d - pageobjectdata->fade_out+0.1, 0.15, 0.15, 1);
			keyframehandler->setScale(pageobjectdata->object, t + pageobjectdata->duration + d , 0.1, 0.1, 0);
		}
		
		
		// *** ROTATION ***
		
		// On transitions 2, ... > Rotate from right to left
		
		if(pageobjectdata->fade_in_type == 2){
			keyframehandler->setRotation(pageobjectdata->object, t + pageobjectdata->delay + d, 0, 0, 2000);
			keyframehandler->setRotation(pageobjectdata->object, t + pageobjectdata->delay + d + 0.8 * pageobjectdata->fade_in, 0, 0, 200);
			keyframehandler->setRotation(pageobjectdata->object, t + pageobjectdata->delay + d + pageobjectdata->fade_in, 0, 0, 0);
		}
	
		if(pageobjectdata->fade_out_type == 2){
			keyframehandler->setRotation(pageobjectdata->object, t + pageobjectdata->duration + d - pageobjectdata->fade_out, 0, 0, 0);
			keyframehandler->setRotation(pageobjectdata->object, t + pageobjectdata->duration + d - 0.8 * pageobjectdata->fade_out, 0, 0, -200);
			keyframehandler->setRotation(pageobjectdata->object, t + pageobjectdata->duration + d , 0, 0, -2000);
		}

		// On transition 3 > Stretch sideways

		if(pageobjectdata->fade_in_type == 3){
			keyframehandler->setRotation(pageobjectdata->object, t, 0, 0, 0);
			if(rand()>(RAND_MAX/2))
				keyframehandler->setRotation(pageobjectdata->object, t + pageobjectdata->delay + d, 0, 0, -20);	
			else
				keyframehandler->setRotation(pageobjectdata->object, t + pageobjectdata->delay + d, 0, 0, 20);	
			keyframehandler->setRotation(pageobjectdata->object, t + pageobjectdata->delay + d + pageobjectdata->fade_in, 0, 0, 0);
		}
	
		if(pageobjectdata->fade_out_type == 3){
			keyframehandler->setRotation(pageobjectdata->object, t + pageobjectdata->duration + d - pageobjectdata->fade_out, 0, 0, 0);
			if(rand()>(RAND_MAX/2))
				keyframehandler->setRotation(pageobjectdata->object, t + pageobjectdata->duration + d , 0, 0, -20);
			else
				keyframehandler->setRotation(pageobjectdata->object, t + pageobjectdata->duration + d , 0, 0, 20);
		}
		
		// On transition 4 > Rotate against the movement

		if(pageobjectdata->fade_in_type == 4){
			keyframehandler->setRotation(pageobjectdata->object, t + d + pageobjectdata->delay, 0, 0, -600);
			keyframehandler->setRotation(pageobjectdata->object, t + pageobjectdata->delay + d + pageobjectdata->fade_in, 0, 0, 0);
		}
		if(pageobjectdata->fade_out_type == 4){
			keyframehandler->setRotation(pageobjectdata->object, t + pageobjectdata->duration + d - pageobjectdata->fade_out, 0, 0, 0);
			keyframehandler->setRotation(pageobjectdata->object, t + pageobjectdata->duration + d , 0, 0, 600);
		}
		
		// *** POSITIONING ***

		// On transition 1 > Move in and out, to and from a random spot around the final position

		if(pageobjectdata->fade_in_type == 1){
			keyframehandler->setPosition(pageobjectdata->object, t + pageobjectdata->delay + d,
				pageobjectdata->object->getPosition()->x + sin(rand()) * 50.0,
				pageobjectdata->object->getPosition()->y + cos(rand()) * 50.0,
				0
			);
			keyframehandler->setPosition(pageobjectdata->object, t + pageobjectdata->fade_in + d + pageobjectdata->delay,
				pageobjectdata->object->getPosition()->x,
				pageobjectdata->object->getPosition()->y,
				0
			);
		}
		
		if(pageobjectdata->fade_out_type == 1){
			keyframehandler->setPosition(pageobjectdata->object, t + pageobjectdata->duration + d - pageobjectdata->fade_out,
				pageobjectdata->object->getPosition()->x,
				pageobjectdata->object->getPosition()->y,
				0
			);
			keyframehandler->setPosition(pageobjectdata->object, t + pageobjectdata->duration + d,
				pageobjectdata->object->getPosition()->x + cos(rand()) * 50.0,
				pageobjectdata->object->getPosition()->y + sin(rand()) * 50.0,
				0
			);
		}
		
		// On transition 2 > Move from left to position to right
		
		if(pageobjectdata->fade_in_type == 2){
			keyframehandler->setPosition(pageobjectdata->object, t + pageobjectdata->delay + d,
				pageobjectdata->object->getPosition()->x + 1000,
				pageobjectdata->object->getPosition()->y,
				0
			);
			keyframehandler->setPosition(pageobjectdata->object, t + pageobjectdata->fade_in + d + pageobjectdata->delay,
				pageobjectdata->object->getPosition()->x,
				pageobjectdata->object->getPosition()->y,
				0
			);
		}
		
		if(pageobjectdata->fade_out_type == 2){
			keyframehandler->setPosition(pageobjectdata->object, t + pageobjectdata->duration + d - pageobjectdata->fade_out,
				pageobjectdata->object->getPosition()->x,
				pageobjectdata->object->getPosition()->y,
				0
			);
			keyframehandler->setPosition(pageobjectdata->object, t + pageobjectdata->duration + d,
				pageobjectdata->object->getPosition()->x - 1000,
				pageobjectdata->object->getPosition()->y,
				0
			);
		}
		
		// Transition 4 > Swirl

		if(pageobjectdata->fade_in_type == 4){
			for(double i=0; i<pageobjectdata->fade_in-0.1; i+=0.1)
				keyframehandler->setPosition(pageobjectdata->object, t + i + d + pageobjectdata->delay,
					pageobjectdata->object->getPosition()->x + 200*(1.0-(i/pageobjectdata->fade_in))*sin(counter+5.0*i),
					pageobjectdata->object->getPosition()->y + 200*(1.0-(i/pageobjectdata->fade_in))*cos(counter+5.0*i),
					0
				);
			keyframehandler->setPosition(pageobjectdata->object, t + pageobjectdata->fade_in + d + pageobjectdata->delay,
				pageobjectdata->object->getPosition()->x,
				pageobjectdata->object->getPosition()->y,
				0
			);
		}
		
		if(pageobjectdata->fade_out_type == 4){

			keyframehandler->setPosition(pageobjectdata->object, t + pageobjectdata->duration + d - pageobjectdata->fade_out,
				pageobjectdata->object->getPosition()->x,
				pageobjectdata->object->getPosition()->y,
				0
			);

			for(double i=0; i<pageobjectdata->fade_out-0.1; i+=0.1)
				keyframehandler->setPosition(pageobjectdata->object, t + pageobjectdata->duration + d - i,
					pageobjectdata->object->getPosition()->x + 200*(1.0-(i/pageobjectdata->fade_out))*sin(counter+5.0*i),
					pageobjectdata->object->getPosition()->y + 200*(1.0-(i/pageobjectdata->fade_out))*cos(counter+5.0*i),
					0
				);
		}		

		  
		pageobjectdata->object->getColor()->A = 0;

		// Return amount of update objects
		updated++;
		counter++;
	}
	
	return updated;
}
	
void	PagePlayer::onTime(Timer* timer){
	if(timer == timer_nextpage){
		
		// Unattached objects are removed from memory
		
		// Screen objects are both in the engine object list, and in the keyframe object list, when it is being animated
		// If an object is in the engine object list but not in the keyframe object list, it's animation is done
		// and ready to be removed from memory.

		for(int i=0;i<objects->size();++i)
			if(!engine->getKeyFrameHandler()->hasKeyFrames((*objects)[i])){
				RootObject* object = (*objects)[i];
				objects->erase(objects->begin()+i);
				i--;
				engine->delObject(object);
				delete object;
			}

		// Get next page info
		PageData*	pagedata = cue_manager->getNextPageData();
		
		// Build page objects from the next page file
		pagebuilder->buildPageFromFile(config->client_cache + pagedata->filename);

		// Setup timer for next page
		timer_nextpage->setDuration(pagedata->duration);
	}
}
