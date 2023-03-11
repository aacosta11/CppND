#include "Timer.h"

Timer::Timer()
{
    _startTicks = 0;
    _pausedTicks = 0;

    _paused = false;
    _started = false;
}

void Timer::start()
{
    _started = true;
    _paused = false;

    // Get the current clock time
    _startTicks = SDL_GetTicks();
    _pausedTicks = 0;
}

void Timer::stop()
{
    // Stop the timer
    _started = false;

    // Unpause the timer
    _paused = false;

    // Clear tick variables
    _startTicks = 0;
    _pausedTicks = 0;
}

void Timer::pause()
{
    if (_started && !_paused)
    {
        // Pause the timer
        _paused = true;

        // Calculate the paused ticks
        _pausedTicks = SDL_GetTicks() - _startTicks;
        _startTicks = 0;
    }
}

void Timer::unpause()
{
    if (_started && _paused)
    {
        _paused = false;
        _startTicks = SDL_GetTicks() - _pausedTicks;
        _pausedTicks = 0;
    }
}

Uint32 Timer::getTicks()
{
    Uint32 time = 0;

    if (_started)
        time = _paused ? _pausedTicks : SDL_GetTicks() - _startTicks;
    
    return time;
}

bool Timer::isStarted()
{
    return _started;
}

bool Timer::isPaused()
{
    return _paused && _started;
}
