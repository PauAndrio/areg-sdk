/************************************************************************
 * \file        areg/base/private/RemoteMessage.cpp
 * \ingroup     AREG Asynchronous Event-Driven Communication Framework
 * \author      Artak Avetyan (mailto:artak@aregtech.com)
 * \brief       Remote Shared Buffer class. This Buffer is used for 
 *              Read and Write of data during remote communication
 *              between different processes running either on same or 
 *              different hosts. The Data in buffer within same process
 *              remains valid until  the number of references to the 
 *              buffer is more than zero.
 *
 ************************************************************************/
#include "areg/base/RemoteMessage.hpp"

#include "areg/base/NEMemory.hpp"
#include "areg/base/NEMath.hpp"
#include "areg/trace/GETrace.h"

inline unsigned int RemoteMessage::_checksumCalculate( const NEMemory::sRemoteMessage & remoteMessage )
{
    unsigned int result = NEMemory::INVALID_VALUE;

    if ( &remoteMessage != &NEMemory::InvalidRemoteMessage )
    {
        const unsigned int offset   = MACRO_OFFSETOF(NEMemory::sRemoteMessageHeader, rbhSource);
        const unsigned char * data  = reinterpret_cast<const unsigned char *>(&remoteMessage.rbHeader.rbhSource);
        const unsigned int remain   = remoteMessage.rbHeader.rbhBufHeader.biOffset - offset;
        const unsigned int used     = remoteMessage.rbHeader.rbhBufHeader.biUsed;
        const unsigned int length   = used + remain;

        result = NEMath::crc32Calculate(data, static_cast<int>(length));
    }

    return result;
}

RemoteMessage::RemoteMessage(unsigned int blockSize /*= NEMemory::BLOCK_SIZE*/)
    : SharedBuffer  ( blockSize )
{
    ; // do nothing
}

RemoteMessage::RemoteMessage(unsigned int reserveSize, unsigned int blockSize /*= NEMemory::BLOCK_SIZE*/)
    : SharedBuffer  ( blockSize )
{
    resize(reserveSize, false);
}

RemoteMessage::RemoteMessage(const unsigned char * buffer, unsigned int size, unsigned int blockSize /*= NEMemory::BLOCK_SIZE*/)
    : SharedBuffer  ( blockSize )
{
    resize(size, false);
    writeData(buffer, size);
}

RemoteMessage::RemoteMessage(NEMemory::sRemoteMessage & remoteMessage)
    : SharedBuffer    ( reinterpret_cast<NEMemory::sByteBuffer &>(remoteMessage) )
{
    ; // do nothing
}

RemoteMessage::RemoteMessage( const RemoteMessage & src )
    : SharedBuffer    ( static_cast<const SharedBuffer &>(src) )
{
    ; // do nothing
}

RemoteMessage::~RemoteMessage(void)
{
    ; // do nothing
}

const RemoteMessage & RemoteMessage::operator = ( const RemoteMessage & src )
{
    static_cast<SharedBuffer &>(*this) = static_cast<const SharedBuffer &>(src);
    return (*this);
}

unsigned int RemoteMessage::initBuffer(unsigned char *newBuffer, unsigned int bufLength, bool makeCopy) const
{
    unsigned int result = IECursorPosition::INVALID_CURSOR_POSITION;

    if (newBuffer != NULL)
    {
        result                  = 0;
        unsigned int dataOffset = getDataOffset();
        unsigned int dataLength = bufLength - dataOffset;

        NEMemory::zeroBuffer(newBuffer, sizeof(NEMemory::sRemoteMessage));
        NEMemory::sRemoteMessageHeader & header = NEMemory::constructElems<NEMemory::sRemoteMessage>(newBuffer, 1)->rbHeader;
        header.rbhBufHeader.biBufSize   = bufLength;
        header.rbhBufHeader.biLength    = dataLength;
        header.rbhBufHeader.biOffset    = dataOffset;
        header.rbhBufHeader.biBufType   = NEMemory::BufferRemote;
        header.rbhBufHeader.biRefCount  = 1;

        if ( makeCopy )
        {
            unsigned char* data         = NEMemory::getBufferDataWrite(reinterpret_cast<NEMemory::sByteBuffer &>(header.rbhBufHeader));
            const unsigned char* srcBuf = NEMemory::getBufferDataRead(mByteBuffer);
            unsigned int srcCount       = getSizeUsed();
            srcCount                    = MACRO_MIN(srcCount, dataLength);
            result                      = srcCount;

            header.rbhBufHeader.biUsed  = srcCount;
            header.rbhTarget            = getTarget();
            header.rbhChecksum          = getChecksum();
            header.rbhSource            = getSource();
            header.rbhMessageId         = getMessageId();
            header.rbhResult            = getResult();
            header.rbhSequenceNr        = getSequenceNr();

            NEMemory::memCopy(data, static_cast<int>(dataLength), srcBuf, static_cast<int>(srcCount));
        }
        else
        {
            header.rbhBufHeader.biUsed  = 0;
            header.rbhTarget            = NEMemory::IGNORE_VALUE;
            header.rbhChecksum          = NEMemory::IGNORE_VALUE;
            header.rbhSource            = NEMemory::IGNORE_VALUE;
            header.rbhMessageId         = NEMemory::IGNORE_VALUE;
            header.rbhResult            = NEMemory::ResultSucceed;
            header.rbhSequenceNr        = NEMemory::IGNORE_VALUE;;
        }
    }
    else
    {
        ; // do nothing
    }

    return result;
}

bool RemoteMessage::isChecksumValid(void) const
{
    return isValid() ? getChecksum() == RemoteMessage::_checksumCalculate( _getRemoteMessage() ) : false;
}

DEF_TRACE_SCOPE(areg_base_RemoteMessage_bufferCompletionFix);
void RemoteMessage::bufferCompletionFix(void)
{
    TRACE_SCOPE(areg_base_RemoteMessage_bufferCompletionFix);

    if ( isValid() )
    {
        NEMemory::sRemoteMessage & msg = _getRemoteMessage();
        NEMemory::sRemoteMessageHeader & header = msg.rbHeader;

        unsigned int checksum   = RemoteMessage::_checksumCalculate( msg );
        unsigned int dataUsed   = header.rbhBufHeader.biUsed;
        unsigned int dataLen    = header.rbhBufHeader.biUsed;
        unsigned int bufSize    = header.rbhBufHeader.biOffset + dataUsed;

        dataLen = MACRO_MAX(dataLen, sizeof(NEMemory::BufferData));
        dataLen = MACRO_ALIGN_SIZE(dataLen, sizeof(int));

        bufSize = MACRO_MAX(bufSize, sizeof(NEMemory::sRemoteMessage));
        bufSize = MACRO_ALIGN_SIZE(bufSize, sizeof(int));

        ASSERT(dataLen <= header.rbhBufHeader.biLength);

        header.rbhBufHeader.biBufSize   = bufSize;
        header.rbhBufHeader.biLength    = dataLen;
        header.rbhChecksum              = checksum;

        TRACE_INFO("Remote message completion: bufSize [ %u ], msg.biBufSize = [ %u ]; dataLen [ %u ], msg.biLength = [ %u ], dataUsed [ %u ], checksum [ %u ]"
                        , bufSize, header.rbhBufHeader.biBufSize
                        , dataLen, header.rbhBufHeader.biLength
                        , dataUsed, checksum);
    }
}

unsigned char * RemoteMessage::initMessage(const NEMemory::sRemoteMessageHeader & rmHeader )
{
    decreaseLock();

    unsigned int sizeUsed   = MACRO_MAX(rmHeader.rbhBufHeader.biUsed, 1);
    unsigned int sizeData   = MACRO_ALIGN_SIZE(sizeUsed, mBlockSize);
    unsigned int sizeBuffer = getHeaderSize() + sizeData;
    unsigned char * result  = DEBUG_NEW unsigned char[sizeBuffer];
    if ( result != NULL )
    {
        NEMemory::zeroBuffer(result, sizeof(NEMemory::sRemoteMessage));
        NEMemory::sRemoteMessage * msg      = NEMemory::constructElems<NEMemory::sRemoteMessage>(result, 1);
        NEMemory::sRemoteMessageHeader & dst= msg->rbHeader;
        dst.rbhBufHeader.biBufSize  = sizeBuffer;
        dst.rbhBufHeader.biLength   = sizeData;
        dst.rbhBufHeader.biOffset   = getDataOffset();
        dst.rbhBufHeader.biBufType  = NEMemory::BufferRemote;
        dst.rbhBufHeader.biUsed     = rmHeader.rbhBufHeader.biUsed;
        dst.rbhBufHeader.biRefCount = 1;
        dst.rbhTarget               = rmHeader.rbhTarget;
        dst.rbhChecksum             = rmHeader.rbhChecksum;
        dst.rbhSource               = rmHeader.rbhSource;
        dst.rbhMessageId            = rmHeader.rbhMessageId;
        dst.rbhResult               = rmHeader.rbhResult;
        dst.rbhSequenceNr           = rmHeader.rbhSequenceNr;
        msg->rbData[0]              = static_cast<NEMemory::BufferData>(0);

        mByteBuffer = reinterpret_cast<NEMemory::sByteBuffer *>(msg);
    }

    return getBuffer();
}

unsigned int RemoteMessage::getDataOffset(void) const
{
    return sizeof(NEMemory::sRemoteMessageHeader);
}

unsigned int RemoteMessage::getHeaderSize(void) const
{
    return sizeof(NEMemory::sRemoteMessage);
}