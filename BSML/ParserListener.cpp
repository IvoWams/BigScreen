#include <BSML/ParserListener.h>

// Note: This is a test implementation of ParserListener...

ParserListener::ParserListener(){}
ParserListener::~ParserListener(){}

void ParserListener::onStartOpeningTag(const string& tag){
	printf("Opening tag [%s]\n", tag.c_str());
//	system("PAUSE");
}

void ParserListener::onFinishOpeningTag(const string& tag){
	printf("Opening tag [%s]\n", tag.c_str());
//	system("PAUSE");
}

void ParserListener::onAttribute(const string& tag, const string& attrib, const string& value){
	printf("Attribute for tag [%s]: [%s]=[%s]\n", tag.c_str(), attrib.c_str(), value.c_str());
//	system("PAUSE");
}

void ParserListener::onAttribute(const string& tag, const string& attrib, double value){
	printf("Attribute for tag [%s]: [%s]=[%f]\n", tag.c_str(), attrib.c_str(), value);
//	system("PAUSE");
}

void ParserListener::onClosingTag(const string& tag){
	printf("Closing tag [%s]\n", tag.c_str());
//	system("PAUSE");
}
		
// Creation of chartile methods:
void ParserListener::onAnyText(const string& text){
	printf("Text:\n%s\n", text.c_str());
}
