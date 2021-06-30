#ifndef AREG_COMPONENT_PRIVATE_COMPONENTINFO_HPP
#define AREG_COMPONENT_PRIVATE_COMPONENTINFO_HPP
/************************************************************************
 * \file        areg/component/private/ComponentInfo.hpp
 * \ingroup     AREG SDK, Asynchronous Event Generator Software Development Kit 
 * \author      Artak Avetyan (mailto:artak@aregtech.com)
 * \brief       AREG Platform, Component Info class declaration.
 *              Contains information of threads binded with component.
 *
 ************************************************************************/

/************************************************************************
 * Include files.
 ************************************************************************/
#include "areg/base/GEGlobal.h"

#include "areg/base/TEHashMap.hpp"
#include "areg/base/Thread.hpp"
#include "areg/component/ComponentAddress.hpp"

/************************************************************************
 * Dependencies
 ************************************************************************/
class ComponentThread;
class WorkerThread;
class DispatcherThread;

//////////////////////////////////////////////////////////////////////////
// ComponentInfo class declaration
//////////////////////////////////////////////////////////////////////////
/**
 * \brief   Component Info is an object containing information of
 *          main thread of Component and its worker threads.
 *          The Component Info object gives possibility to send and
 *          receive messages from Worker Threads and provides
 *          communication between Worker Threads binded to the same
 *          Component. The information of threads is saved in Hash Map.
 **/
class AREG_API ComponentInfo
{
//////////////////////////////////////////////////////////////////////////
// Internal class and types
//////////////////////////////////////////////////////////////////////////
private:
    /**
     * \brief   Helper class for worker thread map.
     **/
    class AREG_API _WorkerThreadMapImpl   : public TEHashMapImpl<const ThreadAddress &, WorkerThread *>
    {
    public:
        /**
         * \brief   Called to calculate the 32-bit hash key value.
         * \ param  Key     The object to calculate 32-bit hash key.
         * \return  Returns 32-bit hash key value.
         **/
        inline unsigned int implHashKey(const ThreadAddress & Key) const
        {
            return static_cast<unsigned int>(Key);
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // ComponentInfo::_WorkerThreadMap type declaration
    //////////////////////////////////////////////////////////////////////////
    /**
     * \brief   The Hash Map object to save information of threads
     *          saved in specified Component Info object.
     **/
    typedef TEHashMap<ThreadAddress, WorkerThread*, const ThreadAddress &, WorkerThread*, _WorkerThreadMapImpl>   _WorkerThreadMap;

    /**
     * \brief   Resource mapping object type. 
     *          As a Key, used thread address (Worker Thread address)
     *          As a value, it saves pointers of Worker Thread object
     *          As a Hash Map used ComponentInfo::_WorkerThreadMap object
     **/
    typedef TELockResourceMap<ThreadAddress, WorkerThread, _WorkerThreadMap, TEResourceMapImpl<ThreadAddress, WorkerThread>>     MapWorkerThread;

//////////////////////////////////////////////////////////////////////////
// Constructor / Destructor
//////////////////////////////////////////////////////////////////////////
public:

    /**
     * \brief   Initialization constructor.
     *          Creates Component Info object by passing the Master Thread
     *          of Component (Component Thread) and the role name of Component.
     * \param   masterThread    The Master Thread of Component (Component Thread)
     * \param   roleName        The Role Name of component, must not be empty
     **/
    ComponentInfo( ComponentThread & masterThread, const char * const roleName );

    /**
     * \brief   Destructor
     **/
    ~ComponentInfo( void );

//////////////////////////////////////////////////////////////////////////
// Attributes
//////////////////////////////////////////////////////////////////////////

    /**
     * \brief   Returns Component Thread object
     **/
    inline ComponentThread & getMasterThread( void ) const;

    /**
     * \brief   Returns the address of Component
     **/
    inline const ComponentAddress & getAddress( void ) const;

    /**
     * \brief   Returns the Role Name of Component
     **/
    inline const String & getRoleName( void ) const;

    /**
     * \brief   Returns true, if specified thread address is the address of registered Worker Thread.
     * \param   threadAddress   The thread address to check.
     * \return  Returns true, if specified thread address is the address of registered Worker Thread.
     **/
    inline bool isWorkerThreadAddress( const ThreadAddress & threadAddress) const;

    /**
     * \brief   Returns true, if Component Info has at least one registered Worker Thread.
     **/
    inline bool hasWorkerThreads( void ) const;

    /**
     * \brief   Returns true if specified Worker Thread is registered in Component Info
     * \param   workerThread    The Worker Thread object to check.
     * \return  Returns true if specified Worker Thread is registered in Component Info.
     **/
    bool isWorkerThreadRegistered( WorkerThread & workerThread ) const;

    /**
     * \brief   Returns true, if specified thread address is the address of Component Thread.
     * \param   threadAddress   The thread address to check.
     * \return  Returns true, if specified thread address is the address of Component Thread.
     **/
    bool isMasterThreadAddress( const ThreadAddress & threadAddress ) const;

//////////////////////////////////////////////////////////////////////////
// Operations
//////////////////////////////////////////////////////////////////////////

    /**
     * \brief   Registers specified Worker Thread object in Component Info
     *          and returns true, if registration succeeded.
     * \param   workerThread    The Worker Thread to register in Component Info
     **/
    bool registerWorkerThread( WorkerThread & workerThread );

    /**
     * \brief   Unregisters specified Worker Thread object in Component Info
     *          and returns true, if operation succeeded. The operation may fail,
     *          if thread previously was not registered in Component Info.
     **/
    bool unregisterWorkerThread( WorkerThread & workerThread );

    /**
     * \brief   Looks up for Worker Thread by specified unique Thread address.
     *          If there is registered thread with specified address,
     *          it returns pointer of valid Worker Thread object.
     *          Otherwise, it returns NULL.
     * \param   threadAddress   The unique Worker Thread address to search.
     * \return  Returns pointer of valid Worker Thread if there is registered
     *          Worker Thread with specified unique address. Otherwise, returns NULL.
     **/
    inline WorkerThread * findWorkerThread( const ThreadAddress & threadAddress ) const;

    /**
     * \brief   Looks up for Worker Thread by specified unique Thread name.
     *          If there is registered thread with specified name,
     *          it returns pointer of valid Worker Thread object.
     *          Otherwise, it returns NULL.
     * \param   threadName  The unique Worker Thread name to search.
     * \return  Returns pointer of valid Worker Thread if there is registered
     *          Worker Thread with specified unique name. Otherwise, returns NULL.
     **/
    inline WorkerThread * findWorkerThread( const char* threadName ) const;

    /**
     * \brief   Looks up for Worker Thread by specified unique Component Path,
     *          which is containing Thread address. The Component Thread address
     *          is extracted from, then search is provided by extracted thread address.
     *          If there is registered thread with extracted address,
     *          it returns pointer of valid Worker Thread object.
     *          Otherwise, it returns NULL.
     * \param   componentPath   The unique path of Component, containing Worker Thread address.
     * \return  Returns pointer of valid Worker Thread if there is registered
     *          Worker Thread with extracted unique address. Otherwise, returns NULL.
     **/
    inline WorkerThread * findThreadByPath( const char* componentPath ) const;

    /**
     * \brief   By specified Event Consumer runtime class ID object, looks up for registered
     *          Event Consumer in the Component Thread and in all Worker Threads.
     *          Returns pointer of valid Dispatcher Thread, where the Event Consumer is registered.
     *          Returns NULL if could not find Event Consumer.
     * \param   whichClass  The runtime class ID of Event Consumer object
     * \return  Returns valid pointer of Dispatcher Thread, where the Event Consumer is registered.
     *          If there is no such Event Consumer registered in Component Thread and in Worker Threads,
     *          it returns NULL.
     **/
    DispatcherThread * findEventConsumer( const RuntimeClassID & whichClass ) const;

    /**
     * \brief   Returns pointer of first Worker Thread. On output, out_threadAddress
     *          parameter will contain address of first valid thread, or invalid address
     *          ThreadAddress::INVALID_THREAD_ADDRESS, if Component Info
     *          has no registered Worker Thread, and the function returns NULL.
     * \param   out_threadAddress   On output, if Component Info has registered
     *                              Worker Thread, it will contain valid address.
     *                              Otherwise, it will contain invalid address
     *                              ThreadAddress::INVALID_THREAD_ADDRESS
     * \return  If Component has Worker Threads, it will return valid pointer and the
     *          Worker Thread object will have address specified in out_threadAddress.
     *          If Component has no Worker Thread, it will return NULL.
     **/
    inline WorkerThread * getFirstWorkerThread( ThreadAddress & out_threadAddress );

    /**
     * \brief   Returns pointer of next Worker Thread after registered thread specified in
     *          int_out_threadAddress. On input, int_out_threadAddress should be address
     *          of valid thread. On output, if there is next thread, it will contain
     *          address of valid Worker Thread. Otherwise, on output int_out_threadAddress
     *          is invalid ThreadAddress::INVALID_THREAD_ADDRESS address.
     * \param   int_out_threadAddress   On input, this should be valid address of Worker Thread.
     *                                  On output, if there is next thread, it will be valid address
     *                                  of next Worker Thread. Otherwise, it will be invalid address
     *                                  ThreadAddress::INVALID_THREAD_ADDRESS
     * \return  If there is next worker thread, the function will return valid pointer of
     *          next Worker Thread object and the address of thread is equal to address
     *          in int_out_threadAddress parameter on output. Otherwise, it returns NULL.
     **/
    inline WorkerThread * getNextWorkerThread( ThreadAddress & int_out_threadAddress );

//////////////////////////////////////////////////////////////////////////
// Member variables
//////////////////////////////////////////////////////////////////////////
private:
    /**
     * \brief   The address of Component associated with Component Info object
     **/
    ComponentAddress  mComponentAddress;

    /**
     * \brief   The Master Thread of Component (Component Thread)
     **/
    ComponentThread & mMasterThread;

#if defined(_MSC_VER) && (_MSC_VER > 1200)
    #pragma warning(disable: 4251)
#endif  // _MSC_VER
    /**
     * \brief   The map of Worker Threads binded with Component
     **/
    MapWorkerThread     mWorkerThreadMap;
#if defined(_MSC_VER) && (_MSC_VER > 1200)
    #pragma warning(default: 4251)
#endif  // _MSC_VER

//////////////////////////////////////////////////////////////////////////
// Hidden / Forbidden methods
//////////////////////////////////////////////////////////////////////////
private:
    ComponentInfo( void );
    ComponentInfo( const ComponentInfo & );
    const ComponentInfo & operator = ( const ComponentInfo & );
};

//////////////////////////////////////////////////////////////////////////
// ComponentInfo class inline functions implementation
//////////////////////////////////////////////////////////////////////////

inline ComponentThread& ComponentInfo::getMasterThread( void ) const
{
    return mMasterThread;
}

inline const ComponentAddress& ComponentInfo::getAddress( void ) const
{
    return mComponentAddress;
}

inline const String& ComponentInfo::getRoleName( void ) const
{
    return mComponentAddress.getRoleName();
}

inline bool ComponentInfo::isWorkerThreadAddress( const ThreadAddress& threadAddress ) const
{
    return mWorkerThreadMap.existResource(threadAddress);
}

inline WorkerThread* ComponentInfo::findWorkerThread( const ThreadAddress& threadAddress ) const
{
    return mWorkerThreadMap.findResourceObject(threadAddress);
}

inline WorkerThread* ComponentInfo::findWorkerThread( const char* threadName ) const
{
    Thread* targetThread = Thread::findThreadByName(threadName);
    return (targetThread != NULL ? findWorkerThread(targetThread->getAddress()) : static_cast<WorkerThread *>(NULL));
}

inline WorkerThread* ComponentInfo::findThreadByPath( const char* componentPath ) const
{
    ComponentAddress componentAddress = ComponentAddress::convPathToAddress(componentPath);
    return findWorkerThread(componentAddress.getThreadAddress());
}

inline WorkerThread* ComponentInfo::getFirstWorkerThread( ThreadAddress & out_threadAddress )
{
    return mWorkerThreadMap.resourceFirstKey(out_threadAddress);
}

inline WorkerThread* ComponentInfo::getNextWorkerThread( ThreadAddress & int_out_threadAddress )
{
    return mWorkerThreadMap.resourceNextKey(int_out_threadAddress);
}

inline bool ComponentInfo::hasWorkerThreads( void ) const
{
    return (mWorkerThreadMap.isEmpty() == false);
}

#endif  // AREG_COMPONENT_PRIVATE_COMPONENTINFO_HPP
