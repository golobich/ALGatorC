//
//  Timer.cpp
//  AlgatorC
//
//  Created by Tadej Golobič on 01/07/15.
//  Copyright © 2015 Tadej Golobič. All rights reserved.
//

#include "Timer.hpp"

Timer::Timer()
{
    curr_timer = 0;
//    for (int i = 0; i<MAX_TIMERS; i++)
//    {
//        start_time[i] = 0;
//        stop_time[i] = 0;
//    }
}

void Timer::start()
{
    start(curr_timer);
}

void Timer::start(const int &i)
{
    start_time[i] = std::chrono::high_resolution_clock::now();
    /*
    if (i <= MAX_TIMERS && i >= 0)
    {
        start_time[i] = std::clock();
    }
    else
    {
        LOG(WARNING) << __FILE__ << "(" << __LINE__ << ") : Null pointer exception in start timer. Given parameter => " << i << ". MAX timers => " << MAX_TIMERS;
    }
    */
}

void Timer::stop()
{
    stop(curr_timer);
}

void Timer::stop(const int &i)
{
    stop_time[i] = std::chrono::high_resolution_clock::now();
}

void Timer::resume()
{
    resume(curr_timer);
}

void Timer::resume(const int &i)
{
    start_time[i] = (std::chrono::high_resolution_clock::time_point)(std::chrono::high_resolution_clock::now() - start_time[i]);
}

void Timer::next()
{
    stop();
    curr_timer++;
    if (curr_timer > MAX_TIMERS)
    {
        LOG(ERROR) << __FILE__ << "(" << __LINE__ << ") : Current timer is higher than MAX timers. Current timer is " << curr_timer << " and MAX timers is " << MAX_TIMERS;
    }
    else
    {
        start();
    }
}

std::chrono::duration<double> Timer::time()
{
    return time(curr_timer);
}

std::chrono::duration<double> Timer::time(const int &i)
{
    std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(stop_time[i] - start_time[i]);
    return time_span;
}
