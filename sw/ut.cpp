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

#include "ut.h"

TestCaseBase::TestCaseBase( const char* name ) : m_Name( name ) {
    printf( "\nStarting %s...\n\n", name );
}

void TestCaseBase::execute() {
    printDivider();
    printf( "%s\n", m_Name );
    printDivider();
}

void TestCaseBase::startStep( uint32_t num ) {
    printDivider();
    printf( "TC step %u\n", num );
    printDivider();
}

void TestCaseBase::checkExpectedOutput( bool arg ) {
    if( arg ) {
        // TODO: Success case
    }
    else {
        // TODO: Failure case
    }
}
/*
void TestCaseBase::comment( const char* str ) {
    printf( "%s\n", str );
}
*/
