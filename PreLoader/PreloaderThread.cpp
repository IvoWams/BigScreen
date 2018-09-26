#include <preloader/PreloaderThread.h>

PreloaderThread::PreloaderThread(HTTP* http, Config* config){
	preloader = new Preloader(http, config);
	locked = false;
}

void	PreloaderThread::update(){

	// Handle and thread id
    DWORD dwThreadId = 0;
    HANDLE hThread = NULL;

	// Run thread
	hThread = CreateThread(
              NULL,                  		// Default thread security descriptor
              1024,                     	// Default stack size
              PreloaderThread::run,  		// Start routine
              this,                 		// Start routine parameter
              0,                     		// Run immediately
              &dwThreadId            		// Thread ID
              );	
	
}

DWORD 	PreloaderThread::run(PVOID p){
	
	PreloaderThread* pt = (PreloaderThread*)p;
	if(pt){
		if(!pt->locked){
			pt->locked = true;
			pt->preloader->update();
			pt->locked = false;
		}
	}
	
}
