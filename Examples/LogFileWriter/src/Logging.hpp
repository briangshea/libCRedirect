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
#ifndef LOGGING_HPP
#define LOGGING_HPP
#include <LogFileWriterConfig.h>
#include <atomic>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>

/**
 * @enum LogLevel
 * @brief Defines the various levels of logging.
 */
enum class LogLevel {
    Debug,      /**< Detailed information, typically of interest only when diagnosing problems. */
    Info,       /**< Confirmation that things are working as expected. */
    Audit,      /**< Audit log level for tracking Python events. */
    Warning,    /**< An indication that something unexpected happened, or indicative of some problem in the near future. */
    Error,      /**< Due to a more serious problem, the software has not been able to perform some function. */
    Critical    /**< A serious error, indicating that the program itself may be unable to continue running. */
};

#define MIN_LEVEL LogLevel::Debug

/**
 * @class basic_logging
 * @brief Base class for logging functionality.
 */
class basic_logging
{
public:
    static LogLevel setLogLevel(LogLevel newLevel){
        LogLevel tll = _curLogLevel;
        _curLogLevel = newLevel;
        return tll;
    };

protected:
    /**
     * @brief Constructor for basic_logging.
     */
    basic_logging() {};

    /**
     * @brief Destructor for basic_logging.
     */
    ~basic_logging() {};

protected:
    static LogLevel _curLogLevel; /**< Current logging level. */
};

/**
 * @class logging
 * @brief Template class for logging messages at a specific log level.
 * @tparam _level The log level for this instance of the logging class.
 */
template<LogLevel _level>
class logging : private basic_logging {

public:
    /**
     * @brief Constructor for logging.
     */
    logging() : basic_logging() {
        //if constexpr (_level >= MIN_LEVEL) {
        //    terminate = false;
        //    flush_thread = new std::thread([this]()
        //    {
        //        std::unique_lock<std::mutex> lock(mtx);
        //        while (!terminate) {
        //            cv.wait_for(lock, std::chrono::seconds(1));
        //            if (terminate) break;
        //            this->flush();
        //        }
        //    });
        //}
    };
    
    ~logging() {
        //if constexpr (_level >= MIN_LEVEL)
        //{
        //    terminate = true;
        //    cv.notify_all();
        //    if(flush_thread->joinable())
        //        flush_thread->join();
        //}
    };

    constexpr const char * level() const {
        switch(_level)
        {
            case LogLevel::Debug:    return "[debug] ";
            case LogLevel::Info:     return "[info] ";
            case LogLevel::Audit:    return "[audit] ";
            case LogLevel::Warning:  return "[warn] ";
            case LogLevel::Error:    return "[error] ";
            case LogLevel::Critical: return "[crit] ";
        }
        return "[info] ";
    };

    /**
     * @brief Overloaded stream insertion operator for logging messages.
     * @tparam T The type of the message to log.
     * @param msg The message to log.
     * @return Reference to the logging object.
     */
    template<typename T>
    inline
    logging& operator<<(const T& msg) {
        // used to compile out excess logging message when not wanted
        if constexpr (_level >= MIN_LEVEL) {
            if(_level >= _curLogLevel) {
                std::lock_guard<std::mutex> lock(mtx);
                if(!level_printed) {
                    ss << level();
                    level_printed = true;
                }
                ss << msg;
            }
        }
        return *this;
    };

    /**
     * @brief Overloaded stream insertion operator for manipulators (e.g., std::endl).
     * @param manip The manipulator to apply.
     * @return Reference to the logging object.
     */
    inline
    logging& operator<<(std::ostream& (*manip)(std::ostream&)) {
        // used to compile out excess logging message when not wanted
        if constexpr (_level >= MIN_LEVEL) {
            if(_level >= _curLogLevel) 
            {
                std::lock_guard<std::mutex> lock(mtx);

                // Send messages to either clog or cerr based on level
                // routing is deteremined at compile time
                if constexpr (_level == LogLevel::Critical)
                    std::cerr << ss.str() << manip;
                
                // All messages are printed to the log file
                std::clog << ss.str() << manip;

                // Send info message to the console
                //if constexpr (_level == LogLevel::Info)
                    std::cout << ss.str() << manip;
                
                ss.str("");
                level_printed = false;
            }
        }
        return *this;
    };

    void flush() {
        if constexpr (_level >= MIN_LEVEL) {
            if(_level >= _curLogLevel) 
            {
                std::lock_guard<std::mutex> lock(mtx);
                
                if constexpr (_level == LogLevel::Critical)
                    std::cerr << ss.str();

                std::clog << ss.str();

                std::cout << ss.str();

                ss.str("");
            }
        }
    }

private:
    std::stringstream ss;
    std::thread* flush_thread;
    std::condition_variable cv;
    std::atomic<bool> terminate;
    std::atomic<bool> level_printed;
    std::mutex mtx;
};

/**
 * @brief Global logging instances for different log levels.
 * These instances can be used to log messages at various levels.
 * They are defined as global variables so that they can be used throughout the application.
 * The logging level for each instance is determined at compile time based on the template parameter.
 * The `MIN_LEVEL` constant is used to determine the minimum logging level that will be compiled into the application.
 * If the logging level is below `MIN_LEVEL`, the instance will not log any messages.
 * The `logging` class is a template class that takes a `LogLevel` as a template parameter.
 * Each instance of `logging` will only log messages that are at or above its specified level.
 * The `level()` method returns a string representation of the logging level, which is used to prefix log messages.
 * The `operator<<` method is overloaded to allow for easy logging of messages using the stream insertion operator.
 * The `operator<<` method also handles manipulators like `std::endl` to flush the output stream.
 */
extern logging<LogLevel::Debug>    debug;   /**< Logging instance for debug level messages. */
extern logging<LogLevel::Info>     info;    /**< Logging instance for info level messages. */
extern logging<LogLevel::Warning>  warn;    /**< Logging instance for warning level messages. */
extern logging<LogLevel::Error>    err;     /**< Logging instance for error level messages. */
extern logging<LogLevel::Critical> crit;    /**< Logging instance for critical level messages. */
extern logging<LogLevel::Audit>    audit;   /**< Logging instance for audit level messages. */

#endif // LOGGING_HPP