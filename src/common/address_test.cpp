//
// Created by 莫显峰 on 2023/12/27.
//
#include <gtest/gtest.h>
#include "address.h"

TEST(IPDirect, BasicAssertions) {
    asio::io_context io_context;
    auto domain = std::string("3.3.3.3");

    Address addr(domain);

    addr.address(io_context, [&domain](const boost::system::error_code &ec, const std::vector<ip::address> &addresses) {
        for (const auto &addr: addresses) {
            assert(addr.to_string() == domain);
        }
    });

    io_context.run();
}

TEST(DomainResolve, BasicAssertions) {
    asio::io_context io_context;
    auto domain = "www.baidu.com";

    Address addr(domain);

    addr.address(io_context, [&domain](const boost::system::error_code &ec, const std::vector<ip::address> &addresses) {
        for (const auto &addr: addresses) {
            std::cout << domain << ": " << addr << std::endl;
        }
    });

    io_context.run();
}