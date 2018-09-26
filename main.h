#include <iostream>
#include <stdlib.h>
#include <windows.h>
#include <main/Engine.h>

#ifndef MAIN_H
#define MAIN_H

using namespace std;

LRESULT CALLBACK 	WndProc(HWND, UINT, WPARAM, LPARAM);
int WINAPI 			WinMain(HINSTANCE, HINSTANCE, LPSTR, int);

#endif
