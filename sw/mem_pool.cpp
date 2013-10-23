/******************************************************************************/
/**
    Memory pool implementation for Testocore utilities.
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
/* class MemoryPool see mem_pool.h */
/*----------------------------------------------------------------------------*/

MemoryPool::MemoryPool( uint32_t block_size, uint32_t block_count )
    : m_BlockSize( block_size ),
      m_BlockCount( block_count ) {

    assert( block_size >= sizeof( void* ) &&
        "Error: Block size must be big enough to hold one pointer when the block is not used\n" );

    m_pPool = malloc( ( SIZE_MEM_BLOCK_HEADER + block_size ) * block_count );
//    DPRINT( "allocated pool size: %lu\n", ( SIZE_MEM_BLOCK_HEADER + block_size ) * block_count )
    assert( m_pPool && "ERROR: could not allocate memory for the pool" );

    // Init Pointer to first free mem block
    m_pFreeMemBlock = ( MemBlockStr* )m_pPool;
    MemBlockStr* pBlock = m_pFreeMemBlock;
//    printf( "pBlock: %p\n", pBlock );
    for( uint32_t i = 0; i < block_count; i++ ) {
        pBlock->header = 0;
        pBlock->pData = NULL;
        //  Set data field to point to next free block
        if( i < block_count - 1 ) {
            pBlock->pData = ( void* )( ( uint8_t* )pBlock + SIZE_MEM_BLOCK_HEADER + block_size );
            }
        pBlock = ( MemBlockStr* )pBlock->pData;
//        DPRINT( "pBlock: %p\n", pBlock )
    }
}

MemoryPool::~MemoryPool( void )
    {
    if( m_pPool )
        {
        free( m_pPool );
        }
    }

/**
 * Allocates a fixed size block from the pool and returns
 * the address to the data section of the block.
 * If no free blocks left, returns NULL.
 */
void* MemoryPool::alloc( void )
    {
    MemBlockStr* pBlock = m_pFreeMemBlock;

    assert( pBlock != NULL && "Error: Out of memory. Memory pool is full.\n" );

    if( pBlock == NULL ) {
        return NULL;
        }

    // Update pFreeMemBlock to point to next free memory block,
    // which is stored in the block's data segment
    m_pFreeMemBlock = ( MemBlockStr* )( pBlock->pData );

    // put pool id as header data and set the MSB to indicate that
    // this blocks is reserved.
    pBlock->header = ( uint32_t )m_PoolId;
    pBlock->header |= ( 1 << 31 );
    return &( pBlock->pData );
    }

/**
 * Frees one block in the pool and adds it to the head of the free block list.
 */
void MemoryPool::dealloc( void* ptr )
    {
    MemBlockStr* pBlock = ( MemBlockStr* )(
        ( uint8_t* )ptr - SIZE_MEM_BLOCK_HEADER );
    pBlock->pData = ( void* )m_pFreeMemBlock;
    pBlock->header &= ~( 1 << 31 );
    m_pFreeMemBlock = pBlock;
    }

/* Calculates the number of free memory blocks */
uint32_t MemoryPool::getFreeBlockCount( void ) {

    uint32_t count = 0;

    if( m_pFreeMemBlock == NULL ) {
       return count;
    }

    MemBlockStr* pBlock = m_pFreeMemBlock;
    while( pBlock != NULL ) {
    if( !( pBlock->header & ( 1 << 31 ) ) ) {
        count++;
    }
        pBlock = ( MemBlockStr* )pBlock->pData;
    }
    return count;
}

/*----------------------------------------------------------------------------*/
/* class MemPoolManager see mem_pool.h */
/*----------------------------------------------------------------------------*/

MemPoolManager::MemPoolManager():  m_PoolIdCounter( 0 ){
    m_PoolList.pHead = NULL;
    m_PoolList.pTail = NULL;
}

MemPoolManager::~MemPoolManager() {
    clearAllPools();
}

/**
 * Inserts a new node into the linked list of memory pools.
 */
void MemPoolManager::insertPoolNode( MemPoolNodeStr* node_ptr ) {
    if( node_ptr == NULL ) {
        return;
    }
    if( m_PoolList.pTail == NULL ) {
        m_PoolList.pHead = node_ptr;
        m_PoolList.pTail = node_ptr;
        node_ptr->pNext = NULL;
        node_ptr->pPrev = NULL;
    }
    else {
        MemPoolNodeStr* temp_node_ptr = m_PoolList.pHead;
        while( temp_node_ptr != NULL ) {
            if( temp_node_ptr->pPool->getBlockSize() >
                node_ptr->pPool->getBlockSize() ) {
                break;
            }
            temp_node_ptr = temp_node_ptr->pNext;
        }
        if( temp_node_ptr != NULL ) {
            // Set new node's 'prev' and 'next'
            node_ptr->pPrev = temp_node_ptr->pPrev;
            node_ptr->pNext = temp_node_ptr;
            // Update the old nodes in the list
            if( temp_node_ptr->pPrev != NULL ) {
                temp_node_ptr->pPrev->pNext = node_ptr;
            }
            temp_node_ptr->pPrev = node_ptr;
        }
        // Add to the end of the list
        else {
            node_ptr->pPrev = m_PoolList.pTail;
            node_ptr->pNext = NULL;
            m_PoolList.pTail->pNext = node_ptr;
            m_PoolList.pTail = node_ptr;
        }
    }
}

/**
 * Removes a node from the linked list of memory pools.
 */
void MemPoolManager::removePoolNode( MemPoolNodeStr* node_ptr ) {
    if( node_ptr == NULL ) {
        return;
    }
    node_ptr->pPool = NULL;

    // Update list head and tail if needed
    if( node_ptr == m_PoolList.pHead &&
        node_ptr == m_PoolList.pTail ) {
        m_PoolList.pHead = NULL;
        m_PoolList.pTail = NULL;
    }
    else if( node_ptr == m_PoolList.pHead ) {
        m_PoolList.pHead = node_ptr->pNext;
    }
    else if( node_ptr == m_PoolList.pTail ) {
        m_PoolList.pTail = node_ptr->pPrev;
    }

    // Update adjacent nodes
    if( node_ptr->pPrev != NULL ) {
        node_ptr->pPrev->pNext = node_ptr->pNext;
    }
    if( node_ptr->pNext != NULL ) {
        node_ptr->pNext->pPrev = node_ptr->pPrev;
    }
}

/**
 * Adds a new pool to the list of memory pools and assigns id to it.
 */
bool MemPoolManager::addPool( MemoryPool* pool_ptr ) {
    // Create new node for the pool list
    MemPoolNodeStr* node_ptr = new MemPoolNodeStr;
    if( node_ptr == NULL ) {
        return false; // Could not allocate memory for node
    }
    // Add it to the end of the list and set id
    node_ptr->pPool = pool_ptr;
    insertPoolNode( node_ptr );
    if( node_ptr->pPrev != NULL ) {
        pool_ptr->setPoolId( m_PoolIdCounter++ );
    }
    else {
        m_PoolIdCounter = 0;
        pool_ptr->setPoolId( m_PoolIdCounter++ );
    }
    return true;
}

/**
 * Removes a pool from the list based on id and destroys the pool.
 * Deletes both the node and the pool.
 */
bool MemPoolManager::removePool( uint16_t id ) {
    // Find the correct pool from the list
    MemPoolNodeStr* node_ptr = m_PoolList.pHead;
    while( node_ptr != NULL ) {
        if( node_ptr->pPool->getPoolId() == id ) {
            // Destroy the pool
            delete node_ptr->pPool;
            node_ptr->pPool = NULL;
            // Update pool list and delete node
            removePoolNode( node_ptr );
            delete node_ptr;

            return true;
        }
        node_ptr = node_ptr->pNext;
    }
    return false;
}

/**
 * Removes a pool from the list based on block size and destroys the pool.
 * Deletes both the node and the pool.
 */
bool MemPoolManager::removePoolByBlockSize( uint32_t size ) {
    // Find the correct pool from the list
    MemPoolNodeStr* node_ptr = m_PoolList.pHead;
    while( node_ptr != NULL ) {
        if( node_ptr->pPool->getBlockSize() == size ) {
            // Destroy the pool
            delete node_ptr->pPool;
            node_ptr->pPool = NULL;
            // Update pool list and delete node
            removePoolNode( node_ptr );
            delete node_ptr;

            return true;
        }
        node_ptr = node_ptr->pNext;
    }
    return false;
}

/**
 * Calculates and returns the number of pools in the list.
 */
uint16_t MemPoolManager::getPoolCount( void ) {
    uint16_t count = 0;
    MemPoolNodeStr* node_ptr = m_PoolList.pHead;
    while( node_ptr != NULL ) {
        count++;
        node_ptr = node_ptr->pNext;
    }
    return count;
}

/**
 * Removes all pools from the list.
 * All pools and the list nodes are deallocated.
 */
void MemPoolManager::clearAllPools( void ) {
    // Find the correct pool from the list
    MemPoolNodeStr* node_ptr = m_PoolList.pHead;
    while( node_ptr != NULL ) {
        // Destroy the pool
        delete node_ptr->pPool;
        node_ptr->pPool = NULL;
        // Update pool list and delete node
        MemPoolNodeStr* next_node_ptr = node_ptr->pNext;
        removePoolNode( node_ptr );
        delete node_ptr;

        node_ptr = next_node_ptr;
    }
}

/**
 * Finds the most suitable pool for allocating given amount of bytes.
 * Pools are ordered in the list based on increasing block sizes and
 * search is started from the pool with smallest block size.
 */
void* MemPoolManager::alloc( uint32_t bytes ) {
    // Find the first pool which block size is large enough
    MemPoolNodeStr* node_ptr = m_PoolList.pHead;
    while( node_ptr != NULL &&
           node_ptr->pPool->getBlockSize() < bytes ) {
        node_ptr = node_ptr->pNext;
    }
    // Try to allocate block from the pool
    void* ret_ptr = NULL;
    while( node_ptr != NULL &&
        ( ret_ptr = node_ptr->pPool->alloc() ) == NULL ) {
        node_ptr = node_ptr->pNext; // Pool is full, jump to next pool (larger one)
    }
    // Return valid pointer the allocation was successfull, otherwise NULL
    return ret_ptr;
}

/**
 * Deallocates the given address. Casts the address into MemBlockStr
 * and selects the correct pool based on the id in the block header.
 */
void MemPoolManager::dealloc( void* ptr ) {
    // Parse pool id from the memory block
    MemBlockStr* block_ptr = ( MemBlockStr* )( ( uint8_t* )ptr - SIZE_MEM_BLOCK_HEADER );
    uint16_t pool_id = block_ptr->header & 0xFFFF;

    // Find the correct pool and call dealloc
    MemPoolNodeStr* node_ptr = m_PoolList.pHead;
    while( node_ptr != NULL ) {
        if( node_ptr->pPool->getPoolId() == pool_id ) {
            node_ptr->pPool->dealloc( ptr );
            return; // Function exists here on success
        }
        node_ptr = node_ptr->pNext;
    }
    //TODO: Failed deallocation
}
