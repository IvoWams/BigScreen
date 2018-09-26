using namespace std;

#ifndef BSML2CONFIG_H
#define BSML2CONFIG_H

#include <bsml/ParserListener.h>
#include <config/Config.h>

class BSML2Config : public ParserListener {
	private:
		// Config object manipulated
		Config*			config;
		
	public:
		BSML2Config();
		~BSML2Config();
		
		Config*	getConfig(){ return config; }
		
		void onStartOpeningTag(const string&);
		void onFinishOpeningTag(const string&);
		
		void onClosingTag(const string&);

		void onAttribute(const string&, const string&, const string&);
		void onAttribute(const string&, const string&, double);
		
		void onAnyText(const string&);
		
};

#endif
