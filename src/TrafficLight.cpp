#include <iostream>
#include <random>
#include <vector>
#include <future>
#include <thread>
#include <algorithm>
#include <chrono>
#include "TrafficLight.h"

/* Implementation of class "MessageQueue" */

template <class T>
T MessageQueue<T>::receive()
{
    std::unique_lock<std::mutex> ul(_mutex);
    _condition.wait(ul, [this]{ return !_queue.empty(); });

    T lastItem = std::move(_queue.back());
    _queue.pop_back();

    return lastItem;
}

template <typename T>
void MessageQueue<T>::send(T &&msg)
{
    std::lock_guard<std::mutex> lg(_mutex);
    _queue.clear();
    _queue.emplace_back(msg);
    _condition.notify_one();
}

/* Implementation of class "TrafficLight" */

TrafficLight::TrafficLight()
{
    _currentPhase = TrafficLightPhase::red;
}

void TrafficLight::waitForGreen()
{
    while (_mq.receive() != TrafficLightPhase::green) 
    { 
        continue; 
    }
}

TrafficLightPhase const TrafficLight::getCurrentPhase()
{
    return _currentPhase;
}

void TrafficLight::simulate()
{
    threads.emplace_back(std::thread(&TrafficLight::cycleThroughPhases, this));
}

// virtual function which is executed in a thread
void TrafficLight::cycleThroughPhases()
{
    // time start
    auto timeStart = std::chrono::system_clock::now().time_since_epoch().count();
    int elapsedTime;

    // random numbers
    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(4, 6);
    int random = dist(rng);

    while (true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        elapsedTime = (int)((std::chrono::system_clock::now().time_since_epoch().count() - timeStart) / 1000000000);

        if (elapsedTime > random)
        {
            _currentPhase = _currentPhase == TrafficLightPhase::red ? TrafficLightPhase::green : TrafficLightPhase::red;
            timeStart = std::chrono::system_clock::now().time_since_epoch().count();
            random = dist(rng);
        }
        _mq.send(std::move(_currentPhase));
    };
}
