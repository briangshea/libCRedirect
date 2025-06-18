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
 * @file StreamRedirect.cpp
 * @brief Implementation of the StreamRedirect class for redirecting std::ostream streams.
 * 
 * This file contains the implementation of the StreamRedirect class, which provides
 * functionality to redirect a standard ostream (cerr, cout, etc) to a custom
 * stream buffer.
 */

/**
 * @struct StreamRedirect::StreamRedirectPimpl
 * @brief Private implementation (Pimpl) for the StreamRedirect class.
 * 
 * This structure encapsulates the internal details of the StreamRedirect class,
 * providing a mechanism to redirect and monitor the standard stream.
 * It uses a custom stream buffer and manages observers for stream monitoring.
 * 
 * @details
 * - `streamBuf`: A custom synchronous stream buffer used for capturing output.
 * - `stream`: An output stream associated with the custom stream buffer.
 * - `oldStreamBuf`: Pointer to the original stream buffer, used for restoration.
 * - `originalStream`: Reference to the original std::ostream that is being redirected.
 * - `running`: Atomic flag indicating whether the redirection is active.
 * - `monitorThread`: Thread used for monitoring the redirected stream.
 * - `observers`: List of observers that receive notifications about stream updates.
 * - `mtx`: Mutex used for synchronizing access to observers.
 * 
 * @note This structure is intended for internal use within the StreamRedirect class
 * and should not be accessed directly by external code.
 */
struct HIDDEN StreamRedirect::StreamRedirectPimpl {
    StreamRedirectPimpl(std::ostream& origStream, std::streamsize initial_size = 1024) :
        streamBuf(initial_size), 
        stream(&streamBuf), 
        oldStreamBuf(nullptr),
        originalStream(origStream),
        running(false) {}
    
    ~StreamRedirectPimpl() {}

    SynchronousStreamBuf streamBuf;
    std::ostream stream;
    std::streambuf* oldStreamBuf;
    std::ostream& originalStream;
    std::atomic<bool> running;
    std::thread monitorThread;
    std::vector<StreamObserver*> observers;
    std::mutex mtx;
};

/**
 * @brief Constructor for the StreamRedirect class.
 * 
 * This constructor initializes the StreamRedirect instance, setting up the custom stream buffer
 * and redirecting a std::ostream to it. It also starts a monitoring thread to process output from the stream.
 */
StreamRedirect::StreamRedirect(std::ostream& stream) { 
    //struct StreamRedirect::StreamRedirectPimpl* d;
    d = new StreamRedirectPimpl(stream);
    
    // Redirect std::ostream to the custom stream buffer
    d->oldStreamBuf = stream.rdbuf(d->stream.rdbuf());

    // Start the monitoring thread
    d->running = true;
    d->monitorThread = std::thread(&StreamRedirect::monitorStream, this);        
}

/**
 * @brief Destructor for the StreamRedirect class.
 * 
 * This destructor cleans up the resources used by the StreamRedirect instance,
 * restoring std::ostream to its original state and stopping the monitoring thread.
 */
StreamRedirect::~StreamRedirect() {
    // Ensure that the static instance is cleaned up only once
    if(d) {
        d->running = false;
        d->streamBuf.terminate();
        if(d->monitorThread.joinable()) {
            d->monitorThread.join();
        }
        
        std::lock_guard<std::mutex> lock(d->mtx);
        // Restore std::Stream to its original stream buffer
        d->originalStream.rdbuf(d->oldStreamBuf);

        delete d;
        d = nullptr;
    }
}

/**
 * @brief Monitors the redirected stream and notifies observers of new lines.
 * 
 * This method continuously reads from the custom stream buffer and notifies
 * all attached observers whenever a new line is read. It runs in a separate thread
 * to avoid blocking the main application flow.
 */
void HIDDEN StreamRedirect::monitorStream() {
    std::istream instream(&d->streamBuf);
    std::string line;
    
    while(d->running) {
        while (std::getline(instream, line)) {
            // Process the line read from the stream
            notify(line);
            if(instream.eof()) {
                break;
            }
        }
    }
}

/**
 * @brief Attaches an observer to the StreamRedirect instance.
 * 
 * This method allows an observer to be attached to the StreamRedirect instance,
 * enabling it to receive notifications about new lines written to std::ostream.
 * 
 * @param observer Pointer to the StreamObserver instance to attach.
 */
void StreamRedirect::attach(StreamObserver* observer) {
    if(!observer) 
        return;
    
    // Only this section of code requires a lock guard
    {
        std::lock_guard<std::mutex> lock(d->mtx);
        d->observers.push_back(observer);
    }
}

/**
 * @brief Detaches an observer from the StreamRedirect instance.
 * 
 * This method allows an observer to be detached from the StreamRedirect instance,
 * stopping it from receiving further notifications about new lines written to std::ostream.
 * 
 * @param observer Pointer to the StreamObserver instance to detach.
 */
void StreamRedirect::detach(StreamObserver* observer) {
    if(!observer)
        return;
    
    {
        std::lock_guard<std::mutex> lock(d->mtx);
        d->observers.erase(
            std::remove(d->observers.begin(), d->observers.end(), observer), 
            d->observers.end()
        );
    }
}

/**
 * @brief Notifies all observers with a new message.
 * 
 * This method iterates through all attached observers and calls their update method
 * with the provided message. It ensures that all observers are notified of new lines
 * written to std::ostream.
 * 
 * @param message The message to notify observers with.
 */
void StreamRedirect::notify(const std::string& message) {
    std::lock_guard<std::mutex> lock(d->mtx);
    for(auto o : d->observers) {
        o->update(message);
    }
}

LIB_CREDIRECT_NAMESPACE_END
