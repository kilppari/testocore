#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#include "../sw/mem_pool.h"
#include "../sw/ut.h"

#define BLOCK_COUNT 10000
#define BLOCK_SIZE 1000


class TestCase : public TestCaseBase {
    public:
    TestCase( const char* name ) : TestCaseBase( name ) {}
    ~TestCase() { }
    void runTest();
};


int main( void ) {

    TestCase TC( "ut_mem_pool" );

    TC.runTest();

    return 0;
}

/* -----------------------------------------------------------------------------
 * Define test script here.
 */
void TestCase::runTest( void ) {

/* ------------------------------
   TC 1
   ------------------------------ */

    UT_START_STEP( 1 );

    UT_COMMENT( "Memory pool speed benchmark\n" );
    UT_COMMENT( "%d allocations and deallocations with block size of %d bytes:\n", BLOCK_COUNT, BLOCK_SIZE );

    /* Preconditions
     */
    MemoryPool MemPool( 1, BLOCK_SIZE, BLOCK_COUNT );
    void* temp_ptr[ BLOCK_COUNT ];
#if 0
    struct timespec ts;
    struct timespec ts2;


    // Using mempool:
    clock_gettime( CLOCK_MONOTONIC,&ts );
    for( int i = 0; i < BLOCK_COUNT; i++ ) {
        temp_ptr[ i ] = MemPool.alloc();
    }
    for( int i = 0; i < BLOCK_COUNT; i++ ) {
        MemPool.dealloc( temp_ptr[ i ] );
    }
    clock_gettime( CLOCK_MONOTONIC,&ts2 );

    UT_COMMENT( "Total time for memory pool:\t%luus\n", ( ts2.tv_nsec - ts.tv_nsec ) / 1000 );

    // Using malloc:
    clock_gettime( CLOCK_MONOTONIC,&ts );
    for( int i = 0; i < BLOCK_COUNT; i++ ) {
        temp_ptr[ i ] = malloc( BLOCK_SIZE );
    }
    for( int i = 0; i < BLOCK_COUNT; i++ ) {
        free( temp_ptr[ i ] );
    }
    clock_gettime( CLOCK_MONOTONIC,&ts2 );

    UT_COMMENT( "Total time for malloc:\t\t%luus\n", ( ts2.tv_nsec - ts.tv_nsec ) / 1000 );
#endif
/* ------------------------------
   TC 2
   ------------------------------ */

    UT_START_STEP( 2 );
    UT_COMMENT( "Checking expected free block counts\n" );

    UT_COMMENT( "Free blocks: %u\n", MemPool.getFreeBlockCount() );
    UT_COMMENT( "Allocating 10000 blocks..\n" );
    for( int i = 0; i < BLOCK_COUNT; i++ ) {
        temp_ptr[ i ] = MemPool.alloc();
    }
    UT_COMMENT( "Free blocks: %u\n", MemPool.getFreeBlockCount() );
    UT_COMMENT( "Deallocating 10000 blocks..\n" );
    for( int i = 0; i < BLOCK_COUNT; i++ ) {
        MemPool.dealloc( temp_ptr[ i ] );
    }
    UT_COMMENT( "Free blocks: %u\n", MemPool.getFreeBlockCount() );
    UT_COMMENT( "Allocating 5000 blocks..\n" );
    for( int i = 0; i < BLOCK_COUNT / 2; i++ ) {
        temp_ptr[ i ] = MemPool.alloc();
    }
    UT_COMMENT( "Free blocks: %u\n", MemPool.getFreeBlockCount() );
    UT_COMMENT( "Deallocating 5000 blocks..\n" );
    for( int i = 0; i < BLOCK_COUNT / 2; i++ ) {
        MemPool.dealloc( temp_ptr[ i ] );
    }
    UT_COMMENT( "Free blocks: %u\n", MemPool.getFreeBlockCount() );

    printDivider();
    return;
}
