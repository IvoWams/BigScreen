using namespace std;

#ifndef STRIPE_H
#define STRIPE_H

#include <global/RootObject.h>

// This is actually a prop that graffiti uses, it's the background color of the scroller.

class Stripe : public RootObject {
	private:
	public:
		Stripe();
		~Stripe();
		
		void	paint();
};

#endif
