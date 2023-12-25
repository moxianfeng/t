//
// Created by 莫显峰 on 2023/12/20.
//
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <functional>

using namespace boost::asio::ip;

class tcp_connection : public std::enable_shared_from_this<tcp_connection> {
public:
    typedef std::shared_ptr<tcp_connection> pointer;

    static pointer create(boost::asio::io_context &io_context) {
        return pointer(new tcp_connection(io_context));
    }

    tcp::socket &socket() {
        return socket_;
    }

    void start() {

    }

private:
    explicit tcp_connection(boost::asio::io_context &io_context) : socket_(io_context) {}

    tcp::socket socket_;
    std::string message_;
};
class tcp_server
{
public:
    tcp_server(boost::asio::io_context& io_context, int port)
            : io_context_(io_context),
              acceptor_(io_context,  tcp::endpoint(tcp::v4(), port))
    {
        start_accept();
    }

private:
    void start_accept()
    {
        tcp_connection::pointer new_connection =
                tcp_connection::create(io_context_);

//        acceptor_.async_accept(new_connection->socket(),
//                               std::bind(&tcp_server::handle_accept, this, new_connection,
//                                         boost::asio::placeholders::error));

//        acceptor_.async_accept(new_connection->socket(), std::bind(&tcp_server::handle_error, this, std::placeholders::_1));
        acceptor_.async_accept(new_connection->socket(), [this](auto && PH1) { handle_error(std::forward<decltype(PH1)>(PH1)); });

//        acceptor_.async_accept(new_connection->socket(), std::bind(&tcp_server::handle_accept, this, new_connection, std::placeholders::_1));
        acceptor_.async_accept(new_connection->socket(), [this, new_connection](auto && PH1) { handle_accept(new_connection, std::forward<decltype(PH1)>(PH1)); });
    }

    void handle_error(const boost::system::error_code& error) {}

    void handle_accept(const tcp_connection::pointer& new_connection,
                       const boost::system::error_code& error)
    {
        if (!error)
        {
            new_connection->start();
        }

        start_accept();
    }

    boost::asio::io_context& io_context_;
    boost::asio::ip::tcp::acceptor acceptor_;
};