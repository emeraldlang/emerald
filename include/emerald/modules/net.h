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

#include <boost/asio/ip/tcp.hpp>

#include "emerald/module_registry.h"
#include "emerald/object.h"

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

namespace emerald {
namespace modules {

    class IPAddress final : public Object {
    public:
        IPAddress(ExecutionContext* context);
        IPAddress(ExecutionContext* context, Object* parent);

        std::string as_str() const override;

        const boost::asio::ip::address& get_native_address() const;

        void init(String* address);

        Boolean* is_loopback() const;
        Boolean* is_multicast() const;
        Boolean* is_unspecified() const;

        Boolean* is_ipv4() const;
        Boolean* is_ipv6() const;

    private:
        boost::asio::ip::address _address;
    };

    class IPEndpoint final : public Object {
    public:
        IPEndpoint(ExecutionContext* context);
        IPEndpoint(ExecutionContext* context, Object* parent);

        const boost::asio::ip::tcp::endpoint& get_native_endpoint() const;

        void init(IPAddress* address, Number* port);

        IPAddress* get_address() const;
        Number* get_port() const;

    private:
        IPAddress* _address;
        Number* _port;

        boost::asio::ip::tcp::endpoint _endpoint;
    };

    class TcpClient final : public Object {
    public:
        TcpClient(ExecutionContext* context);
        TcpClient(ExecutionContext* context, Object* parent);
    };

    class TcpListener final : public Object {
    public:
        TcpListener(ExecutionContext* context);
        TcpListener(ExecutionContext* context, Object* parent);

        void init(IPEndpoint* endpoint);

        void start();
        void stop();

        Boolean* is_listening();

        TcpClient* accept();

        IPEndpoint* get_endpoint() const;

    private:
        bool _listening;
        IPEndpoint* _endpoint;

        boost::asio::ip::tcp::acceptor _acceptor;
    };

    MODULE_INITIALIZATION_FUNC(init_net_module);

} // namespace modules
} // namespace emerald

#endif // _EMERALD_MODULES_NET_H
