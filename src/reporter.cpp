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

#include <cassert>
#include <iostream>
#include <sstream>
#include <vector>

#include "emerald/reporter.h"
#include "emerald/colors.h"

namespace emerald {

    std::string Severity::as_str(Level level) {
        switch (level) {
        case info:
            return "info";
        case warning:
            return "warning";
        case error:
            return "error";
        default:
            return "";
        }
    }

    const std::string& ReportCode::as_str(Code code) {
        assert(code < NUM_REPORT_CODES);
        return _strings[code];
    }

    const std::string& ReportCode::get_report_template(Code code) {
        assert(code < NUM_REPORT_CODES);
        return _templates[code];
    }

    Severity::Level ReportCode::get_severity(Code code) {
        assert(code < NUM_REPORT_CODES);
        return _severities[code];
    }

#define X(code, template, severity) #code,
    const std::string ReportCode::_strings[] = { REPORT_CODES };
#undef X

#define X(code, template, severity) template,
    const std::string ReportCode::_templates[] = { REPORT_CODES };
#undef X

#define X(code, template, severity) severity,
    const Severity::Level ReportCode::_severities[] = { REPORT_CODES };
#undef X

    Reporter::Report::Report(ReportCode::Code code, const std::string& report, Severity::Level severity,
        std::shared_ptr<SourcePosition> source_position)
        : _code(code),
        _report(report),
        _severity(severity),
        _source_position(source_position) {}

    ReportCode::Code Reporter::Report::get_report_code() const {
        return _code;
    }

    const std::string& Reporter::Report::get_report() const {
        return _report;
    }

    Severity::Level Reporter::Report::get_severity() const {
        return _severity;
    }

    std::shared_ptr<SourcePosition> Reporter::Report::get_source_position() const {
        return _source_position;
    }

    bool Reporter::Report::in_source() const {
        return _source_position != nullptr;
    }

    Reporter::Reporter()
        : _num_errors(0),
        _num_warnings(0) {}

    size_t Reporter::num_errors() const {
        return _num_errors;
    }

    bool Reporter::has_errors() const {
        return _num_errors > 0;
    }

    size_t Reporter::num_warnings() const {
        return _num_warnings;
    }

    bool Reporter::has_warnings() const {
        return _num_warnings > 0;
    }

    const std::vector<Reporter::Report>& Reporter::get_reports() const {
        return _reports;
    }

    size_t Reporter::num_reports() const {
        return _reports.size();
    }

    void Reporter::report(ReportCode::Code code, const std::string& report_message) {
        report(code, report_message, nullptr);
    }

    void Reporter::report(ReportCode::Code code, const std::string& report_message,
        std::shared_ptr<SourcePosition> source_position) {
        Severity::Level severity = ReportCode::get_severity(code);
        switch (severity) {
        case Severity::warning:
            _num_warnings++;
            break;
        case Severity::error:
            _num_errors++;
            break;
        default:
            break;
        }

        _reports.push_back(Report(code, report_message, severity, source_position));
    }

    void Reporter::print() const {
        std::ostringstream oss;

        for (const Report& report : _reports) {
            if (&report != &_reports.front()) oss << '\n';
            oss << colors::bold;

            if (report.in_source()) {
                std::shared_ptr<SourcePosition> source_position = report.get_source_position();
                oss << source_position->get_source()->get_name() << ":"
                    << " lines " << source_position->get_start_line() << ":" << source_position->get_end_line()
                    << " cols " << source_position->get_start_col() << ":" << source_position->get_end_col()
                    << ": "; 
            }

            Severity::Level severity = report.get_severity();
            oss << colors::reset;
            switch (severity) {
            case Severity::info:
                oss << colors::fg_blue;
                break;
            case Severity::warning:
                oss << colors::fg_yellow;
                break;
            case Severity::error:
                oss << colors::fg_red;
                break;
            }

            oss << Severity::as_str(severity) << ": ";

            oss << colors::fg_default;

            oss << ReportCode::as_str(report.get_report_code()) << ": " << report.get_report();
        }

        std::cout << oss.str() << std::endl;
    }

} // namespace emerald
