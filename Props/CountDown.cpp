#include <Props/CountDown.h>
#include <global/Globals.h>
#include <gl/gl.h>
#include <time.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/timeb.h>

CountDown::CountDown(Font* _font, long _t) : RootObject(){
//	fout.open("CountDown.txt", ios::out | ios::app);
//	fout << "CountDown(" << _font << ", " << _t << ")\n";
//	fout.flush();
	
	font = _font;
	flat = true;
	order = 5;

    blinkcolor.setColor(1, 0.5, 0.5, 0.8);
	blinkstart = 60;
	
	// Table :
		
	// 0      |  engine_start   |   system_zero
	// ..     |  ..             |   ...
	// finish |  engine_finish  |   t
	
	time_t ltime;
	time(&ltime);
	
	system_zero   = ltime;
	system_finish = _t;

	engine_zero  = Globals::getGlobals()->engine->getDuration();
	engine_finish = engine_zero + (system_finish-system_zero);
	
//	fout << "System zero: " << system_zero << "\n";
//	fout << "System finish: " << system_finish << "\n";
//	fout << "Engine zero: " << engine_zero << "\n";
//	fout << "Engine finish: " << engine_finish << "\n\n";
//	fout.flush();
	
	// These are dynamically changed when painting
	width = 0;
	height = 0;
}

CountDown::~CountDown(){
//	fout.close();
}

void	CountDown::drawClock(const string& text){
	width = 0;
	height = 0;
	for(int i=0;i<text.size();i++){
		font->paint(text[i]);
		if(text[i]<'0'||text[i]>'9'){
			width += font->getAdvance(text[i]);
			if(font->getHeight(text[i])>height)height = font->getAdvance(text[i]);
			glTranslatef(font->getAdvance(text[i]), 0, 0);
		} else {
			width += font->getAdvance('0');
			if(font->getHeight('0')>height)height = font->getAdvance('0');
			glTranslatef(font->getAdvance('0'), 0, 0);
		}
	}
}

void	CountDown::paint(){
	// calculate variables
	
	
	double	time_to_go = engine_finish - Globals::getGlobals()->engine->getDuration();
	if(time_to_go<0)time_to_go = 0;
	
	// If time to go is like, 10 seconds, start flashing
	bool	flashing = (time_to_go<blinkstart);
	
	int	  	rest = (int)time_to_go;
	int		msec = ((int)(100 * time_to_go) % 100);
	int		hour = time_to_go / 3600;
	rest = (int)time_to_go % 3600;
	int		min = rest / 60;
	rest = rest % 60;
	int		sec = rest;
	
	char	hours[3];
	char	minutes[3];
	char	seconds[3];
	char	mseconds[3];
	
	if(hour>9)sprintf(hours, "%d", hour); else sprintf(hours, "0%d", hour);
	if(min>9)sprintf(minutes, "%d", min); else sprintf(minutes, "0%d", min);
	if(sec>9)sprintf(seconds, "%d", sec); else sprintf(seconds, "0%d", sec);
	if(msec>9)sprintf(mseconds, "%d", msec); else sprintf(mseconds, "0%d", msec);

	char	buffer[255];
	sprintf(buffer, "%s:%s:%s.%s", hours, minutes, seconds, mseconds);
	string r = buffer;
	
	// Lets go paint

    double border_size = 2.0;
    Color dark = Color(0, 0, 0, 0.1*color->A);
    Color frame = Color(0, 0, 0, 0.8*color->A);
    
    double x_offset = 0.5 * width;
    double y_offset = 0.5 * height;

	glEnable(GL_TEXTURE_2D);
	glDisable(GL_COLOR_MATERIAL);
	glEnable(GL_BLEND);

	glLoadIdentity();
	glTranslatef(position->x + border_size + x_offset, position->y + border_size + y_offset, 0);
	glRotatef(rotation->z, 0, 0, 1);
	glScalef(scale->x, scale->y, scale->z);
	glTranslatef(-x_offset, -y_offset, 0);

	glColor4f(frame.R, frame.G, frame.B, frame.A);
	drawClock(r);

	glLoadIdentity();
	glTranslatef(position->x + x_offset, position->y + y_offset, 0);
	glRotatef(rotation->z, 0, 0, 1);
	glScalef(scale->x, scale->y, scale->z);
	glTranslatef(-x_offset, -y_offset, 0);

	if(flashing && msec < 50){
		glColor4f(blinkcolor.R, blinkcolor.G, blinkcolor.B, color->A);
	} else {
		glColor4f(color->R, color->G, color->B, color->A);
	}
	drawClock(r);
	
	
}
