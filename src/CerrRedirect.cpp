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
#ifdef LIB_CREDIRECT_ENABLE_CERR
#include <CerrRedirect.hpp>
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
 * @file CerrRedirect.cpp
 * @brief Implementation of the CerrRedirect class for redirecting std::cerr.
 * 
 * This file contains the implementation of the CerrRedirect class, which provides
 * functionality to redirect the standard error stream (std::cerr) to a custom
 * stream buffer. It allows observers to be notified of any output written to std::cerr.
 */

/**
 * @brief Static pointer to the StreamRedirect instance that manages the redirection of std::cerr.
 *
 * This static member is used to access the StreamRedirect instance from static methods
 * without needing an instance of CerrRedirect. It ensures that there is a single instance
 * of StreamRedirect managing the redirection of std::cerr throughout the application.
 */
 StreamRedirect* CerrRedirect::streamRedirect = nullptr;

/**
 * @brief Constructor for the CerrRedirect class.
 * 
 * This constructor initializes the CerrRedirect instance, setting up the custom stream buffer
 * and redirecting std::cerr to it. It also starts a monitoring thread to process output from the stream.
 */
CerrRedirect::CerrRedirect() {
    static std::mutex initMutex;
    std::lock_guard<std::mutex> lock(initMutex);
    // Ensure that the static instance is created only once
    if(nullptr == streamRedirect) {
        streamRedirect = new StreamRedirect(std::cerr);
    }
}

/**
 * @brief Destructor for the CerrRedirect class.
 * 
 * This destructor cleans up the resources used by the CerrRedirect instance,
 * restoring std::cerr to its original state and stopping the monitoring thread.
 */
CerrRedirect::~CerrRedirect() {
    static std::mutex cleanupMutex;
    std::lock_guard<std::mutex> lock(cleanupMutex);
    // Ensure that the static instance is cleaned up only once
    if(streamRedirect) {
        delete streamRedirect;
    }
}

/**
 * @brief Attaches an observer to the CerrRedirect instance.
 * 
 * This method allows an observer to be attached to the CerrRedirect instance,
 * enabling it to receive notifications about new lines written to std::cerr.
 * 
 * @param observer Pointer to the StreamObserver instance to attach.
 */
void CerrRedirect::attach(StreamObserver* observer) {
    streamRedirect->attach(observer);
}

/**
 * @brief Detaches an observer from the CerrRedirect instance.
 * 
 * This method allows an observer to be detached from the CerrRedirect instance,
 * stopping it from receiving further notifications about new lines written to std::cerr.
 * 
 * @param observer Pointer to the StreamObserver instance to detach.
 */
void CerrRedirect::detach(StreamObserver* observer) {
    streamRedirect->detach(observer);
}

#ifdef LIB_CREDIRECT_AUTOSTART_CERR
/**
 * @brief Automatically starts the CerrRedirect instance if LIB_CREDIRECT_AUTOSTART_CERR is defined.
 * 
 * This function is called to ensure that the CerrRedirect instance is created and started
 * automatically when the library is used, without requiring explicit user intervention.
 */
static CerrRedirect autostartCerr{};
#endif // LIB_CREDIRECT_AUTOSTART_CERR

LIB_CREDIRECT_NAMESPACE_END
#endif