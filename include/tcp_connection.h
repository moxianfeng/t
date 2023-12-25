//
// Created by 莫显峰 on 2023/12/25.
//

#ifndef T_TCP_CONNECTION_H
#define T_TCP_CONNECTION_H

#include <memory>
#include <boost/asio.hpp>

class TCPConnection {
private:
    boost::asio::io_context &io_context_;
    boost::asio::ip::tcp::socket socket_;

public:
    typedef std::shared_ptr<TCPConnection> Pointer;

    static Pointer create(boost::asio::io_context &io_context) {
        return Pointer(new TCPConnection(io_context));
    }

    boost::asio::ip::tcp::socket &socket() {
        return socket_;
    }

    void start() {}

    void handleWrite() {}

protected:
    explicit TCPConnection(boost::asio::io_context &io_context) : io_context_(io_context), socket_(io_context) {}

};

#endif //T_TCP_CONNECTION_H
