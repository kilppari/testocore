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
#ifndef MEM_POOL_H
#define MEM_POOL_H

/**
 * Structure for a memory block.
 *
 *   header   Additional header data. E.g., to hold pool id, version
 *            and such information.
 *   pData    Payload of the block. When block is not in use, this
 *            field is used to point to the next available free
 *            block. Hence, the reserved payload size must always be
 *            greater or equal to a pointer size.
 *
 *   Total size of one memory block: header size + block size
 */
struct MemBlockStr {
    uint32_t header;
    void* pData;
    };
#define SIZE_MEM_BLOCK_HEADER sizeof( uint32_t )

/**
 * Simple and fast memory pool with fixed size blocks.
 * Uses malloc to allocate one chunk of memory which is then divided into
 * smaller blocks to be used by the client.
 */
class MemoryPool {
    private:
    // Pointer to pool's address space (the address returned by malloc)
    void*               m_pPool;
    // Pointer to next free memory block in the pool
    MemBlockStr*        m_pFreeMemBlock;
    // Pool's id for distinguishing multiple pools
    uint32_t            m_PoolId;
    // Size of one memory block (in bytes)
    uint32_t            m_BlockSize;
    // Number of memory block in the pool
    uint32_t            m_BlockCount;

    public:

    /**
     * Constructor.
     * Allocates memory and initializes the blocks. Sets pool id.
     */
    MemoryPool( uint32_t pool_index, uint32_t block_size, uint32_t block_count );
    // Destructor, frees the allocated memory.
    ~MemoryPool( void );

    // Getters and setters:
    void* getPoolPtr( void ) { return m_pPool; }

    // Returns one free block from the pool. If no free blocks, returns NULL.
    void* alloc( void );
    // Sets the given block back into the pool as a free block.
    void  dealloc( void* ptr );
    };
#endif /* #ifndef MEM_POOL_H */
