/*
 * This file is part of libCRedirect.
 *
 * libCRedirect is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * libCRedirect is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with libCRedirect. If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright (C) Brian G Shea <bgshea@gmail.com>
 */
#include <CRedirect_config.h>
#include <SynchronousStreamBuf.hpp>

#include <atomic>
#include <condition_variable>
#include <cstring>
#include <mutex>
#include <vector>

LIB_CREDIRECT_NAMESPACE_BEGIN
/**
 * @file SynchronousStreamBuf.cpp
 * @brief Implementation of the SynchronousStreamBuf class.
 * * This file contains the implementation of the SynchronousStreamBuf class, which provides
 * functionality for a synchronous stream buffer that can be used to redirect output streams.
 * It allows for thread-safe reading and writing operations, with support for termination and synchronization.
 */

/**
 * @struct SynchronousStreamBuf::SynchronousStreamBufPimpl
 * @brief Private implementation (Pimpl) for the SynchronousStreamBuf class.
 * 
 * This structure encapsulates the internal details of the SynchronousStreamBuf class,
 * providing a mechanism to manage the stream buffer, synchronization, and termination.
 */
struct HIDDEN SynchronousStreamBuf::SynchronousStreamBufPimpl 
{
    SynchronousStreamBufPimpl() : terminated(false) {};
    ~SynchronousStreamBufPimpl() {};

    std::mutex mtx;
    //std::recursive_mutex mtx;
    std::condition_variable cv;
    std::vector<char> buffer;
    std::atomic<bool> terminated;
};

/**
 * @brief Constructor for the SynchronousStreamBuf class.
 * 
 * This constructor initializes the SynchronousStreamBuf instance with a specified initial buffer size.
 * It sets up the internal buffer and initializes the stream pointers.
 * 
 * @param initial_size The initial size of the buffer (default is 1024 bytes).
 */
SynchronousStreamBuf::SynchronousStreamBuf(std::streamsize initial_size) 
{
    d = new SynchronousStreamBufPimpl();

    d->buffer.resize(initial_size);

    setp(d->buffer.data(), d->buffer.data() + d->buffer.size() - 1);
    setg(d->buffer.data(), d->buffer.data(), d->buffer.data());
}

/**
 * @brief Destructor for the SynchronousStreamBuf class.
 * 
 * This destructor cleans up the resources used by the SynchronousStreamBuf instance,
 * ensuring that any pending data is synchronized and the buffer is properly terminated.
 */
SynchronousStreamBuf::~SynchronousStreamBuf() 
{
    sync();
    terminate();
    delete d;
}

/**
 * @brief Terminates the SynchronousStreamBuf instance.
 * 
 * This method marks the stream buffer as terminated, allowing any waiting threads to wake up
 * and handle the termination condition. It also notifies all waiting threads that the buffer
 * has been terminated.
 */
void SynchronousStreamBuf::terminate() 
{
    std::lock_guard<std::mutex> lock(d->mtx);
    d->terminated = true;
    d->cv.notify_all();
}

/**
 * @brief Underflow function for the SynchronousStreamBuf class.
 * 
 * This method is called when the stream buffer needs more data to read. It waits for data to be available
 * or for the stream to be terminated. If data is available, it returns the next character; otherwise, it returns EOF.
 * 
 * @return The next character in the stream or EOF if the stream is terminated.
 */
std::streambuf::int_type SynchronousStreamBuf::underflow() 
{
    std::unique_lock<std::mutex> lock(d->mtx);
    if(d->terminated) {
        return traits_type::eof();
    }

    d->cv.wait(lock,
        [this]
        {
            auto a = gptr();
            auto b = egptr();
            return (a != b) || d->terminated; 
        }
    );

    if (d->terminated) {
        return traits_type::eof();
    }

    return traits_type::to_int_type(*gptr());
}

/**
 * @brief Overflow function for the SynchronousStreamBuf class.
 * 
 * This method is called when the stream buffer is full and needs to write data. It handles the overflow condition
 * by resizing the buffer if necessary and writing the character to the buffer. It then synchronizes the buffer
 * and returns the character written or EOF if an error occurs.
 * 
 * @param ch The character to write to the stream buffer.
 * @return The character written or EOF if an error occurs.
 */
std::streambuf::int_type SynchronousStreamBuf::overflow(int_type ch)
{
    {
        std::lock_guard<std::mutex> lock(d->mtx);
        if (ch != traits_type::eof()) {
            if (pptr() == epptr()) {
                // Buffer is full, resize it
                std::streamsize old_size = d->buffer.size();
                d->buffer.resize(old_size * 2);
                setp(d->buffer.data(), d->buffer.data() + d->buffer.size() - 1);
                pbump(old_size); // Move the put pointer to the correct position
            }
            *pptr() = ch;
            pbump(1);
        }
    }
    return sync() == 0 ? ch : traits_type::eof();
}

/**
 * @brief Synchronizes the SynchronousStreamBuf instance.
 * 
 * This method flushes the current contents of the stream buffer, moving any data from the get area to the put area.
 * It resizes the internal buffer if necessary and notifies any waiting threads that new data is available.
 * 
 * @return 0 on success, or -1 if the stream has been terminated.
 */
int SynchronousStreamBuf::sync() 
{
    std::lock_guard<std::mutex> lock(d->mtx);
    
    if(d->terminated) {
        return -1;
    }

    if (pbase() != pptr()) {
        std::streamsize psize = pptr() - pbase();
        std::streamsize gsize = egptr() - gptr();
        
        if(d->buffer.size() < psize + gsize) {
            d->buffer.resize(psize+gsize);
        }

        std::memmove(d->buffer.data(), gptr(), gsize);
        std::memcpy(d->buffer.data() + gsize, pbase(), psize);

        setg(d->buffer.data(), d->buffer.data(), d->buffer.data() + gsize + psize);
        setp(d->buffer.data() + gsize + psize, d->buffer.data() + d->buffer.size());

        d->cv.notify_all(); // Notify waiting threads that new data is available
    }
    return 0;
}

LIB_CREDIRECT_NAMESPACE_END