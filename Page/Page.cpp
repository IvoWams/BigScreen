#include <page/Page.h>
#include <global/Globals.h>

Page::Page(){
	engine = Globals::getGlobals()->engine;
	pageobjectdatalist = new vector<PageObjectData*>;
	
	filename = "Unknown";
}

Page::~Page(){
	delete pageobjectdatalist;
}




