#include <player/BSML2CueList.h>

BSML2CueList::BSML2CueList(CueList* _cuelist){
	cuelist = _cuelist;
}

BSML2CueList::~BSML2CueList(){}

void BSML2CueList::onStartOpeningTag(const string& tag){
	if(tag == "bsml"){
		cuelist->clearList();
	} else if(tag == "page"){
		cuelist->flush();
	}
}

void BSML2CueList::onFinishOpeningTag(const string& tag){}

void BSML2CueList::onAttribute(const string& tag, const string& attr, const string& value){
	if(tag == "page"){
		if(attr == "file"){
			cuelist->setFile(value);
		} else if(attr == "priority"){
			if(value == "true"){
				cuelist->setPriority(true);
			}else cuelist->setPriority(false);
		}
	}
}

void BSML2CueList::onAttribute(const string& tag, const string& attr, double value){
	if(tag == "page"){
		if(attr == "duration"){
			cuelist->setDuration(value);
		}
	}	
}

void BSML2CueList::onClosingTag(const string& tag){
	if(tag == "bsml"){
		cuelist->flush();
	}
}

void BSML2CueList::onAnyText(const string& tag){}
