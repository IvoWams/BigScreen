using namespace std;

#ifndef CHARTILE_H
#define CHARTILE_H

#include "Blinker.h"
#include <global/RootObject.h>
#include <global/Color.h>
#include <font/Font.h>
#include <fstream.h>

class CharTile : public RootObject,
				 public Blinker {
    private:
	protected:
        Font*   font;
        char    character;
		string	style;
		
		double	xoffset;
		double	yoffset;
		
                
    public:
        CharTile(Font*, char);
        ~CharTile();
        
        // todo
        double	getWidth(){ return font->getWidth(character); }
        double	getHeight(){ return font->getHeight(character); }
        
        void    setFont(Font* _font){ font = _font; }
        void    setChar(char _character){ character = _character; }
		void	setStyle(const string& _style){ style = _style; }
		

        void    paint();
};

#endif
