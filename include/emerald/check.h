/*  Emerald - Procedural and Object Oriented Programming Language
**  Copyright (C) 2018  Zach Perkitny
**
**  This program is free software: you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef _EMERALD_CHECK_H
#define _EMERALD_CHECK_H

#include <exception>
#include <sstream>

#ifdef DEBUG
#define DCHECK(condition, message)                                      \
    do {                                                                \
        if (!(condition)) {                                             \
            std::cerr                                                   \
                << "Assertion Failed " #condition " " << __FILE__       \
                << ':' << __LINE__ << ' ' << message << std::endl;      \
            std::terminate();                                           \
        }                                                               \
    } while (false)
#else
#define DCHECK(condition, message) do {} while(false)
#endif

#define DCHECK_EQ(lhs, rhs, message) CHECK((lhs) == (rhs), message)
#define DCHECK_NE(lhs, rhs, message) CHECK((lhs) != (rhs), message)
#define DCHECK_GT(lhs, rhs, message) CHECK((lhs) > (rhs), message)
#define DCHECK_GE(lhs, rhs, message) CHECK((lhs) >= (rhs), message)
#define DCHECK_LT(lhs, rhs, message) CHECK((lhs) < (rhs), message)
#define DCHECK_LE(lhs, rhs, message) CHECK((lhs) <= (rhs), message)
#define DCHECK_NOT_NULL(val, message) CHECK((val) != nullptr, message)
#define DCHECK_NULL(val, message) CHECK((val) == nullptr, message)

#define CHECK_THROW(condition, exception, message)                     \
    do {                                                                \
        if (condition) break;                                           \
        std::ostringstream oss("Assertion Failed: " #condition " ");    \
        oss << __FILE__ << ':' << __LINE__ << ' ' << message;           \
        throw exception(oss.str());                                     \
    } while (false)

#define CHECK_THROW_LOGIC_ERROR(condition, message) CHECK_THROW(condition, std::logic_error, message)
#define CHECK_THROW_DOMAIN_ERROR(condition, message) CHECK_THROW(condition, std::domain_error, message)
#define CHECK_THROW_INVALID_ARGUMENT(condition, message) CHECK_THROW(condition, std::invalid_argument, message)
#define CHECK_THROW_LENGTH_ERROR(condition, message) CHECK_THROW(condition, std::length_error, message)
#define CHECK_THROW_OUT_OF_RANGE(condition, message) CHECK_THROW(condition, std::out_of_range, message)

#define CHECK_THROW_RUNTIME_ERROR(condition, message) CHECK_THROW(condition, std::runtime_error, message)

#endif // _EMERALD_CHECK_H
