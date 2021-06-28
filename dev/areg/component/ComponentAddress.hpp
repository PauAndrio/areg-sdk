#ifndef AREG_COMPONENT_COMPONENTADDRESS_HPP
#define AREG_COMPONENT_COMPONENTADDRESS_HPP
/************************************************************************
 * \file        areg/component/ComponentAddress.hpp
 * \ingroup     AREG SDK, Asynchronous Event Generator Software Development Kit 
 * \author      Artak Avetyan (mailto:artak@aregtech.com)
 * \brief       AREG Platform, Component Address class.
 *              This class defines address object of component.
 *              Every created component has address to be able to find
 *              in system and use for event dispatching.
 *
 ************************************************************************/
/************************************************************************
 * Include files.
 ************************************************************************/
#include "areg/base/GEGlobal.h"

#include "areg/base/ThreadAddress.hpp"
#include "areg/base/String.hpp"

/************************************************************************
 * Dependencies
 ************************************************************************/
class IEInStream;
class IEOutStream;

//////////////////////////////////////////////////////////////////////////
// ComponentAddress class declaration
//////////////////////////////////////////////////////////////////////////
/**
 * \brief       Component Address should be unique in the system. The
 *              uniqueness is provided by combination of master Thread
 *              Address and Role Name. It is possible to convert address
 *              to string by creating component path and from component
 *              path string to restore component address. The valid address
 *              of component should differ from INVALID_COMPONENT_ADDRESS.
 **/
class AREG_API ComponentAddress
{
//////////////////////////////////////////////////////////////////////////
// Predefined types and constants.
//////////////////////////////////////////////////////////////////////////

/************************************************************************/
// Static variables
/************************************************************************/

    /**
     * \brief   Constant. The name of invalid component.
     *          Do not use this name as role name for component.
     **/
    static const char* const        INVALID_COMPONENT_NAME  /*= "INVALID_COMPONENT_NAME"*/;

public:
    /**
     * \brief   Constant. Defined invalid component.
     *          Invalid component address is excluded from any event
     *          dispatching or registration. The valid component
     *          address should differ from this value.
     *          To check whether component address or not, call
     *          isValid() method.
     **/
    static const ComponentAddress INVALID_COMPONENT_ADDRESS;

/************************************************************************/
// Static methods
/************************************************************************/

    /**
     * \brief   Converts Component address to string object containing special syntax,
     *          which is considered component path.
     * \param   componentAddress    The address of Component to create path
     * \return  Returns converted path of Component as string, containing Component address information
     **/
    static String convAddressToPath( const ComponentAddress & componentAddress );

    /**
     * \brief   From given component path creates component address and returns pointer
     *          to remaining part of path.
     * \param   componentPath   String, containing component path
     * \param   out_nextPart    If not NULL, on output it will contain remaining part after
     *                          component address in the path.
     * \return  Returns parsed and instantiated component address object. 
     *          Verify validation before use.
     **/
    static ComponentAddress convPathToAddress( const char* componentPath, const char ** out_nextPart = NULL );

//////////////////////////////////////////////////////////////////////////
// Constructors / Destructor
//////////////////////////////////////////////////////////////////////////
public:
    /**
     * \brief   Initialization component.
     *          It creates component by given thread address.
     *          The role name of component should be set additionally.
     *          Otherwise component address is invalid and component
     *          cannot be included in dispatching process.
     **/
    ComponentAddress( const ThreadAddress & threadAddress );
    
    /**
     * \brief	Initialization component.
     *          It creates component by given thread address and role name.
     *          The thread address and role name should be valid.
     * \param	threadAddress	The address of master component thread.
     * \param	roleName	    The role name of component.
     **/
    ComponentAddress( const ThreadAddress & threadAddress, const char * roleName );

    /**
     * \brief	Initialization component.
     *          It creates component by given role name.
     *          As a master thread it will take current thread.
     *          Use this constructor if Component is created in current
     *          component thread.
     * \param	roleName	The role name of component.
     * \return	
     **/
    ComponentAddress( const char * roleName );

    /**
     * \brief	Initialization component.
     *          It creates component by given role name.
     *          As a master thread it will search component 
     *          thread by given thread name.
     * \param	roleName	The role name of component.
     * \param	nameThread	The thread name of master component thread.
     **/
    ComponentAddress( const char * roleName, const char * nameThread );

    /**
     * \brief   Copy constructor.
     * \param   src     The source of data to copy.
     **/
    ComponentAddress( const ComponentAddress & src );
    /**
     * \brief   Initialization constructor.
     *          De-serialize component address information stored in stream.
     **/
    ComponentAddress( const IEInStream & stream );

    /**
     * \brief   Destructor.
     **/
    ~ComponentAddress( void );

//////////////////////////////////////////////////////////////////////////
// Operators
//////////////////////////////////////////////////////////////////////////
public:
/************************************************************************/
// Basic operators
/************************************************************************/

    /**
     * \brief   Converting operator.
     **/
    inline operator unsigned int ( void ) const;

    /**
     * \brief   Assigning operator. Copies address data from given source.
     **/
    inline const ComponentAddress & operator = ( const ComponentAddress & src );

    /**
     * \brief   Comparing operator. Returns true if 2 addresses are equal.
     **/
    inline bool operator == ( const ComponentAddress & other ) const;

    /**
     * \brief   Comparing operator. Returns true if 2 addresses are not equal.
     **/
    inline bool operator != ( const ComponentAddress & other ) const;

/************************************************************************/
// Friend global operators to support streaming.
/************************************************************************/

    /**
     * \brief   Streaming operator. Read from stream and initialize component address.
     **/
    friend inline const IEInStream & operator >> ( const IEInStream & stream, ComponentAddress & input );

    /**
     * \brief   Streaming operator. Writes to stream component address.
     **/
    friend inline IEOutStream & operator << ( IEOutStream & stream, const ComponentAddress & output );

//////////////////////////////////////////////////////////////////////////
// Attributes and operations
//////////////////////////////////////////////////////////////////////////
public:
    /**
     * \brief   Returns component master thread address.
     **/
    inline const ThreadAddress & getThreadAddress( void ) const;

    /**
     * \brief   Returns the name of component (role name)
     **/
    inline const String & getRoleName( void ) const;

    /**
     * \brief   Return true if component address if valid.
     *          Address is valid if it is not equal to INVALID_COMPONENT_ADDRESS
     **/
    bool isValid( void) const;

    /**
     * \brief	Creates component address to string.
     *          Every part of component address has a special path separator.
     * \return  Returns converted path of Component as a string.
     **/
    String convToString( void ) const;

    /**
     * \brief	Parses component path string and retrieves component address data from path.
     * \param	pathComponent   The component path as a string.
     * \param	out_nextPart	If not a NULL, on output this will contain remaining
     *                          part after getting component path. On output usually
     *                          should be NULL.
     **/
    void convFromString(const char * pathComponent, const char** out_nextPart = NULL);

private:
/************************************************************************/
// Private member variables
/************************************************************************/
    /**
     * \brief   Component name. Or Role Name of component
     **/
    String          mRoleName;
    /**
     * \brief   Thread address object.
     **/
    ThreadAddress   mThreadAddress;
    /**
     * \brief   The numeric value of Component Address object
     **/
    unsigned int    mMagicNum;

//////////////////////////////////////////////////////////////////////////
// Private / Hidden members
//////////////////////////////////////////////////////////////////////////
private:
/************************************************************************/
// Private methods
/************************************************************************/
    /**
     * \brief   Default constructor. Cannot be accessed. For internal use only.
     **/
    ComponentAddress( void );
    /**
     * \brief   Returns the calculated numeric value of specified component address object.
     **/
    static unsigned int _magicNumber( const ComponentAddress & addrComp );
};

//////////////////////////////////////////////////////////////////////////
// ComponentAddress class inline function implementation
//////////////////////////////////////////////////////////////////////////

inline const ComponentAddress& ComponentAddress::operator = ( const ComponentAddress& src )
{
    mThreadAddress  = src.mThreadAddress;
    mRoleName       = src.mRoleName;
    mMagicNum       = src.mMagicNum;

    return (*this);
}

inline bool ComponentAddress::operator == ( const ComponentAddress& other ) const
{
    return (mThreadAddress == other.mThreadAddress) && (mRoleName == other.mRoleName);
}

inline bool ComponentAddress::operator != ( const ComponentAddress& other ) const
{
    return (mThreadAddress != other.mThreadAddress) || (mRoleName != other.mRoleName);
}

ComponentAddress::operator unsigned int ( void ) const
{
    return mMagicNum;
}

inline const ThreadAddress& ComponentAddress::getThreadAddress( void ) const
{
    return mThreadAddress;
}

inline const String& ComponentAddress::getRoleName( void ) const
{
    return mRoleName;
}

//////////////////////////////////////////////////////////////////////////
// Friend global operators to support streaming
//////////////////////////////////////////////////////////////////////////
inline const IEInStream & operator >> (const IEInStream & stream, ComponentAddress & input)
{
    stream >> input.mRoleName;
    stream >> input.mThreadAddress; 
    return stream;                                   }

inline IEOutStream & operator << (IEOutStream & stream, const ComponentAddress & output)
{
    stream << output.mRoleName;
    stream << output.mThreadAddress;
    return stream;
}

#endif  // AREG_COMPONENT_COMPONENTADDRESS_HPP
