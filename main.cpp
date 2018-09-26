#include <main.h>
#include <BigScreen.h>

Globals*	globals	= Globals::getGlobals();
MainEngine* engine	= globals->engine;

// Hook windows messages into the engine

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){
	// Pass message interfacing onto the engine
	return EngineWndProc(hWnd, message, wParam, lParam);
}

// HERE STARTS THE BIGSCREEN !

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow){
	// Create your demo here :
	BigScreen* bigscreen = new BigScreen();
		
	// Pass windows parameters onto engine
	EngineWinMain(hInstance, hPrevInstance, lpCmdLine, iCmdShow);

	// Return stuff on exit
	MSG		msg;
    return msg.wParam;
}
