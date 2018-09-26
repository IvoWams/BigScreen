// Preloader
// Passes media file references found in bsml pages to the httpgrabber
// This way, media files are loaded before the page is being build (ie. during each cuelist update),
// IF the http transfer is fast enough. Media is being loaded for textures even if they (semi/dont) exist

using namespace std;

#ifndef BSML2PRELOADER_H
#define BSML2PRELOADER_H

#include <main/MainEngine.h>
#include <BSML/ParserListener.h>
#include <preloader/Preloader.h>
#include <config/Config.h>
#include <string>

class Preloader;

class BSML2Preloader : public ParserListener {
	private:
		MainEngine*	engine;
		Preloader*	preloader;
		
	public:
		BSML2Preloader(Preloader*);
		~BSML2Preloader();
		
		void	onOpeningTag(const string&);
		void	onClosingTag(const string&);

		void	onAttribute(const string&, const string&, const string&);
		void	onAttribute(const string&, const string&, double);
		
		void	onAnyText(const string&);		
};

#endif
