// Put preloader into thread
using namespace std;

#ifndef PRELOADERTHREAD_H
#define PRELOADERTHREAD_H

#include <windows.h>
#include <preloader/Preloader.h>
#include <http/http.h>

// Provides the PreLoader update routing (the main loop of the thing)
// into a thread, which makes the client application continue to run

class PreloaderThread {
	private:
		static DWORD WINAPI 	run(PVOID p);

	public:
		PreloaderThread(HTTP*, Config*);
		~PreloaderThread();
		
		Preloader*				preloader;
		bool					locked;

		void					update();		
};

#endif
