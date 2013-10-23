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


#define UT_START_STEP( num ) { startStep( num )
#define UT_END_STEP endStep(); }
#define UT_CHECK_OUTPUT( expr ) checkExpectedOutput( expr, __LINE__, __FILE__ )
#define UT_COMMENT( ... ) printf( __VA_ARGS__ )
#define UT_DIVIDER printDivider()

class TestCaseBase {

private:
    virtual void runTest( void ) = 0;
    const char* m_Name;
    std::ofstream m_OutFile;
    static const std::string kDivider;
    static const std::string kLogName;
    uint32_t m_CurrentStepPassCounter;
    uint32_t m_CurrentStepFailCounter;

protected:
    uint32_t m_CurrentStep;

    void startStep( uint32_t num );
    void endStep( void );
    void checkExpectedOutput( bool arg, uint32_t line, const char* file );
    void printDivider( void ) { log( kDivider ); }
    void log( std::string str );

public:
    TestCaseBase( const char* name );
    virtual ~TestCaseBase() {}

    void execute();
};

#endif /* #ifndef UT_H_*/
