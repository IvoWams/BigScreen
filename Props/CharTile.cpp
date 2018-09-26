#include <props/CharTile.h>
#include <gl/gl.h>
#include <global/Globals.h>

CharTile::CharTile(Font* _font, char _character) : RootObject(){
	flat = true;
	order = 5;
    font = _font;
    character = _character;
    style = "";
    
    xoffset = 0.5*font->getWidth(character);
    yoffset = 0.5*font->getHeight(character);
}

CharTile::~CharTile(){}

void    CharTile::paint(){
	// Blink
	if(isBlinked())return;

	// Do not paint when:
	// - engine time is below blink_start
	// - engine time is above blink_stop
	// - (engine time - blink_start) % (blink_duration + blink_offset) > blink_duration

	// Needed or not?
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_COLOR_MATERIAL);
	glEnable(GL_BLEND);
    
    double border_size = 1.0;
    Color dark = Color(0, 0, 0, 0.1*color->A);
    Color frame = Color(0, 0, 0, 0.8*color->A);
    
    if(style == "emboss"){

	    glLoadIdentity();
	    glTranslatef(position->x, position->y, 0);
		glTranslatef(xoffset, -yoffset, 0);
	    glRotatef(rotation->z, 0, 0, 1);
		glScalef(scale->x, scale->y, scale->z);
		glTranslatef(-xoffset, yoffset, 0);			 
			 
		glColor4f(frame.R, frame.G, frame.B, frame.A);
		glTranslatef(-border_size, -border_size, 0);
		font->paint(character);
		glTranslatef(2*border_size, 0, 0);
		font->paint(character);
		glTranslatef(-2*border_size, 2*border_size, 0);
		font->paint(character);
		glTranslatef(2*border_size, 0, 0);
		font->paint(character);
		glTranslatef(-border_size, border_size, 0);
		font->paint(character);
		glTranslatef(2*border_size, 0, 0);
		font->paint(character);

		glColor4f(color->R, color->G, color->B, color->A);
		glTranslatef(-2*border_size, -2*border_size, 0);
		font->paint(character);

		glColor4f(dark.R, dark.G, dark.B, dark.A);
		glTranslatef(2*border_size, 2*border_size, 0);
		font->paint(character);

	} else if(style == "Bleut"){

	} else {

	    glLoadIdentity();
	    glTranslatef(position->x + 2 * border_size, position->y + 2 * border_size, 0);
		glTranslatef(xoffset, -yoffset, 0);
	    glRotatef(rotation->z, 0, 0, 1);
		glScalef(scale->x, scale->y, scale->z);
		glTranslatef(-xoffset, yoffset, 0);

		glColor4f(frame.R, frame.G, frame.B, frame.A);
		font->paint(character);

	    glLoadIdentity();
	    glTranslatef(position->x, position->y, 0);
		glTranslatef(xoffset, -yoffset, 0);
	    glRotatef(rotation->z, 0, 0, 1);
		glScalef(scale->x, scale->y, scale->z);
		glTranslatef(-xoffset, yoffset, 0);

		glColor4f(color->R, color->G, color->B, color->A);
		font->paint(character);
		
		
	}
}        

