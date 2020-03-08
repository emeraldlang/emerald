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

#ifndef _EMERALD_MODULES_NET_H
#define _EMERALD_MODULES_NET_H

#include <boost/asio.hpp>

#include "emerald/module_registry.h"
#include "emerald/object.h"
#include "emerald/objectutils.h"

#define IP_ADDRESS_NATIVES          \
    X(ip_address_clone)             \
    X(ip_address_init)              \
    X(ip_address_is_loopback)       \
    X(ip_address_is_multicast)      \
    X(ip_address_is_unspecified)    \
    X(ip_address_is_ipv4)           \
    X(ip_address_is_ipv6)

#define IP_ENDPOINT_NATIVES     \
    X(ip_endpoint_clone)        \
    X(ip_endpoint_init)         \
    X(ip_endpoint_get_address)  \
    X(ip_endpoint_get_port)

#define TCP_CLIENT_NATIVES  \
    X(tcp_client_clone)     \
    X(tcp_client_connect)   \
    X(tcp_client_read)      \
    X(tcp_client_write)

#define TCP_LISTENER_NATIVES        \
    X(tcp_listener_clone)           \
    X(tcp_listener_init)            \
    X(tcp_listener_start)           \
    X(tcp_listener_stop)            \
    X(tcp_listener_is_listening)    \
    X(tcp_listener_accept)          \
    X(tcp_listener_get_endpoint)

#define NET_NATIVES     \
    X(net_resolve)

namespace emerald {
namespace modules {

    class IPAddress final : public Object {
    public:
        IPAddress(Process* process);
        IPAddress(Process* process, Object* parent);

        static IPAddress* from_native_address(
            Process* process,
            Object* parent,
            const boost::asio::ip::address& address);

        std::string as_str() const override;

        const boost::asio::ip::address& get_native_address() const;

        void init(String* address);

        Boolean* is_loopback() const;
        Boolean* is_multicast() const;
        Boolean* is_unspecified() const;

        Boolean* is_ipv4() const;
        Boolean* is_ipv6() const;

        IPAddress* clone(Process* process, CloneCache& cache) override;

    private:
        boost::asio::ip::address _address;
    };

    class IPEndpoint final : public Object {
    public:
        IPEndpoint(Process* process);
        IPEndpoint(Process* process, Object* parent);

        static IPEndpoint* from_native_endpoint(
            Process* process,
            Object* address_parent,
            Object* endpoint_parent,
            const boost::asio::ip::tcp::endpoint& endpoint);

        std::string as_str() const override;

        const boost::asio::ip::tcp::endpoint& get_native_endpoint() const;

        void init(IPAddress* address, Number* port);

        IPAddress* get_address() const;
        Number* get_port() const;

        IPEndpoint* clone(Process* process, CloneCache& cache) override;

    private:
        IPAddress* _address;
        Number* _port;

        boost::asio::ip::tcp::endpoint _endpoint;
    };

    class TcpListener;

    class TcpClient final : public Object {
    public:
        TcpClient(Process* process);
        TcpClient(Process* process, Object* parent);

        Boolean* connect(IPEndpoint* endpoint);

        String* read(Number* bytes);
        void write(String* buffer);

        TcpClient* clone(Process* process, CloneCache& cache) override;

    private:
        friend class TcpListener;

        boost::asio::io_service _service;
        boost::asio::ip::tcp::socket _socket;
    };

    class TcpListener final : public Object {
    public:
        TcpListener(Process* process);
        TcpListener(Process* process, Object* parent);

        void init(IPEndpoint* endpoint);

        void start();
        void stop();

        Boolean* is_listening() const;

        void accept(TcpClient* client);

        IPEndpoint* get_endpoint() const;

        TcpListener* clone(Process* process, CloneCache& cache) override;

    private:
        bool _listening;
        IPEndpoint* _endpoint;

        boost::asio::io_service _service;
        boost::asio::ip::tcp::acceptor _acceptor;
    };

#define X(name) NATIVE_FUNCTION(name);
    IP_ADDRESS_NATIVES
    IP_ENDPOINT_NATIVES
    TCP_CLIENT_NATIVES
    TCP_LISTENER_NATIVES
    NET_NATIVES
#undef X

    MODULE_INITIALIZATION_FUNC(init_net_module);

} // namespace modules
} // namespace emerald

#endif // _EMERALD_MODULES_NET_H
