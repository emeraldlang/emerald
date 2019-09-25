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

#ifndef _EMERALD_REPORTER_H
#define _EMERALD_REPORTER_H

#include <memory>
#include <string>
#include <vector>

#include "fmt/format.h"

#include "emerald/source.h"

#define REPORT_CODES                                                                            \
    X(illegal_token, "illegal token '{0}'", Severity::error)                                    \
    X(unexpected_token, "unexpected token '{0}'", Severity::error)                              \
    X(unexpected_eosf, "unexpected eosf", Severity::error)                                      \
    X(illegal_return, "return statement not in function", Severity::error)                      \
    X(undeclared_variable, "'{0}' has not been declared in this scope", Severity::error)        \
    X(invalid_lvalue, "invalid lvalue", Severity::error)

namespace emerald {

    class Severity {
    public:
        enum Level {
            info,
            warning,
            error
        };

        static std::string as_str(Level level);
    };

    class ReportCode {
    public:
#define X(code, template, severity) code,
        enum Code {
            REPORT_CODES NUM_REPORT_CODES
        };
#undef X

        static const std::string& as_str(Code code);

        static const std::string& get_report_template(Code code);

        template<class... Args>
        static std::string format_report(Code code, Args&&... args);

        static Severity::Level get_severity(Code code);

    private:
        static const std::string _strings[NUM_REPORT_CODES];
        static const std::string _templates[NUM_REPORT_CODES];
        static const Severity::Level _severities[NUM_REPORT_CODES];
    };

    class Reporter {
    public:
        class Report {
        public:
            Report(ReportCode::Code code, const std::string& report, Severity::Level severity,
                std::shared_ptr<SourcePosition> source_position = nullptr);

            ReportCode::Code get_report_code() const;
            const std::string& get_report() const;
            Severity::Level get_severity() const;
            std::shared_ptr<SourcePosition> get_source_position() const;

            bool in_source() const;

        private:
            ReportCode::Code _code;
            std::string _report;
            Severity::Level _severity;
            std::shared_ptr<SourcePosition> _source_position;
        };

        Reporter();

        size_t num_errors() const;
        bool has_errors() const;

        size_t num_warnings() const;
        bool has_warnings() const;

        const std::vector<Report>& get_reports() const;
        size_t num_reports() const;

        void report(ReportCode::Code code, const std::string& report_message);
        void report(ReportCode::Code code, const std::string& report_message,
            std::shared_ptr<SourcePosition> source_position);

        void print() const;

    private:
        size_t _num_errors;
        size_t _num_warnings;

        std::vector<Report> _reports;
    };

    template<class... Args>
    std::string ReportCode::format_report(Code code, Args&&... args) {
        assert(code < NUM_REPORT_CODES);
        return fmt::format(_templates[code], args...);
    }

} // namespace emerald

#endif // _EMERALD_REPORTER_H
