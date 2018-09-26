#include "ImageData.h"

ImageData::ImageData(const string& _filename){
	filename = _filename;
	width = 0;
	height = 0;
	bpp = 0;
	error = false;
	dib = NULL;
}

ImageData::~ImageData(){
	// If there is imagedata, clear imagedata
	if(dib)SDL_FreeSurface(dib);
}
