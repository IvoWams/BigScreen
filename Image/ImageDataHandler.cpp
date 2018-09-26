#include "ImageDataHandler.h"

// Singleton:
ImageDataHandler* ImageDataHandler::imagedatahandler = 0;
ImageDataHandler* ImageDataHandler::getImageDataHandler(){
	if(imagedatahandler==0)imagedatahandler = new ImageDataHandler;
	return imagedatahandler;
}

ImageDataHandler::ImageDataHandler(){
	imagedata = new vector<ImageData*>;
}

ImageDataHandler::~ImageDataHandler(){
	// Nobody is actually deleting this singleton except on termination of the app itself
	delete imagedata;
}

ImageData*	ImageDataHandler::getImageData(const string& filename){
    for(int i=0;i<imagedata->size();++i){
    	if((*imagedata)[i]->filename == filename){
	    	return (*imagedata)[i];
		}
	}
	
	ImageData* id = new ImageData(filename);
	id->dib = NULL;
	id->available = false;
	id->error = false;
		
	imagedata->push_back(id);
    return id;	
}
