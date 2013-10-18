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
#ifndef UT_H_
#define UT_H_

const char kDivider[ 81 ] =
"-------------------------------------------------------------------------------\0";

#define UT_START_STEP( num ) startStep( num )
#define UT_CHECK_OUTPUT( expr ) checkExpectedOutput( expr )
#define UT_COMMENT( ... ) printf( __VA_ARGS__ )
#define UT_DIVIDER printDivider()

class TestCaseBase {
    protected:
    uint32_t m_CurrentStep;
    const char* m_Name;
//std::string m_Name;

    void startStep( uint32_t num );
    void checkExpectedOutput( bool arg );
    void printDivider( void ) { printf( "%s\n", kDivider ); }
//    void comment( const char* str);

    public:
    TestCaseBase( const char* name );
    virtual ~TestCaseBase() {}
    virtual void runTest( void ) = 0;
    void execute();

};

#endif /* #ifndef UT_H_*/
