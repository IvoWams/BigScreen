using namespace std;

#ifndef DROMETILE_H
#define DROMETILE_H

#include <global/RootObject.h>
#include <props/CharTile.h>
#include <texture/Texture.h>

class DromeTile : public CharTile {
	private:
		Texture*	texture;
		Texture*	mask;
		
		double		width;
		double		height;
		
	public:
		DromeTile();
		~DromeTile();
		
		void	setWidth(double _width){ width = _width; }
		void	setHeight(double _height){ height = _height; }
		
		double	getWidth(){ return width; }
		double	getHeight(){ return height; }
		
		void	paint();
};

#endif
