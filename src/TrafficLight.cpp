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
    // FP.5a : The method receive should use std::unique_lock<std::mutex> and _condition.wait() 
    // to wait for and receive new messages and pull them from the queue using move semantics. 
    // The received object should then be returned by the receive function. 

    std::unique_lock<std::mutex> ul(_mutex);
    _condition.wait(ul, [this]{ return !_queue.empty(); });

    T lastItem = std::move(_queue.back());
    _queue.pop_back();

    return lastItem;
}

template <typename T>
void MessageQueue<T>::send(T &&msg)
{
    // FP.4a : The method send should use the mechanisms std::lock_guard<std::mutex> 
    // as well as _condition.notify_one() to add a new message to the queue and afterwards send a notification.
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
    // FP.5b : add the implementation of the method waitForGreen, in which an infinite while-loop 
    // runs and repeatedly calls the receive function on the message queue. 
    // Once it receives TrafficLightPhase::green, the method returns.
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
    // FP.2b : Finally, the private method „cycleThroughPhases“ should be started in a thread when the public method „simulate“ is called. To do this, use the thread queue in the base class. 
    threads.emplace_back(std::thread(&TrafficLight::cycleThroughPhases, this));
}

// virtual function which is executed in a thread
void TrafficLight::cycleThroughPhases()
{
    // FP.2a : Implement the function with an infinite loop that measures the time between two loop cycles 
    // and toggles the current phase of the traffic light between red and green and sends an update method 
    // to the message queue using move semantics. The cycle duration should be a random value between 4 and 6 seconds. 
    // Also, the while-loop should use std::this_thread::sleep_for to wait 1ms between two cycles. 

    // time start
    auto timeStart = std::chrono::system_clock::now().time_since_epoch().count();
    int elapsedTime;

    // random numbers
    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(4, 6);
    int random = dist(rng);

    while (true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1500));
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
