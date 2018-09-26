#include <BSML/Parser.h>

Parser::Parser(){}

Parser::Parser(ParserListener* _listener){
	listener = _listener;
}

Parser::~Parser(){}

// BSML Document	== {BSML}
void	Parser::parseBSMLDocument(Tokenizer* t){
	while(t->hasMoreTokens())
		parseBSML(t);
}

// BSML				== AnyText | Tag | Comments
void	Parser::parseBSML(Tokenizer* t){
	// Decide if AnyText, Tag or Comments
	// Note: Dont move cursor when parsing decision BNF
	
	t->read(1);
	
	if(t->token[0]=='<'){			// Tag/Comment
		t->read(2);
		if(t->token[1]=='!')		// Comment
			parseComments(t);
		else
			parseTag(t);		// Tag
	}else{
		parseAnyText(t);		// Any Text
	}
}

// AnyText			== Any character, except <
void	Parser::parseAnyText(Tokenizer* t){
	anytext = "";

	// Remove leading spaces (start out with text)
	parseLeadingSpaces(t);
	
	// Find the first occurance of <	
	int ending = t->find("<");
	
	// copy text to buffer
	t->read(ending);
	anytext = t->token;
	
	// Remove EndingSpaces
	while(anytext[ending] == ' ' ||
		  anytext[ending] == '\n' ||
		  anytext[ending] == '\r' ||
		  anytext[ending] == '\f' ||
		  anytext[ending] == '\t' )anytext.resize( anytext.size() -1 );

	// If any text left:
	if(anytext.size()>0)listener->onAnyText(anytext);

	// Set cursor after anytext
	t->move(ending);
}

// Tag				== <SubTag>
void	Parser::parseTag(Tokenizer* t){
	// Skip <	
	t->move(1);

	parseSubTag(t);
	
	// Skip >
	t->move(1);
}

// SubTag				== OpeningTag | ClosingTag
void	Parser::parseSubTag(Tokenizer* t){
	// Starting token is a /, makes it a closing tag
	t->read(1);

	if(t->token[0]=='/')
		parseClosingTag(t);
	else
		parseOpeningTag(t);
}

// Comments			== <!--CommentText-->
void	Parser::parseComments(Tokenizer* t){
	// Skip <!--
	t->move(4);
	
	// Parse comment text
	parseCommentText(t);
	
	// Skip -->
	t->move(3);
}

// OpeningTag		== LimitedText AttributeList
void	Parser::parseOpeningTag(Tokenizer* t){
	// First, parse limited text, this text is the tagname
	parseLimitedText(t);
	
	// amount of bytes
	int size = limitedtext.size();

	// Copy bytes into tagname
	tagname = limitedtext;
	
	// notify interpreter
	listener->onStartOpeningTag(tagname);

	// on to attributelist parsing	
	parseAttributeList(t);
	
	// notify interpreter on completion of opening tag
	listener->onFinishOpeningTag(tagname);
}

// AttributeList	== {AttributePair}
void	Parser::parseAttributeList(Tokenizer* t){
	t->read(1);
	
	// Deciding if the list has been closed, or a new pair is coming
	while(t->token[0]!='>'){
		parseAttributePair(t);
		t->read(1);
	}
}

// ClosingTag		== /LimitedText
void	Parser::parseClosingTag(Tokenizer* t){
	// Skip /
	t->move(1);	
	
	// Parse limited text, which is the name of our closing tag
	parseLimitedText(t);

	listener->onClosingTag(limitedtext);
}

// CommentText		== Any character, except -->
void	Parser::parseCommentText(Tokenizer* t){
	int ending = t->find("-->");	// Finding -->

	// Ignore comments
	t->move(ending);
}

// LimitedText		== {0..9|a..z|A..Z}
void	Parser::parseLimitedText(Tokenizer* t){
	limitedtext = "";
	
	int	pos = 0;

	t->read(1);	
	
	// loop until token is not 0..9, a..z or A..Z
	while(	t->token[0]>='0' && t->token[0]<='9' ||
			t->token[0]>='A' && t->token[0]<='Z' ||
			t->token[0]>='a' && t->token[0]<='z' ){
		limitedtext += t->token;
		t->move(1);
		t->read(1);
	}
}

// AttributePair	== LeadingSpaces AttributeName LeadingSpaces = LeadingSpaces AttributeValue LeadingSpaces
void	Parser::parseAttributePair(Tokenizer* t){
	// Skip leading spaces
	parseLeadingSpaces(t);
	
	// Parse the attribute name
	parseAttributeName(t);
	
	// Skip leading spaces
	parseLeadingSpaces(t);

	// Skip =
	t->move(1);
		
	parseLeadingSpaces(t);
	
	parseAttributeValue(t);

	parseLeadingSpaces(t);
	
}

// AttributeName	== LimitedText
void	Parser::parseAttributeName(Tokenizer* t){
	parseLimitedText(t);
	
	attributename = limitedtext;
}


// AttributeValue	== AnyValue | Number
void	Parser::parseAttributeValue(Tokenizer* t){
	t->read(1);

	// If starts with ", its anyvalue
	if(t->token[0]=='"'){
		parseAnyValue(t);
		listener->onAttribute(tagname, attributename, anyvalue);

	} else {
		parseNumber(t);
		listener->onAttribute(tagname, attributename, atof(number.c_str()));
	}
}

// AnyValue			== "AnyValueText"
void	Parser::parseAnyValue(Tokenizer* t){
	// Skip "
	t->move(1);
	
	// Parse Text
	parseAnyValueText(t);
	
	// Skip "
	t->move(1);
}

// AnyValueText			== Any character, except "
void	Parser::parseAnyValueText(Tokenizer* t){
//	printf("Parser::parseAnyValueText()\n");

	int ending = t->find("\"");
	t->read(ending);

	anyvalue = t->token;
	
	t->move(ending);
}

// Number			== [-]{0..9}[.]{0..9}
void	Parser::parseNumber(Tokenizer* t){
	int pos = 0;
	
	number = "";
	
	t->read(1);

	if(t->token[0]=='-'){
		number += "-";
		
		t->move(1);
		t->read(1);
	}
	
	while(t->token[0]>='0' && t->token[0]<='9'){
		number += t->token;
		
		t->move(1);
		t->read(1);
	}
	
	if(t->token[0]=='.'){
		number += ".";
	
		t->move(1);
		t->read(1);
	}
	
	while(t->token[0]>='0' && t->token[0]<='9'){
		number += t->token[0];
		
		t->move(1);
		t->read(1);
	}
}

// Helper function, skip leading spaces, carriage returns and line feeds
void	Parser::parseLeadingSpaces(Tokenizer* t){
	t->read(1);

	while((	t->token[0]==' ' || t->token[0]=='\n' || t->token[0]=='\r' || t->token[0]=='\f' || t->token[0]=='\t' )
			&& t->hasMoreTokens()){
		t->move(1);
		t->read(1);
	}
}

// Interfacing method:
void	Parser::parseBSML(const string& text){
	Tokenizer tokenizer(text);
	parseBSMLDocument(&tokenizer);
}
