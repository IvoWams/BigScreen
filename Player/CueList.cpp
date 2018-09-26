#include <player/CueList.h>
#include <global/Globals.h>

CueList::CueList(){
	engine = Globals::getGlobals()->engine;
	pagedatalist = new vector<PageData*>;

	filename = "";
	duration = 0;
	priority = false;
}

CueList::~CueList(){
	delete pagedatalist;
}

void	CueList::clearList(){
	for(int i=0;i<pagedatalist->size();++i)
		delete (*pagedatalist)[i];

	pagedatalist->clear();

//	if(current)delete current;	// 15-6, delete if already exists
//	current = new PageData();
	filename = "";
	duration = 0;
	priority = false;
}

void	CueList::flush(){
	if(filename!=""){
//	if(current->filename != ""){
		PageData* data = new PageData();

		data->filename = filename;
		data->duration = duration;
		data->priority = priority;
		
		pagedatalist->push_back(data);

		// Copy filename, in case it is removed
//		string filename = current->filename;
//		string* filename = new string(current->filename);
//		engine->getCacheSynchronizer()->addCacheFile(current->filename);
//		current = new PageData();

	}
}
