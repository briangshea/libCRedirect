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
#ifndef __CREDIRECT_STREAM_REDIRECT_HPP__
#define __CREDIRECT_STREAM_REDIRECT_HPP__
#include <CRedirect_config.h>
#include <StreamObserver.hpp>
#include <string>

LIB_CREDIRECT_NAMESPACE_BEGIN

/**
 * @class StreamRedirect
 * @brief Redirects std::ostream to a custom stream buffer and notifies observers.
 * 
 * This class provides functionality to redirect a standard stream (std::cerr,cout, etc.)
 * to a custom stream buffer, allowing for monitoring and processing of error messages.
 * It supports attaching and detaching observers that will be notified of new lines written
 * to the redirected stream.
 */
class StreamRedirect final {
public:
    StreamRedirect(std::ostream& stream);
    ~StreamRedirect();

    void attach(StreamObserver* observer);
    void detach(StreamObserver* observer);
    void notify(const std::string& line);

private:    
    void monitorStream();

    struct StreamRedirectPimpl;
    struct StreamRedirectPimpl* d;
};

LIB_CREDIRECT_NAMESPACE_END

#endif // LIB_CREDIRECT_STREAM_REDIRECT_HPP_
