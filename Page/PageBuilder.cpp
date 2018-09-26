#include <page/PageBuilder.h>
#include <global/Globals.h>

PageBuilder::PageBuilder(Config* _config){
	engine = Globals::getGlobals()->engine;
	
	config = _config;
	
	bsml2pagebuilder = new BSML2PageBuilder(this);
	parser = new Parser(bsml2pagebuilder);

	// Default design, and push them onto the stack

	// Default fade settings
	FadeData* fadedata = new FadeData();
	fadedata->delay = 0.0;
	fadedata->in = 2.0;
	fadedata->duration = 20.0;
	fadedata->out = 2.0;
	fadedata->type = 1;
	
	fadestack.push(fadedata);
	
	// Default font settings
	FontData* fontdata = new FontData();
	fontdata->face = "fzbasic";
	fontdata->size = 35;
	fontdata->color.setColor(1,1,1);
	fontdata->style = "flat";
	
	fontstack.push(fontdata);
	
	MarginData* margindata = new MarginData();
	margindata->left = 0;
	margindata->top = 0;
	margindata->right = 0;
	margindata->bottom = 0;
	
	marginstack.push(margindata);
	
	alignstack.push(ALIGN_LEFT);

	sequence = new vector<CharTile*>;
	line = new vector<CharTile*>;
	
	color.R = 1;
	color.G = 1;
	color.B = 1;
	color.A = 1;
	
	blink_page = 0;
	blink_on = 0;
	blink_off = 0;
	blink_walk = 0;
	
	current_page = new Page();
}

PageBuilder::~PageBuilder(){
	// remove any leftover objects in sequence
	for(int i=0;i<sequence->size();++i)delete(*sequence)[i];
	
	delete current_page;
		
	delete sequence;
	delete line;
	delete parser;
	delete bsml2pagebuilder;
}

int	PageBuilder::hexPair(const string& hex){
	// I know, odd way to do this

	char	major = hex[0];
	char	minor = hex[1];
	int		major_int = 0;
	int		minor_int = 0;
	
	if(major>='0' && major<='9')major_int = major-'0';
	if(major>='a' && major<='f')major_int = major-'a' + 10;
	if(major>='A' && major<='F')major_int = major-'A' + 10;
	
	if(minor>='0' && minor<='9')minor_int = minor-'0';
	if(minor>='a' && minor<='f')minor_int = minor-'a' + 10;
	if(minor>='A' && minor<='F')minor_int = minor-'A' + 10;
	
	return major_int * 16 + minor_int;
}

void	PageBuilder::buildPageFromFile(const string& filename){
	string fn = filename;
	current_page->filename = filename;

	// Change ? characters in ! characters
	while(fn.find("?") != string::npos)fn.replace(fn.find("?"), 1, "!");

	// Open file		
	ifstream 	fin(fn.data(), ios::in);
	if(fin){
		string	 	result;
		getline(fin, result, '\0');
		parser->parseBSML(result);		
	} else {
		write("File [" + fn + "] not found.");
	}

	
	fin.close();
}

void	PageBuilder::newPage(){
	// Issue a new page, if the previous page has been closed normally, everything should be normal
	// However, we can't guarantee that this is the case, so as a precaution, initialize everything

	// Reset cursor
	cursor_x = 0;
	cursor_y = 0;
	line_height = 0;

	// Clear sequence
	while(!sequence->empty()){
		CharTile* ct = (CharTile*)sequence->back();
		delete ct;
		sequence->pop_back();
	}
	
	seq_width = 0;
	seq_height = 0;
	
	// Strip stacks to initial values
	while(fadestack.size()>1){
		delete (FadeData*)fadestack.top();
		fadestack.pop();
	}
	while(fontstack.size()>1){
		delete (FontData*)fontstack.top();
		fontstack.pop();
	}
	while(marginstack.size()>1){
		delete (MarginData*)marginstack.top();
		marginstack.pop();
	}
	while(alignstack.size()>1){
		alignstack.pop();
	}
	
	// Clean canvas
	current_page->pageobjectdatalist->clear();
	
	// Reset blinking timers
	blink_on = 0;
	blink_off = 0;
	blink_walk = 0;
	blink_page = Globals::getGlobals()->engine->getDuration();
}

void	PageBuilder::closePage(){
	// Signalled the end of the page. What happens is that everything should be initialized back
	// to starting positions. However this will also happen when a newPage is issued.

	// Flush out any remaining characters
	flushText();

	// Finish up any remaining lines
	newLine();
}

void	PageBuilder::newAlignment(int alignment){
	// New alignment style, flush previous text
	flushText();
	// Set new alignment
	alignstack.push(alignment);
	// reset horizontal cursor
	cursor_x = 0;
}

void	PageBuilder::closeAlignment(int alignment){
	if(alignstack.size()>1){
		// dont pop the stack if this is a different alignment
		if(alignment==alignstack.top()){
			flushText();
			cursor_x = 0;
			alignstack.pop();
		}
	}
}

void	PageBuilder::newFade(){
	// Issuing a new fade. Copy fadedata and push it on the stack.
	flushText();
	// Further settings will be applied to the top of the stack.
	FadeData* fd = new FadeData();
	// Copy stuff
	fd->delay = fadestack.top()->delay;
	fd->in = fadestack.top()->in;
	fd->duration = fadestack.top()->duration;
	fd->out = fadestack.top()->out;
	fd->type = fadestack.top()->type;
	// Push on stack
	fadestack.push(fd);
}

// Apply settings
void	PageBuilder::setFadeDelay(double delay){
	fadestack.top()->delay = delay;
}

void	PageBuilder::setFadeIn(double in){
	fadestack.top()->in = in;
}

void	PageBuilder::setFadeOut(double out){
	fadestack.top()->out = out;
}

void	PageBuilder::setFadeDuration(double duration){
	fadestack.top()->duration = duration;
}

void	PageBuilder::setFadeType(double type){
	fadestack.top()->type = type;
}

void	PageBuilder::finishFade(){
	// Nothing really happens here ...
}

void	PageBuilder::closeFade(){
	// Takes the top from the fadestack and destroy it (if the fadestack isnt empty after that)
	if(fadestack.size()>1){
		// Finish off the current sequence
		flushText();
		
		FadeData* fd = fadestack.top();
		fadestack.pop();
		delete fd;
	}
}

void	PageBuilder::newImage(){
	// Initialize image parameters
	image_file = "";
	image_left = 0;
	image_top = 0;
	image_width = 0;
	image_height = 0;
}

void	PageBuilder::setImageSource(const string& file){
	image_file = file;
}

void	PageBuilder::setImageLeft(double left){
	image_left = left;
}

void	PageBuilder::setImageTop(double top){
	image_top = top;
}

void	PageBuilder::setImageWidth(double width){
	image_width = width;
}

void	PageBuilder::setImageHeight(double height){
	image_height = height;
}

void	PageBuilder::finishImage(){
	// All parameters set, lets create the object itself (image data is loaded by the preloader)
	Image*	image = new Image(config->client_cache + image_file, image_left, image_top);

	if(image_width!=0)image->width = image_width;
	if(image_height!=0)image->height = image_height;
		
	image->setScale(1,1,1);
	image->setRotation(0,0,0);
	image->setColor(1,1,1,1);
	
	// Blinking
	if(blink_off!=0){
		image->setBlinkStart(blink_page);
		image->setBlinkStop(blink_page + 10000);		// 10000 -> blink as long as needed
		image->setBlinkDuration(blink_on);
		image->setBlinkOffset(blink_off);
		blink_page -= blink_walk;
	}
	
	
	// create pageobjectdata
	PageObjectData* pod = new PageObjectData();
	pod->delay = fadestack.top()->delay;
	pod->fade_in = fadestack.top()->in;
	pod->fade_in_type = fadestack.top()->type;
	pod->duration = fadestack.top()->duration;
	pod->fade_out = fadestack.top()->out;
	pod->fade_out_type = fadestack.top()->type;

	pod->object = image;
	current_page->pageobjectdatalist->push_back(pod);
}

void	PageBuilder::newFont(){
	flushText();
	FontData* fd = new FontData();
	// Copy
	fd->style = fontstack.top()->style;
	fd->face = fontstack.top()->face;
	fd->size = fontstack.top()->size;
	fd->color = fontstack.top()->color;
	
	fontstack.push(fd);
}

void	PageBuilder::setFontStyle(const string& style){
	fontstack.top()->style = style;
}

void	PageBuilder::setFontFace(const string& face){
	fontstack.top()->face = face;
}

void	PageBuilder::setFontSize(double size){
	fontstack.top()->size = size;
}

void	PageBuilder::getColor(const string& c){
	if(c == "blue")
		color.setColor(0, 0, 1);
	else if(c == "green")
		color.setColor(0, 1, 0);
	else if(c == "red")
		color.setColor(1, 0, 0);
	else if(c == "yellow")
		color.setColor(1, 1, 0);
	else if(c == "purple")
		color.setColor(1, 0, 1);
	else if(c == "cyan")
		color.setColor(0, 1, 1);
	else if(c == "white")
		color.setColor(1, 1, 1);
	else if(c == "gray")
		color.setColor(0.5, 0.5, 0.5);
	else if(c == "black")
		color.setColor(0, 0, 0);
	else if(c == "darkgreen")
		color.setColor(0, 0.5, 0);
	else if(c == "darkred")
		color.setColor(0.5, 0, 0);
	else if(c == "darkyellow")
		color.setColor(0.5, 0.5, 0);
	else if(c == "darkpurple")
		color.setColor(0.5, 0, 0.5);
	else if(c == "darkcyan")
		color.setColor(0, 0.5, 0.5);
	else if(c == "orange")
		color.setColor(1, 0.7, 0);
	else {
		// Try to get a #RRGGBB value
		string hex = c;
		// Remove # prefix
		if(hex.length() == 7)hex = hex.substr(1, 6);
		// If enough characters to work with
		if(hex.length() == 6)
			color.setColor(hexPair(hex.substr(0, 2))/255.0,
						   hexPair(hex.substr(2, 2))/255.0,
						   hexPair(hex.substr(4, 2))/255.0);
	}
}

void	PageBuilder::setFontColor(const string& c){
	getColor(c);
	fontstack.top()->color.setColor(color.R, color.G, color.B);
}

void	PageBuilder::finishFont(){
	// Yeah uh stuff
}

void	PageBuilder::closeFont(){
	if(fontstack.size()>1){
		flushText();
		FontData* fd = fontstack.top();
		fontstack.pop();
		delete fd;
	}
}

void	PageBuilder::newMargin(){
	flushText();
	MarginData* md = new MarginData();
	md->left = marginstack.top()->left;
	md->top = marginstack.top()->top;
	md->right = marginstack.top()->right;
	md->bottom = marginstack.top()->bottom;
	marginstack.push(md);
}

void	PageBuilder::setMarginLeft(double left){
	marginstack.top()->left = left;
}

void	PageBuilder::setMarginTop(double top){
	marginstack.top()->top = top;
}

void	PageBuilder::setMarginRight(double right){
	marginstack.top()->right = right;
}

void	PageBuilder::setMarginBottom(double bottom){
	marginstack.top()->bottom = bottom;
}

void	PageBuilder::finishMargin(){
	// Hmzz
}

void	PageBuilder::closeMargin(){
	if(marginstack.size()>1){
		flushText();
		MarginData* md = marginstack.top();
		marginstack.pop();
		delete md;
	}
}

void	PageBuilder::newCountdown(){
	// Dumdidum...
}

void	PageBuilder::setCountdownBlinkcolor(const string& c){
	getColor(c);
	countdown_blinkcolor.setColor(color.R, color.G, color.B);
}

void	PageBuilder::setCountdownBlinkstart(double _start){
	countdown_blinkstart = _start;
}

void	PageBuilder::setCountdownTime(double t){
	countdown_time = t;
}

void	PageBuilder::setCountdownLeft(double _left){
	countdown_left = _left;
}

void	PageBuilder::setCountdownTop(double _top){
	countdown_top = _top;
}

void	PageBuilder::setCountdownWidth(double _width){
//	fout << "setCountdownWidth(" << _width << ")\n";
//	fout.flush();
	
	countdown_width = _width;
}

void	PageBuilder::setCountdownHeight(double _height){
	countdown_height = _height;
}

void	PageBuilder::finishCountdown(){
	// Finalize creation of a countdown prop.
	
	// Get font	
	Font* font = engine->getFontHandler()->getFont(config->client_cache + fontstack.top()->face + ".ttf", fontstack.top()->size);
	
	CountDown*	c = new CountDown(font, countdown_time);
	c->setBlinkcolor(countdown_blinkcolor.R, countdown_blinkcolor.G, countdown_blinkcolor.B);
	c->setBlinkstart(countdown_blinkstart);
	c->setPosition(countdown_left, countdown_top, 0);
	c->setWidth(countdown_width);
	c->setHeight(countdown_height);
	c->order = 7;
	c->setScale(1,1,1);
	c->setRotation(0,0,0);
	c->setColor(fontstack.top()->color);
	
	PageObjectData* pod = new PageObjectData();

	pod->delay = fadestack.top()->delay;
	pod->fade_in = fadestack.top()->in;
	pod->fade_in_type = fadestack.top()->type;
	pod->duration = fadestack.top()->duration;
	pod->fade_out = fadestack.top()->out;
	pod->fade_out_type = fadestack.top()->type;
	pod->object = c;
	current_page->pageobjectdatalist->push_back(pod);
}

void	PageBuilder::closeCountdown(){
	// Nothing to do here ...
}

void	PageBuilder::newBlink(){
	// Nothing yet...
}

void	PageBuilder::setBlinkOn(double _on){
	blink_on = _on;
}

void	PageBuilder::setBlinkOff(double _off){
	blink_off = _off;
}

void	PageBuilder::setBlinkWalk(double _walk){
	blink_walk = _walk;
}

void	PageBuilder::finishBlink(){}

void	PageBuilder::closeBlink(){
	// disable blinking on </blink>
	// no blinking if the duration of the 'blink' itself is set to 0
	blink_off = 0;
}


void	PageBuilder::flushText(){
	// Going to flush the current sequence of chartiles
	// The chartiles are stored in a vector without any translation, so we
	// going to offset each character against the ALIGNMENT and the MARGIN
	
	// Drop out if sequence is empty
	if(sequence->size()==0)return;
	
	// The X offset for character creation to start
	double move_align = 0;
	
	// In case of left alignment, don't move (only align)
	if(alignstack.top()==ALIGN_LEFT) 		move_align = marginstack.top()->left;

	// In case of center alignment, screen X divided by 2, minus the width of the sequence, is the center
	else if(alignstack.top()==ALIGN_CENTER)	move_align = marginstack.top()->left + (0.5 * ((engine->getRenderer()->getWidth() - marginstack.top()->right) + (marginstack.top()->left))) - (0.5 * seq_width);

	// In case of right alignment, screen X minus the width of the sequence
	else if(alignstack.top()==ALIGN_RIGHT)	move_align = engine->getRenderer()->getWidth() - seq_width - marginstack.top()->right;

	// Apply translation to each character in the sequence, and add it to the page
	for(int i=0;i<sequence->size();++i){

		(*sequence)[i]->incPosition(move_align, marginstack.top()->top, 0);
		if(++elements<config->client_maxelements);
		
		// Create page objectdata, for the player
		PageObjectData* pod = new PageObjectData();

		// Apply fader info
		pod->delay = fadestack.top()->delay;
		pod->fade_in = fadestack.top()->in;
		pod->fade_in_type = fadestack.top()->type;
		pod->duration = fadestack.top()->duration;
		pod->fade_out = fadestack.top()->out;
		pod->fade_out_type = fadestack.top()->type;
		pod->object = (*sequence)[i];
		current_page->pageobjectdatalist->push_back(pod);
		
		// Add it to our current 'line'.
		line->push_back((*sequence)[i]);
	}
	
	// Clear sequence	
	sequence->clear();
	
	// Reset sequence info
	seq_width = 0;
	
	// Our cursor is now at the end of this sequence. Until a line break happens or anything
	// that affects our cursor, everything is stored in the 'line' vector.
}

void	PageBuilder::newLine(){
	// if we have stuffs in the sequence vector, flush that first to our line
	flushText();
	
	// A new line, important stuffs
	// Our horizontal cusor becomes zero
	cursor_x = 0;

	// Our vertical cursor moves to a spot that's clear for more text
	// this is stored in line_height
	
	// Adjust all the objects in the current line
	for(int i=0;i<line->size();++i)(*line)[i]->incPosition(0, line_height, 0);
	
	// 	All objects in the line are finished, reset the line vector
	line->clear();
	
	// Adjust the vertical cursor, add a bit of margin.
	cursor_y += line_height+20;

	// line_height gets adjusted while rendering characters
	line_height = 0;
}
		
void	PageBuilder::write(const string& text){

	// Build chartiles for each character. First get the font for this sequence
	Font* font = engine->getFontHandler()->getFont(config->client_cache + fontstack.top()->face + ".ttf", fontstack.top()->size);
	
	// For each character do
	for(int i=0;i<text.size();++i){
		// The availability of a certain character is for the discretion of the freeType library.
		// I assume at this point that all available characters are render-able.
		
		// Create a character tile
		CharTile* tile = new CharTile(font, text[i]);
		
		// In front of everything
		tile->order = 7;
		
		// Set initial position
		tile->setPosition(cursor_x, cursor_y, 0);
		
		tile->setScale(1,1,1);
		tile->setRotation(0,0,0);
		tile->setColor(fontstack.top()->color);
		tile->setStyle(fontstack.top()->style);
			
		// Blinking
		if(blink_off!=0){
			tile->setBlinkStart(blink_page);
			tile->setBlinkStop(blink_page + 10000);
			tile->setBlinkDuration(blink_on);
			tile->setBlinkOffset(blink_off);
			blink_page -= blink_walk;
		}

		// Add to our sequence
		sequence->push_back(tile);
		
		// Adapt the horizontal cursor
		cursor_x += font->getAdvance(text[i]);
		// Manage the sequence width
		seq_width += font->getAdvance(text[i]);

		// keep track of how high this line is, so it can be used to adjust the line during a newLine()
		if(tile->getHeight()>line_height)line_height = tile->getHeight();
	}		
}
