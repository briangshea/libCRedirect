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
#ifndef __CREDIRECT_H__
#define __CREDIRECT_H__

#include <CRedirect_config.h>

#ifdef LIB_CREDIRECT_ENABLE_CERR
#include <CerrRedirect.hpp>
#endif
#ifdef LIB_CREDIRECT_ENABLE_CLOG
#include <ClogRedirect.hpp>
#endif
#ifdef LIB_CREDIRECT_ENABLE_COUT
#include <CoutRedirect.hpp>
#endif

#endif  // __CREDIRECT_H__
