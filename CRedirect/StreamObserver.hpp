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
#ifndef __STREAM_OBSERVER_HPP__
#define __STREAM_OBSERVER_HPP__

#include <CRedirect_config.h>
#include <string>

LIB_CREDIRECT_NAMESPACE_BEGIN

class StreamObserver {
public:
    virtual ~StreamObserver() = default;
    virtual void update(const std::string& line) = 0;
};

LIB_CREDIRECT_NAMESPACE_END

#endif // __STREAM_OBSERVER_HPP__