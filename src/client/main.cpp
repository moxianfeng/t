#include <iostream>
#include <regex>
#include <boost/asio.hpp>
#include <boost/program_options.hpp>
#include <utility>
#include <glog/logging.h>
#include <boost/regex.hpp>
#include "tcp_client.h"
#include "config.h"
#include "address.h"

namespace po = boost::program_options;
namespace asio = boost::asio;
namespace ip = boost::asio::ip;

int stunPort;
Address stunAddr;

void processCommandLine(int argc, char **argv) {
    po::options_description mainCommand("Program Usage", 1024, 512);


    mainCommand.add_options()
            ("help,h", "produce help message")
            ("port,p", po::value<int>(&stunPort)->default_value(9980), "stun server port")
            ("address,a", po::value<Address>(&stunAddr)->required(), "stun server address");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, mainCommand), vm);
    po::notify(vm);

    if (vm.count("help")) {
        std::cout << mainCommand << "\n";
        exit(1);
    }
    if (!vm.count("address")) {
        std::cout << mainCommand << "\n";
        exit(1);
    }
}

int main(int argc, char **argv) {
    FLAGS_logtostderr = true;
    google::InitGoogleLogging(argv[0]);

    processCommandLine(argc, argv);

    LOG(INFO) << "use stun server: " << stunAddr << ":" << stunPort << std::endl;
    asio::io_context io_context_;

    TCPClient client(io_context_, stunAddr, stunPort);
    io_context_.run();
    return 0;
}
