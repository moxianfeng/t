//
// Created by 莫显峰 on 2023/12/25.
//

#include "address.h"

const std::regex Address::domainRegex_ = std::regex(R"(^(?!:\/\/)(?=.{1,255}$)((.{1,63}\.){1,127}(?![0-9]*$)[a-z0-9-]+\.?)$)");
