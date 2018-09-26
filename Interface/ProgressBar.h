using namespace std;

#ifndef PROGRESSBAR_H
#define PROGRESSBAR_H

#include <Interface/InterfaceObject.h>
#include <Interface/Label.h>

/*

	UI Element for tracking progress and stuff
	
*/	

class ProgressBar : public InterfaceObject {
	
	private:
		float	min;		// left border
		float	max;		// right border
		float	current;	// position
				
	public:
		ProgressBar(double, double, double, double);
		~ProgressBar();
				
		void	setMin(float _min){ min = _min; }
		void	setMax(float _max){ max = _max; }
		void	setProgress(float _current){ current = _current; }
		
		void	paint();
};

#endif
