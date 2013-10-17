#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
//#define NDEBUG
#include <assert.h>
#include <time.h>

#include "mem_pool.h"
#include "ut.h"

#define BLOCK_COUNT 8



int main( void )
    {
    void* pool_ptr = NULL;
    MemoryPool MemPool( 1, 1000, BLOCK_COUNT );

    pool_ptr = MemPool.getPoolPtr();
    printf( "Pool ptr = %p\n", pool_ptr );

    struct timespec ts;
    clock_gettime( CLOCK_MONOTONIC,&ts );

    void* temp_ptr[ 1000 ];
    for( int i = 0; i < BLOCK_COUNT; i++ )
        {
        temp_ptr[ i ] = MemPool.alloc();
        }
    for( int i = 0; i < BLOCK_COUNT; i++ )
        {
        MemPool.dealloc( temp_ptr[ i ] );
        }

    struct timespec ts2;
    clock_gettime( CLOCK_MONOTONIC,&ts2 );
    printf( "allocation time %luus\n", ( ts2.tv_nsec - ts.tv_nsec ) / 1000 );

    clock_gettime( CLOCK_MONOTONIC,&ts );
    for( int i = 0; i < BLOCK_COUNT; i++ )
        {
        temp_ptr[ i ] = malloc( 1000 );
        }
    for( int i = 0; i < BLOCK_COUNT; i++ )
        {
        free( temp_ptr[ i ] );
        }

    clock_gettime( CLOCK_MONOTONIC,&ts2 );
    printf( "allocation time %luus\n", ( ts2.tv_nsec - ts.tv_nsec ) / 1000 );


    return 0;
    }
