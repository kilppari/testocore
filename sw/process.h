/******************************************************************************/
/**
    
    Copyright (C) 2013 Pekka Mäkinen
    makinpek [ at ] gmail

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
*/
/******************************************************************************/

class Process {
private:
    // Disable copy cosntructor and assignment operator
    Process( const Process& );
    void operator=( const Process& );

protected:
    // Type of the process
    unsigned m_Type;

    // Status flags:
    bool m_IsDead;
    bool m_IsPaused;
    bool m_IsActive;
    bool m_IsAttached;
    bool m_InitRequired;

    // Pointer to next process
    Process* m_pNext;

    // Initializes the process
    virtual void onInitialize( void ) {}

public:
    Process( unsigned type );

    // Overloadable methods
    virtual ~Process() {}

    // Updates the process (see default inline further below)
    virtual void onUpdate( const uint32_t delta_millis );

    // Kills the process
    virtual void kill( void ) { m_IsDead = true; }

    // Sets pointer to next process (see default inline further below)
    virtual void setNext( Process* ptr );

    // Getters and setters
    unsigned getType( void ) const { return m_Type; }
    void setType( const unsigned type ) { m_Type = type; }

    bool isActive( void ) const { return m_IsActive; }
    void setActive( const bool b ) { m_IsActive = b; }

    bool isAttached( void ) const { return m_IsAttached; }
    void setAttached( const bool b ) { m_IsAttached = b; }

    bool isDead( void ) const { return m_IsDead; }

    void togglePause( void ) { m_IsPaused = !m_IsPaused; }
    bool isPaused( void ) const { return m_IsPaused; }

    bool isInitialized( void ) const { return !m_InitRequired; }

    Process* getNext( void ) const { return m_pNext; }

    bool initRequired( void ) const { return m_InitRequired; }
};

// Default constructor
inline Process::Process( unsigned type ) :
    m_Type( type ), m_IsDead( false ), m_IsPaused( false ),
    m_IsActive( true ), m_IsAttached( false ),
    m_InitRequired( false ), m_pNext( NULL )
    {}


// Default onUpdate implementation which just calls onInitialize()
inline void Process::onUpdate( const uint32_t delta_millis ) {
    if( m_InitRequired ) {
        onInitialize();
        m_InitRequired = false;
    }
}

// Sets pointer to the next process.
inline void Process::setNext( Process* ptr ) {
    if( ptr != NULL ) {
        ptr->setAttached( true );
    }
    m_pNext = ptr;
}
