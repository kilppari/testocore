/******************************************************************************/
/**
    Timer implementation for Testocore
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

#define _USE_32BIT_TIME_T
#include <sys/timeb.h>
#include <stdint.h>

#include "timer.h"

const uint32_t Timer::kMaxSeconds = 0xFFFFF;
const uint32_t Timer::kWrapCount = 0x3E800000; // 0x100000 * 1000;

/**
 * Constructor, starts timer immediately.
 */
Timer::Timer() :
    m_MilliCounter( 0 ), m_LastStartCount( 0 ), m_LastStopCount( 0 ),
    m_IsRunning( true ), // Start timer instantly
    m_WasResumed( false ) {

    timeb tb;
    ftime( &tb );
    m_LastStartCount = tb.millitm + ( tb.time & kMaxSeconds ) * 1000;
}

/**
 * Stops timer if timer is running.
 */
void Timer::stop( void ) {
    if( !m_IsRunning ) {
        // Timer is already stopped, do nothing.
        return;
    }
    // Get stop time and update millicounter and status flags
    timeb tb;
    ftime( &tb );
    m_LastStopCount = tb.millitm + ( tb.time & kMaxSeconds ) * 1000;

    if( m_WasResumed ) {
        m_MilliCounter += checkWrapAround( m_LastStopCount - m_LastStartCount );
    }
    else {
        m_MilliCounter = checkWrapAround( m_LastStopCount - m_LastStartCount );
    }
    m_IsRunning = false;
    m_WasResumed = false;
//    m_WasReset = false;
}

/**
 * Resumes timer if timer has been stopped.
 */
void Timer::resume( void ) {
    if( m_IsRunning ) {
        // Timer is already running, do nothing.
        return;
    }
    // Get new start time and update status flags
    timeb tb;
    ftime( &tb );
    m_LastStartCount =  tb.millitm + ( tb.time & kMaxSeconds ) * 1000;
    m_IsRunning = true;
    m_WasResumed = true;
//    m_WasReset = false;
}

/**
 * Resets timer. Does not matter whether timer is running or not.
 */
void Timer::reset( void ) {
    timeb tb;
    ftime( &tb );
    m_LastStartCount = tb.millitm + ( tb.time & kMaxSeconds ) * 1000;
    m_MilliCounter = 0;
//    m_WasReset = true;
    m_WasResumed = false;
}

/**
 * Returns total elapsed running time in milliseconds.
 */
uint32_t Timer::getElapsed( void ) {
    timeb tb;
    ftime( &tb );
    uint32_t current_count = tb.millitm + ( tb.time & kMaxSeconds ) * 1000;

    if( m_IsRunning && m_WasResumed ) {
        // Timer is running and has been resumed,
        // return stored running time plus the time since last resume.
        return( m_MilliCounter +
            checkWrapAround( current_count - m_LastStartCount ) );
    }
    else if( m_IsRunning ) {
        //Timer is running, return time since last start or reset.
        return( current_count - m_LastStartCount );
    }
    else {
        // Timer is not running, return stored running time.
        return( m_MilliCounter );
    }
}
