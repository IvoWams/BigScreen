using namespace std;

#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <vector>
#include <string>

// Tokenizer
// Helper class for the parser to move and read parts of the entire text

class Tokenizer {
	private:
		string		text;
		int			cursor;

	public:
		Tokenizer(const string&);
		~Tokenizer();

		string	token;
		
		// Read x bytes into token:
		void	read(int);
		
		// Displace cursor x bytes
		void	move(int);
		
		// Read all remaining bytes into token:
		void	readAll();
		
		// Move cursor to end of string
		void	moveEnd();
		
		// Find offset of first
		int		find(const string&);
		
		// are we done yet?
		bool	hasMoreTokens();
};

#endif
