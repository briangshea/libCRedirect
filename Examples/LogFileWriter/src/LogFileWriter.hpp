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
 #ifndef LOG_FILE_WRITER_HPP
#define LOG_FILE_WRITER_HPP

#include <LogFileWriterConfig.h>
#include <filesystem>
#include <fstream>
#include <string>
#include <StreamObserver.hpp>

namespace fs = std::filesystem;

class LogFileWriter : public StreamObserver {
public:
    LogFileWriter(const fs::path& logFileName);
    ~LogFileWriter();

    void update(const std::string& message) override;
    void changeLogFileName(const std::string& newLogFileName);

private:
    struct LogFileWriterPimpl;
    struct LogFileWriterPimpl *d;
    
    static void flushLogfile(LogFileWriterPimpl* d);
};

#endif // LOG_FILE_WRITER_HPP