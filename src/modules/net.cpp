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

#include "fmt/format.h"

#include "emerald/interpreter.h"
#include "emerald/magic_methods.h"
#include "emerald/module.h"
#include "emerald/modules/net.h"
#include "emerald/native_variables.h"
#include "emerald/process.h"

namespace emerald {
namespace modules {

    IPAddress::IPAddress(Process* process)
        : Object(process, OBJECT_PROTOTYPE) {}

    IPAddress::IPAddress(Process* process, Object* parent)
        : Object(process, parent) {}

    IPAddress* IPAddress::from_native_address(
            Process* process,
            Object* parent,
            const boost::asio::ip::address& address) {
        IPAddress* ip_address = process->get_heap().allocate<IPAddress>(
            process, parent);
        ip_address->_address = address;
        return ip_address;
    }

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
        return BOOLEAN_IN_CTX(_address.is_loopback(), get_process());
    }

    Boolean* IPAddress::is_multicast() const {
        return BOOLEAN_IN_CTX(_address.is_multicast(), get_process());
    }

    Boolean* IPAddress::is_unspecified() const {
        return BOOLEAN_IN_CTX(_address.is_unspecified(), get_process());
    }

    Boolean* IPAddress::is_ipv4() const {
        return BOOLEAN_IN_CTX(_address.is_v4(), get_process());
    }

    Boolean* IPAddress::is_ipv6() const {
        return BOOLEAN_IN_CTX(_address.is_v6(), get_process());
    }

    IPAddress* IPAddress::clone(Process* process, CloneCache& cache) {
        IPAddress* clone = clone_impl<IPAddress>(process, cache);
        clone->_address = _address;
        return clone;
    }

    IPEndpoint::IPEndpoint(Process* process)
        : Object(process, OBJECT_PROTOTYPE),
        _address(nullptr),
        _port(nullptr) {}

    IPEndpoint::IPEndpoint(Process* process, Object* parent)
        : Object(process, parent),
        _address(nullptr),
        _port(nullptr) {}

    IPEndpoint* IPEndpoint::from_native_endpoint(
            Process* process,
            Object* address_parent,
            Object* endpoint_parent,
            const boost::asio::ip::tcp::endpoint& endpoint) {
        NativeStack::ScopedNativeFrame scoped_frame(process->get_native_stack().push_frame());

        Local<IPAddress> address = Interpreter::create_obj<IPAddress>(
            address_parent,
            { ALLOC_STRING(endpoint.address().to_string()) },
            process);
        Local<Number> port = ALLOC_NUMBER(endpoint.port());
        Local<IPEndpoint> ip_endpoint = process->get_heap().allocate<IPEndpoint>(
            process, endpoint_parent);
        ip_endpoint->_address = address.val();
        ip_endpoint->_port = port.val();
        ip_endpoint->_endpoint = endpoint;

        return ip_endpoint.val();
    }

    std::string IPEndpoint::as_str() const {
        return fmt::format("{0}:{1}", _endpoint.address().to_string(), _endpoint.port());
    }

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

    IPEndpoint* IPEndpoint::clone(Process* process, CloneCache& cache) {
        IPEndpoint* clone = clone_impl<IPEndpoint>(process, cache);
        clone->_address = _address->clone(process, cache);
        clone->_port = _port->clone(process, cache);
        clone->_endpoint = _endpoint;
        return clone;
    }

    TcpClient::TcpClient(Process* process)
        : Object(process, OBJECT_PROTOTYPE),
        _socket(_service) {}

    TcpClient::TcpClient(Process* process, Object* parent)
        : Object(process, parent),
        _socket(_service) {}

    Boolean* TcpClient::connect(IPEndpoint* endpoint) {
        boost::system::error_code error;
        _socket.connect(endpoint->get_native_endpoint(), error);
        if (error) {
            return BOOLEAN_IN_CTX(false, get_process());
        }

        return BOOLEAN_IN_CTX(true, get_process());
    }

    String* TcpClient::read(Number* bytes) {
        boost::asio::streambuf buffer;
        boost::asio::read(_socket, buffer, boost::asio::transfer_exactly(bytes->get_native_value()));
        return ALLOC_STRING_IN_CTX(
            std::string(boost::asio::buffer_cast<const char*>(buffer.data())),
            get_process());
    }

    void TcpClient::write(String* buffer) {
        boost::asio::write(_socket, boost::asio::buffer(buffer->get_native_value()));
    }

    TcpClient* TcpClient::clone(Process* process, CloneCache& cache) {
        return clone_impl<TcpClient>(process, cache);
    }

    TcpListener::TcpListener(Process* process)
        : Object(process, OBJECT_PROTOTYPE),
        _listening(false),
        _endpoint(nullptr),
        _acceptor(_service) {}

    TcpListener::TcpListener(Process* process, Object* parent)
        : Object(process, parent),
        _listening(false),
        _endpoint(nullptr),
        _acceptor(_service) {}

    void TcpListener::init(IPEndpoint* endpoint) {
        _endpoint = endpoint;
    }

    void TcpListener::start() {
        const boost::asio::ip::tcp::endpoint& endpoint = _endpoint->get_native_endpoint();
        _acceptor.open(endpoint.protocol());
        _acceptor.bind(endpoint);
        _acceptor.listen();
        _listening = true;
    }

    void TcpListener::stop() {
        _acceptor.close();
    }

    Boolean* TcpListener::is_listening() const {
        return BOOLEAN_IN_CTX(_listening, get_process());
    }

    void TcpListener::accept(TcpClient* client) {
        _acceptor.accept(client->_socket);
    }

    IPEndpoint* TcpListener::get_endpoint() const {
        return _endpoint;
    }

    TcpListener* TcpListener::clone(Process* process, CloneCache& cache) {
        TcpListener* clone = clone_impl<TcpListener>(process, cache);
        clone->_endpoint = _endpoint->clone(process, cache);
        return clone;
    }

    NATIVE_FUNCTION(ip_address_clone) {
        EXPECT_NUM_ARGS(0);

        CONVERT_RECV_TO(IPAddress, self);

        return process->get_heap().allocate<IPAddress>(process, self);
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

        return process->get_heap().allocate<IPEndpoint>(process, self);
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

    NATIVE_FUNCTION(tcp_client_clone) {
        EXPECT_NUM_ARGS(0);

        CONVERT_RECV_TO(TcpClient, self);

        return process->get_heap().allocate<TcpClient>(process, self);
    }

    NATIVE_FUNCTION(tcp_client_connect) {
        EXPECT_NUM_ARGS(1);

        CONVERT_RECV_TO(TcpClient, self);
        CONVERT_ARG_TO(0, IPEndpoint, endpoint);

        self->connect(endpoint);

        return NONE;
    }

    NATIVE_FUNCTION(tcp_client_read) {
        EXPECT_NUM_ARGS(1);

        CONVERT_RECV_TO(TcpClient, self);
        CONVERT_ARG_TO(0, Number, bytes);

        return self->read(bytes);
    }

    NATIVE_FUNCTION(tcp_client_write) {
        EXPECT_NUM_ARGS(1);

        CONVERT_RECV_TO(TcpClient, self);
        CONVERT_ARG_TO(0, String, buffer);

        self->write(buffer);

        return NONE;
    }

    NATIVE_FUNCTION(tcp_listener_clone) {
        EXPECT_NUM_ARGS(0);

        CONVERT_RECV_TO(TcpListener, self);

        return process->get_heap().allocate<TcpListener>(process, self);
    }

    NATIVE_FUNCTION(tcp_listener_init) {
        EXPECT_NUM_ARGS(1);

        CONVERT_RECV_TO(TcpListener, self);
        CONVERT_ARG_TO(0, IPEndpoint, endpoint);
        self->init(endpoint);

        return NONE;
    }

    NATIVE_FUNCTION(tcp_listener_start) {
        EXPECT_NUM_ARGS(0);

        CONVERT_RECV_TO(TcpListener, self);
        self->start();

        return NONE;
    }

    NATIVE_FUNCTION(tcp_listener_stop) {
        EXPECT_NUM_ARGS(0);

        CONVERT_RECV_TO(TcpListener, self);
        self->stop();

        return NONE;
    }

    NATIVE_FUNCTION(tcp_listener_is_listening) {
        EXPECT_NUM_ARGS(0);

        CONVERT_RECV_TO(TcpListener, self);

        return BOOLEAN(self->is_listening());
    }

    NATIVE_FUNCTION(tcp_listener_accept) {
        EXPECT_NUM_ARGS(0);

        CONVERT_RECV_TO(TcpListener, self);
        TcpClient* client = Interpreter::create_obj<TcpClient>(
            frame->get_global("TcpClient"),
            {},
            process);
        self->accept(client);

        return client;
    }

    NATIVE_FUNCTION(tcp_listener_get_endpoint) {
        EXPECT_NUM_ARGS(0);

        CONVERT_RECV_TO(TcpListener, self);

        return self->get_endpoint();
    }

    NATIVE_FUNCTION(net_resolve) {
        EXPECT_NUM_ARGS(1);

        CONVERT_ARG_TO(0, String, hostname);

        boost::asio::io_service service;
        boost::asio::ip::tcp::resolver resolver(service);
        boost::asio::ip::tcp::resolver::iterator iter = resolver.resolve(hostname->get_native_value(), "");
        boost::asio::ip::tcp::resolver::iterator end;
        Local<Array> res = ALLOC_EMPTY_ARRAY();
        while (iter != end) {
            const boost::asio::ip::tcp::endpoint& endpoint = *iter;
            res->push(IPAddress::from_native_address(
                process,
                frame->get_global("IPAddress"),
                endpoint.address()));
            iter++;
        }

        return res.val();
    }

    MODULE_INITIALIZATION_FUNC(init_net_module) {
        Process* process =  module->get_process();

        Local<IPAddress> ip_address = process->get_heap().allocate<IPAddress>(process);
        ip_address->set_property(magic_methods::clone, ALLOC_NATIVE_FUNCTION(ip_address_clone));
        ip_address->set_property(magic_methods::init, ALLOC_NATIVE_FUNCTION(ip_address_init));
        ip_address->set_property("is_loopback", ALLOC_NATIVE_FUNCTION(ip_address_is_loopback));
        ip_address->set_property("is_multicast", ALLOC_NATIVE_FUNCTION(ip_address_is_multicast));
        ip_address->set_property("is_unspecified", ALLOC_NATIVE_FUNCTION(ip_address_is_unspecified));
        ip_address->set_property("is_ipv4", ALLOC_NATIVE_FUNCTION(ip_address_is_ipv4));
        ip_address->set_property("is_ipv6", ALLOC_NATIVE_FUNCTION(ip_address_is_ipv6));
        module->set_property("IPAddress", ip_address.val());

        Local<IPEndpoint> ip_endpoint = process->get_heap().allocate<IPEndpoint>(process);
        ip_endpoint->set_property(magic_methods::clone, ALLOC_NATIVE_FUNCTION(ip_endpoint_clone));
        ip_endpoint->set_property(magic_methods::init, ALLOC_NATIVE_FUNCTION(ip_endpoint_init));
        ip_endpoint->set_property("get_address", ALLOC_NATIVE_FUNCTION(ip_endpoint_get_address));
        ip_endpoint->set_property("get_port", ALLOC_NATIVE_FUNCTION(ip_endpoint_get_port));
        module->set_property("IPEndpoint", ip_endpoint.val());

        Local<TcpClient> tcp_client = process->get_heap().allocate<TcpClient>(process);
        tcp_client->set_property(magic_methods::clone, ALLOC_NATIVE_FUNCTION(tcp_client_clone));
        tcp_client->set_property("connect", ALLOC_NATIVE_FUNCTION(tcp_client_connect));
        tcp_client->set_property("read", ALLOC_NATIVE_FUNCTION(tcp_client_read));
        tcp_client->set_property("write", ALLOC_NATIVE_FUNCTION(tcp_client_write));
        module->set_property("TcpClient", tcp_client.val());

        Local<TcpListener> tcp_listener = process->get_heap().allocate<TcpListener>(process);
        tcp_listener->set_property(magic_methods::clone, ALLOC_NATIVE_FUNCTION(tcp_listener_clone));
        tcp_listener->set_property(magic_methods::init, ALLOC_NATIVE_FUNCTION(tcp_listener_init));
        tcp_listener->set_property("start", ALLOC_NATIVE_FUNCTION(tcp_listener_start));
        tcp_listener->set_property("stop", ALLOC_NATIVE_FUNCTION(tcp_listener_stop));
        tcp_listener->set_property("is_listening", ALLOC_NATIVE_FUNCTION(tcp_listener_is_listening));
        tcp_listener->set_property("accept", ALLOC_NATIVE_FUNCTION(tcp_listener_accept));
        tcp_listener->set_property("get_endpoint", ALLOC_NATIVE_FUNCTION(tcp_listener_get_endpoint));
        module->set_property("TcpListener", tcp_listener.val());

        module->set_property("resolve", ALLOC_NATIVE_FUNCTION(net_resolve));
    }

} // namespace modules
} // namespace emerald
