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

    const boost::asio::ip::address& IPAddress::get_native_address() const {
        return _address;
    }

    void IPAddress::init(String* address) {
        _address = boost::asio::ip::address::from_string(
            address->get_native_value());
    }

    Boolean* IPAddress::is_loopback() const {
        return BOOLEAN_IN_CTX(_address.is_loopback(), get_context());
    }

    Boolean* IPAddress::is_multicast() const {
        return BOOLEAN_IN_CTX(_address.is_multicast(), get_context());
    }

    Boolean* IPAddress::is_unspecified() const {
        return BOOLEAN_IN_CTX(_address.is_unspecified(), get_context());
    }

    Boolean* IPAddress::is_ipv4() const {
        return BOOLEAN_IN_CTX(_address.is_v4(), get_context());
    }

    Boolean* IPAddress::is_ipv6() const {
        return BOOLEAN_IN_CTX(_address.is_v6(), get_context());
    }

    IPEndpoint::IPEndpoint(ExecutionContext* context)
        : Object(context, context->get_native_objects().get_object_prototype()),
        _address(nullptr),
        _port(nullptr) {}

    IPEndpoint::IPEndpoint(ExecutionContext* context, Object* parent)
        : Object(context, parent),
        _address(nullptr),
        _port(nullptr) {}

    const boost::asio::ip::tcp::endpoint& IPEndpoint::get_native_endpoint() const {
        return _endpoint;
    }

    void IPEndpoint::init(IPAddress* address, Number* port) {
        _address = address;
        _port = port;
        _endpoint.address(address->get_native_address());
        _endpoint.port(port->get_native_value());
    }

    IPAddress* IPEndpoint::get_address() const {
        return _address;
    }

    Number* IPEndpoint::get_port() const {
        return _port;
    }

    // TcpListener::TcpListener(ExecutionContext* context)
    //     : Object(context, context->get_native_objects().get_object_prototype()),
    //     _listening(false),
    //     _endpoint(nullptr) {}

    // TcpListener::TcpListener(ExecutionContext* context, Object* parent)
    //     : Object(context, parent),
    //     _listening(false),
    //     _endpoint(nullptr) {}

    // void TcpListener::init(IPEndpoint* endpoint) {
    //     _endpoint = endpoint;
    // }

    // void TcpListener::start() {
    //     const boost::asio::ip::tcp::endpoint& endpoint = _endpoint.get_native_endpoint();
    //     _acceptor.open(endpoint.protocol());
    //     _acceptor.bind(endpoint);
    //     _acceptor.listen();
    //     _listening = true;
    // }

    // void TcpListener::stop() {
    //     _acceptor.close();
    // }

    // Boolean* TcpListener::is_listening() {
    //     return BOOLEAN_IN_CTX(_listening, get_context());
    // }

    // TcpClient* TcpListener::accept() {
    //     return nullptr;
    // }

    NATIVE_FUNCTION(ip_address_clone) {
        EXPECT_NUM_ARGS(0);

        CONVERT_RECV_TO(IPAddress, self);

        return context->get_heap().allocate<IPAddress>(context, self);
    }

    NATIVE_FUNCTION(ip_address_init) {
        EXPECT_NUM_ARGS(1);

        CONVERT_RECV_TO(IPAddress, self);
        CONVERT_ARG_TO(0, String, str);

        self->init(str);

        return NONE;
    }

    NATIVE_FUNCTION(ip_address_is_loopback) {
        EXPECT_NUM_ARGS(0);

        CONVERT_RECV_TO(IPAddress, self);

        return self->is_loopback();
    }

    NATIVE_FUNCTION(ip_address_is_multicast) {
        EXPECT_NUM_ARGS(0);

        CONVERT_RECV_TO(IPAddress, self);

        return self->is_multicast();
    }

    NATIVE_FUNCTION(ip_address_is_unspecified) {
        EXPECT_NUM_ARGS(0);

        CONVERT_RECV_TO(IPAddress, self);

        return self->is_unspecified();
    }

    NATIVE_FUNCTION(ip_address_is_ipv4) {
        EXPECT_NUM_ARGS(0);

        CONVERT_RECV_TO(IPAddress, self);

        return self->is_ipv4();
    }

    NATIVE_FUNCTION(ip_address_is_ipv6) {
        EXPECT_NUM_ARGS(0);

        CONVERT_RECV_TO(IPAddress, self);

        return self->is_ipv6();
    }

    NATIVE_FUNCTION(ip_endpoint_clone) {
        EXPECT_NUM_ARGS(0);

        CONVERT_RECV_TO(IPEndpoint, self);

        return context->get_heap().allocate<IPEndpoint>(context, self);
    }

    NATIVE_FUNCTION(ip_endpoint_init) {
        EXPECT_NUM_ARGS(2);

        CONVERT_RECV_TO(IPEndpoint, self);
        CONVERT_ARG_TO(0, IPAddress, address);
        CONVERT_ARG_TO(1, Number, port);
        self->init(address, port);

        return NONE;
    }

    NATIVE_FUNCTION(ip_endpoint_get_address) {
        EXPECT_NUM_ARGS(0);

        CONVERT_RECV_TO(IPEndpoint, self);

        return self->get_address();
    }

    NATIVE_FUNCTION(ip_endpoint_get_port) {
        EXPECT_NUM_ARGS(0);

        CONVERT_RECV_TO(IPEndpoint, self);

        return self->get_port();
    }

    MODULE_INITIALIZATION_FUNC(init_net_module) {
        Heap& heap = context->get_heap();
        NativeObjects& native_objects = context->get_native_objects();

        Module* module = ALLOC_MODULE("net");

        IPAddress* ip_address = heap.allocate<IPAddress>(context, native_objects.get_object_prototype());
        ip_address->set_property(magic_methods::clone, ALLOC_NATIVE_FUNCTION(ip_address_clone));
        ip_address->set_property(magic_methods::init, ALLOC_NATIVE_FUNCTION(ip_address_init));
        ip_address->set_property("is_loopback", ALLOC_NATIVE_FUNCTION(ip_address_is_loopback));
        ip_address->set_property("is_multicast", ALLOC_NATIVE_FUNCTION(ip_address_is_multicast));
        ip_address->set_property("is_unspecified", ALLOC_NATIVE_FUNCTION(ip_address_is_unspecified));
        ip_address->set_property("is_ipv4", ALLOC_NATIVE_FUNCTION(ip_address_is_ipv4));
        ip_address->set_property("is_ipv6", ALLOC_NATIVE_FUNCTION(ip_address_is_ipv6));

        IPEndpoint* ip_endpoint = heap.allocate<IPEndpoint>(context, native_objects.get_object_prototype());
        ip_endpoint->set_property(magic_methods::clone, ALLOC_NATIVE_FUNCTION(ip_endpoint_clone));
        ip_endpoint->set_property(magic_methods::init, ALLOC_NATIVE_FUNCTION(ip_endpoint_init));
        ip_endpoint->set_property("get_address", ALLOC_NATIVE_FUNCTION(ip_endpoint_get_address));
        ip_endpoint->set_property("get_port", ALLOC_NATIVE_FUNCTION(ip_endpoint_get_port));

        return module;
    }

} // namespace modules
} // namespace emerald
