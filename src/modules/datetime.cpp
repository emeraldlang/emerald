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

#include <boost/lexical_cast.hpp>

#include "fmt/format.h"

#include "emerald/magic_methods.h"
#include "emerald/module.h"
#include "emerald/modules/datetime.h"
#include "emerald/native_variables.h"
#include "emerald/objectutils.h"
#include "emerald/process.h"

namespace emerald {
namespace modules {

    Date::Date(Process* process)
        : Object(process) {}

    Date::Date(Process* process, Object* parent)
        : Object(process, parent) {}

    Date* Date::from_native_date(
            Process* process,
            Object* parent,
            const boost::gregorian::date& date) {
        Date* obj = process->get_heap().allocate<Date>(process, parent);
        obj->_date = date;
        return obj;
    }

    std::string Date::as_str() const {
        return boost::gregorian::to_simple_string(_date);
    }

    void Date::init(Number* year, Number* month, Number* day) {
        _date = boost::gregorian::date(
            year->get_native_value(),
            month->get_native_value(),
            day->get_native_value());
    }

    Number* Date::year() const {
        return ALLOC_NUMBER_IN_CTX(_date.year(), get_process());
    }

    Number* Date::month() const {
        return ALLOC_NUMBER_IN_CTX(_date.month(), get_process());
    }

    Number* Date::day() const {
        return ALLOC_NUMBER_IN_CTX(_date.day(), get_process());
    }

    String* Date::day_of_week() const {
        return ALLOC_STRING_IN_CTX(boost::lexical_cast<std::string>(_date.day_of_week()), get_process());
    }

    Number* Date::day_of_year() const {
        return ALLOC_NUMBER_IN_CTX(_date.day_of_year(), get_process());
    }

    void Date::add(Number* days) {
        boost::gregorian::date_duration dd(days->get_native_value());
        _date = _date + dd;
    }

    void Date::sub(Number* days) {
        boost::gregorian::date_duration dd(days->get_native_value());
        _date = _date - dd;
    }

    Date* Date::clone(Process* process, CloneCache& cache) {
        Date* clone = clone_impl<Date>(process, cache);
        clone->_date = _date;
        return clone;
    }

    TimeDuration::TimeDuration(Process* process)
        : Object(process) {}

    TimeDuration::TimeDuration(Process* process, Object* parent)
        : Object(process, parent) {}

    TimeDuration* TimeDuration::from_native_duration(
            Process* process,
            Object* parent,
            const boost::posix_time::time_duration& duration) {
        TimeDuration* time_duration = process->get_heap().allocate<TimeDuration>(
            process, parent);
        time_duration->_duration = duration;
        return time_duration;
    }

    std::string TimeDuration::as_str() const {
        std::string str = boost::posix_time::to_simple_string(_duration);
        str.erase(str.size() - 3); // remove trailing zeros
        return str;
    }

    void TimeDuration::init(
            Number* hours,
            Number* minutes,
            Number* seconds,
            Number* milliseconds) {
        _duration = boost::posix_time::time_duration(
            hours->get_native_value(),
            minutes->get_native_value(),
            seconds->get_native_value(),
            milliseconds->get_native_value() * 1000);
    }

    Number* TimeDuration::hours() const {
        return ALLOC_NUMBER_IN_CTX(_duration.hours(), get_process());
    }

    Number* TimeDuration::minutes() const {
        return ALLOC_NUMBER_IN_CTX(_duration.minutes(), get_process());
    }

    Number* TimeDuration::seconds() const {
        return ALLOC_NUMBER_IN_CTX(_duration.seconds(), get_process());
    }

    Number* TimeDuration::milliseconds() const {
        return ALLOC_NUMBER_IN_CTX(_duration.fractional_seconds() / 1000, get_process());
    }

    Number* TimeDuration::total_seconds() const {
        return ALLOC_NUMBER_IN_CTX(_duration.total_seconds(), get_process());
    }

    Number* TimeDuration::total_milliseconds() const {
        return ALLOC_NUMBER_IN_CTX(_duration.total_milliseconds(), get_process());
    }

    void TimeDuration::add(TimeDuration* other) {
        _duration = _duration + other->_duration;
    }

    void TimeDuration::sub(TimeDuration* other) {
        _duration = _duration - other->_duration;
    }

    const boost::posix_time::time_duration& TimeDuration::get_native_value() {
        return _duration;
    }

    TimeDuration* TimeDuration::clone(Process* process, CloneCache& cache) {
        TimeDuration* clone = clone_impl<TimeDuration>(process, cache);
        clone->_duration = _duration;
        return clone;
    }

    Time::Time(Process* process)
        : Object(process) {}

    Time::Time(Process* process, Object* parent)
        : Object(process, parent) {}

    Time* Time::from_native_time(
            Process* process,
            Object* date_parent,
            Object* time_duration_parent,
            Object* time_parent,
            const boost::posix_time::ptime& time) {
        NativeStack::ScopedNativeFrame scoped_frame(process->get_native_stack().push_frame());

        Local<Time> obj = process->get_heap().allocate<Time>(process, time_parent);
        obj->_date = Date::from_native_date(process, date_parent, time.date());
        obj->_time_of_day = TimeDuration::from_native_duration(process, time_duration_parent, time.time_of_day());

        return obj.val();
    }

    std::string Time::as_str() const {
        return fmt::format("{0} {1}", _date->as_str(), _time_of_day->as_str());
    }

    void Time::init(Date* date, TimeDuration* time) {
        _date = date;
        _time_of_day = time;
    }

    Date* Time::date() const {
        return _date;
    }

    TimeDuration* Time::time_of_day() const {
        return _time_of_day;
    }

    Time* Time::clone(Process* process, CloneCache& cache) {
        Time* time = clone_impl<Time>(process, cache);
        time->_date = _date->clone(process, cache);
        time->_time_of_day = _time_of_day->clone(process, cache);
        return time;
    }

    void Time::add(Number* days) {
        _date->add(days);
    }

    void Time::add(TimeDuration* time) {
        _time_of_day->sub(time);
    }

    void Time::sub(Number* days) {
        _date->sub(days);
    }

    void Time::sub(TimeDuration* time) {
        _time_of_day->sub(time);
    }

    NATIVE_FUNCTION(date_iadd) {
        EXPECT_NUM_ARGS(1);

        CONVERT_RECV_TO(Date, self);
        CONVERT_ARG_TO(0, Number, days);

        self->add(days);

        return self;
    }

    NATIVE_FUNCTION(date_isub) {
        EXPECT_NUM_ARGS(1);

        CONVERT_RECV_TO(Date, self);
        CONVERT_ARG_TO(0, Number, days);

        self->sub(days);

        return self;
    }

    NATIVE_FUNCTION(date_clone) {
        EXPECT_NUM_ARGS(0);

        CONVERT_RECV_TO(Date, self);

        return process->get_heap().allocate<Date>(process, self);
    }

    NATIVE_FUNCTION(date_init) {
        EXPECT_NUM_ARGS(3);

        CONVERT_RECV_TO(Date, self);
        CONVERT_ARG_TO(0, Number, year);
        CONVERT_ARG_TO(1, Number, month);
        CONVERT_ARG_TO(2, Number, day);

        self->init(year, month, day);

        return NONE;
    }

    NATIVE_FUNCTION(date_year) {
        EXPECT_NUM_ARGS(0);

        CONVERT_RECV_TO(Date, self);

        return self->year();
    }

    NATIVE_FUNCTION(date_month) {
        EXPECT_NUM_ARGS(0);

        CONVERT_RECV_TO(Date, self);

        return self->month();
    }

    NATIVE_FUNCTION(date_day) {
        EXPECT_NUM_ARGS(0);

        CONVERT_RECV_TO(Date, self);

        return self->day();
    }

    NATIVE_FUNCTION(date_day_of_week) {
        EXPECT_NUM_ARGS(0);

        CONVERT_RECV_TO(Date, self);

        return self->day_of_week();
    }

    NATIVE_FUNCTION(date_day_of_year) {
        EXPECT_NUM_ARGS(0);

        CONVERT_RECV_TO(Date, self);

        return self->day_of_year();
    }

    NATIVE_FUNCTION(time_duration_add) {
        EXPECT_NUM_ARGS(1);

        CONVERT_RECV_TO(TimeDuration, self);
        CONVERT_ARG_TO(0, TimeDuration, other);

        return TimeDuration::from_native_duration(
            process,
            frame->get_global("TimeDuration"),
            self->get_native_value() + other->get_native_value());
    }

    NATIVE_FUNCTION(time_duration_sub) {
        EXPECT_NUM_ARGS(1);

        CONVERT_RECV_TO(TimeDuration, self);
        CONVERT_ARG_TO(0, TimeDuration, other);

        return TimeDuration::from_native_duration(
            process,
            frame->get_global("TimeDuration"),
            self->get_native_value() - other->get_native_value());
    }

    NATIVE_FUNCTION(time_duration_iadd) {
        EXPECT_NUM_ARGS(1);

        CONVERT_RECV_TO(TimeDuration, self);
        CONVERT_ARG_TO(0, TimeDuration, other);

        self->add(other);

        return self;
    }

    NATIVE_FUNCTION(time_duration_isub) {
        EXPECT_NUM_ARGS(1);

        CONVERT_RECV_TO(TimeDuration, self);
        CONVERT_ARG_TO(0, TimeDuration, other);

        self->sub(other);

        return self;
    }

    NATIVE_FUNCTION(time_duration_clone) {
        EXPECT_NUM_ARGS(0);

        CONVERT_RECV_TO(TimeDuration, self);

        return process->get_heap().allocate<TimeDuration>(process, self);
    }

    NATIVE_FUNCTION(time_duration_init) {
        EXPECT_NUM_ARGS(4);

        CONVERT_RECV_TO(TimeDuration, self);
        CONVERT_ARG_TO(0, Number, hours);
        CONVERT_ARG_TO(1, Number, minutes);
        CONVERT_ARG_TO(2, Number, seconds);
        CONVERT_ARG_TO(3, Number, milliseconds);

        self->init(hours, minutes, seconds, milliseconds);

        return NONE;
    }

    NATIVE_FUNCTION(time_duration_hours) {
        EXPECT_NUM_ARGS(0);

        CONVERT_RECV_TO(TimeDuration, self);

        return self->hours();
    }

    NATIVE_FUNCTION(time_duration_minutes) {
        EXPECT_NUM_ARGS(0);

        CONVERT_RECV_TO(TimeDuration, self);

        return self->minutes();
    }

    NATIVE_FUNCTION(time_duration_seconds) {
        EXPECT_NUM_ARGS(0);

        CONVERT_RECV_TO(TimeDuration, self);

        return self->seconds();
    }

    NATIVE_FUNCTION(time_duration_milliseconds) {
        EXPECT_NUM_ARGS(0);

        CONVERT_RECV_TO(TimeDuration, self);

        return self->milliseconds();
    }

    NATIVE_FUNCTION(time_duration_total_seconds) {
        EXPECT_NUM_ARGS(0);

        CONVERT_RECV_TO(TimeDuration, self);

        return self->total_seconds();
    }

    NATIVE_FUNCTION(time_duration_total_milliseconds) {
        EXPECT_NUM_ARGS(0);

        CONVERT_RECV_TO(TimeDuration, self);

        return self->total_milliseconds();
    }

    NATIVE_FUNCTION(time_iadd) {
        EXPECT_NUM_ARGS(1);

        CONVERT_RECV_TO(Time, self);

        if (TRY_CONVERT_ARG_TO(0, Number, days)) {
            self->add(days);
        } else if (TRY_CONVERT_ARG_TO(0, TimeDuration, time)) {
            self->add(time);
        } else {
            throw ALLOC_EXCEPTION("");
        }

        return self;
    }

    NATIVE_FUNCTION(time_isub) {
        EXPECT_NUM_ARGS(1);

        CONVERT_RECV_TO(Time, self);

        if (TRY_CONVERT_ARG_TO(0, Number, days)) {
            self->sub(days);
        } else if (TRY_CONVERT_ARG_TO(0, TimeDuration, time)) {
            self->sub(time);
        } else {
            throw ALLOC_EXCEPTION("");
        }

        return self;
    }

    NATIVE_FUNCTION(time_clone) {
        EXPECT_NUM_ARGS(0);

        CONVERT_RECV_TO(Time, self);

        return process->get_heap().allocate<Time>(process, self);
    }

    NATIVE_FUNCTION(time_init) {
        EXPECT_NUM_ARGS(2);

        CONVERT_RECV_TO(Time, self);
        CONVERT_ARG_TO(0, Date, date);
        CONVERT_ARG_TO(1, TimeDuration, time);

        self->init(date, time);

        return NONE;
    }

    NATIVE_FUNCTION(time_date) {
        EXPECT_NUM_ARGS(0);

        CONVERT_RECV_TO(Time, self);

        return self->date();
    }

    NATIVE_FUNCTION(time_time_of_day) {
        EXPECT_NUM_ARGS(0);

        CONVERT_RECV_TO(Time, self);

        return self->time_of_day();
    }

    NATIVE_FUNCTION(datetime_universal_time) {
        EXPECT_NUM_ARGS(0);

        return Time::from_native_time(
            process,
            frame->get_global("Date"),
            frame->get_global("TimeDuration"),
            frame->get_global("Time"),
            boost::posix_time::second_clock::universal_time());
    }

    NATIVE_FUNCTION(datetime_local_time) {
        EXPECT_NUM_ARGS(0);

        return Time::from_native_time(
            process,
            frame->get_global("Date"),
            frame->get_global("TimeDuration"),
            frame->get_global("Time"),
            boost::posix_time::second_clock::local_time());
    }

    MODULE_INITIALIZATION_FUNC(init_datetime_module) {
        Process* process =  module->get_process();

        Local<Date> date = process->get_heap().allocate<Date>(process, OBJECT_PROTOTYPE);
        date->set_property(magic_methods::iadd, ALLOC_NATIVE_FUNCTION(date_iadd));
        date->set_property(magic_methods::isub, ALLOC_NATIVE_FUNCTION(date_isub));
        date->set_property(magic_methods::clone, ALLOC_NATIVE_FUNCTION(date_clone));
        date->set_property(magic_methods::init, ALLOC_NATIVE_FUNCTION(date_init));
        date->define_property("year", ALLOC_PROP_ACC_DESC(ALLOC_NATIVE_FUNCTION(date_year), nullptr));
        date->define_property("month", ALLOC_PROP_ACC_DESC(ALLOC_NATIVE_FUNCTION(date_month), nullptr));
        date->define_property("day", ALLOC_PROP_ACC_DESC(ALLOC_NATIVE_FUNCTION(date_day), nullptr));
        date->define_property("day_of_week", ALLOC_PROP_ACC_DESC(ALLOC_NATIVE_FUNCTION(date_day_of_week), nullptr));
        date->define_property("day_of_year", ALLOC_PROP_ACC_DESC(ALLOC_NATIVE_FUNCTION(date_day_of_year), nullptr));
        module->set_property("Date", date.val());

        Local<TimeDuration> time_duration = process->get_heap().allocate<TimeDuration>(process, OBJECT_PROTOTYPE);
        time_duration->set_property(magic_methods::add, ALLOC_NATIVE_FUNCTION(time_duration_add));
        time_duration->set_property(magic_methods::sub, ALLOC_NATIVE_FUNCTION(time_duration_sub));
        time_duration->set_property(magic_methods::iadd, ALLOC_NATIVE_FUNCTION(time_duration_iadd));
        time_duration->set_property(magic_methods::isub, ALLOC_NATIVE_FUNCTION(time_duration_isub));
        time_duration->set_property(magic_methods::clone, ALLOC_NATIVE_FUNCTION(time_duration_clone));
        time_duration->set_property(magic_methods::init, ALLOC_NATIVE_FUNCTION(time_duration_init));
        time_duration->define_property("hours", ALLOC_PROP_ACC_DESC(ALLOC_NATIVE_FUNCTION(time_duration_hours), nullptr));
        time_duration->define_property("minutes", ALLOC_PROP_ACC_DESC(ALLOC_NATIVE_FUNCTION(time_duration_minutes), nullptr));
        time_duration->define_property("seconds", ALLOC_PROP_ACC_DESC(ALLOC_NATIVE_FUNCTION(time_duration_seconds), nullptr));
        time_duration->define_property("milliseconds", ALLOC_PROP_ACC_DESC(ALLOC_NATIVE_FUNCTION(time_duration_milliseconds), nullptr));
        time_duration->define_property("total_seconds", ALLOC_PROP_ACC_DESC(ALLOC_NATIVE_FUNCTION(time_duration_total_seconds), nullptr));
        time_duration->define_property("total_milliseconds", ALLOC_PROP_ACC_DESC(ALLOC_NATIVE_FUNCTION(time_duration_total_milliseconds), nullptr));
        module->set_property("TimeDuration", time_duration.val());

        Local<Time> time = process->get_heap().allocate<Time>(process, OBJECT_PROTOTYPE);
        time->set_property(magic_methods::iadd, ALLOC_NATIVE_FUNCTION(time_iadd));
        time->set_property(magic_methods::isub, ALLOC_NATIVE_FUNCTION(time_isub));
        time->set_property(magic_methods::clone, ALLOC_NATIVE_FUNCTION(time_clone));
        time->set_property(magic_methods::init, ALLOC_NATIVE_FUNCTION(time_init));
        time->set_property("date", ALLOC_PROP_ACC_DESC(ALLOC_NATIVE_FUNCTION(time_date), nullptr));
        time->set_property("time_of_day", ALLOC_PROP_ACC_DESC(ALLOC_NATIVE_FUNCTION(time_time_of_day), nullptr));
        module->set_property("Time", time.val());

        module->set_property("universal_time", ALLOC_NATIVE_FUNCTION(datetime_universal_time));
        module->set_property("local_time", ALLOC_NATIVE_FUNCTION(datetime_local_time));
    }

} // namespace modules
} // namespace emerald
