using namespace std;

// PageBuilder builds Page Objects
// PageBuilder only uses setters
// PageBuilder creates an interface for the BSML interpreter to create Page object

#ifndef PAGEBUILDER_H
#define PAGEBUILDER_H

#include <string>
#include <fstream>
#include <vector>
#include <stack>
#include <main/MainEngine.h>
#include <global/Color.h>
#include <global/RootObject.h>
#include <interface/Label.h>
#include <font/Font.h>

#include <bsml/Parser.h>
#include <page/BSML2PageBuilder.h>
#include <page/Page.h>
#include <props/CharTile.h>
#include <props/Image.h>
#include <props/CountDown.h>
#include <config/Config.h>

class BSML2PageBuilder;

class PageBuilder {
	private:
		MainEngine*				engine;
		
		// Parser
		Parser*					parser;
		BSML2PageBuilder*		bsml2pagebuilder;

		// Counting amount of elements
		int						elements;

		// Resource locations
		Config*		config;

		// Debug out
		ofstream	fout;
		
		// Stack structures
		typedef struct {
			double	delay;
			double	in;
			double	duration;
			double	out;
			int		type;
		} FadeData;
		
		typedef struct {
			string	face;
			int		size;
			Color	color;
			string	style;
		} FontData;
		
		typedef struct {
			double	left;
			double	top;
			double	right;
			double	bottom;
		} MarginData;
		
		// Design stacks (make sure there is always one remaining in the stack)
		stack<FadeData*>		fadestack;
		stack<FontData*>		fontstack;
		stack<MarginData*>		marginstack;
		stack<int>				alignstack;
		
		// Writing sequence
		vector<CharTile*>*		sequence;
		// All tiles of previous line (flushed at newline())
		vector<CharTile*>*		line;
		
		// Building image data
		string	image_file;
		double	image_left;
		double	image_top;
		double	image_width;
		double	image_height;
		
		// Countdown data
		Color	countdown_blinkcolor;
		double	countdown_blinkstart;
		double	countdown_left;
		double	countdown_top;
		double	countdown_width;
		double	countdown_height;
		double	countdown_time;
		
		// Text blink
		double	blink_page;		// helps synchronizing blinking
		double	blink_on;
		double	blink_off;
		double	blink_walk;
		
		// Writing cursor
		double	cursor_x;
		double	cursor_y;
		double	line_height;
		
		// Writing sequence info
		double	seq_width;
		double	seq_height;
		
		Page*	current_page;
		
		// Store converted color
		Color	color;
		
		int				hexPair(const string&);
		void			getColor(const string& color);
		
	public:
		PageBuilder(Config*);
		~PageBuilder();

		// This is our canvas:
		Page*	getCurrentPage(){ return current_page; }
//		Page*					current_page;

		// Initiate parsing from a file:		
		void	buildPageFromFile(const string&);		// Main interface method for the cuemanager
		
		// ?
//		void	clean();								// Resets the current_page and removes any unused objects
		
		// deprecated
		// Page*	getPage(const string&);
		
		// Builder methods (from the interpreter)
		void	newPage();						// <bsml> Prepare
		void	closePage();					// </bsml> aaaaand done
		
		void	newAlignment(int);				// <...>
		void	closeAlignment(int);			// </...>
		
		void	newFade();						// <fade ... Prepare
		void	setFadeDelay(double);
		void	setFadeIn(double);
		void	setFadeOut(double);
		void	setFadeDuration(double);
		void	setFadeType(double);
		void	finishFade();					// > Create fade, push on the stack
		void	closeFade();					// </fade> Pop from the stack
		
		void	newImage();						// <img ... Prepare
		void	setImageSource(const string&);
		void	setImageLeft(double);
		void	setImageTop(double);
		void	setImageWidth(double);
		void	setImageHeight(double);
		void	finishImage();					// > Create image (no stack, ie. no </img>)
		
		void	newFont();						// <font ...
		void	setFontStyle(const string&);
		void	setFontFace(const string&);
		void	setFontSize(double);
		void	setFontColor(const string&);
		void	finishFont();					// > Create font, push on the stack
		void	closeFont();					// </font> Pop from the stack
		
		void	newMargin();					// <margin ...
		void	setMarginLeft(double);
		void	setMarginTop(double);
		void	setMarginRight(double);
		void	setMarginBottom(double);
		void	finishMargin();					// > Create margin, push on the stack
		void	closeMargin();					// </margin> Pop from the stack
		
		// Countdown Element
		void	newCountdown();
		void	setCountdownBlinkcolor(const string&);
		void	setCountdownBlinkstart(double);
		void	setCountdownTime(double);
		void	setCountdownLeft(double);
		void	setCountdownTop(double);
		void	setCountdownWidth(double);
		void	setCountdownHeight(double);
		void	finishCountdown();
		void	closeCountdown();
		
		// Blink
		void	newBlink();
		void	setBlinkOn(double);
		void	setBlinkOff(double);
		void	setBlinkWalk(double);
		void	finishBlink();
		void	closeBlink();
		
		// Writing methods
		void	write(const string&);			// text to add to the sequence
		void	newLine();						// Next line
		void	flushText();					// Flush out the current sequence
};

#endif
