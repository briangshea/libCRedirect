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
#include <ClogRedirect.hpp>
#include <StreamRedirect.hpp>
#include <StreamObserver.hpp>
#include <SynchronousStreamBuf.hpp>

#include <algorithm>
#include <atomic>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <vector>
#include <condition_variable>

LIB_CREDIRECT_NAMESPACE_BEGIN
/**
 * @file ClogRedirect.cpp
 * @brief Implementation of the ClogRedirect class for redirecting std::clog.
 * 
 * This file contains the implementation of the ClogRedirect class, which provides
 * functionality to redirect the standard log stream (std::clog) to a custom
 * stream buffer. It allows observers to be notified of any output written to std::clog.
 */

/**
 * @brief Static pointer to the StreamRedirect instance that manages the redirection of std::clog.
 *
 * This static member is used to access the StreamRedirect instance from static methods
 * without needing an instance of CerrRedirect. It ensures that there is a single instance
 * of StreamRedirect managing the redirection of std::cerr throughout the application.
 */
StreamRedirect* ClogRedirect::streamRedirect = nullptr;

/**
 * @brief Constructor for the ClogRedirect class.
 * 
 * This constructor initializes the ClogRedirect instance by creating a new
 * ClogRedirectPimpl object, redirecting std::clog to a custom stream buffer,
 * and starting a monitoring thread to process output from the stream.
 */
ClogRedirect::ClogRedirect() { 
    static std::mutex initMutex;
    std::lock_guard<std::mutex> lock(initMutex);
    // Ensure that the ClogRedirectPimpl instance is created only once
    if(nullptr == streamRedirect) {
        streamRedirect = new StreamRedirect(std::clog);
    }
}

/**
 * @brief Destructor for the ClogRedirect class.
 * 
 * This destructor cleans up the resources used by the ClogRedirect instance,
 * restoring std::clog to its original state and stopping the monitoring thread.
 */
ClogRedirect::~ClogRedirect() {
    static std::mutex cleanupMutex;
    std::lock_guard<std::mutex> lock(cleanupMutex);
    // Ensure that the ClogRedirectPimpl instance is deleted only once
    if(streamRedirect) {
        delete streamRedirect;
    }
}

/**
 * @brief Attaches an observer to the ClogRedirect instance.
 * 
 * This method allows an observer to be attached to the ClogRedirect instance,
 * enabling it to receive notifications about new lines written to std::clog.
 * 
 * @param observer Pointer to the StreamObserver instance to attach.
 */
void ClogRedirect::attach(StreamObserver* observer) {
    streamRedirect->attach(observer);
}

/**
 * @brief Detaches an observer from the ClogRedirect instance.
 * 
 * This method allows an observer to be removed from the ClogRedirect instance,
 * stopping it from receiving further notifications about new lines written to std::clog.
 * 
 * @param observer Pointer to the StreamObserver instance to detach.
 */
void ClogRedirect::detach(StreamObserver* observer) {
    streamRedirect->detach(observer);
}

#ifdef LIB_CREDIRECT_AUTOSTART_CLOG
/**
 * @brief Automatically starts the ClogRedirect instance if LIB_CREDIRECT_AUTOSTART_CLOG is defined.
 * 
 * This function is called to ensure that the ClogRedirect instance is created and started
 * automatically when the library is used, without requiring explicit user intervention.
 */
static ClogRedirect& autostartClog{};
#endif // LIB_CREDIRECT_AUTOSTART_CLOG

LIB_CREDIRECT_NAMESPACE_END