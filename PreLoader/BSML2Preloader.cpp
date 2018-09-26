#include <preloader/BSML2PreLoader.h>
#include <global/Globals.h>
#include <fstream>

BSML2Preloader::BSML2Preloader(Preloader* _preloader){
	engine = Globals::getGlobals()->engine;
	preloader = _preloader;
}

BSML2Preloader::~BSML2Preloader(){}

void BSML2Preloader::onOpeningTag(const string& tag){}
void BSML2Preloader::onClosingTag(const string& tag){}

void BSML2Preloader::onAttribute(const string& tag, const string& attr, const string& value){

	// From PageBuilder
	if(tag=="font" && attr=="face"){
		// Tell preloader we need to load a font
		preloader->loadFont(value);
	}
	
	if(tag=="img" && attr=="src"){
		// Tell preloader we need to load a image
		preloader->loadImage(value);
	}
	
	if(tag=="page" && attr=="file"){
		// Tell preloader we need to load a page
		preloader->loadPage(value);
	}
}

void BSML2Preloader::onAttribute(const string& tag, const string& attr, double value){}
void BSML2Preloader::onAnyText(const string& text){}
