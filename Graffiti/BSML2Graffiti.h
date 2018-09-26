using namespace std;

#ifndef BSML2GRAFFITI_H
#define BSML2GRAFFITI_H

#include <bsml/ParserListener.h>
#include <graffiti/Graffiti.h>
#include <string>

class Graffiti;

class BSML2Graffiti : public ParserListener {
	private:
		Graffiti*	graffiti;
		
		string		author;
		string		text;
		double		height;
		
		void		flush();
		
	public:
		BSML2Graffiti(Graffiti*);
		~BSML2Graffiti();
		
		void onStartOpeningTag(const string&);
		void onFinishOpeningTag(const string&);

		void onClosingTag(const string&);

		void onAttribute(const string&, const string&, const string&);
		void onAttribute(const string&, const string&, double);
		
		void onAnyText(const string&);		
};

#endif
