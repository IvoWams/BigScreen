#include <props/DromeTile.h>
#include <gl/gl.h>
#include <global/Globals.h>

DromeTile::DromeTile() : CharTile(NULL, ' '){
    texture = Globals::getGlobals()->engine->getTextureHandler()->getTexture("data/dromelogo.bmp");
    width = texture->size;
    height = texture->size;

    mask = Globals::getGlobals()->engine->getTextureHandler()->getTexture("data/dromelogo_mask.bmp");
}

DromeTile::~DromeTile(){
	Globals::getGlobals()->engine->getTextureHandler()->freeTexture("data/dromelogo.bmp");
	Globals::getGlobals()->engine->getTextureHandler()->freeTexture("data/dromelogo_mask.bmp");
}

void	DromeTile::paint(){
    glLoadIdentity(); 

    glTranslatef(position->x, position->y, position->z);
    glScalef(scale->x, scale->y, scale->z);
		
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_COLOR_MATERIAL);
	
	glBlendFunc(GL_DST_COLOR,GL_ZERO);    
	
	glBindTexture(GL_TEXTURE_2D, mask->glref);

	glColor4f(color->R, color->G, color->B, color->A);    
 			
	//Specify vertices and texture coordinates
    glBegin(GL_QUADS);
	glTexCoord2f(0.01,0.99);
	glVertex2f(0, height);
	glTexCoord2f(0.01,0.01);
	glVertex2f(0, 0);
	glTexCoord2f(0.99,0.01);
	glVertex2f(width, 0);
	glTexCoord2f(0.99,0.99);
	glVertex2f(width, height);
	glEnd();
	

//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBindTexture(GL_TEXTURE_2D, texture->glref);

	glColor4f(color->R, color->G, color->B, color->A);    
 			
	//Specify vertices and texture coordinates
    glBegin(GL_QUADS);
	glTexCoord2f(0.01,0.99);
	glVertex2f(0, height);
	glTexCoord2f(0.01,0.01);
	glVertex2f(0, 0);
	glTexCoord2f(0.99,0.01);
	glVertex2f(width, 0);
	glTexCoord2f(0.99,0.99);
	glVertex2f(width, height);
	glEnd();

	// set default blend function
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}    
