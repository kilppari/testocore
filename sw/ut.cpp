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

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/
/******************************************************************************/
#include <stdio.h>
#include <stdint.h>

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

#include "ut.h"

const std::string TestCaseBase::kDivider =
    "-------------------------------------------------------------------------------\n";
const std::string TestCaseBase::kLogName = "log.txt";

TestCaseBase::TestCaseBase( const char* name ) : m_Name( name ) {}

void TestCaseBase::execute() {
    m_OutFile.open( kLogName.c_str(), std::ofstream::trunc );
    m_OutFile.close();

    log( std::string( "Starting " ) + m_Name + "...\n\n" );

    runTest();
}

void TestCaseBase::startStep( uint32_t num ) {
    std::stringstream ss;
    ss << kDivider << "TC step " << num << "\n\n";
    log( ss.str() );

    m_CurrentStep = num;
    m_CurrentStepPassCounter = 0;
    m_CurrentStepFailCounter = 0;
}

void TestCaseBase::endStep( void ) {
    std::stringstream ss;
    uint32_t step_count = m_CurrentStepPassCounter + m_CurrentStepFailCounter;
    ss << "\nTC step " << m_CurrentStep << " finished: " << m_CurrentStepPassCounter
        << "/" << step_count << " checks PASSED.\n\n";
    log( ss.str() );
}

void TestCaseBase::checkExpectedOutput(
    bool arg, uint32_t line, const char* file ) {

    std::stringstream ss;

    if( arg ) {
// ss << "Checking expected output: PASS on line " << line << std::endl;
//        log( ss.str() );
        m_CurrentStepPassCounter++;
    }
    else {
        ss << "Checking expected output: FAIL (on line " << line << ")"
            << std::endl;
        log( ss.str() );
        m_CurrentStepFailCounter++;
    }
}

void TestCaseBase::log( std::string str ) {
    std::cout << str;
    m_OutFile.open( kLogName.c_str(), std::ofstream::app );
    m_OutFile << str;
    m_OutFile.close();
}
/*
void TestCaseBase::comment( const char* str ) {
    printf( "%s\n", str );
}
*/
