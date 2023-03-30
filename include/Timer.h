#ifndef Timer_H
#define Timer_H

#include <SDL2/SDL.h>

/*
This class is from Lazy Foo' Productions
(http://lazyfoo.net/)
*/
class Timer
{
    public:
		// CONSTRUCTORS / DESTRUCTORS
		Timer();
		Timer(const Timer &timer);
		Timer(Timer &&timer);
		Timer &operator=(const Timer &timer);
		Timer &operator=(Timer &&timer);
		~Timer();

		// CLOCK ACTIONS
		void start();
		void stop();
		void pause();
		void unpause();

		// GET TICKS
		Uint32 getTicks();

		// TIMER STATUS
		bool isStarted();
		bool isPaused();

    private:
		// clock time
		Uint32 _startTicks;
		Uint32 _pausedTicks;

		// timer status
		bool _paused;
		bool _started;
};

#endif