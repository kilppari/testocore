/******************************************************************************/
/**
    Unit testing for Timer class.

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
*/
/******************************************************************************/
#include "ut_includes.h"

#include "ut.h"
#include "timer.h"

class TestCase : public TestCaseBase {
    public:
    TestCase( const char* name ) : TestCaseBase( name ) {}
    ~TestCase() { }
    void runTest();
};

int main( void ) {

    TestCase TC( "ut_timer" );

    TC.execute();

    return 0;
}

/* -----------------------------------------------------------------------------
 * Define test script here.
 */
void TestCase::runTest( void ) {

/* ------------------------------ */
// Multiple timers running

    UT_START_STEP( 1 );

    // Start timers
    Timer timer = Timer();
    Timer timer1s = Timer();
    Timer timer2s = Timer();
    Timer timer3s = Timer();

    int timer1s_counter = 0;
    int timer2s_counter = 0;
    int timer3s_counter = 0;

    UT_COMMENT( "1s, 2s, 3s and 10.5s timers started.\n\n" );
    while( timer.getElapsed() < 10500 ) {

        if( timer1s.getElapsed() > 1000 ) {
            timer1s.reset();
            UT_COMMENT( "1s timer triggered!\n\n" );
            timer1s_counter++;
        }
        if( timer2s.getElapsed() > 2000 ) {
            timer2s.reset();
            UT_COMMENT( "2s timer triggered!\n" );
            timer2s_counter++;
        }
        if( timer3s.getElapsed() > 3000 ) {
            timer3s.reset();
            UT_COMMENT( "3s timer triggered!\n" );
            timer3s_counter++;
        }
    }
    UT_COMMENT( "10.5s timer expired\n" );

    UT_CHECK_OUTPUT( timer1s_counter == 10 );
    UT_CHECK_OUTPUT( timer2s_counter == 5 );
    UT_CHECK_OUTPUT( timer3s_counter == 3 );

    UT_END_STEP;

/* ------------------------------ */
// Timer starting, stopping and reseting

    UT_START_STEP( 2 );

    // Start timer1, wait 1s and stop timer
    UT_COMMENT( "Timer1 started for 1s\n" );
    Timer timer1 = Timer();
    while( timer1.getElapsed() < 1000 ) {}
    timer1.stop();
    UT_COMMENT( "Timer1 stopped\n" );

    // Start timer2, wait 2s and stop timer
    UT_COMMENT( "Timer2 started for 2s\n" );
    Timer timer2 = Timer();
    while( timer2.getElapsed() < 2000 ) {} // wait 2s
    timer2.stop();
    UT_COMMENT( "Timer2 stopped\n" );

    UT_CHECK_OUTPUT( timer1.getElapsed() < timer2.getElapsed() );

    // Resume timer1 and wait 2 seconds
    UT_COMMENT( "Resuming timer1, waiting 2s\n" );
    timer1.resume();
    while( timer1.getElapsed() < 3000 ) {} // wait 2s
    UT_CHECK_OUTPUT( timer1.getElapsed() > timer2.getElapsed() );

    // Check reset for timer1
    UT_COMMENT( "Stopping timer1\n" );
    timer1.reset();
    UT_CHECK_OUTPUT( timer1.getElapsed() < timer2.getElapsed() );

    // Check stop-reset
    UT_COMMENT( "Stopping and reseting timer1\n" );
    timer1.stop();
    timer1.reset();
    UT_CHECK_OUTPUT( timer1.getElapsed() == 0 );

    // Check that timer1 stays stopped
    UT_COMMENT( "Reseting and resuming timer2\n" );
    timer2.reset();
    timer2.resume();
    while( timer2.getElapsed() < 500 ) {} // wait 0.5s
    UT_CHECK_OUTPUT( timer1.getElapsed() == 0 );

    UT_END_STEP;

/* ------------------------------ */

    return;
}
