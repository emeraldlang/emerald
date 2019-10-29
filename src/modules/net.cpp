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

#include "emerald/execution_context.h"
#include "emerald/magic_methods.h"
#include "emerald/module.h"
#include "emerald/modules/net.h"
#include "emerald/native_frame.h"
#include "emerald/objectutils.h"

namespace emerald {
namespace modules {

    IPAddress::IPAddress(ExecutionContext* context)
        : Object(context, context->get_native_objects().get_object_prototype()) {}

    IPAddress::IPAddress(ExecutionContext* context, Object* parent)
        : Object(context, parent) {}

    std::string IPAddress::as_str() const {
        return _address.to_string();
    }

    void IPAddress::init(const std::string& str) {
        _address = boost::asio::ip::address::from_string(str);
    }

    bool IPAddress::is_loopback() const {
        return _address.is_loopback();
    }

    bool IPAddress::is_multicast() const {
        return _address.is_multicast();
    }

    bool IPAddress::is_unspecified() const {
        return _address.is_unspecified();
    }

    bool IPAddress::is_ipv4() const {
        return _address.is_v4();
    }

    bool IPAddress::is_ipv6() const {
        return _address.is_v6();
    }

    NATIVE_FUNCTION(ip_address_init) {
        EXPECT_NUM_ARGS(1);

        CONVERT_RECV_TO(IPAddress, self);
        CONVERT_ARG_TO(0, String, str);

        self->init(str->get_value());

        return NONE;
    }

    NATIVE_FUNCTION(ip_address_is_loopback) {
        EXPECT_NUM_ARGS(0);

        CONVERT_RECV_TO(IPAddress, self);

        return BOOLEAN(self->is_loopback());
    }

    NATIVE_FUNCTION(ip_address_is_multicast) {
        EXPECT_NUM_ARGS(0);

        CONVERT_RECV_TO(IPAddress, self);

        return BOOLEAN(self->is_multicast());
    }

    NATIVE_FUNCTION(ip_address_is_unspecified) {
        EXPECT_NUM_ARGS(0);

        CONVERT_RECV_TO(IPAddress, self);

        return BOOLEAN(self->is_unspecified());
    }

    NATIVE_FUNCTION(ip_address_is_ipv4) {
        EXPECT_NUM_ARGS(0);

        CONVERT_RECV_TO(IPAddress, self);

        return BOOLEAN(self->is_ipv4());
    }

    NATIVE_FUNCTION(ip_address_is_ipv6) {
        EXPECT_NUM_ARGS(0);

        CONVERT_RECV_TO(IPAddress, self);

        return BOOLEAN(self->is_ipv6());
    }

    MODULE_INITIALIZATION_FUNC(init_net_module) {
        Module* module = ALLOC_MODULE("net");

        return module;
    }

} // namespace modules
} // namespace emerald
