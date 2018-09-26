using namespace std;

#ifndef PLAYERLISTENER_H
#define PLAYERLISTENER_H

// used for listening to player events (player as in, plays the bigscreen video)

class PlayerListener {
	private:
	public:
		virtual void	onFinishedPage(Page*);		// Fires when a page starts to fade out (time to read next page)
};

#endif