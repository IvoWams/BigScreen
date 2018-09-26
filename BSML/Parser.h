using namespace std;

#ifndef PARSER_H
#define PARSER_H

#include <BSML/Tokenizer.h>
#include <BSML/ParserListener.h>

// Behaves like a normal LALR parser with a basic type of SGML functionality
//
// Syntaxis:
// BSML Document	== {BSML}
// BSML				== AnyText | Tag | Comments
// AnyText			== Any character, except '<'
// Tag				== <SubTag>
// SubTag			== OpeningTag | ClosingTag
// Comments			== "<!--" CommentText "-->"
// OpeningTag		== LimitedText* AttributeList
// AttributeList	== {AttributePair}
// ClosingTag		== /LimitedText*
// CommentText		== Any character, except "-->" sequence
// LimitedText		== {'0'..'9'|'a'..'z'|'A'..'Z'}
// AttributePair	== LeadingSpaces AttributeName LeadingSpaces = LeadingSpaces AttributeValue
// AttributeName	== LimitedText
// AttributeValue	== AnyValue* | Number*
// AnyValue			== '"' AnyValueText '"'
// AnyValueText		== LeadingSpaces Any character, except "
// Number			== ['-']{'0'..'9'}['.']{'0'..'9'}
// LeadingSpaces	== {' '}
//
// *-> listener involvement

class Parser {
	private:
		ParserListener*		listener;
		
		string	limitedtext;
		string	anyvalue;
		string	anytext;
		string	number;
		string	tagname;
		string	attributename;
		
		// Parsing methods
		void	parseBSMLDocument(Tokenizer*);
		void	parseBSML(Tokenizer*);
		void	parseAnyText(Tokenizer*);
		void	parseTag(Tokenizer*);
		void	parseSubTag(Tokenizer*);
		void	parseComments(Tokenizer*);
		void	parseOpeningTag(Tokenizer*);
		void	parseAttributeList(Tokenizer*);
		void	parseClosingTag(Tokenizer*);
		void	parseCommentText(Tokenizer*);
		void	parseLimitedText(Tokenizer*);
		void	parseAttributePair(Tokenizer*);
		void	parseAttributeName(Tokenizer*);
		void	parseAttributeValue(Tokenizer*);
		void	parseAnyValue(Tokenizer*);
		void	parseAnyValueText(Tokenizer*);
		void	parseNumber(Tokenizer*);
		void	parseLeadingSpaces(Tokenizer*);

	public:
		Parser();
		Parser(ParserListener*);
		~Parser();
		
		void	parseBSML(const string&);
};

#endif
