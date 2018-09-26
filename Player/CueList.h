using namespace std;

#ifndef CUELIST_H
#define CUELIST_H

// Handles the incoming messages from bsml2cuelist
// and arranges a list of files usable for the cuemanager

#include <vector>
#include <main/MainEngine.h>
#include <PreLoader/PreLoader.h>

// Data about page needed for cuelist
typedef struct {
	string		filename;
	long		duration;
	bool		priority;
} PageData;

class CueList {
	private:
		MainEngine*				engine;
		vector<PageData*>*		pagedatalist;
		
		
		// All current page data
//		PageData*				current;
		
		// Unflushed page data
		string	filename;
		long	duration;
		bool	priority;
		
	public:
		CueList();
		~CueList();
		
		void	clearList();
		void	flush();
		
//		void	setFile(const string& _filename){ if(current)current->filename = _filename; }
		
//		void	setFile(const char* _filename){ if(current)current->filename = _filename; }
//		void	setDuration(long _duration){ if(current)current->duration = _duration; }
//		void	setPriority(bool _priority){ if(current)current->priority = _priority; }

		void	setFile(const string& _filename){ filename = _filename; }
		void	setDuration(long _duration){ duration = _duration; }
		void	setPriority(bool _priority){ priority = _priority; }
		
		vector<PageData*>*	getPageDataList(){ return pagedatalist; }
};

#endif
