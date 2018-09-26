using namespace std;

#ifndef PARSERLISTENER_H
#define PARSERLISTENER_H
#include <iostream>


// Abstract listening to changes made by the parser

class ParserListener {
	private:
	public:
		ParserListener();
		virtual ~ParserListener();
		
		// onStartOpeningTag fires when the parser just read the name of an
		// opening tag, and is going to parse the attributelist
		virtual void onStartOpeningTag(const string&);
		
		// onFinishOpeningTag fires when the parser has just finished the
		// arraylist
		virtual void onFinishOpeningTag(const string&);
		
		virtual void onAttribute(const string&, const string&, const string&);
		virtual void onAttribute(const string&, const string&, double);
		virtual void onClosingTag(const string&);
		
		// Creation of chartile methods:
		virtual void onAnyText(const string&);
};

#endif
