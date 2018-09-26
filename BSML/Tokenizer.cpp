#include <BSML/Tokenizer.h>

Tokenizer::Tokenizer(const string& _text){
	text = _text;
	cursor = 0;
}

Tokenizer::~Tokenizer(){}

void	Tokenizer::read(int x){
	token = text.substr(cursor, x);
}

void	Tokenizer::readAll(){
	token = text.substr(cursor, text.size() - cursor);
}

void	Tokenizer::move(int x){
	cursor += x;
	if(cursor > text.size())cursor = text.size();
}

void	Tokenizer::moveEnd(){
	cursor = text.size();
}

int		Tokenizer::find(const string& target){
	int position = text.substr(cursor, text.size() - cursor).find(target);
	if(position == string::npos)return text.size() - cursor;
	return position;
}

bool	Tokenizer::hasMoreTokens(){
	return cursor < text.size();
}
