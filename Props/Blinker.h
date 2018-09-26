using namespace std;

#ifndef BLINKER_H
#define BLINKER_H

class Blinker {
	private:
		// engine time when to start blinking
		double	blink_start;
		// engine time when to stop blinking
		double	blink_stop;
		// the 'on' duration
		double	blink_duration;
		// the 'off' duration
		double	blink_offset;
		
	protected:
		// the result
		bool	isBlinked();

	public:
		Blinker();
		~Blinker();
		
		void	setBlinkStart(double _start){ blink_start = _start; }
		void	setBlinkStop(double _stop){ blink_stop = _stop; }
		void	setBlinkDuration(double _duration){ blink_duration = _duration; }
		void	setBlinkOffset(double _offset){ blink_offset = _offset; }
				
};

#endif
