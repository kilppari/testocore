/******************************************************************************/
/**
    Unit testing utility for Testocore project
    Copyright (C) 2013 Pekka Mäkinen

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
#ifndef UT_H_
#define UT_H_


// Starts TC step and begins encapsulation (should be used with UT_END_STEP)
#define UT_START_STEP( num ) { startStep( num )
// Stops TC step and ends the step encapsulation
#define UT_END_STEP endStep(); }
// Wrapper for checkExpectedOutput with line and file information
#define UT_CHECK_OUTPUT( expr ) checkExpectedOutput( expr, __LINE__, __FILE__ )
// Prints given comment to log file
#define UT_COMMENT( stream ) {\
    std::stringstream ss;\
    ss << stream;\
    log( ss.str() );\
    }
// Wrapper for printDivider
#define UT_DIVIDER printDivider()

/**
 * Abstract base class for unit test case
 * where runTest must be implemented for the test procedure.
 */
class TestCaseBase {

private:

    const char* m_Name;
    std::ofstream m_OutFile;
    uint32_t m_CurrentStepPassCounter;
    uint32_t m_CurrentStepFailCounter;
    static const std::string kDivider;
    static const std::string kLogName;
    bool m_TestCasePassed;

    // Implements the test procedure (all steps).
    virtual void runTest( void ) = 0;

protected:
    // Current step of the test case.
    uint32_t m_CurrentStep;

    // Sets current step and prints info about it.
    void startStep( uint32_t num );
    // Ends current step and prints results.
    void endStep( void );

    /* Keeps track of successfull/failed checks and in failed case,
     * stores line and file information about the failure point
     */
    void checkExpectedOutput( bool arg, uint32_t line, const char* file );

    // Prints divider line.
    void printDivider( void ) { log( kDivider ); }
    // Prints given string to log file.
    void log( std::string str );

public:
    // Constructor, defines name for the test case
    explicit TestCaseBase( const char* name );
    virtual ~TestCaseBase() {}

    // Runs the test case.
    void execute();
};

#endif /* #ifndef UT_H_*/
