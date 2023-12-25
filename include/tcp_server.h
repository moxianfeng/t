//
// Created by 莫显峰 on 2023/12/25.
//

#ifndef T_SERVER_H
#define T_SERVER_H

#include <boost/asio.hpp>
#include "tcp_connection.h"

class TCPServer {
private:
    boost::asio::io_context &io_context_;
    int port_;
    boost::asio::ip::tcp::acceptor acceptor_;

public:
    TCPServer(boost::asio::io_context &io_context, int port) :
            io_context_(io_context),
            port_(port),
            acceptor_(io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v6(), port)) {
        boost::asio::socket_base::reuse_address option(true);
        acceptor_.set_option(option);

        startAccept();
    }

    void startAccept() {
        TCPConnection::Pointer newConnection = TCPConnection::create(io_context_);

        acceptor_.async_accept(newConnection->socket(), [this, newConnection](auto && PH1) { handleAccept(newConnection, std::forward<decltype(PH1)>(PH1)); });
    }

    void handleAccept(const TCPConnection::Pointer& newConnection, const boost::system::error_code& error) {
        if ( !error ) {
            newConnection->start();
        }

        startAccept();
    }
};

#endif //T_SERVER_H
