/************************************************************************
 * \file        areg/ipc/private/SocketConnectionBase.cpp
 * \ingroup     AREG Asynchronous Event-Driven Communication Framework
 * \author      Artak Avetyan (mailto:artak@aregtech.com)
 * \brief       AREG Platform Connection base class declaration.
 ************************************************************************/

#include "areg/ipc/private/SocketConnectionBase.hpp"
#include "areg/base/Socket.hpp"
#include "areg/base/RemoteMessage.hpp"
#include "areg/base/NEMemory.hpp"

#include "areg/trace/GETrace.h"
DEF_TRACE_SCOPE(areg_ipc_private_SocketConnectionBase_sendMessage);
DEF_TRACE_SCOPE(areg_ipc_private_SocketConnectionBase_receiveMessage);

SocketConnectionBase::SocketConnectionBase( void )
{
    ; // do nothing
}

SocketConnectionBase::~SocketConnectionBase( void )
{

}

int SocketConnectionBase::sendMessage(const RemoteMessage & in_message, const Socket & clientSocket) const
{
    TRACE_SCOPE(areg_ipc_private_SocketConnectionBase_sendMessage);

    int result = -1;
    const NEMemory::sRemoteMessageHeader & buffer = reinterpret_cast<const NEMemory::sRemoteMessageHeader &>( in_message.getByteBuffer() );

    TRACE_DBG("Sending message with ID [ %p ] from source [ %p ] to target [ %p ] via client socket [ %u ] to address [ %s : %d ]. Buffer length [ %u ], used data [ %u ], data lengt [ %u ], checksum [ %u ]"
                    , static_cast<id_type>(in_message.getMessageId())
                    , static_cast<id_type>(in_message.getSource())
                    , static_cast<id_type>(in_message.getTarget())
                    , static_cast<unsigned int>(clientSocket.getHandle())
                    , clientSocket.getAddress().getHostAddress().getString()
                    , clientSocket.getAddress().getHostPort()
                    , buffer.rbhBufHeader.biBufSize
                    , buffer.rbhBufHeader.biUsed
                    , buffer.rbhBufHeader.biLength
                    , buffer.rbhChecksum);

    if ( clientSocket.isValid() )
    {
        // result = clientSocket.SendData( reinterpret_cast<const unsigned char *>(&buffer), buffer.rbhBufHeader.biBufSize );
        result = clientSocket.sendData( reinterpret_cast<const unsigned char *>(&buffer), sizeof(NEMemory::sRemoteMessageHeader) );
        if (buffer.rbhBufHeader.biUsed != 0)
        {
            ASSERT(buffer.rbhBufHeader.biLength >= buffer.rbhBufHeader.biUsed);
            TRACE_DBG("Sending [ %d ] bytes of aligned data, used size [ %d ]", buffer.rbhBufHeader.biLength, buffer.rbhBufHeader.biUsed);
            // send the aligned length.
            result += clientSocket.sendData(in_message.getBuffer(), buffer.rbhBufHeader.biLength);
        }

        TRACE_DBG("Sent [ %d ] bytes of data. The remote buffer size is [ %u ], checksum [ %u ]", result, buffer.rbhBufHeader.biBufSize, buffer.rbhChecksum);
    }

    return result;
}

int SocketConnectionBase::receiveMessage(RemoteMessage & out_message, const Socket & clientSocket) const
{
    TRACE_SCOPE(areg_ipc_private_SocketConnectionBase_receiveMessage);

    int result = -1;
    if ( clientSocket.isValid() )
    {
        NEMemory::sRemoteMessageHeader msgHeader;
        DBG_ZERO_MEM(&msgHeader, sizeof(NEMemory::sRemoteMessageHeader));

        out_message.invalidate();
        if ( clientSocket.receiveData(reinterpret_cast<unsigned char *>(&msgHeader), sizeof(NEMemory::sRemoteMessageHeader)) == sizeof(NEMemory::sRemoteMessageHeader) )
        {
            TRACE_DBG("Going to receive remote message data of ID [ %p ] from target [ %p ] for source [ %p ], buffer length [ %u ], used data length [ %u ], checksum [ %u ]"
                                , static_cast<id_type>(msgHeader.rbhMessageId)
                                , static_cast<id_type>(msgHeader.rbhTarget)
                                , static_cast<id_type>(msgHeader.rbhSource)
                                , msgHeader.rbhBufHeader.biBufSize
                                , msgHeader.rbhBufHeader.biUsed
                                , msgHeader.rbhChecksum);

            result = sizeof(NEMemory::sRemoteMessageHeader);
            unsigned char * buffer = out_message.initMessage( msgHeader );
            if ( buffer != NULL && msgHeader.rbhBufHeader.biUsed > 0)
            {
                ASSERT(msgHeader.rbhBufHeader.biLength <= out_message.getSizeAvailable());
                ASSERT(msgHeader.rbhBufHeader.biLength >= msgHeader.rbhBufHeader.biUsed);

                TRACE_DBG("Receiving [ %d ] bytes of aligned data, used size [ %d ]", msgHeader.rbhBufHeader.biLength, msgHeader.rbhBufHeader.biUsed);
                // receive aligned length of data.
                result += clientSocket.receiveData(buffer, msgHeader.rbhBufHeader.biLength);
            }

            out_message.moveToBegin();
            if ( out_message.isChecksumValid() == false )
            {
                TRACE_DBG("Received [ %u ] bytes of remote message, but checksum is invalid, ignoring and invalidating message", result);
                result = 0;
                out_message.invalidate();
            }
            else
            {
                TRACE_DBG("Received [ %u ] bytes of remote message, checksum is valid, going to process", result);
            }
        }
        else
        {
            TRACE_WARN("Failed to receive remote message data. Probably the connection is closed");
        }
    }
    return result;
}
