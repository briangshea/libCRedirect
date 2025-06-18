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
#include "Logging.hpp"

/**
 * @brief Set the current logging level.
 * @param newLevel The new logging level to set.
 * @return The previous logging level.
 */
LogLevel basic_logging::_curLogLevel = MIN_LEVEL;

/**
 * @brief Logging instances for different log levels.
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
logging<LogLevel::Debug>    debug;
logging<LogLevel::Info>     info;
logging<LogLevel::Warning>  warn;
logging<LogLevel::Error>    err;
logging<LogLevel::Critical> crit;
logging<LogLevel::Audit>    audit;
