#include <page/BSML2PageBuilder.h>
#include <global/Globals.h>
#include <interface/Label.h>
#include <fstream>

BSML2PageBuilder::BSML2PageBuilder(PageBuilder* _pagebuilder){
	pagebuilder = _pagebuilder;
	engine = Globals::getGlobals()->engine;
}

BSML2PageBuilder::~BSML2PageBuilder(){}

void BSML2PageBuilder::onStartOpeningTag(const string& tag){
	// A new object onto the building stack
	if(tag == "bsml") 			pagebuilder->newPage();

	// Start building page objects
	else if(tag == "img")		pagebuilder->newImage();
	else if(tag == "font")		pagebuilder->newFont();
	else if(tag == "fade")		pagebuilder->newFade();
	else if(tag == "margin")	pagebuilder->newMargin();
	
	else if(tag == "countdown")	pagebuilder->newCountdown();
	
	else if(tag == "blink")		pagebuilder->newBlink();

	else if(tag == "left")		pagebuilder->newAlignment(ALIGN_LEFT);
	else if(tag == "center")	pagebuilder->newAlignment(ALIGN_CENTER);
	else if(tag == "right")		pagebuilder->newAlignment(ALIGN_RIGHT);

	// Single page formatting commands
	else if(tag == "br")		pagebuilder->newLine();
}

void BSML2PageBuilder::onFinishOpeningTag(const string& tag){
	// Finish tags that expect certain parameters, push on the stack
	if(tag == "img")			pagebuilder->finishImage();
	else if(tag == "font")		pagebuilder->finishFont();
	else if(tag == "fade")		pagebuilder->finishFade();
	else if(tag == "margin")	pagebuilder->finishMargin();
	else if(tag == "countdown")	pagebuilder->finishCountdown();
	else if(tag == "blink")		pagebuilder->finishBlink();
}

void	BSML2PageBuilder::onClosingTag(const string& tag){
	// Pop from the stack
	if(tag == "bsml")			pagebuilder->closePage();	
	else if(tag == "font")		pagebuilder->closeFont();
	else if(tag == "fade")		pagebuilder->closeFade();
	else if(tag == "margin")	pagebuilder->closeMargin();
	else if(tag == "countdown")	pagebuilder->closeCountdown();
	else if(tag == "blink")		pagebuilder->closeBlink();
	
	else if(tag == "left")		pagebuilder->closeAlignment(ALIGN_LEFT);
	else if(tag == "center")	pagebuilder->closeAlignment(ALIGN_CENTER);
	else if(tag == "right")		pagebuilder->closeAlignment(ALIGN_RIGHT);
}


// Attributes with AnyValue (values with quotes "")
void BSML2PageBuilder::onAttribute(const string& tag, const string& attr, const string& value){
	if(tag == "img"){
		
		if(attr == "src")			pagebuilder->setImageSource(value);

	} else if(tag == "font"){
		
		if(attr == "style")			pagebuilder->setFontStyle(value);
		else if(attr == "color")	pagebuilder->setFontColor(value);
		else if(attr == "face")		pagebuilder->setFontFace(value);

	} else if(tag == "countdown"){
		
		if(attr == "blinkcolor")	pagebuilder->setCountdownBlinkcolor(value);
		
	}
}

// Attributes with a numeric value (double)
void BSML2PageBuilder::onAttribute(const string& tag, const string& attr, double value){
	if(tag == "fade"){

		if(attr == "delay")			pagebuilder->setFadeDelay(value);
		else if(attr == "in")		pagebuilder->setFadeIn(value);
		else if(attr == "duration")	pagebuilder->setFadeDuration(value);
		else if(attr == "out")		pagebuilder->setFadeOut(value);
		else if(attr == "type")		pagebuilder->setFadeType(value);

	} else if(tag == "font"){

		if(attr == "size")			pagebuilder->setFontSize((int)value);

	}else if(tag == "img"){

		if(attr == "left")			pagebuilder->setImageLeft(value);
		else if(attr == "top")		pagebuilder->setImageTop(value);
		else if(attr == "width")	pagebuilder->setImageWidth(value);
		else if(attr == "height")	pagebuilder->setImageHeight(value);

	}else if(tag == "margin"){

		if(attr == "left")			pagebuilder->setMarginLeft(value);
		else if(attr == "top")		pagebuilder->setMarginTop(value);
		else if(attr == "right")	pagebuilder->setMarginRight(value);
		else if(attr == "bottom")	pagebuilder->setMarginBottom(value);

	}else if(tag == "countdown"){
		
		if(attr == "time")			pagebuilder->setCountdownTime(value);
		else if(attr == "left")		pagebuilder->setCountdownLeft(value);
		else if(attr == "top")		pagebuilder->setCountdownTop(value);
		else if(attr == "blinkstart")	pagebuilder->setCountdownBlinkstart(value);
		
	} else if(tag == "blink"){

		if(attr == "on")			pagebuilder->setBlinkOn(value);
		else if(attr == "off")		pagebuilder->setBlinkOff(value);
		else if(attr == "walk")		pagebuilder->setBlinkWalk(value);

	}
}

		
void BSML2PageBuilder::onAnyText(const string& text){
	pagebuilder->write(text);
}
