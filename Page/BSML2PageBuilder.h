using namespace std;

#ifndef BSML2PAGEBUILDER_H
#define BSML2PAGEBUILDER_H

#include <bsml/ParserListener.h>
#include <page/PageBuilder.h>

class PageBuilder;

class BSML2PageBuilder : public ParserListener {
	private:
		MainEngine*		engine;
		PageBuilder*	pagebuilder;
			
	public:
		BSML2PageBuilder(PageBuilder*);
		~BSML2PageBuilder();

		void onStartOpeningTag(const string&);
		void onFinishOpeningTag(const string&);

		void onClosingTag(const string&);

		void onAttribute(const string&, const string&, const string&);
		void onAttribute(const string&, const string&, double);
		
		void onAnyText(const string&);
};

#endif
