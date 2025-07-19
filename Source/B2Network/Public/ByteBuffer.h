#pragma once

#include "Config.h"

namespace b2network {

template<int Page=4096, int Min=1, int Max=1024>
class ByteBuffer
{
public:
    static const int PageSize = Page;
    static const int MinSize = (Page * Min);
    static const int MaxSize = (Page * Max);

private:
    char* m_headIndex;
    char* m_readIndex;
    char* m_writeIndex;
    char* m_tailIndex;

public:
    ByteBuffer()
    {
        const auto size = Page * Min;

        m_headIndex = new char[size];
        m_readIndex = m_headIndex;
        m_writeIndex = m_readIndex;
        m_tailIndex = m_writeIndex + size;
    }

    ~ByteBuffer()
    {
        delete [] m_headIndex;
    }

    ByteBuffer(const ByteBuffer&) = delete;
    ByteBuffer& operator=(const ByteBuffer&) = delete;

    std::size_t Space() const
    {
        return m_readIndex - m_headIndex;
    }

    std::size_t Size() const
    {
        return m_writeIndex - m_readIndex;
    }

    std::size_t Capacity() const
    {
        return m_tailIndex - m_writeIndex;
    }

    std::size_t AllocSize() const
    {
        return m_tailIndex - m_headIndex;
    }

    const char* ReadPtr()
    {
        return m_readIndex;
    }

    char* WritePtr()
    {
        return m_writeIndex;
    }

    void Read(char* data, std::size_t length)
    {
        if (length > Size())
        {
#if defined(B2NETWORK_PLATFORM_WINDOWS)
            throw ByteBufferException("Read Overflow");
#elif defined(B2NETWORK_PLATFORM_ANDROID)
            // TODO fix
            checkf(false, TEXT("Buffer Read Overflow"));
#elif defined(B2NETWORK_PLATFORM_IOS)
            // TODO fix
            checkf(false, TEXT("Buffer Read Overflow"));
#else
#error Platform not supported
#endif
        }

        memcpy(data, m_readIndex, length);
        m_readIndex += length;

        CheckValid();
    }

    void Write(const char* data, std::size_t length)
    {
        if (length > Capacity())
        {
            Reserve(length);
        }

        memcpy(m_writeIndex, data, length);
        m_writeIndex += length;

        CheckValid();
    }

    void Clear()
    {
        m_readIndex = m_headIndex;
        m_writeIndex = m_headIndex;
    }

    enum SeekType { Rd, Wr };

    void Seek(SeekType type, int offset)
    {
        switch (type)
        {
        case SeekType::Rd:
            m_readIndex += offset;
            break;

        case SeekType::Wr:
            m_writeIndex += offset;
            break;

        default:
#if defined(B2NETWORK_PLATFORM_WINDOWS)
            throw ByteBufferException("Invalid Seek Type");
#elif defined(B2NETWORK_PLATFORM_ANDROID)
            checkf(false, TEXT("Invalid Seek Type"));
            break;
#elif defined(B2NETWORK_PLATFORM_IOS)
            checkf(false, TEXT("Invalid Seek Type"));
            break;
#else
#error Platform not supported
#endif
        }

        CheckValid();
    }

    bool Reserve(std::size_t capacity)
    {
        if (Capacity() < capacity)
        {
            if (Space() + Capacity() < capacity)
            {
                ReallocIndex(capacity);
            }
            else
            {
                DiscardSpace();
            }

            return true;
        }

        return false;
    }

    bool Shrink(const size_t threshold)
    {
        if (Size() == 0 && (AllocSize() > MinSize))
        {
            ReallocIndex(MinSize);
            return true;
        }

        return false;
    }

    std::string ToString() const
    {
        std::ostringstream oss;
        oss << "ByteBuffer("
            << "Page=" << PageSize << ","
            << "Min=" << MinSize << ","
            << "Max=" << MaxSize << ","
            << "size=" << Size() << ","
            << "space=" << Space() << ","
            << "capacity=" << Capacity() << ","
            << "length=" << AllocSize() << ")";

        return oss.str();
    }

private:
    bool CheckValid() const
    {
        return (m_tailIndex >= m_writeIndex) && (m_writeIndex >= m_readIndex) &&
               (m_readIndex >= m_headIndex);
    }

    void DiscardSpace()
    {
        const auto size = Size();

#if defined(B2NETWORK_PLATFORM_WINDOWS)
        std::memmove(m_headIndex, m_readIndex, size);
#elif defined(B2NETWORK_PLATFORM_ANDROID)
        memmove(m_headIndex, m_readIndex, size);
#elif defined(B2NETWORK_PLATFORM_IOS)
        memmove(m_headIndex, m_readIndex, size);
#else
#error Platform not supported
#endif

        m_readIndex = m_headIndex;
        m_writeIndex = m_readIndex + size;
    }

    void ReallocIndex(size_t capacity)
    {
        const int size = Size();

        size_t allocSize = MinSize;
        while (true)
        {
            if (allocSize > MaxSize)
            {
#if defined(B2NETWORK_PLATFORM_WINDOWS)
                throw ByteBufferException("Realloc Exceeds Limit");
#elif defined(B2NETWORK_PLATFORM_ANDROID)
                checkf(false, TEXT("Realloc Exceeds Limit capacity=%d, allocSize=%d, MaxSize=%d"),
                    capacity, allocSize, MaxSize);
#elif defined(B2NETWORK_PLATFORM_IOS)
                checkf(false, TEXT("Realloc Exceeds Limit capacity=%d, allocSize=%d, MaxSize=%d"),
                    capacity, allocSize, MaxSize);
#else
#error Platform not supported
#endif
            }
            else if (allocSize >= size + capacity)
            {
                break;
            }
            else
            {
                allocSize += PageSize;
            }
        }

        auto headIndex = new char[allocSize];
        memcpy(headIndex, m_readIndex, size);

        delete[] m_headIndex;

        m_headIndex = headIndex;
        m_readIndex = m_headIndex;
        m_writeIndex = m_readIndex + size;
        m_tailIndex = m_headIndex + allocSize;
    }
};

} // namespace b2network
