//
// Created by 莫显峰 on 2023/12/25.
//

#ifndef T_TCP_CLIENT_H
#define T_TCP_CLIENT_H

#include <boost/asio.hpp>
#include <utility>
#include "address.h"

class TCPClient: public std::enable_shared_from_this<TCPClient>  {
    using io_context = boost::asio::io_context;
    using tcp = boost::asio::ip::tcp;
    using resolver = boost::asio::ip::tcp::resolver;


private:
    io_context &io_context_;
    tcp::socket socket_;
    Address address_;
    int port_;

public:
    explicit TCPClient(io_context &io_context, Address address) : io_context_(io_context), socket_(io_context),
                                                                        address_(std::move(address)), port_(0) {
        start();
    }

    explicit TCPClient(io_context &io_context, Address address, int port) : io_context_(io_context),
                                                                                  socket_(io_context),
                                                                                  address_(std::move(address)), port_(port) {
        start();
    }

    tcp::socket &socket() {
        return socket_;
    }

    void start() {
        address_.address(io_context_, [](const boost::system::error_code &ec, const std::vector<ip::address> &addresses){
            for ( const auto addr : addresses ) {
                std::cout << "address: " << addr << std::endl;
            }
        });
        std::cout << "start finished" << std::endl;
    }
};

#endif //T_TCP_CLIENT_H
