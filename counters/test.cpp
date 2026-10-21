#include "counters.h"

#include <gtest/gtest.h>

TEST(Counters, Ok)
{
    std::map<std::string, std::vector<Break>> report = checkDirectory(OK_DIR);
    ASSERT_EQ(report.size(), 8u);
    for (const auto &file : report)
        EXPECT_EQ(file.second.size(), 0u);
}

TEST(Counters, Error)
{
    std::map<std::string, std::vector<Break>> report = checkDirectory(ERROR_DIR);
    ASSERT_EQ(report.size(), 8u);
    for (const auto &file : report)
        EXPECT_EQ(file.second.size(), 128u);
}
