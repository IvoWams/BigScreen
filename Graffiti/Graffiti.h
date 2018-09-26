using namespace std;

#ifndef GRAFFITI_H
#define GRAFFITI_H

#include <main/MainEngine.h>
#include <config/Config.h>
#include <main/MainListener.h>
#include <timer/TimerListener.h>
#include <timer/Timer.h>
#include <bsml/Parser.h>
#include <bsml/ParserListener.h>
#include <graffiti/BSML2Graffiti.h>
#include <vector>
#include <props/CharTile.h>
#include <global/Vector.h>
#include <font/FontHandler.h>
#include <font/Font.h>
#include <graffiti/Stripe.h>
#include <fstream.h>

/*

	Graffiti engine
	This is basically a side scroller, of a textpair: an author and a text that the author has written.
	This is also very old code that has been made working with the current iteration of the main engine.
	
*/

class BSML2Graffiti;

typedef struct {
	string	author;
	string	text;
	double	height;
} Tag;

class Graffiti :	public MainListener,
					public TimerListener {

	private:
		MainEngine*		engine;
		Config*			config;

		Parser*			parser;
		BSML2Graffiti*	bsml2graffiti;
		
		vector<Tag*>*		tags;
		vector<CharTile*>*	tiles;

		Vector			scroll_start;
		double			border;
		double			speed;
		
		Timer*			timer_next;

		// Seperator stripe
		Stripe*			stripe;
		
		Font*			font_author;
		Font*			font_text;

		void			nextTag();
		void			update();
		
	public:
		Graffiti(Config*);
		~Graffiti();
		
		void	addTag(const string& author, const string& text, double height){
			Tag* tag = new Tag();
			tag->author = author;
			tag->text = text;
			tag->height = height;
			tags->push_back(tag);
		}
		
		void	onStart();
		void	onFrame();
		void	onStop();
		
		void	onTime(Timer*);
};

#endif
