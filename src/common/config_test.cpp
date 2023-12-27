//
// Created by 莫显峰 on 2023/12/25.
//
#include <gtest/gtest.h>
#include "config.h"

TEST(ConfigLoadFromContent, BasicAssertions) {
    auto cfg = Config::fromContent("port: 99");
    ASSERT_EQ(cfg.port, 99);
}