#include <preloader/Preloader.h>

Preloader::Preloader(HTTP* _http, Config*	_config){
	engine = Globals::getGlobals()->engine;
	config = _config;

//	fout.open("PreLoader.txt", ios::out);
//	fout << "Starting\n";
//	fout.flush();
	
	http = _http;

	pl = new BSML2Preloader(this);	// Preloader tells to me
	parser = new Parser(pl);		// Parser tells to preloader

	updating = false;
}

Preloader::~Preloader(){
	fout.close();

	delete parser;
	delete pl;
}

void	Preloader::loadPage(const string& filename){
	string remote_file = config->server_root+config->server_bsml + filename;
	string local_file = config->client_cache + filename;
	while(local_file.find("?") != string::npos)local_file.replace(local_file.find("?"), 1, "!");
	
//	fout << "Downloading page " << filename << "\n";
//	fout.flush();
	
	HTTPRequest	request;
	request.remote_file = remote_file;
	request.local_file = local_file + ".part";
	request.finished = false;
	request.result = 0;
	http->addRequest(&request);
	
	while(!request.finished && request.result==0)Sleep(1);
	if(request.result==0){
			::remove(local_file.data()); // try to delete old file
			::rename(string(local_file+".part").data(), local_file.data());
	}
	
	// Try to parse the file, even if download was unsuccesfull (parsing succeeds if it is in the cache)
	parseFile(local_file);
}

void	Preloader::loadFont(const string& filename){
	string remote_file = config->server_root+config->server_fonts + filename + ".ttf";
	string local_file = config->client_cache + filename + ".ttf";
	
	// First, check if the file exists, were not going to overwrite existing fonts
	FILE*	f = fopen(local_file.data(), "r");
	if(f){
		fclose(f);
		return;
	}

//	fclose(f);
// ???
	
	HTTPRequest	request;
	request.remote_file = remote_file;
	request.local_file = local_file + ".part";
	request.finished = false;
	request.result = 0;
	http->addRequest(&request);
	
	while(!request.finished && request.result==0)Sleep(1);
	if(request.result==0)::rename(string(local_file+".part").data(), local_file.data());
}

void	Preloader::initImage(const string& filename){
	
	// At first, nothing is set, thus start try to load image
	// If the image load was successfull, set the available flag
	// or else set the error flag
	// At second pass if an error flag is set, retry load image
	
//	fout << "initImage(" << filename << ")\n";
//	fout.flush();
				
	// Create and load imagedata if needed
	ImageData* imagedata = ImageDataHandler::getImageDataHandler()->getImageData(filename);
	if(!imagedata->available){
//		fout << "Image data " << imagedata << "\n";
//		fout.flush();
		imagedata->dib = IMG_Load(filename.data());
		if(imagedata->dib){
//			fout << "Loaded ok\n";
//			fout.flush();
			// If image data was successfully loaded, copy variables
			imagedata->width = imagedata->dib->w;
			imagedata->height = imagedata->dib->h;
			imagedata->bpp = imagedata->dib->format->BytesPerPixel;
			imagedata->available = true;	// prevent it from being loaded a second time
		} else {
			imagedata->error = true;
			// Image error, retry on the next loop
		}
	} else {
		// Image could not be loaded ...
	}
}

void	Preloader::loadImage(const string& filename){
	string remote_file = config->server_root+config->server_images + filename;
	string local_file = config->client_cache + filename;
	
//	fout << "loadImage(" << filename << ")\n";
//	fout.flush();2

	// First, check if the image exists, were not going to overwrite existing images
	
	FILE*	f = fopen(local_file.data(), "r");
	if(f){
		fclose(f);

		// Load image data !
		initImage(local_file);
		return;
	}
	// close ???
	
	HTTPRequest	request;
	request.remote_file = remote_file;
	request.local_file = local_file + ".part";
	request.finished = false;
	request.result = 0;
	http->addRequest(&request);
	
	while(!request.finished && request.result==0)Sleep(10);
	if(request.result==0)::rename(string(local_file+".part").data(), local_file.data());
	// ::remove(local_file.data()); // try to delete old file
	// Creating Texture

	initImage(local_file);
}

bool	Preloader::parseFile(const string& local){
	ifstream f(local.data(), ios::in);

	// file exists, start parsing
	if(f){
		string	bsml;
		getline(f, bsml, '\0');
		parser->parseBSML(bsml);
		
		return true;
	}
	
	f.close();

	return false;
}

void	Preloader::update(){
	updating = true;
	
	string	local = config->client_cache + config->cue_file;
	while(local.find("?") != string::npos)local.replace(local.find("?"), 1, "!");
	string	remote = config->server_root + config->cue_file;
		
	// New request!
	HTTPRequest request;
	
	// Set stuff
	request.remote_file = remote;
	request.local_file = local + ".part";	// save the download in a temp file
	request.finished = false;
	request.downloaded = 0;
	request.size = 0;			// Would be nice if we could see the remote file size (Content-Length header only seems to be coming with scripts)
	request.result = 0;
	request.duration = 0;
	request.locked = false;
	http->addRequest(&request);
	
	// Suspend preloader until the file has been downloaded, or an error has occured
	while(!request.finished && request.result==0)Sleep(10);
	// If download is OK, move downloaded file into spot
	if(request.result == 0){
		// if result is OK then reset last_error
		http->last_error = "";
		::remove(local.data()); // try to delete old file
		::rename(string(local+".part").data(), local.data());
	}
	// Try to parse
	
	parseFile(local);
	

	// *** Update graffiti file
	
	if(config->graffiti_enabled==1){
		local = config->client_cache + config->graffiti_file;
		while(local.find("?") != string::npos)local.replace(local.find("?"), 1, "!");
		remote = config->server_root + config->server_bsml + config->graffiti_file;
	
		// Set stuff
		request.remote_file = remote;
		request.local_file = local + ".part";	// save the download in a temp file
		request.finished = false;
		request.downloaded = 0;
		request.size = 0;			// Would be nice if we could see the remote file size (Content-Length header only seems to be coming with scripts)
		request.result = 0;
		request.duration = 0;
		request.locked = false;
		http->addRequest(&request);
	
		// Suspend preloader until the file has been downloaded, or an error has occured
		while(!request.finished && request.result==0)Sleep(10);
		if(request.result == 0){
			// if result is OK then reset last_error
			http->last_error = "";
			::remove(local.data()); // try to delete old file
			::rename(string(local+".part").data(), local.data());
		}
	}
	
	updating = false;
}
