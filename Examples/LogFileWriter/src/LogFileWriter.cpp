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
#include <LogFileWriterConfig.h>
#include <LogFileWriter.hpp>
#include <CerrRedirect.hpp>
#include <ClogRedirect.hpp>

#include <atomic>
#include <condition_variable>
#include <filesystem>
#include <mutex>
#include <ostream>
#include <thread>

namespace fs = std::filesystem;

struct HIDDEN LogFileWriter::LogFileWriterPimpl {
    std::ofstream logFile;
    fs::path logFileName;
    std::thread flushThread;
    std::atomic<bool> terminate;
    std::atomic<int> flushInterval;
    std::mutex mtx;
    std::condition_variable cv;
};

LogFileWriter::LogFileWriter(const fs::path& logFileName) 
{
    d = new LogFileWriterPimpl();
    d->logFileName = logFileName;
    d->logFile.open(logFileName, std::ios::out | std::ios::app);
    
    if (!d->logFile.is_open()) {
        std::cerr << "Unable to open log file: " + logFileName.string();
    }

    d->terminate = false;
    d->flushInterval = 10;
    d->flushThread = std::thread(LogFileWriter::flushLogfile, d);

#ifdef USE_CLOG_REDIRECT
    ClogRedirect::attach(this);
#endif
#ifdef USE_CERR_REDIRECT
    CerrRedirect::attach(this);
#endif
}

LogFileWriter::~LogFileWriter()
{
#ifdef USE_CLOG_REDIRECT
    ClogRedirect::detach(this);
#endif
#ifdef USE_CERR_REDIRECT
    CerrRedirect::detach(this);
#endif
    d->terminate = true;
    d->cv.notify_all();
    if(d->flushThread.joinable()) {
        d->flushThread.join();
    }
    if (d->logFile.is_open()) {
        d->logFile.close();
    }
    delete d;
}

void HIDDEN LogFileWriter::flushLogfile(LogFileWriterPimpl* d) 
{
    std::unique_lock<std::mutex> lock(d->mtx);
    while(!d->terminate) {
        d->cv.wait_for(lock, 
            std::chrono::seconds(d->flushInterval), 
            [d] { return d->terminate.load(); }
        );
        
        if(d->terminate) break;;

        if(d->logFile.is_open()) {
            d->logFile.flush();
        }
    }
    return;
}

void LogFileWriter::update(const std::string& message)
{
    std::unique_lock<std::mutex> lock(d->mtx);
    if (!d->logFile.is_open()) {
        d->logFile.open(d->logFileName, std::ios::out | std::ios::app);
        if(!d->logFile.is_open()) {
            std::cerr << "Log file is not open: " + d->logFileName.string();
            return;
        }
    }

    d->logFile << message << "\r\n";
}
