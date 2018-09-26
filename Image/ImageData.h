using namespace std;

#ifndef IMAGEDATA_H
#define IMAGEDATA_H

#include <string>
#include <sdl/sdl.h>
#include <sdl/sdl_image.h>

// Could be a struct
class ImageData {
	private:
	public:
		ImageData(const string&);
		~ImageData();
		
		string			filename;
		int				width;
		int				height;
		int				bpp;
		SDL_Surface*	dib;
		bool			error;		// set on error
		bool			available;	// set on when image data is successfully loaded
};

#endif
