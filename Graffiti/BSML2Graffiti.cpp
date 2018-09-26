#include <graffiti/BSML2Graffiti.h>

BSML2Graffiti::BSML2Graffiti(Graffiti* _graffiti){
	graffiti = _graffiti;
	
	author = "";
	text = "";
	height = -1;
}

void	BSML2Graffiti::flush(){
	if(author!="" && text!="" && height!=-1){
		graffiti->addTag(author, text, height);
		author = "";
		text = "";
		height = -1;
	}
}

BSML2Graffiti::~BSML2Graffiti(){}

void BSML2Graffiti::onStartOpeningTag(const string&){}
void BSML2Graffiti::onFinishOpeningTag(const string&){}

void BSML2Graffiti::onClosingTag(const string&){}

void BSML2Graffiti::onAttribute(const string& tag, const string& attr, const string& value){
	if(tag=="tag" && attr=="author"){
		author = value;
		flush();
	}else if(tag=="tag" && attr=="text"){
		text = value;
		flush();
	}
}

void BSML2Graffiti::onAttribute(const string& tag, const string& attr, double value){
	if(tag=="tag" && attr=="height"){
		height = value;
		flush();
	}
}
		
void BSML2Graffiti::onAnyText(const string&){}

