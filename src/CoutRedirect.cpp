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
#include <CoutRedirect.hpp>
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
 * @file CoutRedirect.cpp
 * @brief Implementation of the CoutRedirect class for redirecting std::cout.
 * 
 * This file contains the implementation of the CoutRedirect class, which provides
 * functionality to redirect the standard output stream (std::cout) to a custom
 * stream buffer. It allows observers to be notified of any output written to std::cout.
 */

/**
 * @brief Static pointer to the StreamRedirect instance that manages the redirection of std::clog.
 *
 * This static member is used to access the StreamRedirect instance from static methods
 * without needing an instance of CerrRedirect. It ensures that there is a single instance
 * of StreamRedirect managing the redirection of std::cerr throughout the application.
 */
StreamRedirect* CoutRedirect::streamRedirect = nullptr;

/**
 * @brief Constructor for the CoutRedirect class.
 * 
 * This constructor initializes the CoutRedirect instance by creating a new
 * CoutRedirectPimpl object, redirecting std::cout to a custom stream buffer,
 * and starting a monitoring thread to process output from the stream.
 */
CoutRedirect::CoutRedirect() { 
    static std::mutex initMutex;
    std::lock_guard<std::mutex> lock(initMutex);

    // Ensure that the CoutRedirectPimpl instance is created only once
    if(nullptr == streamRedirect) {
        streamRedirect = new StreamRedirect(std::cout);
    }
}

/**
 * @brief Destructor for the CoutRedirect class.
 * 
 * This destructor cleans up the resources used by the CoutRedirect instance,
 * restoring std::cout to its original state and stopping the monitoring thread.
 */
CoutRedirect::~CoutRedirect() {
    static std::mutex cleanupMutex;
    std::lock_guard<std::mutex> lock(cleanupMutex);

    // Ensure that the CoutRedirectPimpl instance is deleted only once
    if(streamRedirect) {
        delete streamRedirect;
    }
}

/**
 * @brief Attaches an observer to the CoutRedirect instance.
 * 
 * This method allows an observer to be attached to the CoutRedirect instance,
 * enabling it to receive notifications about new lines written to std::cout.
 * 
 * @param observer Pointer to the StreamObserver instance to attach.
 */
void CoutRedirect::attach(StreamObserver* observer) {
    streamRedirect->attach(observer);
}

/**
 * @brief Detaches an observer from the CoutRedirect instance.
 * 
 * This method allows an observer to be removed from the CoutRedirect instance,
 * stopping it from receiving further notifications about new lines written to std::cout.
 * 
 * @param observer Pointer to the StreamObserver instance to detach.
 */
void CoutRedirect::detach(StreamObserver* observer) {
    streamRedirect->detach(observer);
}

#ifdef LIB_CREDIRECT_AUTOSTART_COUT
/**
 * @brief Automatically starts the CoutRedirect instance if LIB_CREDIRECT_AUTOSTART_COUT is defined.
 * 
 * This function is called to ensure that the CoutRedirect instance is created and started
 * automatically when the library is used, without requiring explicit user intervention.
 */
static CoutRedirect autostartCout{};
#endif // LIB_CREDIRECT_AUTOSTART_COUT

LIB_CREDIRECT_NAMESPACE_END