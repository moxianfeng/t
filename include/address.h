//
// Created by 莫显峰 on 2023/12/25.
//

#ifndef T_ADDRESS_H
#define T_ADDRESS_H

#include <memory>
#include <regex>
#include <boost/program_options.hpp>
#include <boost/asio.hpp>
#include <glog/logging.h>

namespace po = boost::program_options;
namespace asio = boost::asio;
namespace ip = boost::asio::ip;


class Address {
    using io_context = boost::asio::io_context;
    using error_code = boost::system::error_code;

protected:
    bool is_addr_;
    ip::address ip_address_;
    std::string domain_;

    static const std::regex domainRegex_;
public:
    Address() : is_addr_(false) {};

    explicit Address(ip::address addr) : ip_address_(std::move(addr)), is_addr_(true) {}
    explicit Address(std::string addr) : domain_(std::move(addr)), is_addr_(false) {}

    void address(io_context &io_context, const std::function<void(const error_code &, const std::vector<ip::address> &)>& cb) {
        if ( is_addr_ ) {
            auto _ = std::async(std::launch::async, [this, cb](){cb(error_code(), {this->ip_address_});});
        } else {
            using resolver = ip::tcp::resolver;
            using query = resolver::query;

            VLOG(2) << "async_resolve resolve " << domain_ << std::endl;
            auto _resolver = std::make_shared<resolver>(io_context);
            query q(domain_, "1", query::numeric_host);

            _resolver->async_resolve(domain_, "80", [_resolver, cb](boost::system::error_code ec, ip::tcp::resolver::iterator it){
                VLOG(2) << "async_resolve callback" << std::endl;
                if ( ec ) {
                    LOG(ERROR) << "async_resolve error, code: " << std::error_code(ec) << ", message: " << ec.message();
                }
                if ( !ec ) {
                    ip::tcp::resolver::iterator end;
                    std::vector<ip::address> addrs;
                    for ( ;it != end;it++ ) {
                        addrs.push_back(it->endpoint().address());
                    }

                    cb(ec, addrs);
                }
            });
        }
    }

    friend std::ostream &operator<<(std::ostream &os, Address const &ep) {
        return ep.is_addr_
        ? os << ep.ip_address_
        : os << ep.domain_;
    }

    friend void validate(boost::any &value, std::vector<std::string> const &values, Address *, int) {
        po::validators::check_first_occurrence(value);
        const std::string& s  = po::validators::get_single_string(values);

        boost::system::error_code ec;
        auto addr = ip::address::from_string(s, ec);
        if ( ec ) {
            if ( !std::regex_match(s, domainRegex_) ) {
                throw std::invalid_argument("invalid address `" + s + "`");
            }
            value = Address(s);
        } else {
            value = Address(addr);
        }
    }
};

#endif //T_ADDRESS_H
