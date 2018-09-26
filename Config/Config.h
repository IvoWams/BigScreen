using namespace std;

#ifndef CONFIG_H
#define CONFIG_H

#include <string>

// A set of info that's being passed to the main big objects of the bigscreen engine
// These values are being set while parsing the config.bsml file

typedef struct {

		string	server_ip;
		int		server_port;
		string	server_root;
		string	server_bsml;
		string	server_fonts;
		string	server_images;
		string	server_media;

		string	graffiti_file;
		double	graffiti_speed;
		int		graffiti_enabled;
		
		string	client_cache;
		int		client_maxelements;
		
		string	cue_file;
		int		cue_update;
		
		// Video settings
		int		video_left;
		int		video_top;
		int		video_width;
		int		video_height;
		int		video_fullscreen;
		int		video_bpp;
		int		video_vsync;
		
} Config;

#endif
