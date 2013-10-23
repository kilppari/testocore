#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <fstream>
#include <string>

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

    TC.execute();

    return 0;
}

/* -----------------------------------------------------------------------------
 * Define test script here.
 */
void TestCase::runTest( void ) {


/* ------------------------------
   TC step 1
   ------------------------------ */
#if 0
    UT_START_STEP( 1 );

    MemoryPool MemPool( 1, BLOCK_SIZE, BLOCK_COUNT );
    void* temp_ptr[ BLOCK_COUNT ];

    UT_COMMENT( "Memory pool speed benchmark\n" );
    UT_COMMENT( "%d allocations and deallocations with block size of %d bytes:\n", BLOCK_COUNT, BLOCK_SIZE );

    /* Preconditions
     */

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

    UT_END_STEP;
#endif
/* ------------------------------
   TC step 2
   ------------------------------ */

    UT_START_STEP( 2 );

    MemoryPool MemPool( BLOCK_SIZE, BLOCK_COUNT );
    void* temp_ptr[ BLOCK_COUNT ];

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


    UT_END_STEP;

/* ------------------------------
   TC step 3

   MemPoolManager: Check pool adding and removal,
   ------------------------------ */

    UT_START_STEP( 3 );

    MemPoolManager PoolManager;

    // Add Five pools of various sizes:
    UT_CHECK_OUTPUT( PoolManager.addPool( new MemoryPool( 32, 100 ) ) == true );
    UT_CHECK_OUTPUT( PoolManager.addPool( new MemoryPool( 64, 100 ) ) == true );
    UT_CHECK_OUTPUT( PoolManager.addPool( new MemoryPool( 128, 80 ) ) == true );
    UT_CHECK_OUTPUT( PoolManager.addPool( new MemoryPool( 256, 30 ) ) == true );
    UT_CHECK_OUTPUT( PoolManager.addPool( new MemoryPool( 512, 20 ) ) == true );

    UT_CHECK_OUTPUT( PoolManager.getPoolCount() == 5 );

    // Start removal of pools, check also that removal returns false if pool is
    // not found.
    UT_CHECK_OUTPUT( PoolManager.removePoolByBlockSize( 64 ) == true );
    UT_CHECK_OUTPUT( PoolManager.removePoolByBlockSize( 64 ) == false );
    UT_CHECK_OUTPUT( PoolManager.getPoolCount() == 4 );

    UT_CHECK_OUTPUT( PoolManager.removePoolByBlockSize( 256 ) == true );
    UT_CHECK_OUTPUT( PoolManager.removePoolByBlockSize( 256 ) == false );
    UT_CHECK_OUTPUT( PoolManager.getPoolCount() == 3 );

    UT_CHECK_OUTPUT( PoolManager.removePoolByBlockSize( 512 ) == true );
    UT_CHECK_OUTPUT( PoolManager.removePoolByBlockSize( 512 ) == false );
    UT_CHECK_OUTPUT( PoolManager.getPoolCount() == 2 );

    // Add one new pool and continue removal
    UT_CHECK_OUTPUT( PoolManager.addPool( new MemoryPool( 640, 10 ) ) == true );
    UT_CHECK_OUTPUT( PoolManager.getPoolCount() == 3 );

    UT_CHECK_OUTPUT( PoolManager.removePoolByBlockSize( 32 ) == true );
    UT_CHECK_OUTPUT( PoolManager.removePoolByBlockSize( 32 ) == false );
    UT_CHECK_OUTPUT( PoolManager.getPoolCount() == 2 );

    UT_CHECK_OUTPUT( PoolManager.removePoolByBlockSize( 128 ) == true );
    UT_CHECK_OUTPUT( PoolManager.removePoolByBlockSize( 128 ) == false );
    UT_CHECK_OUTPUT( PoolManager.getPoolCount() == 1 );

    UT_CHECK_OUTPUT( PoolManager.removePoolByBlockSize( 640 ) == true );
    UT_CHECK_OUTPUT( PoolManager.removePoolByBlockSize( 640 ) == false );
    UT_CHECK_OUTPUT( PoolManager.getPoolCount() == 0 );

    // Check that all pools can be destroyed at once
    UT_CHECK_OUTPUT( PoolManager.addPool( new MemoryPool( 32, 100 ) ) == true );
    UT_CHECK_OUTPUT( PoolManager.addPool( new MemoryPool( 64, 100 ) ) == true );
    UT_CHECK_OUTPUT( PoolManager.addPool( new MemoryPool( 128, 80 ) ) == true );
    UT_CHECK_OUTPUT( PoolManager.getPoolCount() == 3 );
    PoolManager.clearAllPools();
    UT_CHECK_OUTPUT( PoolManager.getPoolCount() == 0 );

    UT_END_STEP;

/* ------------------------------
   TC step 4

   MemPoolManager: check allocation
   (linear allocation in for loop)
   ------------------------------ */

    UT_START_STEP( 4 );

    MemPoolManager PoolManager;

    // Add Five pools of various sizes:
    MemoryPool* pool_32_ptr = new MemoryPool( 32, 20 );
    MemoryPool* pool_64_ptr = new MemoryPool( 64, 10 );
    MemoryPool* pool_128_ptr = new MemoryPool( 128, 5 );

    UT_CHECK_OUTPUT( PoolManager.addPool( pool_32_ptr ) == true );
    UT_CHECK_OUTPUT( PoolManager.addPool( pool_64_ptr ) == true );
    UT_CHECK_OUTPUT( PoolManager.addPool( pool_128_ptr ) == true );
    UT_CHECK_OUTPUT( PoolManager.getPoolCount() == 3 );

    // Check allocation and deallocation from pool_32, other pools
    // should be unaffected.
    void* ptr_array[ 100 ];
    for( int i = 0; i < 20; i++ ) {
        ptr_array[ i ] = PoolManager.alloc( 32 );
        UT_CHECK_OUTPUT( pool_32_ptr->getFreeBlockCount() == 19 - i );
    }
    UT_CHECK_OUTPUT( pool_64_ptr->getFreeBlockCount() == 10 );
    UT_CHECK_OUTPUT( pool_128_ptr->getFreeBlockCount() == 5 );

    for( int i = 0; i < 20; i++ ) {
        PoolManager.dealloc( ptr_array[ i ] );
        UT_CHECK_OUTPUT( pool_32_ptr->getFreeBlockCount() == i + 1 );
    }
    UT_CHECK_OUTPUT( pool_64_ptr->getFreeBlockCount() == 10 );
    UT_CHECK_OUTPUT( pool_128_ptr->getFreeBlockCount() == 5 );

    // Check allocation and deallocation from pool_128, other pools
    // should be unaffected.
    for( int i = 0; i < 5; i++ ) {
        ptr_array[ i ] = PoolManager.alloc( 128 );
        UT_CHECK_OUTPUT( pool_128_ptr->getFreeBlockCount() == 4 - i );
    }
    UT_CHECK_OUTPUT( pool_64_ptr->getFreeBlockCount() == 10 );
    UT_CHECK_OUTPUT( pool_32_ptr->getFreeBlockCount() == 20 );

    for( int i = 0; i < 5; i++ ) {
        PoolManager.dealloc( ptr_array[ i ] );
        UT_CHECK_OUTPUT( pool_128_ptr->getFreeBlockCount() == i + 1 );
    }
    UT_CHECK_OUTPUT( pool_64_ptr->getFreeBlockCount() == 10 );
    UT_CHECK_OUTPUT( pool_32_ptr->getFreeBlockCount() == 20 );

    PoolManager.clearAllPools();
    UT_CHECK_OUTPUT( PoolManager.getPoolCount() == 0 );

    UT_END_STEP;

/* ------------------------------
   TC step 5

   MemPoolManager: Check allocation and deallocation of unoptimized block sizes
   ------------------------------ */

    UT_START_STEP( 5 );

    MemPoolManager PoolManager;

    // Add Five pools of various sizes:
    MemoryPool* pool_32_ptr = new MemoryPool( 32, 20 );
    MemoryPool* pool_64_ptr = new MemoryPool( 64, 10 );
    MemoryPool* pool_128_ptr = new MemoryPool( 128, 5 );

    UT_CHECK_OUTPUT( PoolManager.addPool( pool_32_ptr ) == true );
    UT_CHECK_OUTPUT( PoolManager.addPool( pool_64_ptr ) == true );
    UT_CHECK_OUTPUT( PoolManager.addPool( pool_128_ptr ) == true );
    UT_CHECK_OUTPUT( PoolManager.getPoolCount() == 3 );

    void* ptr_array[ 4 ];

    // Allocations
    ptr_array[ 0 ] = PoolManager.alloc( 40 );
    UT_CHECK_OUTPUT( pool_32_ptr->getFreeBlockCount() == 20 );
    UT_CHECK_OUTPUT( pool_64_ptr->getFreeBlockCount() == 9 );
    UT_CHECK_OUTPUT( pool_128_ptr->getFreeBlockCount() == 5 );

    ptr_array[ 1 ] = PoolManager.alloc( 50 );
    UT_CHECK_OUTPUT( pool_32_ptr->getFreeBlockCount() == 20 );
    UT_CHECK_OUTPUT( pool_64_ptr->getFreeBlockCount() == 8 );
    UT_CHECK_OUTPUT( pool_128_ptr->getFreeBlockCount() == 5 );

    ptr_array[ 2 ] = PoolManager.alloc( 16 );
    UT_CHECK_OUTPUT( pool_32_ptr->getFreeBlockCount() == 19 );
    UT_CHECK_OUTPUT( pool_64_ptr->getFreeBlockCount() == 8 );
    UT_CHECK_OUTPUT( pool_128_ptr->getFreeBlockCount() == 5 );

    ptr_array[ 3 ] = PoolManager.alloc( 96 );
    UT_CHECK_OUTPUT( pool_32_ptr->getFreeBlockCount() == 19 );
    UT_CHECK_OUTPUT( pool_64_ptr->getFreeBlockCount() == 8 );
    UT_CHECK_OUTPUT( pool_128_ptr->getFreeBlockCount() == 4 );

    // Deallocations
    PoolManager.dealloc( ptr_array[ 2 ] );
    UT_CHECK_OUTPUT( pool_32_ptr->getFreeBlockCount() == 20 );
    UT_CHECK_OUTPUT( pool_64_ptr->getFreeBlockCount() == 8 );
    UT_CHECK_OUTPUT( pool_128_ptr->getFreeBlockCount() == 4 );

    PoolManager.dealloc( ptr_array[ 1 ] );
    UT_CHECK_OUTPUT( pool_32_ptr->getFreeBlockCount() == 20 );
    UT_CHECK_OUTPUT( pool_64_ptr->getFreeBlockCount() == 9 );
    UT_CHECK_OUTPUT( pool_128_ptr->getFreeBlockCount() == 4 );

    PoolManager.dealloc( ptr_array[ 3 ] );
    UT_CHECK_OUTPUT( pool_32_ptr->getFreeBlockCount() == 20 );
    UT_CHECK_OUTPUT( pool_64_ptr->getFreeBlockCount() == 9 );
    UT_CHECK_OUTPUT( pool_128_ptr->getFreeBlockCount() == 5 );

    PoolManager.dealloc( ptr_array[ 0 ] );
    UT_CHECK_OUTPUT( pool_32_ptr->getFreeBlockCount() == 20 );
    UT_CHECK_OUTPUT( pool_64_ptr->getFreeBlockCount() == 10 );
    UT_CHECK_OUTPUT( pool_128_ptr->getFreeBlockCount() == 5 );

    PoolManager.clearAllPools();
    UT_CHECK_OUTPUT( PoolManager.getPoolCount() == 0 );

    UT_END_STEP;

/* ------------------------------ */
    return;
}
