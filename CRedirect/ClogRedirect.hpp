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
#ifndef __CREDIRECT_CERR_REDIRECT_HPP_
#define __CREDIRECT_CERR_REDIRECT_HPP__

#include <CRedirect_config.h>
#ifdef LIB_CREDIRECT_ENABLE_CERR
#include <StreamRedirect.hpp>
#include <StreamObserver.hpp>
#include <iostream>
#include <streambuf>
#include <string>

LIB_CREDIRECT_NAMESPACE_BEGIN

class ClogRedirect {
public:
    /**
     * @brief Constructs a ClogRedirect instance that redirects std::clog.
     * 
     * This constructor initializes the ClogRedirect instance, setting up the necessary
     * stream redirection and observer notification mechanisms.
     */
    CREDIRECT_EXPORT
    ClogRedirect();

    /**
     * @brief Destroys the ClogRedirect instance and restores std::clog.
     * 
     * This destructor cleans up the resources used by the ClogRedirect instance,
     * restoring std::clog to its original state.
     */
    CREDIRECT_EXPORT
    ~ClogRedirect();
    
    /**
     * @brief Attaches an observer to the ClogRedirect instance.
     * 
     * This method allows an observer to be registered, which will receive notifications
     * whenever a new line is written to std::clog.
     * 
     * @param observer Pointer to the StreamObserver instance to attach.
     */
    CREDIRECT_EXPORT
    static void attach(StreamObserver* observer);

    /**
     * @brief Detaches an observer from the ClogRedirect instance.
     * 
     * This method allows an observer to be unregistered, stopping it from receiving
     * notifications of new lines written to std::clog.
     * 
     * @param observer Pointer to the StreamObserver instance to detach.
     */
    CREDIRECT_EXPORT
    static void detach(StreamObserver* observer);

private:
    /**
     * @brief Disables copy and move operations for the ClogRedirect class.
     * 
     * This prevents instances of ClogRedirect from being copied or moved,
     * ensuring that the redirection state remains consistent.
     */
    ClogRedirect(const ClogRedirect&) = delete;
    ClogRedirect& operator=(const ClogRedirect&) = delete;
    ClogRedirect(ClogRedirect&&) = delete;
    ClogRedirect& operator=(ClogRedirect&&) = delete;
    
    /**
     * @brief Pointer to the StreamRedirect instance that manages the redirection of std::clog.
     * 
     * This static member is used to access the StreamRedirect instance from static methods
     * without needing an instance of ClogRedirect.
     */
    static StreamRedirect* streamRedirect;
};

LIB_CREDIRECT_NAMESPACE_END

#endif // LIB_CREDIRECT_ENABLE_CERR
#endif // __CREDIRECT_CERR_REDIRECT_HPP__