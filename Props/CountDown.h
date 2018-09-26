using namespace std;

#ifndef COUNTDOWN_H
#define COUNTDOWN_H

#include <global/RootObject.h>
#include <props/CharTile.h>
#include <Font/Font.h>
#include <string>
#include <fstream>

class CountDown : public RootObject {
	private:
		Font*		font;
		string		format;
		double		width;
		double		height;
		Color		blinkcolor;
		int			blinkstart;
		
		ofstream	fout;

		double		engine_zero;
		double		engine_finish;
		long		system_zero;
		long		system_finish;
		
		void		drawClock(const string&);
		
	public:
		CountDown(Font*, long);
		~CountDown();
		
		void	setBlinkcolor(double R, double G, double B){ blinkcolor.setColor(R, G, B); }
		void	setBlinkstart(int _time){ blinkstart = _time; }
		void	setWidth(double _width){ width = _width; }
		void	setHeight(double _height){ height = _height; }
		
		void	paint();
};

#endif
