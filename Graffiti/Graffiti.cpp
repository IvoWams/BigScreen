#include <graffiti/Graffiti.h>
#include <main/MainEngine.h>
#include <props/DromeTile.h>

Graffiti::Graffiti(Config* _config){
	config = _config;
	engine = Globals::getGlobals()->engine;

	tags = new vector<Tag*>;
	tiles = new vector<CharTile*>;
	timer_next = new Timer(0.1);

	bsml2graffiti = new BSML2Graffiti(this);
	parser = new Parser(bsml2graffiti);
	
	stripe = new Stripe();
	stripe->order = 8;
	stripe->setPosition(0, engine->getRenderer()->getHeight(), 0);

	engine->addListener(this);
}

Graffiti::~Graffiti(){
	delete(timer_next);
	delete(parser);
	delete(bsml2graffiti);
}

void	Graffiti::onStart(){
	// todo: make this configurable
	font_author = engine->getFontHandler()->getFont("data/grand stylus.ttf", 50);
	font_text   = engine->getFontHandler()->getFont("data/fzbasic.ttf", 22);
	
	engine->getTimerHandler()->addTimer(timer_next);
	engine->getTimerHandler()->addListener(this);
	
	scroll_start.x = engine->getRenderer()->getWidth();
	scroll_start.y = engine->getRenderer()->getHeight()-100;//670;
	
	border = -100;
	speed  = config->graffiti_speed;
	
	engine->addObject(stripe);
	
	update();
}

void	Graffiti::onStop(){
	engine->delObject(stripe);
	engine->getTimerHandler()->delListener(this);
	engine->getTimerHandler()->delTimer(timer_next);
}

void	Graffiti::onFrame(){
	// Calculate speed depending on framespeed
	double	offset = engine->getFrameTime() * speed * 1000;
	
	// For each tile known
	for(int i=0;i<tiles->size();++i){
		CharTile*	tile = (*tiles)[i];
		tile->incPosition(-offset, 0, 0);		// Scroll to the left
		if(tile->getPosition()->x < border){	// If this moves over certain border
			engine->delObject(tile);			// Remove from engine
			tiles->erase(tiles->begin()+i);		// Remove from deposit
			i--;								// Stack back iterator
			
			delete(tile);						// Remove from memory
		}
	}
}

void	Graffiti::nextTag(){

	// Taglist empty? Update
	if(tags->empty()){
		update();
		// If still empty, quit
		if(tags->empty()){
			engine->getKeyFrameHandler()->setAlpha(stripe, engine->getDuration()+5, 0);
			return;
		}
	}
	
	engine->getKeyFrameHandler()->setAlpha(stripe, engine->getDuration()+5, 1);
	
	Tag*	tag = tags->front();
	
	// For each char in string, create chartile
	double	zero_time = engine->getDuration();	// the time now

	int		author_pos = 0;							// cursor slash total tile size
	int		text_pos = 0;
	int		author_height = 0;
	int		text_height = 0;
	
	for(int i=0;i<tag->author.size();++i){
		CharTile* tile = new CharTile(font_author, tag->author[i]);
		tile->order = 9;
		
		tile->setPosition(scroll_start.x + author_pos, tag->height, 0);
		tile->setScale(1,1,0);
		tile->setRotation(0,0,0);
		tile->setColor(0.3,0.7,0.9,1);
		tile->setStyle("emboss");
		engine->addObject(tile);


		tiles->push_back(tile);
		author_pos += font_author->getAdvance(tag->author[i]); // tile->getWidth();
		if(tile->getHeight()>author_height)author_height=tile->getHeight();
	}
	
	engine->getKeyFrameHandler()->setPosition(stripe, engine->getDuration()+0.3, stripe->getPosition()->x, tag->height - 30 /* + 20 + author_height*/, stripe->getPosition()->y);
	
	for(int i=0;i<tag->text.size();++i){
		CharTile* tile = new CharTile(font_text, tag->text[i]);
		tile->order = 9;
		tile->setPosition(scroll_start.x + text_pos, tag->height + author_height *0.5, 0);
		tile->setColor(1,1,1,1);
		tile->setStyle("emboss");
		engine->addObject(tile);
		tiles->push_back(tile);
		text_pos += font_text->getAdvance(tag->text[i]); // tile->getWidth();
		if(tile->getHeight()>text_height)text_height=tile->getHeight();
	}

	int		size = author_pos;
	if(text_pos>size)size=text_pos;
	
	// Add Drome Tile at end of message
	// Old code
//	DromeTile* dt = new DromeTile();
//	dt->setColor(1,1,1,1);
//	dt->order = 8;
//	dt->setPosition(scroll_start.x + size + 250, tag->height, 0);
	
	// Calculate scale
//	double scale = 85 / dt->getHeight();
//	dt->setWidth(scale * dt->getWidth());
//	dt->setHeight(scale * dt->getHeight());
	
//	dt->setW
//	engine->addObject(dt);
//	tiles->push_back(dt);
	
//	author_pos += dt->getWidth();
//	text_pos += dt->getWidth();

	tags->erase(tags->begin());
	delete(tag);
	
	// Next message calc

	// prediction that
	// position = start_pos - time * speed
	// 0 = 1024 - x * speed
	// x * speed = -1024
	// x = -1024 / speed
	// -1024 -> space to skip
	// speed -> speed / 1024 for msecs
	// size is the exact space of the longest tag
	// add something (250) for added spacing between two tags
		
	timer_next->setDuration((size+250 /*+engine->getRenderer()->getWidth()*/) / speed / 1000);
}

void	Graffiti::update(){
	// Reread graffiti file, parser notifies usss
	
	string file = config->client_cache + config->graffiti_file;

	// In filename, rename ? characters to !		
	int t = file.find("?", 0);
	while(t!=string::npos){
		file.replace(t, 1, "!");
		t = file.find("?", 0);
	}

	ifstream fin(file.data(), ios::in);
	if(fin){
		
		string result;
		getline(fin, result, '\0');
		parser->parseBSML(result);
	}
	fin.close();
}

void	Graffiti::onTime(Timer* timer){
	if(timer == timer_next){
		nextTag();
	}
}
