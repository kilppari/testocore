/******************************************************************************/
/**
    Unit testing for Process class.

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
#include "process.h"

class TestCase : public TestCaseBase {
    public:
    TestCase( const char* name ) : TestCaseBase( name ) {}
    ~TestCase() { }
    void runTest();
};


int main( void ) {

    TestCase TC( "ut_process" );

    TC.execute();

    return 0;
}

/* -----------------------------------------------------------------------------
 * Define test script here.
 */
void TestCase::runTest( void ) {

/* ------------------------------ */

    UT_START_STEP( 1 );

    unsigned type = 0;
    Process process( type );
    UT_COMMENT( "Checking default values after object instantiation...\n" );

    UT_CHECK_OUTPUT( type == process.getType() );
    UT_CHECK_OUTPUT( false == process.isDead() );
    UT_CHECK_OUTPUT( false == process.isPaused() );
    UT_CHECK_OUTPUT( false == process.isAttached() );
    UT_CHECK_OUTPUT( false == process.initRequired() );
    UT_CHECK_OUTPUT( true == process.isActive() );
    UT_CHECK_OUTPUT( NULL == process.getNext() );

    UT_END_STEP;

/* ------------------------------ */

    UT_START_STEP( 2 );

    unsigned type = 0;
    Process process( type );
    UT_COMMENT( "Checking default setters and getters...\n" );

    UT_CHECK_OUTPUT( 0 == process.getType() );
    process.setType( 1 );
    UT_CHECK_OUTPUT( 1 == process.getType() );

    process.setActive( false );
    UT_CHECK_OUTPUT( false == process.isActive() );
    process.setActive( true );
    UT_CHECK_OUTPUT( true == process.isActive() );

    process.setAttached( true );
    UT_CHECK_OUTPUT( true == process.isAttached() );
    process.setAttached( false );
    UT_CHECK_OUTPUT( false == process.isAttached() );

    process.togglePause();
    UT_CHECK_OUTPUT( true == process.isPaused() );
    process.togglePause();
    UT_CHECK_OUTPUT( false == process.isPaused() );

    Process process2( 0 );
    process.setNext( &process2 );
    UT_CHECK_OUTPUT( &process2 == process.getNext() );
    process.setNext( NULL );
    UT_CHECK_OUTPUT( NULL == process.getNext() );

    process.kill();
    UT_CHECK_OUTPUT( true == process.isDead() );

    UT_END_STEP;

/* ------------------------------ */

    UT_START_STEP( 3 );

    UT_COMMENT( "Checking default Process::onUpdate() method...\n" );

    static bool on_initialize_called = false;

    class FooProcess : public Process {
    public:
        FooProcess( unsigned type ) : Process( type ) {
            m_InitRequired = true; }

        // Overwrite onInitialize
    protected:
        virtual void onInitialize( void ) {
            on_initialize_called = true;
        }
    };

    Process process( 0 );
    UT_CHECK_OUTPUT( false == process.initRequired() );
    process.onUpdate( 0 );
    UT_CHECK_OUTPUT( false == on_initialize_called );

    FooProcess fooprocess( 0 );
    UT_CHECK_OUTPUT( true == fooprocess.initRequired() );
    fooprocess.onUpdate( 0 );
    UT_CHECK_OUTPUT( true == on_initialize_called );
    UT_CHECK_OUTPUT( false == fooprocess.initRequired() );

    UT_END_STEP;

/* ------------------------------ */

    return;
}
