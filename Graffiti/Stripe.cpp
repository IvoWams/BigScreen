#include <graffiti/Stripe.h>
#include <gl/gl.h>

// Kinda out of date

Stripe::Stripe(){
	flat = true;
	order = 7;
}

Stripe::~Stripe(){}

void	Stripe::paint(){
	// Draw a nice line?
	glLoadIdentity();

	glDisable(GL_TEXTURE_2D);
	glEnable(GL_COLOR_MATERIAL);
	
	glBegin(GL_QUADS);
	glColor4f(0,0,0, 0.3*color->A);
	glVertex2f(0, position->y);
	glColor4f(0,0,0, 0.3*color->A);
	glVertex2f(1024, position->y);
	glColor4f(0,0,0, 0.3*color->A);
	glVertex2f(1024, position->y+90);
	glColor4f(0,0,0, 0.3*color->A);
	glVertex2f(0, position->y+90);
	glEnd();	
}
