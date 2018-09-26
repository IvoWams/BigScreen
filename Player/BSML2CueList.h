using namespace std;

#ifndef BSML2CUELIST_H
#define BSML2CUELIST_H

#include <bsml/ParserListener.h>
#include <player/CueList.h>

// For interpreting the cue file

class BSML2CueList : public ParserListener {
	private:
		CueList*	cuelist;
		
	public:
		BSML2CueList(CueList*);
		virtual ~BSML2CueList();
		
		virtual void onStartOpeningTag(const string&);
		virtual void onFinishOpeningTag(const string&);
		
		virtual void onAttribute(const string&, const string&, const string&);
		virtual void onAttribute(const string&, const string&, double);
		virtual void onClosingTag(const string&);
		
		virtual void onAnyText(const string&);
};

#endif
