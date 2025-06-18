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
#ifndef __CREDIRECT_SYNCHRONOUSSTREAMBUF_HPP__
#define __CREDIRECT_SYNCHRONOUSSTREAMBUF_HPP__
#include <CRedirect_config.h>
#include <memory>
#include <sstream>

LIB_CREDIRECT_NAMESPACE_BEGIN

class HIDDEN SynchronousStreamBuf : public std::basic_streambuf<char> {
public:
    /**
     * @brief Constructor for the SynchronousStreamBuf class.
     * 
     * This constructor initializes the SynchronousStreamBuf instance with a specified initial buffer size.
     * It sets up the internal buffer and initializes the stream pointers.
     * 
     * @param initial_size The initial size of the buffer (default is LIB_CREDIRECT_INITIAL_BUFFER_SIZE).
     */
    SynchronousStreamBuf(std::streamsize initial_size = LIB_CREDIRECT_INITIAL_BUFFER_SIZE);
    
    /**
     * @brief Destructor for the SynchronousStreamBuf class.
     * 
     * This destructor cleans up the resources used by the SynchronousStreamBuf instance,
     * ensuring that any allocated memory is properly released and the stream buffer is terminated.
     */
    ~SynchronousStreamBuf();

    /**
     * @brief Terminates the SynchronousStreamBuf instance.
     * 
     * This method marks the stream buffer as terminated, allowing any waiting threads to wake up
     * and handle the termination condition. It also notifies all waiting threads that the buffer
     * has been terminated.
     */
    void terminate();

protected:
    /**
     * @brief Underflow function for the SynchronousStreamBuf class.
     * 
     * This method is called when the stream buffer needs more data to read. It waits for data to be available
     * or for the stream to be terminated. If data is available, it returns the next character; otherwise, it returns EOF.
     * 
     * @return The next character in the stream or EOF if the stream is terminated.
     */
    int_type underflow() override;

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
    int_type overflow(int_type ch) override;

    /**
     * @brief Synchronizes the SynchronousStreamBuf instance.
     * 
     * This method flushes the current contents of the stream buffer, moving any data from the get area to the put area.
     * It resizes the internal buffer if necessary and notifies any waiting threads that new data is available.
     * 
     * @return 0 on success, or -1 if the stream has been terminated.
     */
    int sync();

private:
    /**
     * @brief Deleted copy and move constructors and assignment operators.
     * 
     * These methods are deleted to prevent copying or moving of the SynchronousStreamBuf instance,
     * ensuring that it is only used as a unique instance. This is important for thread safety and
     */
    SynchronousStreamBuf(const SynchronousStreamBuf&) = delete;
    SynchronousStreamBuf& operator=(const SynchronousStreamBuf&) = delete;
    SynchronousStreamBuf(SynchronousStreamBuf&&) = delete;
    SynchronousStreamBuf& operator=(SynchronousStreamBuf&&) = delete;

    /**
     * @struct SynchronousStreamBufPimpl
     * @brief Private implementation (Pimpl) for the SynchronousStreamBuf class.
     * 
     * This structure encapsulates the internal details of the SynchronousStreamBuf class,
     * providing a mechanism to manage the stream buffer, synchronization, and termination.
     */
    struct SynchronousStreamBufPimpl;
    struct SynchronousStreamBufPimpl* d;
};

LIB_CREDIRECT_NAMESPACE_END

#endif // __CREDIRECT_SYNCHRONOUSSTREAMBUF_HPP__