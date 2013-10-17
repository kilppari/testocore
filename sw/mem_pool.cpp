/******************************************************************************/
/**
    Memory pool implementation for Testocore 2D engine.
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
#include <stdlib.h>
#include <stdint.h>
//#define NDEBUG
#include <assert.h>


#include "mem_pool.h"

#ifndef NDEBUG
#define DPRINT( ... )                      \
    printf( __VA_ARGS__ );
#else /* #ifndef NDEBUG */
#define DPRINT( printf_params ) {}
#endif /* #ifndef NDEBUG */


/*----------------------------------------------------------------------------*/
/* Constructor, see mem_pool.h */
/*----------------------------------------------------------------------------*/
MemoryPool::MemoryPool(
    uint32_t pool_id, uint32_t block_size, uint32_t block_count )
    : m_PoolId( pool_id ),
      m_BlockSize( block_size ),
      m_BlockCount( block_count )
    {
    assert( block_size >= sizeof( void* ) &&
        "Error: Block size must be big enough to hold one pointer when the block is not used\n" );

    m_pPool = malloc( ( SIZE_MEM_BLOCK_HEADER + block_size ) * block_count );
    DPRINT( "allocated pool size: %lu\n", ( SIZE_MEM_BLOCK_HEADER + block_size ) * block_count )
    assert( m_pPool && "ERROR: could not allocate memory for the pool" );

    // Init Pointer to first free mem block
    m_pFreeMemBlock = ( MemBlockStr* )m_pPool;
    MemBlockStr* pBlock = m_pFreeMemBlock;
    printf( "pBlock: %p\n", pBlock );
    for( uint32_t i = 0; i < block_count; i++ )
        {
        pBlock->header = pool_id;
        pBlock->pData = NULL;
        //  Set data field to point to next free block
        if( i < block_count - 1 ) {
            pBlock->pData = ( void* )( ( uint8_t* )pBlock + SIZE_MEM_BLOCK_HEADER + block_size );
            }
        pBlock = ( MemBlockStr* )pBlock->pData;
        DPRINT( "pBlock: %p\n", pBlock )
        }
    }

MemoryPool::~MemoryPool( void )
    {
    if( m_pPool )
        {
        free( m_pPool );
        }
    }

void* MemoryPool::alloc( void )
    {
    MemBlockStr* pBlock = m_pFreeMemBlock;

    assert( pBlock != NULL && "Error: Out of memory. Memory pool is full.\n" );

    if( pBlock == NULL ) {
        return NULL;
        }

    // Update pFreeMemBlock to point to next free memory block, which is stored in the block's data segment
    m_pFreeMemBlock = ( MemBlockStr* )( pBlock->pData );

    pBlock->header = m_PoolId; // For now, put only pool id as header data

    return &( pBlock->pData );
    }

void MemoryPool::dealloc( void* ptr )
    {
    MemBlockStr* pBlock = ( MemBlockStr* )( ( uint8_t* )ptr - SIZE_MEM_BLOCK_HEADER );
    pBlock->pData = ( void* )m_pFreeMemBlock;
    m_pFreeMemBlock = pBlock;
    }
