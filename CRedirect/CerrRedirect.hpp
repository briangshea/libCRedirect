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
#include <string>

LIB_CREDIRECT_NAMESPACE_BEGIN

/**
 * @class CerrRedirect
 * @brief Redirects std::cerr to a custom stream buffer and notifies observers.
 * 
 * This class provides functionality to redirect the standard error stream (std::cerr)
 * to a custom stream buffer, allowing for monitoring and processing of error messages.
 * It supports attaching and detaching observers that will be notified of new lines written
 * to the redirected stream.
 */
class CerrRedirect {
public:

    /**
     * @brief Constructs a CerrRedirect instance that redirects std::cerr.
     * 
     * This constructor initializes the CerrRedirect instance, setting up the necessary
     * stream redirection and observer notification mechanisms.
     */
    CerrRedirect();

    /**
     * @brief Destroys the CerrRedirect instance and restores std::cerr.
     * 
     * This destructor cleans up the resources used by the CerrRedirect instance,
     * restoring std::cerr to its original state.
     */
    ~CerrRedirect();

    /**
     * @brief Attaches an observer to the CerrRedirect instance.
     * 
     * This method allows an observer to be registered, which will receive notifications
     * whenever a new line is written to std::cerr.
     * 
     * @param observer Pointer to the StreamObserver instance to attach.
     */
    
    static void attach(StreamObserver* observer);
    /**
     * @brief Detaches an observer from the CerrRedirect instance.
     * 
     * This method allows an observer to be unregistered, stopping it from receiving
     * notifications of new lines written to std::cerr.
     * 
     * @param observer Pointer to the StreamObserver instance to detach.
     */
    static void detach(StreamObserver* observer);

private:    
    /**
     * @brief Disables copy and move operations for the CerrRedirect class.
     * 
     * This prevents instances of CerrRedirect from being copied or moved,
     * ensuring that the redirection state remains consistent.
     */
    CerrRedirect(const CerrRedirect&) = delete;
    CerrRedirect& operator=(const CerrRedirect&) = delete;
    CerrRedirect(CerrRedirect&&) = delete;
    CerrRedirect& operator=(CerrRedirect&&) = delete;
    /**
     * @brief Monitors the std::cerr stream for new lines.
     * 
     * This method is responsible for continuously monitoring the std::cerr stream
     * and notifying observers whenever a new line is written to it.
     */
    void monitorStream();
    /**
     * @brief Pointer to the StreamRedirect instance that manages the redirection of std::cerr.
     * 
     * This static member is used to access the StreamRedirect instance from static methods
     * without needing an instance of CerrRedirect.
     */
    static StreamRedirect* streamRedirect;
};

LIB_CREDIRECT_NAMESPACE_END

#endif // LIB_CREDIRECT_ENABLE_CERR
#endif // __CREDIRECT_CERR_REDIRECT_HPP__