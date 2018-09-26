#include <Config/BSML2Config.h>
#include <fstream.h>
 
BSML2Config::BSML2Config(){
	config = new Config();
	
	// default values?
//	config->video_mode = 2;
	config->video_left = 0;
	config->video_top = 0;
	config->video_width = 640;
	config->video_height = 480;
	config->video_bpp = 16;
	config->video_fullscreen = 0;
	config->video_vsync = 0;
	config->client_cache = "";
	config->client_maxelements = 0;
	config->graffiti_enabled = 0;
}

BSML2Config::~BSML2Config(){}

void BSML2Config::onStartOpeningTag(const string& tag){
}

void BSML2Config::onFinishOpeningTag(const string& tag){
}

void BSML2Config::onClosingTag(const string& tag){
}

void BSML2Config::onAttribute(const string& tag, const string& attr, const string& value){	
	
	
	if(tag=="server"){

		if(attr=="ip")config->server_ip = value;
		else if(attr=="root")config->server_root = value;
		else if(attr=="bsml")config->server_bsml = value;
		else if(attr=="fonts")config->server_fonts = value;
		else if(attr=="images")config->server_images = value;
		else if(attr=="media")config->server_media = value;

	}else if(tag=="client"){
	
		if(attr=="cache")config->client_cache = value;
		
	}else if(tag=="cue"){

		if(attr=="file"){

//			string file = value;
		
			config->cue_file = value;
		}
		
	}else if(tag=="graffiti"){
		
		if(attr=="file"){
//			string file = value;
		
			config->graffiti_file = value;
		}
//		if(attr=="font")config->graffiti_font = font;
		
	}
	
}

void BSML2Config::onAttribute(const string& tag, const string& attr, double value){
	if(tag=="video"){

		if(attr=="left")config->video_left = value;
		if(attr=="top")config->video_top = value;
		if(attr=="width")config->video_width = value;
		if(attr=="height")config->video_height = value;
		if(attr=="bpp")config->video_bpp = value;
		if(attr=="fullscreen")config->video_fullscreen = value;
		if(attr=="vsync")config->video_vsync = value;

	}else if(tag=="client"){
	
		if(attr=="maxelements")config->client_maxelements = value;

	}else if(tag=="server"){

		if(attr=="port")config->server_port = value;

	} else if(tag=="cue"){

		if(attr=="update")config->cue_update = value;
		
	} else if(tag=="graffiti"){
		if(attr=="speed")config->graffiti_speed = value;
		if(attr=="enabled")config->graffiti_enabled = value;
//		if(attr=="height")config->graffiti_height = value;
//		if(attr=="update")config->graffiti_update = value;
		
	}
}
	
void BSML2Config::onAnyText(const string& tag){
}
