#using namespace std

#ifndef DROMELOGO_H
#define DROMELOGO_H

#include <global/RootObject.h>

class DromeLogo : public RootObject {
	private:
		Image*	img_logo;
		
	public:
		DromeLogo();
		~DromeLogo();
		
		virtual void	paint();
};

#endif
