#include <iostream>
#include <boost/asio.hpp>
#include <boost/program_options.hpp>
#include <glog/logging.h>
#include "tcp_server.h"
#include "config.h"

namespace po = boost::program_options;
namespace asio = boost::asio;

std::string configFile;

void processCommandLine(int argc, char **argv) {
    po::options_description mainCommand("Program Usage", 1024, 512);

    mainCommand.add_options()
            ("help,h", "produce help message")
            ("config,C", po::value<std::string>(&configFile)->default_value("config.yaml"), "config file");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, mainCommand), vm);
    po::notify(vm);

    if (vm.count("help")) {
        std::cout << mainCommand << "\n";
        exit(1);
    }
}

int main(int argc, char **argv) {
    FLAGS_logtostderr = true;
    google::InitGoogleLogging(argv[0]);

    processCommandLine(argc, argv);

    LOG(INFO) << "use config file: " << configFile << std::endl;

    auto config = Config::fromFile(configFile);

    LOG(INFO) << "listen port: " << config.port << std::endl;

    asio::io_context io_context_;

    TCPServer server(io_context_, config.port);
    io_context_.run();
    return 0;
}
