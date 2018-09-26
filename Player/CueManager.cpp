#include <player/CueManager.h>
#include <global/Globals.h>
#include <fstream>
#include <stdio.h>

CueManager::CueManager(Config* _config){
	engine = Globals::getGlobals()->engine;
	config = _config;
//	fout.open("CueManager.txt", ios::out);
//	fout << "Start\n";
	
	cuelist = new CueList();
	cursor_cue = 0;
	
//	t_preloader = new PreloaderThread(config);
	
	status = "";
	
	bsml2cuelist = new BSML2CueList(cuelist);
	parser = new Parser(bsml2cuelist);
		
	// Change next line from config file?
	cuefile = config->cue_file;
//	fout << "Cue file: " << cuefile << "\n";
	
	// Invoke an immediate cache update
//	t_preloader->preloader->update();
//	t_preloader->update();
	
	// Then start the timer for cache updates	
	timer_update = new Timer(config->cue_update);
	engine->getTimerHandler()->addTimer(timer_update);
	engine->getTimerHandler()->addListener(this);

	// Run update	
//	onTime(timer_update);
	timer_update->setCooldown(0.1);
	
//	fout << "Cue refresh: " << config->cue_update << "s\n";
//	fout.flush();
}

CueManager::~CueManager(){
//	fout << "End\n";
//	fout.close();
	engine->getTimerHandler()->delListener(this);
	engine->getTimerHandler()->delTimer(timer_update);
	delete timer_update;
}

PageData*	CueManager::getNextPageData(){
	// If cursor points beyond list, point to first element
	if(cursor_cue >= cuelist->getPageDataList()->size())cursor_cue = 0;

//	fout << "Getting next page\n";
//	fout << "Cue size: " << cuelist->getPageDataList()->size() << "\n";
//	fout.flush();
	
	// But if list is empty, add at least a notifying page
	if(cuelist->getPageDataList()->size()==0){

		cuelist->flush();	// start of tag
	
		// Should be hardcoded?
		cuelist->setFile("splash.asp");
		cuelist->setDuration(10);
		cuelist->setPriority(false);
		cuelist->flush();	// end of bsml

		// Move to bigscreen
//		t_preloader->update();
	}

	// Get Page Info
	PageData*	pagedata = (*cuelist->getPageDataList())[cursor_cue];
	cursor_cue++;
	
//	fout << "Cue cursor: " << cursor_cue << "\n";
//	fout << "Page data - filename = " << pagedata->filename << " duration = " << pagedata->duration << " priority = " << pagedata->priority << "\n";
//	fout.flush();
	
	// Set up cache
//	engine->getCacheSynchronizer()->addCacheFile(config->server_root + config->server_bsml, pagedata->filename, config->client_cache);
	// Set up page after this
	
	// If cursor points beyond list, point to first element
	if(cursor_cue >= cuelist->getPageDataList()->size())cursor_cue = 0;
	
	// Preload next in line page
	// preloader->loadPage((*cuelist->getPageDataList())[cursor_cue]->filename);
		
	// All done!
	return pagedata;
}

void	CueManager::onTime(Timer* timer){

	if(timer == timer_update){
//		fout << "Update\n";
		// reset preloader;
//		t_preloader->update();
		
		// Try to find file
		string file = config->client_cache + cuefile;

		// In filename, rename ? characters to !		
		int t = file.find("?", 0);
		while(t!=string::npos){
			file.replace(t, 1, "!");
			t = file.find("?", 0);
		}

//	ofstream fout("pages.txt", ios::out | ios::app);
//	if(fout)fout << "Opening " << file << endl;
//	fout.close();
//		fout << "Parsing file: " << file << "\n";

		ifstream fin(file.data(), ios::in);

		if(fin){
			string	result;
			getline(fin, result, '\0');
			parser->parseBSML(result);
			status = "Cue refreshed";
		} else {
			status = "Cue file not found";
		}

		fin.close();
//		fout.flush();

	}
}
