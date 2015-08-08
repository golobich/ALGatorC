//
//  Timer.h
//  AlgatorC
//
//  Created by Tadej Golobič on 28/06/15.
//  Copyright © 2015 Tadej Golobič. All rights reserved.
//

#ifndef Timer_h
#define Timer_h

#include <ctime>
#include <chrono>
#include "Log.hpp"

class Timer
{
public:
    //! Default constructor
    Timer();
    
    //! Start the current timer
    void start();
    
    //! Start the i-th timer
    //! \param int
    void start(const int&);
    
    //! Stop the current timer
    void stop();
    
    //! Stop the i-th timer
    //! \param int
    void stop(const int&);
    
    //! Resume the current timer
    void resume();
    
    //! Resume the i-th timer
    //! \param int
    void resume(const int&);
    
    //! Get duration of current timer in miliseconds
    std::chrono::duration<double> time();
    
    //! Get duration of i-th timer in miliseconds
    //! \param int
    std::chrono::duration<double> time(const int&);
    
    //! Stop current timer, get next timer, and start new current timer
    void next();
private:
    //! Current timer
    int curr_timer;
    
    //! Maximum number of timers suported by AlgatorC
    const static int MAX_TIMERS = 5;
    
    std::chrono::high_resolution_clock::time_point start_time[MAX_TIMERS];
    std::chrono::high_resolution_clock::time_point stop_time[MAX_TIMERS];
    
    //clock_t start_time[MAX_TIMERS];
    //clock_t stop_time[MAX_TIMERS];
};

#endif /* Timer_h */
