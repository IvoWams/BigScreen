using namespace std;

#ifndef IMAGEHANDLER_H
#define IMAGEHANDLER_H

#include <vector>
#include "ImageData.h"

/*
	Some notes how image data is handled.
	The object image itself is considered a 'prop', useable in the main engine
	
	Whenever the engine has to create an image, it checks the image data
	repository (ImageDataHandler) if this image was loaded before (and therefore
	has it's bitmap information stored in a texture). If it is then it's information
	is stored in an ImageData, which ImageDataHandler tracks. ImageData contains
	width, height, textureID, corresponding filename, bits per pixel, or any error
	or status flags.

	If the image is not stored and therefore has not been created before, it goes
	through the process of creating these kinds of information, loads the image data
	into a texture through the texturehandler, and so on.
*/

class ImageDataHandler {
	private:
		vector<ImageData*>*	imagedata;

		// Singleton
		static ImageDataHandler* imagedatahandler;
		
	protected:
		ImageDataHandler();
		ImageDataHandler(const ImageDataHandler&);
		ImageDataHandler& operator= (const ImageDataHandler&);
		
	public:	
		~ImageDataHandler();
		
		static ImageDataHandler*	getImageDataHandler();
		ImageData*					getImageData(const string&);
};

#endif
