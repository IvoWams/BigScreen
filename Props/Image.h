using namespace std;

#ifndef IMAGE_H
#define IMAGE_H

#include "Blinker.h"
#include <main/MainEngine.h>
#include <Global/RootObject.h>
#include <image/ImageData.h>
#include <string>
#include <fstream>
#include <image/ImageDataHandler.h>

class Image : public RootObject,
			  public Blinker {
	private:
		MainEngine*		engine;
		ofstream		fout;

	public:
		Image(const string&, double, double);
		~Image();
		
		// Link to reserved Texture id
		Texture*	texture;
		ImageData*	imagedata;

		// Left and Top are X:Y on rootobject
		// If width/height keyframing is required, need to create new keyframeing object type of class image		
		double		width;
		double		height;
		
		void		paint();	
};

#endif
