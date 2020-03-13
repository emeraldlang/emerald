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

#ifndef _EMERALD_MODULES_DATETIME_H
#define _EMERALD_MODULES_DATETIME_H

#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include "emerald/module_registry.h"
#include "emerald/object.h"

#define DATE_NATIVES        \
    X(date_iadd)            \
    X(date_isub)            \
    X(date_clone)           \
    X(date_init)            \
    X(date_year)            \
    X(date_month)           \
    X(date_day)             \
    X(date_day_of_week)     \
    X(date_day_of_year)

#define TIME_DURATION_NATIVES           \
    X(time_duration_clone)              \
    X(time_duration_init)               \
    X(time_duration_add)                \
    X(time_duration_sub)                \
    X(time_duration_iadd)               \
    X(time_duration_isub)               \
    X(time_duration_hours)              \
    X(time_duration_minutes)            \
    X(time_duration_seconds)            \
    X(time_duration_milliseconds)       \
    X(time_duration_total_seconds)      \
    X(time_duration_total_milliseconds)

#define TIME_NATIVES    \
    X(time_iadd)        \
    X(time_isub)        \
    X(time_clone)       \
    X(time_init)        \
    X(time_date)        \
    X(time_time_of_day)

#define DATETIME_NATIVES        \
    X(datetime_universal_time)  \
    X(datetime_local_time)

namespace emerald {
namespace modules {

    class TimeDuration;

    class Date : public Object {
    public:
        Date(Process* process);
        Date(Process* process, Object* parent);

        static Date* from_native_date(
            Process* process,
            Object* parent,
            const boost::gregorian::date& date);

        std::string as_str() const override;

        void init(Number* year, Number* month, Number* day);

        Number* year() const;
        Number* month() const;
        Number* day() const;

        String* day_of_week() const;
        Number* day_of_year() const;

        void add(Number* days);
        void sub(Number* days);

        Date* clone(Process* process, CloneCache& cache) override;

    private:
        boost::gregorian::date _date;
    };

    class TimeDuration : public Object {
    public:
        TimeDuration(Process* process);
        TimeDuration(Process* process, Object* parent);

        static TimeDuration* from_native_duration(
            Process* process,
            Object* parent,
            const boost::posix_time::time_duration& duration);

        std::string as_str() const override;

        void init(
            Number* hours,
            Number* minutes,
            Number* seconds,
            Number* milliseconds);

        Number* hours() const;
        Number* minutes() const;
        Number* seconds() const;
        Number* milliseconds() const;

        Number* total_seconds() const;
        Number* total_milliseconds() const;

        void add(TimeDuration* other);
        void sub(TimeDuration* other);

        const boost::posix_time::time_duration& get_native_value();

        TimeDuration* clone(Process* process, CloneCache& cache) override;

    private:
        boost::posix_time::time_duration _duration;
    };

    class Time : public Object {
    public:
        Time(Process* process);
        Time(Process* process, Object* parent);

        static Time* from_native_time(
            Process* process,
            Object* date_parent,
            Object* time_duration_parent,
            Object* time_parent,
            const boost::posix_time::ptime& time);

        std::string as_str() const override;

        void init(Date* date, TimeDuration* time);

        Date* date() const;
        TimeDuration* time_of_day() const;

        void add(Number* days);
        void add(TimeDuration* time);
        void sub(Number* days);
        void sub(TimeDuration* time);

        Time* clone(Process* process, CloneCache& cache) override;

    private:
        Date* _date;
        TimeDuration* _time_of_day;
    };

#define X(name) NATIVE_FUNCTION(name);
    DATE_NATIVES
    TIME_DURATION_NATIVES
    TIME_NATIVES
    DATETIME_NATIVES
#undef X

    MODULE_INITIALIZATION_FUNC(init_datetime_module);

} // namespace modules
} // namespace emerald

#endif // _EMERALD_MODULES_DATETIME_H
