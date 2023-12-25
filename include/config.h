//
// Created by 莫显峰 on 2023/12/25.
//

#ifndef T_CONFIG_H
#define T_CONFIG_H

#include <yaml-cpp/yaml.h>

class Config {
public:
    int port;

public:
    static Config fromFile(const std::string &filename) {
        return YAML::LoadFile(filename).as<Config>();
    }

    static Config fromContent(const std::string &content) {
        return YAML::Load(content).as<Config>();
    }
};

namespace YAML {
    template<>
    struct convert<Config> {
        static Node encode(const Config& config) {
            Node node;
            node["port"] = config.port;
            return node;
        }

        static bool decode(const Node& node, Config& config) {
            if( !node.IsMap() ) {
                return false;
            }
            config.port = node["port"].as<int>();
            return true;
        }
    };
}

#endif //T_CONFIG_H
