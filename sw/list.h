/******************************************************************************/
/**
    Linked list implementation for Testocore engine.
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
#ifndef LIST_H_
#define LIST_H_

template <class T> class List; // Forward declaration to be used by Node below

/**
 * Node implementation for a doubly linked list.
 */
template <class T>
class Node {
    // Let List access Node's private data
    friend class List< T >;
private:
    Node* m_pNext; // Link to next node.
    Node* m_pPrev; // Link to previous node.
    T     m_Item;  // Payload

public:
    // Ensure links are init to NULL
    Node() : m_pNext( NULL ), m_pPrev( NULL ) {}

    // Accessors to next and previous nodes
    Node* next() { return m_pNext; }
    Node* prev() { return m_pPrev; }

    // These both returns reference to node's payload object
    T item() { return m_Item; }
    T& operator*() { return m_Item; }
};

/**
 * Simple doubly linked list. Items can be inserted either
 * through the head of the list or the end of the list.
 * Random access not possible.
 */
template <class T>
class List {

private:
    Node< T >* m_pHead; // First node in the list.
    Node< T >* m_pTail; // Last node in the list.
    // Whether to use memory pool or normal new -operator for node allocations.
    const int m_AllocMethod;

    // Utility for deallocating nodes
    void deleteNode( Node< T >* node_ptr ) {
        if( m_AllocMethod == USE_MEM_POOL_ALLOCATION ) {
            POOL_DEALLOC( node_ptr );
        }else{ delete node_ptr; }
    }

public:
    enum AllocTypeEnum {
        USE_MEM_POOL_ALLOCATION, USE_STANDARD_ALLOCATION };

    List() : m_pHead( NULL ), m_pTail( NULL ),
        m_AllocMethod( USE_STANDARD_ALLOCATION ) {}

    List( const AllocTypeEnum alloc_type ) :
        m_pHead( NULL ), m_pTail( NULL ), m_AllocMethod( alloc_type ) {}

    ~List() { clear(); }

    // Inserts new object at the front of the list (allocates new node).
    bool pushFront( T obj );

    // Inserts new object at the end of the list (allocates new node).
    bool pushBack( T obj );

    // Removes and deallocates all nodes from the list
    // (does not deallocate actual contents)
    void clear( void );

    // Returns the number of elements in the list.
    uint32_t size( void );

    // Returns pointer to the first node in the list.
    Node< T >* begin( void ) { return m_pHead; };

    // Returns pointer to the last node in the list.
    Node< T >* end( void ) { return m_pTail; }

    // Removes the given node from the list and updates links.
    void remove( Node< T >* node_ptr );
};

#endif /* #ifndef LIST_H_ */
