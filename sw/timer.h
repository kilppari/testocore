/******************************************************************************/
/**
    Timer class for Testocore
    Copyright (C) 2013 Pekka Mäkinen
    makinpek [ at ] gmail

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/
/******************************************************************************/
#ifndef TIMER_H_
#define TIMER_H_

/**
 * Simple timer implementation with millisecond resolution.
 * If continuously run, the timer wraps around every ~12.1 days.
 * (0x100000 seconds)
 * See reference (public domain, accessed 24-Oct-2013):
 * http://www.firstobject.com/getmillicount-milliseconds-portable-c++.htm
 */
class Timer {
private:
    // The time how long timer has been running (in milliseconds).
    uint32_t m_MilliCounter;

    // Time when timer was last started/resumed/reset
    uint32_t m_LastStartCount;
    // Time when timer was last stopped/reset
    uint32_t m_LastStopCount;

    // Flags to keep track of timer state
    bool m_IsRunning;
    bool m_WasResumed;
    bool m_WasReset;

    // Max number of seconds (just before timer wraps around)
    static const uint32_t kMaxSeconds;
    // Value when timer wraps around (in milliseconds)
    static const uint32_t kWrapCount;

    // corrects wraparound if time is negative
    inline uint32_t checkWrapAround( int32_t time ) {
        return( ( uint32_t ) ( ( time < 0 ) ? ( time + kWrapCount ) : time ) );
    }

public:
    // Constructor, starts timer immediately.
    Timer();
    // Resumes timer if timer has been stopped.
    void resume( void );
    // Stops timer if timer is not running.
    void stop( void );
    // Resets timer (can be stopped or running)
    void reset( void );
    // Returns total running time in milliseconds
    uint32_t getElapsed( void );
};

#endif /* #ifndef TIMER_H_ */
