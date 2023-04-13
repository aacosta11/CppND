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
    // random number generator
    std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<double> dist(4000, 6000);
    
    auto lastUpdate = std::chrono::system_clock::now();
    double fourToSixSeconds = static_cast<double>(dist(rng));

    while (true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        auto &&now = std::chrono::system_clock::now();
        double timeSinceLastUpdate = static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(now - lastUpdate).count());

        if (timeSinceLastUpdate > fourToSixSeconds)
        {
            _currentPhase = _currentPhase == TrafficLightPhase::red ? TrafficLightPhase::green : TrafficLightPhase::red;
            lastUpdate = now;
            fourToSixSeconds = static_cast<double>(dist(rng));
        }
        _mq.send(std::move(_currentPhase));
    };
}
