#ifndef AREG_BASE_TELINKEDLIST_HPP
#define AREG_BASE_TELINKEDLIST_HPP
/************************************************************************
 * \file        areg/src/base/TELinkedList.hpp
 * \ingroup     AREG SDK, Asynchronous Event Generator Software Development Kit 
 * \author      Artak Avetyan (mailto:artak@aregtech.com)
 * \brief       AREG Platform, Linked List class template.
 *              The linked list contains list of values that can be
 *              accessed either by Position or by valid index.
 *              The size of Linked List is dynamic, Elements contain
 *              information of previous and next Block.
 *              Accessing element, insert and remove by Position
 *              operations are proceed fast. Accessing by index
 *              is slower.
 *
 ************************************************************************/
/************************************************************************
 * Include files.
 ************************************************************************/
#include "areg/src/base/GEGlobal.h"
#include "areg/src/base/ETemplateBase.hpp"
#include "areg/src/base/IEIOStream.hpp"

//////////////////////////////////////////////////////////////////////////
// TELinkedList<VALUE, VALUE_TYPE, Implement> class template declaration
//////////////////////////////////////////////////////////////////////////
/**
 * \brief       Linked List class template declaration. Derives private 
 *              class CETemplateConstants to access defined constants.
 *              Accessing, Insert and Remove by Position operations
 *              are fast. Searching element and accessing by index
 *              are slower.
 *
 * \details     Blocks in Linked List are linked bi-directional, i.e.
 *              every Block contains information of next and previous
 *              Block. Linked List does not guaranty unique values,
 *              but it gives possibility to search element after
 *              certain Position.  The type VALUE in Linked List 
 *              should have at least default constructor and
 *              valid assigning operator. By default, VALUE_TYPE
 *              and VALUE are equal.
 *              The LinkedList object is not thread safe and data access 
 *              should be  synchronized manually.
 *
 * \tparam  VALUE       the type of stored items. Either should be 
 *                      primitive or should have default constructor 
 *                      and valid assigning operator. Also, should be 
 *                      possible to convert to type VALUE_TYPE.
 * \tparam  VALUE_TYPE  By default same as VALUE, but can be any other
 *                      type, which is converted from type VALUE.
 **/
template <typename VALUE, typename VALUE_TYPE = VALUE, class Implement = TEListImpl<VALUE_TYPE>> 
class TELinkedList  : protected Implement
                    , private   CETemplateConstants
{
//////////////////////////////////////////////////////////////////////////
// Internal objects and types declaration
//////////////////////////////////////////////////////////////////////////
protected:
    /**
     * \brief   Hidden / Protected class declaration.
     *          The Block class contains information
     *          of element in linked list -- value, pointers to next and previous
     *          Block objects.
     **/
    //////////////////////////////////////////////////////////////////////////
    // TELinkedList<VALUE, VALUE_TYPE, Implement>::CEBlock class declaration
    //////////////////////////////////////////////////////////////////////////
    class CEBlock
    {
    //////////////////////////////////////////////////////////////////////////
    // constructor / destructor
    //////////////////////////////////////////////////////////////////////////
    public:
        /**
         * \brief   Default constructor
         **/
        CEBlock( void );
        /**
         * \brief   Initialization constructor.
         **/
        CEBlock(VALUE_TYPE value);
        /**
         * \brief   Destructor.
         **/
        ~CEBlock( void );
    //////////////////////////////////////////////////////////////////////////
    // member variables
    //////////////////////////////////////////////////////////////////////////
    public:
        /**
         * \brief   Pointer to next element Block. The next element of 'tail' is always NULL
         **/
        CEBlock*    mNext;
        /**
         * \brief   Pointer to previous element Block object. The previous element of 'head' is always NULL
         **/
        CEBlock*    mPrev;
        /**
         * \brief   Container of value.
         **/
        VALUE       mValue;
    };

//////////////////////////////////////////////////////////////////////////
// Constructor / Destructor
//////////////////////////////////////////////////////////////////////////
public:
    /**
     * \brief   Default constructor. Creates empty linked list
     **/
    TELinkedList( void );
    
    /**
     * \brief   Copy constructor.
     * \param   src     The source to copy data.
     **/
    TELinkedList( const TELinkedList<VALUE, VALUE_TYPE, Implement> & src );
    
    /**
     * \brief   Destructor
     **/
    ~TELinkedList( void );

//////////////////////////////////////////////////////////////////////////
// Operators
//////////////////////////////////////////////////////////////////////////
public:
/************************************************************************/
// Basic operators
/************************************************************************/

    /**
     * \brief	Assigning operator, assigns elements of src to the Linked List
     * \param	src	Source Linked List to get elements.
     **/
    const TELinkedList<VALUE, VALUE_TYPE, Implement> & operator = ( const TELinkedList<VALUE, VALUE_TYPE, Implement> & src );

    /**
     * \brief   Checks equality of 2 linked-list objects, and returns true if they are equal.
     *          There should be possible to compare VALUE types entries of linked list.
     * \param   other   The linked-list object to compare
     **/
    bool operator == ( const TELinkedList<VALUE, VALUE_TYPE, Implement> & other ) const;

    /**
     * \brief   Checks inequality of 2 linked-list objects, and returns true if they are not equal.
     *          There should be possible to compare VALUE types entries of linked list.
     * \param   other   The linked-list object to compare
     **/
    bool operator != ( const TELinkedList<VALUE, VALUE_TYPE, Implement> & other ) const;

    /**
     * \brief   Subscript operator. Returns reference to value of element 
     *          by given valid index. The index should be valid, otherwise 
     *          assertion raised. May be used on either the right (r-value)
     *          or the left (l-value) of an assignment statement.
     * \param   atIndex     The valid zero-based index.
     * \return  Returns Value of Element at given index, the value can be modified
     **/
    inline VALUE & operator [] ( int atIndex );
    /**
     * \brief   Subscript operator. Returns reference to value of element 
     *          by given valid Position. May be used on either the right 
     *          (r-value) or the left (l-value) of an assignment statement.
     * \param   atPosition  The valid Linked List position.
     * \return  Returns Value of Element at given Linked List position, the value can be modified
     **/
    inline VALUE & operator [] ( LISTPOS atPosition );

    /**
     * \brief   Subscript operator. Returns value of element by given 
     *          valid index. The index should be valid, otherwise 
     *          assertion raised. May be used only on the right.
     * \param   atIndex     The valid zero-based index.
     * \return  Returns Value of Element at given index, the value should not be modified
     **/
    inline VALUE_TYPE operator [] ( int atIndex ) const;
    /**
     * \brief   Subscript operator. Returns value of element by given 
     *          valid Position. May be used only on the right.
     * \param   atPosition  The valid Linked List position.
     * \return  Returns Value of Element at given Linked List position, the value can be modified
     **/
    inline VALUE_TYPE operator [] ( const LISTPOS atPosition ) const;

/************************************************************************/
// Friend global operators to make Linked List streamable
/************************************************************************/

    /**
     * \brief   Reads out from the stream Linked List values.
     *          If Linked List previously had values, they will be lost.
     *          The values in the Linked List will be initialized in the same sequence
     *          as they were written.
     *          There should be possibility to initialize values from streaming object and
     *          if VALUE is not a primitive, but an object, it should have implemented streaming operator.
     * \param   stream  The streaming object for reading values
     * \param   input   The Linked List object to save initialized values.
     **/
    template<typename V, typename VT, class Impl>
    friend const IEInStream & operator >> ( const IEInStream & stream, TELinkedList<V, VT, Impl> & input );
    /**
     * \brief   Writes to the stream Linked List values.
     *          The values will be written to the stream starting from head position.
     *          There should be possibility to stream every value of Linked List and if VALUE 
     *          is not a primitive, but an object, it should have implemented streaming operator.
     * \param   stream  The streaming object to write values
     * \param   input   The Linked List object to read out values.
     **/
    template<typename V, typename VT, class Impl>
    friend IEOutStream & operator << ( IEOutStream & stream, const TELinkedList<V, VT, Impl> & output );

//////////////////////////////////////////////////////////////////////////
// Operations
//////////////////////////////////////////////////////////////////////////
public:
/************************************************************************/
// Attributes
/************************************************************************/

    /**
     * \brief   Returns true, if Linked List container is empty.
     **/
    inline bool IsEmpty( void ) const;

    /**
     * \brief   Returns the size of Linked List container
     **/
    inline int GetSize( void ) const;

    /**
     * \brief	Checks whether given position is valid or not
     * \param	posCheck	Position to check
     * \return	Returns true, if position is valid within Linked List.
     **/
    inline bool IsPositionValid( const LISTPOS posCheck) const;

/************************************************************************/
// Operations
/************************************************************************/

    /**
     * \brief   Returns position of head element in Linked List container.
     *          If Linked List is empty, function returns NULL (INVALID_POSITION)
     **/
    inline LISTPOS GetHeadPosition( void ) const;

    /**
     * \brief   Returns position of tail element in Linked List container
     *          If Linked List is empty, function returns NULL (INVALID_POSITION)
     **/
    inline LISTPOS GetTailPosition( void ) const;

    /**
     * \brief   Returns value of head element in Linked List container.
     *          On call the Linked List should not be empty, otherwise assertion is raised.
     **/
    inline VALUE_TYPE GetHead( void ) const;

    /**
     * \brief   Returns value of tail element in Linked List container.
     *          On call the Linked List should not be empty, otherwise assertion is raised.
     **/
    inline VALUE_TYPE GetTail( void ) const;

    /**
     * \brief	Returns given position value and on exit position of next element in Linked List container.
     *          On input, the value of 'in_out_NextPosition' should be valid (for example, should be equal to head position).
     *          Otherwise assertion is raised.
     * \param	in_out_NextPosition On input, this should be valid position of element in Linked List container.
     *                              On output, this contains position of next element in Linked List container
     *                              or NULL (INVALID_POSITION) if passed position on input is position of tail element
     * \return	Returns value of element at the given position.
     **/
    inline VALUE_TYPE GetNext( LISTPOS & in_out_NextPosition ) const;

    /**
     * \brief	Returns position of next element in Linked List
     * \param	atPosition	Actual position to get the next position
     * \return	If element at given position has next element, returns position of next element in Linked List.
     *          Otherwise it returns NULL
     **/
    inline LISTPOS GetNextPosition( LISTPOS atPosition ) const;

    /**
     * \brief	Returns given position value and on exit position of previous element in Linked List container.
     *          On input, the value of 'in_out_PrevPosition' should be valid (for example, should be equal to tail position).
     *          Otherwise assertion is raised.
     * \param	in_out_PrevPosition On input, this should be valid position of element in Linked List container.
     *                              On output, this contains position of previous element in Linked List container
     *                              or NULL (INVALID_POSITION) if passed position on input is position of head element
     * \return	Returns value of element at the given position.
     **/
    inline VALUE_TYPE GetPrev( LISTPOS & in_out_PrevPosition ) const;

    /**
     * \brief	Returns position of previous element in Linked List
     * \param	atPosition	Actual position to get the previous position
     * \return	If element at given position has previous element, returns position of previous element in Linked List.
     *          Otherwise it returns NULL
     **/
    inline LISTPOS GetPrevPosition( LISTPOS atPosition ) const;

    /**
     * \brief   Returns value of element at the give position.
     *          On input, the value of atPosition should be valid. 
     *          Returned value should not be modified.
     * \param   atPosition  The valid position in Linked List
     * \return  Returns Value at given position. 
     *          The returned value should not be modified
     **/
    inline VALUE_TYPE GetAt( LISTPOS atPosition ) const;
    /**
     * \brief   Returns value of element at the give position.
     *          On input, the value of in_pos should be valid. 
     *          Otherwise assertion is raised.
     *          Returned value can be modified.
     * \param   atPosition  The valid position in Linked List
     * \return  Returns Value at given position. 
     *          The returned value can be modified
     **/
    inline VALUE & GetAt( LISTPOS atPosition );

    /**
     * \brief	Returns the instance of element by give index. The index of head element is 0.
     *          The index must not be more than GetSize() - 1.
     * \param	index	The index of element to get value
     * \return	The instance of element by give index.
     **/
    VALUE & GetAt( int index );

    /**
     * \brief	Returns the value of element by give index. The index of head element is 0.
     *          The index must not be more than GetSize() - 1.
     * \param	index	The index of element to get value
     * \return	The value of element by give index.
     **/
    VALUE_TYPE GetAt( int index ) const;

    /**
     * \brief	On exit, returns position and value of next element in Linked List container.
     *          On input, the value of 'in_out_NextPosition' should be valid (for example, should be equal to head position).
     *          Otherwise assertion is raised.
     * \param	in_out_NextPosition	On input, this should be valid position of element in Linked List container.
     *                              On output, this contains position of next element in Linked List container 
     *                              or NULL (INVALID_POSITION) if passed position on input is position of tail element
     * \param   out_NextValue       If Linked List has next element, this contains value of next element.
     *                              Otherwise, the value is unchanged. Check function return value before using parameter
     * \return	Returns true if Linked List has next element. Before using out_value function return value
     *          should be validated.
     **/
    bool GetNextElem( LISTPOS & in_out_NextPosition, VALUE & out_NextValue ) const;

    /**
     * \brief	On exit, returns position and value of previous element in Linked List container.
     *          On input, the value of 'in_out_PrevPosition' should be valid (for example, should be equal to tail position).
     *          Otherwise assertion is raised.
     * \param	in_out_PrevPosition	On input, this should be valid position of element in Linked List container.
     *                              On output, this contains position of previous element in Linked List container
     *                              or NULL (INVALID_POSITION) if passed position on input is position of head element
     * \param   out_PrevValue       If Linked List has previous element, this contains value of previous element.
     *                              Otherwise, the value is unchanged. Check function return value before using parameter
     *                              value of this parameter.
     * \return	Returns true if Linked List has previous element. Before using out_value function return value
     *          should be validated.
     **/
    bool GetPrevElem( LISTPOS & in_out_PrevPosition, VALUE & out_PrevValue ) const;

    /**
     * \brief   Removes head element from Linked List and returns value or removed element.
     *          On call the Linked List should not be empty. Otherwise assertion is raised.
     * \return  Returns value of removed element.
     **/
    VALUE RemoveHead( void );

    /**
     * \brief   Removes tail element from Linked List and returns value or removed element.
     *          On call the Linked List should not be empty. Otherwise assertion is raised.
     * \return  Returns value of removed element.
     **/
    VALUE RemoveTail( void );

    /**
     * \brief   Add element to head of Linked List and returns position new head element.
     * \param   newElemnt   New head element to add in Linked List
     * \return  Returns position new head element in Linked List.
     **/
    LISTPOS AddHead( VALUE_TYPE newElemnt );

    /**
     * \brief   Add element to tail of Linked List and returns position new tail element.
     * \param   newElemnt   New tail element to add in Linked List
     * \return  Returns position new tail element in Linked List.
     **/
    LISTPOS AddTail( VALUE_TYPE newElement );

    /**
     * \brief	Inserts new element before given position. 
     *          If given position is head element, it will add new head element
     * \param	beforePosition  The Linked List element position before new element should be inserted
     * \param	newElement	    Value of new element to insert
     * \return	Returns position of new inserted element.
     **/
    LISTPOS InsertBefore( LISTPOS beforePosition, VALUE_TYPE newElement );

    /**
     * \brief	Inserts new element after given position. 
     *          If given position is tail element, it will add new tail element
     * \param	afterPosition   The Linked List element position after new element should be inserted
     * \param	newElement	    Value of new element to insert
     * \return	Returns position of new inserted element.
     **/
    LISTPOS InsertAfter(LISTPOS afterPosition, VALUE_TYPE newElement);

    /**
     * \brief	Sets new value at give position and returns old value of same element.
     * \param	atPosition	The Linked List element position to change value
     * \param	newValue	New Value to change
     * \return	Returns old value of element at given position
     **/
    inline VALUE SetAt( LISTPOS atPosition, VALUE_TYPE newValue );

    /**
     * \brief   Removes all element in Linked List. After call, Linked List is empty.
     **/
    void RemoveAll( void );


    /**
     * \brief	Removes element at given position and returns value of removed element.
     * \param	atPosition  Position of element to remove from Linked List. 
     *                      It should be valid position, otherwise assertion is raised.
     * \return	Returns value of removed Element.
     **/
    VALUE RemoveAt( LISTPOS atPosition );

    /**
     * \brief	Removes element at given position and on output returns value of removed element.
     * \param	atPosition  On input, position of element to remove from Linked List.
     *                      It should be valid position, otherwise assertion is raised.
     * \param   out_Value   On output, it contains value of removed element
     * \return	Returns true if element was removed.
     **/
    bool RemoveAt( LISTPOS atPosition, VALUE & out_Value );

    /**
     * \brief	Searches and removes first match of given element from Linked List.
     *          If 'searchAfter' is NULL, it will start searching element from head.
     *          And returns true if element was found and successfully removed from Linked List.
     * \param	removeElement	Element to search and remove from Linked List
     * \param	searchAfter	    Position at which starts searching, the searching done by moving to next element
     * \return	Returns true if element was found and successfully removed from linked list.
     **/
    bool RemoveElem( VALUE_TYPE removeElement, LISTPOS searchAfter = NULL );

    /**
     * \brief	Searches position of element by given value. If searchAfter is valid, the searching will be started
     *          from given position and will move to next element.
     * \param	searchValue	Value of element to search
     * \param	searchAfter	If valid value, the position to start searching. 
     *                      Otherwise searching will be started from position head element
     * \return	Returns true if element was found in Linked List
     **/
    LISTPOS Find( VALUE_TYPE searchValue, LISTPOS searchAfter = NULL ) const;

    /**
     * \brief	Returns position of element by given index. The index of head element is 0.
     *          The index of last element (tail element) is GetSize() - 1.
     *          If invalid index is passed, function returns NULL
     * \param	index	The index of element in Linked List to get position
     * \return	Returns position of element if index is valid, i.e. more or equal to 0 and less than GetSize().
     *          Otherwise returns NULL (INVALID_POSITION)
     **/
    LISTPOS FindIndex( int index ) const;

    /**
     * \brief	Returns valid index of element by given valid position.
     *          Otherwise returns -1.
     * \param	atPosition  The position of element, which index should be returned.
     * \return	The index of element by given position
     **/
    int GetIndex( LISTPOS atPosition ) const;

    /**
     * \brief	Search element by given searchValue value starting at position searchAfter
     *          and returns valid index of found element. Otherwise it returns invalid 
     *          index -1. If searchAfter is NULL, it will start searching from Head position.
     * \param	searchValue	Value to search
     * \param	startAfter	Position to start searching. If NULL, it starts searching
     *                      from Head position.
     * \return	Returns valid index of found element.
     **/
    inline int GetIndex( VALUE_TYPE searchValue, LISTPOS startAfter /*= NULL */) const;

protected:

    /**
     * \brief   Called when comparing 2 values of element.
     *          Overwrite method when need to change comparison.
     * \param   value1  Value on left side to compare.
     * \param   value2  Value on right side to compare.
     * \return  If function returns true, 2 values are equal.
     *          Otherwise, they are not equal.
     **/
    inline bool EqualValues( VALUE_TYPE value1, VALUE_TYPE value2) const;

//////////////////////////////////////////////////////////////////////////
// Member Variables
//////////////////////////////////////////////////////////////////////////
protected:
    /**
     * \brief   The size of Linked List, contains amount of elements in Linked List.
     *          If Linked List is empty, this is zero.
     **/
    int     mCount;
    /**
     * \brief   The Head element (position) of Linked List.
     *          If Linked List is empty, this is NULL.
     *          If it has only one element, Head is equal to Tail position
     **/
    CEBlock * mHead;
    /**
     * \brief   The Tail element (position) of Linked List.
     *          If Linked List is empty, this is NULL
     *          If it has only one element, Tails is equal to Head position.
     **/
    CEBlock * mTail;
};

//////////////////////////////////////////////////////////////////////////
// Function implementation
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// TELinkedList<VALUE, VALUE_TYPE, Implement>::CEBlock class implementation
//////////////////////////////////////////////////////////////////////////
template <typename VALUE, typename VALUE_TYPE /* = VALUE */, class Implement /* = TEListImpl<VALUE_TYPE> */>
inline TELinkedList<VALUE, VALUE_TYPE, Implement>::CEBlock::CEBlock( void )
    : mNext (NULL)
    , mPrev (NULL)
    , mValue( )
{   ; }

template <typename VALUE, typename VALUE_TYPE /* = VALUE */, class Implement /* = TEListImpl<VALUE_TYPE> */>
inline TELinkedList<VALUE, VALUE_TYPE, Implement>::CEBlock::CEBlock(VALUE_TYPE value)
    : mNext (NULL)
    , mPrev (NULL)
    , mValue((VALUE)value)
{   ; }

template <typename VALUE, typename VALUE_TYPE /* = VALUE */, class Implement /* = TEListImpl<VALUE_TYPE> */>
inline TELinkedList<VALUE, VALUE_TYPE, Implement>::CEBlock::~CEBlock( void )
{   ; }


//////////////////////////////////////////////////////////////////////////
// TELinkedList<VALUE, VALUE_TYPE, Implement> class template implementation
//////////////////////////////////////////////////////////////////////////
template <typename VALUE, typename VALUE_TYPE /* = VALUE */, class Implement /* = TEListImpl<VALUE_TYPE> */> 
TELinkedList<VALUE, VALUE_TYPE, Implement>::TELinkedList( void )
    : Implement             ( )
    , CETemplateConstants   ( )
    
    , mCount    (0)
    , mHead     (NULL)
    , mTail     (NULL)
{
    ; // do nothing
}

template <typename VALUE, typename VALUE_TYPE /* = VALUE */, class Implement /* = TEListImpl<VALUE_TYPE> */>
TELinkedList<VALUE, VALUE_TYPE, Implement>::TELinkedList(const TELinkedList<VALUE, VALUE_TYPE, Implement> & src)
    : Implement             ( )
    , CETemplateConstants   ( )

    , mCount(0)
    , mHead (NULL)
    , mTail (NULL)
{
    TELinkedList<VALUE, VALUE_TYPE, Implement>::CEBlock * block = src.mHead;
    for ( ; block != NULL; block = block->mNext)
    {
        AddTail(block->mValue);
    }
}

template <typename VALUE, typename VALUE_TYPE /* = VALUE */, class Implement /* = TEListImpl<VALUE_TYPE> */>
TELinkedList<VALUE, VALUE_TYPE, Implement>::~TELinkedList( void )
{
    RemoveAll();
}

template <typename VALUE, typename VALUE_TYPE /* = VALUE */, class Implement /* = TEListImpl<VALUE_TYPE> */>
const TELinkedList<VALUE, VALUE_TYPE, Implement> & TELinkedList<VALUE, VALUE_TYPE, Implement>::operator = (const TELinkedList<VALUE, VALUE_TYPE, Implement> & src)
{
    if (this != &src)
    {
        RemoveAll();
        const TELinkedList<VALUE, VALUE_TYPE, Implement>::CEBlock * block = src.mHead;
        for ( ; block != NULL; block = block->mNext)
            AddTail(block->mValue);
    }
    return (*this);
}

template <typename VALUE, typename VALUE_TYPE /* = VALUE */, class Implement /* = TEListImpl<VALUE_TYPE> */>
bool TELinkedList<VALUE, VALUE_TYPE, Implement>::operator == (const TELinkedList<VALUE, VALUE_TYPE, Implement> & other) const
{
    bool result = true;
    if (this != &other) // <== if same list, no need to compare
    {
        result = false; // <== initialy lists are not equal
        if (mCount == other.mCount) // <== if list contain different amount of items, no need to compare
        {
            const TELinkedList<VALUE, VALUE_TYPE, Implement>::CEBlock * thisBlock   = mHead;
            const TELinkedList<VALUE, VALUE_TYPE, Implement>::CEBlock * otherBlock  = other.mHead;
            for (int i = 0; i < mCount; ++ i)
            {
                if ( EqualValues(thisBlock->mValue, otherBlock->mValue) )
                {
                    thisBlock  = thisBlock->mNext;
                    otherBlock = otherBlock->mNext;
                }
                else
                {
                    break;
                }
            }
            result = (thisBlock == otherBlock) && (thisBlock == NULL); // <== we reached end, items are equal?
        }
    }
    return result;
}

template <typename VALUE, typename VALUE_TYPE /* = VALUE */, class Implement /* = TEListImpl<VALUE_TYPE> */>
bool TELinkedList<VALUE, VALUE_TYPE, Implement>::operator != (const TELinkedList<VALUE, VALUE_TYPE, Implement> & other) const
{
    bool result = false;
    if ( this != &other ) // <== if same list, no need to compare
    {
        result = true; // <== initialy lists are not equal
        if ( mCount == other.mCount ) // <== if list contain different amount of items, no need to compare
        {
            const TELinkedList<VALUE, VALUE_TYPE, Implement>::CEBlock * thisBlock = mHead;
            const TELinkedList<VALUE, VALUE_TYPE, Implement>::CEBlock * otherBlock = other.mHead;
            for ( int i = 0; i < mCount; ++ i )
            {
                if ( EqualValues( thisBlock->mValue, otherBlock->mValue ) )
                {
                    thisBlock  = thisBlock->mNext;
                    otherBlock = otherBlock->mNext;
                }
                else
                {
                    break;
                }
            }
            result = (thisBlock != thisBlock); // <== this is possible only if we did not reach end-of-list
        }
    }
    return result;
}

template <typename VALUE, typename VALUE_TYPE /* = VALUE */, class Implement /* = TEListImpl<VALUE_TYPE> */>
inline VALUE & TELinkedList<VALUE, VALUE_TYPE, Implement>::operator []( int atIndex )
{   return GetAt(atIndex);                  }

template <typename VALUE, typename VALUE_TYPE /* = VALUE */, class Implement /* = TEListImpl<VALUE_TYPE> */>
inline VALUE & TELinkedList<VALUE, VALUE_TYPE, Implement>::operator []( LISTPOS atPosition )
{   return GetAt(atPosition);               }

template <typename VALUE, typename VALUE_TYPE /* = VALUE */, class Implement /* = TEListImpl<VALUE_TYPE> */>
inline VALUE_TYPE TELinkedList<VALUE, VALUE_TYPE, Implement>::operator []( int atIndex ) const
{   return GetAt(atIndex);                  }

template <typename VALUE, typename VALUE_TYPE /* = VALUE */, class Implement /* = TEListImpl<VALUE_TYPE> */>
inline VALUE_TYPE TELinkedList<VALUE, VALUE_TYPE, Implement>::operator [] ( const LISTPOS atPosition ) const
{   return GetAt(atPosition);               }

template <typename VALUE, typename VALUE_TYPE /* = VALUE */, class Implement /* = TEListImpl<VALUE_TYPE> */>
inline bool TELinkedList<VALUE, VALUE_TYPE, Implement>::IsEmpty( void ) const	
{   return (mCount == 0);                   }

template <typename VALUE, typename VALUE_TYPE /* = VALUE */, class Implement /* = TEListImpl<VALUE_TYPE> */>
inline int TELinkedList<VALUE, VALUE_TYPE, Implement>::GetSize( void ) const	
{   return mCount;                          }

template <typename VALUE, typename VALUE_TYPE /* = VALUE */, class Implement /* = TEListImpl<VALUE_TYPE> */>
inline LISTPOS TELinkedList<VALUE, VALUE_TYPE, Implement>::GetHeadPosition( void )	const
{   return static_cast<LISTPOS>(mHead);     }

template <typename VALUE, typename VALUE_TYPE /* = VALUE */, class Implement /* = TEListImpl<VALUE_TYPE> */>
inline LISTPOS TELinkedList<VALUE, VALUE_TYPE, Implement>::GetTailPosition( void ) const
{   return static_cast<LISTPOS>(mTail);     }

template <typename VALUE, typename VALUE_TYPE /* = VALUE */, class Implement /* = TEListImpl<VALUE_TYPE> */>
inline VALUE_TYPE TELinkedList<VALUE, VALUE_TYPE, Implement>::GetHead( void ) const
{
    ASSERT(mHead != NULL);
    return static_cast<VALUE_TYPE>(mHead->mValue);
}

template <typename VALUE, typename VALUE_TYPE /* = VALUE */, class Implement /* = TEListImpl<VALUE_TYPE> */>
inline VALUE_TYPE TELinkedList<VALUE, VALUE_TYPE, Implement>::GetTail( void ) const	
{
    ASSERT(mTail != NULL);
    return static_cast<VALUE_TYPE>(mTail->mValue);
}

template <typename VALUE, typename VALUE_TYPE /* = VALUE */, class Implement /* = TEListImpl<VALUE_TYPE> */>
inline VALUE_TYPE TELinkedList<VALUE, VALUE_TYPE, Implement>::GetNext(LISTPOS &in_out_NextPosition) const
{
    ASSERT(in_out_NextPosition != CETemplateConstants::INVALID_POSITION && mCount != 0);
    TELinkedList<VALUE, VALUE_TYPE, Implement>::CEBlock * block = reinterpret_cast<TELinkedList<VALUE, VALUE_TYPE, Implement>::CEBlock *>(in_out_NextPosition);
    in_out_NextPosition = static_cast<LISTPOS>(block->mNext);
    return static_cast<VALUE_TYPE>(block->mValue);
}

template <typename VALUE, typename VALUE_TYPE /* = VALUE */, class Implement /* = TEListImpl<VALUE_TYPE> */>
inline LISTPOS TELinkedList<VALUE, VALUE_TYPE, Implement>::GetNextPosition(LISTPOS atPosition) const
{
    TELinkedList<VALUE, VALUE_TYPE, Implement>::CEBlock * block = reinterpret_cast<TELinkedList<VALUE, VALUE_TYPE, Implement>::CEBlock *>(atPosition);
    return static_cast<LISTPOS>(block != NULL ? block->mNext : NULL);
}

template <typename VALUE, typename VALUE_TYPE /* = VALUE */, class Implement /* = TEListImpl<VALUE_TYPE> */>
inline VALUE_TYPE TELinkedList<VALUE, VALUE_TYPE, Implement>::GetPrev(LISTPOS & in_out_PrevPosition) const
{
    ASSERT(in_out_PrevPosition != CETemplateConstants::INVALID_POSITION && mCount != 0);
    TELinkedList<VALUE, VALUE_TYPE, Implement>::CEBlock * block = reinterpret_cast<TELinkedList<VALUE, VALUE_TYPE, Implement>::CEBlock *>(in_out_PrevPosition);
    in_out_PrevPosition = static_cast<LISTPOS>(block->mPrev);
    return static_cast<VALUE_TYPE>(block->mValue);
}

template <typename VALUE, typename VALUE_TYPE /* = VALUE */, class Implement /* = TEListImpl<VALUE_TYPE> */>
inline LISTPOS TELinkedList<VALUE, VALUE_TYPE, Implement>::GetPrevPosition(LISTPOS atPosition) const
{
    TELinkedList<VALUE, VALUE_TYPE, Implement>::CEBlock * block = reinterpret_cast<TELinkedList<VALUE, VALUE_TYPE, Implement>::CEBlock *>(atPosition);
    return static_cast<LISTPOS>(block != NULL ? block->mPrev : NULL);
}

template <typename VALUE, typename VALUE_TYPE /* = VALUE */, class Implement /* = TEListImpl<VALUE_TYPE> */>
inline VALUE_TYPE TELinkedList<VALUE, VALUE_TYPE, Implement>::GetAt( LISTPOS atPosition ) const
{
    ASSERT(atPosition != CETemplateConstants::INVALID_POSITION && mCount != 0);
    TELinkedList<VALUE, VALUE_TYPE, Implement>::CEBlock * block = reinterpret_cast<TELinkedList<VALUE, VALUE_TYPE, Implement>::CEBlock *>(atPosition);
    return static_cast<VALUE_TYPE>(block->mValue);
}

template <typename VALUE, typename VALUE_TYPE /* = VALUE */, class Implement /* = TEListImpl<VALUE_TYPE> */>
inline VALUE & TELinkedList<VALUE, VALUE_TYPE, Implement>::GetAt(LISTPOS atPosition)
{
    ASSERT(atPosition != CETemplateConstants::INVALID_POSITION && mCount != 0);
    TELinkedList<VALUE, VALUE_TYPE, Implement>::CEBlock * block = reinterpret_cast<TELinkedList<VALUE, VALUE_TYPE, Implement>::CEBlock *>(atPosition);
    return block->mValue;
}

template <typename VALUE, typename VALUE_TYPE /* = VALUE */, class Implement /* = TEListImpl<VALUE_TYPE> */>
VALUE_TYPE TELinkedList<VALUE, VALUE_TYPE, Implement>::GetAt(int index) const
{
    ASSERT(index >= 0 && index < mCount);
    TELinkedList<VALUE, VALUE_TYPE, Implement>::CEBlock * block   = mHead;
    for (int i = 0; i < index; ++ i)
        block = block->mNext;
    
    ASSERT(block != NULL);
    return static_cast<VALUE_TYPE>(block->mValue);
}

template <typename VALUE, typename VALUE_TYPE /* = VALUE */, class Implement /* = TEListImpl<VALUE_TYPE> */>
VALUE & TELinkedList<VALUE, VALUE_TYPE, Implement>::GetAt(int index)
{
    ASSERT(index >= 0 && index < mCount);
    TELinkedList<VALUE, VALUE_TYPE, Implement>::CEBlock * block   = mHead;
    for (int i = 0; i < index; ++ i)
        block = block->mNext;

    ASSERT(block != NULL);
    return block->mValue;
}

template <typename VALUE, typename VALUE_TYPE /* = VALUE */, class Implement /* = TEListImpl<VALUE_TYPE> */>
bool TELinkedList<VALUE, VALUE_TYPE, Implement>::GetNextElem(LISTPOS & in_out_NextPosition, VALUE & out_NextValue) const
{
    ASSERT(in_out_NextPosition != CETemplateConstants::INVALID_POSITION && mCount != 0);
    TELinkedList<VALUE, VALUE_TYPE, Implement>::CEBlock * block = reinterpret_cast<TELinkedList<VALUE, VALUE_TYPE, Implement>::CEBlock *>(in_out_NextPosition);
    in_out_NextPosition = static_cast<LISTPOS>(block->mNext);
    if (in_out_NextPosition != CETemplateConstants::INVALID_POSITION)
        out_NextValue = static_cast<VALUE>(block->mNext->mValue);
    
    return (in_out_NextPosition != CETemplateConstants::INVALID_POSITION);
}

template <typename VALUE, typename VALUE_TYPE /* = VALUE */, class Implement /* = TEListImpl<VALUE_TYPE> */>
bool TELinkedList<VALUE, VALUE_TYPE, Implement>::GetPrevElem(LISTPOS &in_out_PrevPosition, VALUE & out_PrevValue) const
{
    ASSERT(in_out_PrevPosition != CETemplateConstants::INVALID_POSITION && mCount != 0);
    TELinkedList<VALUE, VALUE_TYPE, Implement>::CEBlock * block = reinterpret_cast<TELinkedList<VALUE, VALUE_TYPE, Implement>::CEBlock *>(in_out_PrevPosition);
    in_out_PrevPosition = static_cast<LISTPOS>(block->mPrev);
    if (in_out_PrevPosition != CETemplateConstants::INVALID_POSITION)
        out_PrevValue = static_cast<VALUE>(block->mValue);

    return (in_out_PrevPosition != CETemplateConstants::INVALID_POSITION);
}

template <typename VALUE, typename VALUE_TYPE /* = VALUE */, class Implement /* = TEListImpl<VALUE_TYPE> */>
VALUE TELinkedList<VALUE, VALUE_TYPE, Implement>::RemoveHead( void )
{
    ASSERT(mHead != NULL && mCount != 0);

    VALUE result                                    = mHead->mValue;
    TELinkedList<VALUE, VALUE_TYPE, Implement>::CEBlock * block = mHead->mNext; 

    delete	mHead;
    -- mCount;

    if (mCount == 0)	
    {
        ASSERT(block == NULL);
        mHead = mTail = NULL;
    }
    else if (mCount == 1) 
    {
        ASSERT(block == mTail);
        mTail->mPrev = NULL;
        mHead        = mTail;
    }
    else
    {
        ASSERT(block != NULL);
        mHead        = block;
        mHead->mPrev = NULL;
    }
    return result;
}

template <typename VALUE, typename VALUE_TYPE /* = VALUE */, class Implement /* = TEListImpl<VALUE_TYPE> */>
VALUE TELinkedList<VALUE, VALUE_TYPE, Implement>::RemoveTail( void )
{
    ASSERT(mTail != NULL && mCount != 0);

    VALUE result                                    = mTail->mValue;
    TELinkedList<VALUE, VALUE_TYPE, Implement>::CEBlock * block = mTail->mPrev;
    
    delete	mTail;
    -- mCount;

    if (mCount == 0)
    {
        ASSERT(block == NULL);
        mHead = mTail = NULL;
    }
    else if (mCount == 1)
    {
        ASSERT(block == mHead);
        mHead->mNext = NULL;
        mTail        = mHead;
    }
    else
    {
        ASSERT(block != NULL);
        mTail        = block;
        mTail->mNext = NULL;
    }
    return result;
}


template <typename VALUE, typename VALUE_TYPE /* = VALUE */, class Implement /* = TEListImpl<VALUE_TYPE> */>
LISTPOS TELinkedList<VALUE, VALUE_TYPE, Implement>::AddHead(VALUE_TYPE newElement)
{
    TELinkedList<VALUE, VALUE_TYPE, Implement>::CEBlock * newBlock = DEBUG_NEW TELinkedList<VALUE, VALUE_TYPE, Implement>::CEBlock(newElement);
    if (newBlock != NULL)
    {
        if (mCount == 0)
        {
            ASSERT(mHead == NULL && mTail == NULL);
            mHead = mTail = newBlock;
        }
        else if (mCount == 1)
        {
            ASSERT(mHead == mTail);
            mHead        = newBlock;
            mHead->mNext = mTail;
            mTail->mPrev = mHead;
        }
        else
        {
            ASSERT(mHead != mTail);
            mHead->mPrev    = newBlock;
            newBlock->mNext = mHead;
            mHead           = newBlock;
        }
        mCount ++;
    }
    return static_cast<LISTPOS>(newBlock);
}

template <typename VALUE, typename VALUE_TYPE /* = VALUE */, class Implement /* = TEListImpl<VALUE_TYPE> */>
LISTPOS TELinkedList<VALUE, VALUE_TYPE, Implement>::AddTail(VALUE_TYPE newElement)
{
    TELinkedList<VALUE, VALUE_TYPE, Implement>::CEBlock * newBlock  = DEBUG_NEW TELinkedList<VALUE, VALUE_TYPE, Implement>::CEBlock(newElement);
    if (newBlock != NULL)
    {
        if (mCount == 0)
        {
            ASSERT(mHead == NULL && mTail == NULL);
            mHead = mTail = newBlock;
        }
        else if (mCount == 1)
        {
            ASSERT(mHead == mTail);
            mTail        = newBlock;
            mTail->mPrev = mHead;
            mHead->mNext = mTail;
        }
        else
        {
            ASSERT(mHead != mTail);
            mTail->mNext    = newBlock;
            newBlock->mPrev = mTail;
            mTail           = newBlock;
        }
        mCount ++;
    }

    return static_cast<LISTPOS>(newBlock);
}

template <typename VALUE, typename VALUE_TYPE /* = VALUE */, class Implement /* = TEListImpl<VALUE_TYPE> */>
LISTPOS TELinkedList<VALUE, VALUE_TYPE, Implement>::InsertBefore(LISTPOS beforePosition, VALUE_TYPE newElement)
{
    ASSERT(beforePosition != CETemplateConstants::INVALID_POSITION && mCount != 0);

    TELinkedList<VALUE, VALUE_TYPE, Implement>::CEBlock * newBlock = DEBUG_NEW TELinkedList<VALUE, VALUE_TYPE, Implement>::CEBlock(newElement);
    if (newBlock != NULL)
    {
        TELinkedList<VALUE, VALUE_TYPE, Implement>::CEBlock * block = reinterpret_cast<TELinkedList<VALUE, VALUE_TYPE, Implement>::CEBlock *>(beforePosition);
        if (mCount == 1)
        {
            ASSERT(mHead == block && mTail == block);
            mHead       = newBlock;
            mHead->mNext= mTail;
            mTail->mPrev= mHead;
        }
        else if (block == mHead)
        {
            mHead->mPrev    = newBlock;
            newBlock->mNext = mHead;
            mHead           = newBlock;
        }
        else
        {
            ASSERT(block->mPrev != NULL);
            newBlock->mPrev         = block->mPrev;
            newBlock->mPrev->mNext  = newBlock;
            block->mPrev            = newBlock;
            newBlock->mNext         = block;
        }
        mCount ++;
    }
    return static_cast<LISTPOS>(newBlock);
}

template <typename VALUE, typename VALUE_TYPE /* = VALUE */, class Implement /* = TEListImpl<VALUE_TYPE> */>
LISTPOS TELinkedList<VALUE, VALUE_TYPE, Implement>::InsertAfter(LISTPOS afterPosition, VALUE_TYPE newElement)
{
    ASSERT(afterPosition != CETemplateConstants::INVALID_POSITION && mCount != 0);

    TELinkedList<VALUE, VALUE_TYPE, Implement>::CEBlock * newBlock  = DEBUG_NEW TELinkedList<VALUE, VALUE_TYPE, Implement>::CEBlock(newElement);
    if (newBlock != NULL)
    {
        TELinkedList<VALUE, VALUE_TYPE, Implement>::CEBlock * block = reinterpret_cast<TELinkedList<VALUE, VALUE_TYPE, Implement>::CEBlock *>(afterPosition);
        if (mCount == 1)
        {
            ASSERT(mHead == block && mTail == block);
            mTail       = newBlock;
            mTail->mPrev= mHead; 
            mHead->mNext= mTail;
        }
        else if (block == mTail)
        {
            mTail->mNext    = newBlock;
            newBlock->mPrev = mTail;
            mTail           = newBlock;
        }
        else
        {
            ASSERT(block->mNext != NULL);
            newBlock->mNext         = block->mNext;
            newBlock->mNext->mPrev  = newBlock;
            block->mNext            = newBlock;
            newBlock->mPrev         = block;
        }
        mCount ++;
    }
    return static_cast<LISTPOS>(newBlock);
}


template <typename VALUE, typename VALUE_TYPE /* = VALUE */, class Implement /* = TEListImpl<VALUE_TYPE> */>
inline VALUE TELinkedList<VALUE, VALUE_TYPE, Implement>::SetAt(LISTPOS atPosition, VALUE_TYPE newValue)
{
    ASSERT(atPosition != CETemplateConstants::INVALID_POSITION && mCount != 0);
    TELinkedList<VALUE, VALUE_TYPE, Implement>::CEBlock * block   = reinterpret_cast<TELinkedList<VALUE, VALUE_TYPE, Implement>::CEBlock *>(atPosition);
    VALUE oldValue  = block->mValue;
    block->mValue   = (VALUE)(newValue);

    return oldValue;
}

template <typename VALUE, typename VALUE_TYPE /* = VALUE */, class Implement /* = TEListImpl<VALUE_TYPE> */>
void TELinkedList<VALUE, VALUE_TYPE, Implement>::RemoveAll( void )
{
    TELinkedList<VALUE, VALUE_TYPE, Implement>::CEBlock * block = mHead;
    while (block != NULL)
    {
        block = block->mNext;
        delete mHead;
        mHead = block;
    }
    mHead = mTail = static_cast<TELinkedList<VALUE, VALUE_TYPE, Implement>::CEBlock *>(NULL);
    mCount = 0;
}

template <typename VALUE, typename VALUE_TYPE /* = VALUE */, class Implement /* = TEListImpl<VALUE_TYPE> */>
VALUE TELinkedList<VALUE, VALUE_TYPE, Implement>::RemoveAt(LISTPOS atPosition)
{
    ASSERT(atPosition != CETemplateConstants::INVALID_POSITION && mCount != 0);

    TELinkedList<VALUE, VALUE_TYPE, Implement>::CEBlock * block   = reinterpret_cast<TELinkedList<VALUE, VALUE_TYPE, Implement>::CEBlock *>(atPosition);

    if (mCount == 1)
    {
        ASSERT(mHead == block && mTail == block);
        mTail = mHead = NULL;
    }
    else if (mCount == 2)
    {
        ASSERT(mHead == block || mTail == block);
        mHead = mTail = (block->mPrev != NULL ? block->mPrev : block->mNext);

        ASSERT(mHead != NULL);
        mHead->mPrev = mHead->mNext = mTail->mPrev = mTail->mNext = NULL;
    }
    else
    {
        if (block == mTail)
        {
            block->mPrev->mNext = NULL;
            mTail               = block->mPrev;
        }
        else if (mHead == block)
        {
            block->mNext->mPrev = NULL;
            mHead               = block->mNext;
        }
        else
        {
            ASSERT(block->mNext != NULL && block->mPrev != NULL);
            block->mPrev->mNext = block->mNext;
            block->mNext->mPrev = block->mPrev;
        }
    }
    VALUE oldValue  = block->mValue;
    delete	block;
    mCount --;
    
    return oldValue;
}

template <typename VALUE, typename VALUE_TYPE /* = VALUE */, class Implement /* = TEListImpl<VALUE_TYPE> */>
bool TELinkedList<VALUE, VALUE_TYPE, Implement>::RemoveAt(LISTPOS atPosition, VALUE &out_value)
{
    ASSERT(atPosition != CETemplateConstants::INVALID_POSITION && mCount != 0);

    TELinkedList<VALUE, VALUE_TYPE, Implement>::CEBlock * block = reinterpret_cast<TELinkedList<VALUE, VALUE_TYPE, Implement>::CEBlock *>(atPosition);
    bool result = false;
    if (block != NULL)
    {
        out_value   = block->mValue;
        result      = true;
    }

    if (mCount == 1)
    {
        ASSERT(mHead == block && mTail == block);
        mTail = mHead = NULL;
    }
    else if (mCount == 2)
    {
        ASSERT(mHead == block || mTail == block);
        mHead = mTail = (block->mPrev != NULL ? block->mPrev : block->mNext);

        ASSERT(mHead != NULL);
        mHead->mPrev = mHead->mNext = mTail->mPrev = mTail->mNext = NULL;
    }
    else
    {
        if (block == mTail)
        {
            block->mPrev->mNext = NULL;
            mTail               = block->mPrev;
        }
        else if (mHead == block)
        {
            block->mNext->mPrev = NULL;
            mHead               = block->mNext;
        }
        else
        {
            ASSERT(block->mNext != NULL && block->mPrev != NULL);
            block->mPrev->mNext = block->mNext;
            block->mNext->mPrev = block->mPrev;
        }
    }
    delete	block;
    -- mCount;
    return result;
}

template <typename VALUE, typename VALUE_TYPE /* = VALUE */, class Implement /* = TEListImpl<VALUE_TYPE> */>
bool TELinkedList<VALUE, VALUE_TYPE, Implement>::RemoveElem(VALUE_TYPE removeElement, LISTPOS searchAfter /*= NULL*/)
{
    bool result = false;
    TELinkedList<VALUE, VALUE_TYPE, Implement>::CEBlock * block = mHead;
    if (searchAfter != CETemplateConstants::INVALID_POSITION && mCount != 0)
        block = static_cast<TELinkedList<VALUE, VALUE_TYPE, Implement>::CEBlock *>(searchAfter)->mNext;

    for ( ; block != NULL; block = block->mNext)
    {
        if ( EqualValues(removeElement, block->mValue) )
            break;
    }

    if (block != NULL)
    {
        RemoveAt(static_cast<LISTPOS>(block));
        result = true;
    }

    return result;
}

template <typename VALUE, typename VALUE_TYPE /* = VALUE */, class Implement /* = TEListImpl<VALUE_TYPE> */>
LISTPOS TELinkedList<VALUE, VALUE_TYPE, Implement>::Find(VALUE_TYPE searchValue, LISTPOS searchAfter /*= NULL*/) const
{	
    TELinkedList<VALUE, VALUE_TYPE, Implement>::CEBlock * block   = mHead;
    if (searchAfter != CETemplateConstants::INVALID_POSITION && mCount != 0)
        block = static_cast<TELinkedList<VALUE, VALUE_TYPE, Implement>::CEBlock *>(searchAfter)->mNext;
    
    for ( ; block != NULL; block = block->mNext)
    {
        if ( EqualValues(block->mValue, searchValue) )
            break;
    }
    
    return static_cast<LISTPOS>(block);
}

template <typename VALUE, typename VALUE_TYPE /* = VALUE */, class Implement /* = TEListImpl<VALUE_TYPE> */>
LISTPOS TELinkedList<VALUE, VALUE_TYPE, Implement>::FindIndex(int index) const
{
    TELinkedList<VALUE, VALUE_TYPE, Implement>::CEBlock *block   = static_cast<TELinkedList<VALUE, VALUE_TYPE, Implement>::CEBlock *>(index >= 0 && index < mCount? mHead : NULL);
    if (block != NULL)
    {
        for (int i = 0; i < index; ++ i)
            block = block->mNext;
    }
    
    return static_cast<LISTPOS>(block);
}

template <typename VALUE, typename VALUE_TYPE /* = VALUE */, class Implement /* = TEListImpl<VALUE_TYPE> */>
int TELinkedList<VALUE, VALUE_TYPE, Implement>::GetIndex(LISTPOS atPosition) const
{
    int result = -1;
    if (mCount != 0 && atPosition != CETemplateConstants::INVALID_POSITION)
    {
        TELinkedList<VALUE, VALUE_TYPE, Implement>::CEBlock * search = reinterpret_cast<TELinkedList<VALUE, VALUE_TYPE, Implement>::CEBlock *>(atPosition);
        TELinkedList<VALUE, VALUE_TYPE, Implement>::CEBlock * block  = mHead;
        for (result = 0; result < mCount; result ++)
        {
            ASSERT(block != NULL);
            if (block == search)
                break;
            block = block->mNext;
        }
    }
    
    return (result != mCount ? result : -1);
}

template <typename VALUE, typename VALUE_TYPE /* = VALUE */, class Implement /* = TEListImpl<VALUE_TYPE> */>
inline int TELinkedList<VALUE, VALUE_TYPE, Implement>::GetIndex(VALUE_TYPE searchValue, LISTPOS startAfter /*= NULL*/) const
{
    return GetIndex( static_cast<LISTPOS>(Find(searchValue, startAfter)) );
}

template <typename VALUE, typename VALUE_TYPE /* = VALUE */, class Implement /* = TEListImpl<VALUE_TYPE> */>
inline bool TELinkedList<VALUE, VALUE_TYPE, Implement>::IsPositionValid( const LISTPOS posCheck ) const
{
    bool result = false;
    TELinkedList<VALUE, VALUE_TYPE, Implement>::CEBlock * block = mHead;
    for ( ; result == false && block != NULL; block = block->mNext)
        result = static_cast<const LISTPOS>(block ) == posCheck;
    
    return result;
}

template <typename VALUE, typename VALUE_TYPE /* = VALUE */, class Implement /* = TEListImpl<VALUE_TYPE> */>
inline bool TELinkedList<VALUE, VALUE_TYPE, Implement>::EqualValues(VALUE_TYPE value1, VALUE_TYPE value2) const
{
    return Implement::ImplEqualValues(value1, value2);
}

//////////////////////////////////////////////////////////////////////////
// Friend function implementation
//////////////////////////////////////////////////////////////////////////

template <typename V, typename VT, class Impl> 
const IEInStream & operator >> ( const IEInStream & stream, TELinkedList<V, VT, Impl> & input )
{
    input.RemoveAll();

    int size = 0;
    stream >> size;
    for (int i = 0; i < size; ++ i)
    {
        V newValue;
        stream >> newValue;
        input.AddTail(newValue);
    }

    return stream;
}

template <typename V, typename VT, class Impl> 
IEOutStream & operator << ( IEOutStream & stream, const TELinkedList<V, VT, Impl> & output )
{
    int size = output.GetSize();
    stream << size;
    const typename TELinkedList<V, VT, Impl>::CEBlock * block = output.mHead;
    for ( ; block != NULL; block = block->mNext)
        stream << block->mValue;

    return stream;
}

#endif  // AREG_BASE_TELINKEDLIST_HPP