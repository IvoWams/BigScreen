#include <Interface/ProgressBar.h>
#include <global/Globals.h>
#include <gl/gl.h>

ProgressBar::ProgressBar(double x, double y, double w, double h) : InterfaceObject(x, y, w, h){
	min = 0;
	max = 0;
	current = 0;
}

ProgressBar::~ProgressBar(){}

void	ProgressBar::paint(){
	glLoadIdentity();
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_COLOR_MATERIAL);
	
	Color background_top = Color(0.5,0.3,0.1,0.7);
	Color background_bottom = Color(0.7,0.5,0.1,0.9);
	
	Color frame_top = Color(0.7,0.5,0.3,0.7);
	Color frame_bottom = Color(0.8,0.7,0.5,0.9);
	
	Color bar_top   = Color(0.9,0.7,0.5,0.9);
	Color bar_bottom   = Color(1.0,0.9,0.7,1.0);
	
	float	bar_offset = 2;
	float	bar_edge = 2;
	
	glTranslatef(x, y, 0);
	
	glBegin(GL_QUADS);
	
	// Background
	glColor4f(background_top.R, background_top.G, background_top.B, background_top.A);
	glVertex2f(-bar_edge, -bar_edge);
	glVertex2f(width+bar_edge, -bar_edge);
	glColor4f(background_bottom.R, background_bottom.G, background_bottom.B, background_bottom.A);
	glVertex2f(width+bar_edge, height+bar_edge);
	glVertex2f(-bar_edge, height+bar_edge);
	
	// Frame
	glColor4f(frame_top.R, frame_top.G, frame_top.B, frame_top.A);
	glVertex2f(-bar_offset, -bar_offset);
	glVertex2f(width+bar_offset, -bar_offset);
	glVertex2f(width+bar_offset, -bar_edge);
	glVertex2f(-bar_offset, -bar_edge);

	glColor4f(frame_bottom.R, frame_bottom.G, frame_bottom.B, frame_bottom.A);
	glVertex2f(-bar_offset, height+bar_edge);
	glVertex2f(width+bar_offset, height+bar_edge);
	glVertex2f(width+bar_offset, height+bar_offset);
	glVertex2f(-bar_offset, height+bar_offset);

	glColor4f(frame_top.R, frame_top.G, frame_top.B, frame_top.A);
	glVertex2f(-bar_offset, -bar_edge);
	glVertex2f(-bar_edge, -bar_edge);
	glColor4f(frame_bottom.R, frame_bottom.G, frame_bottom.B, frame_bottom.A);
	glVertex2f(-bar_edge, height+bar_edge);
	glVertex2f(-bar_offset, height+bar_edge);

	glColor4f(frame_top.R, frame_top.G, frame_top.B, frame_top.A);
	glVertex2f(width+bar_edge, -bar_edge);
	glVertex2f(width+bar_offset, -bar_edge);
	glColor4f(frame_bottom.R, frame_bottom.G, frame_bottom.B, frame_bottom.A);
	glVertex2f(width+bar_offset, height+bar_edge);
	glVertex2f(width+bar_edge, height+bar_edge);

	float progress = 0;
	if(current>min)progress = 100*(current-min)/(max-min); else progress=0;
	if(progress>100)progress = 100;
	
	// Bar
	glColor4f(bar_top.R, bar_top.G, bar_top.B, bar_top.A);
	glVertex2f(0, 0);
	glVertex2f((0.01*progress*width), 0);
	glColor4f(bar_bottom.R, bar_bottom.G, bar_bottom.B, bar_bottom.A);
	glVertex2f((0.01*progress*width), height);
	glVertex2f(0, height);
	
	glEnd();

}
