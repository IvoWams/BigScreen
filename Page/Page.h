using namespace std;

#ifndef PAGEFIELD_H
#define PAGEFIELD_H

#include <vector>
#include <queue>
#include <main/MainEngine.h>
#include <global/RootObject.h>

// Page contains a list of objects, and adds fading information

// Definition of data that a page object must contain
typedef struct {
	// object fade data
	double	delay;
	double	fade_in;
	int		fade_in_type;
	double 	duration;
	double	fade_out;
	int		fade_out_type;

	// finish coordinates
	double x;
	double y;
			
	// pointer to target object
	RootObject* object;
} PageObjectData;

class Page {
	
	private:
		MainEngine*					engine;
		// Identification
				
	public:
		Page();
		~Page();

		// A list of objects, and fading info
		string						filename;
		double						duration;
		vector<PageObjectData*>*	pageobjectdatalist;	
};

#endif
