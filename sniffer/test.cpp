#include "sniffer.h"

#include <gtest/gtest.h>

TEST(Sniffer, CountMacs)
{
    std::map<std::string, int> expected = {
        {"00:0c:29:65:08:ee", 3713},
        {"34:1c:f0:d2:78:5a", 5307},
        {"34:1c:f0:d3:40:a2", 5812},
        {"4a:5f:99:ae:ea:99", 98},
        {"52:ff:20:52:16:9a", 14},
        {"6e:52:4e:5f:f9:eb", 107},
        {"70:c9:32:1b:54:e2", 13},
        {"80:b6:55:60:6f:58", 4},
        {"84:c5:a6:07:38:66", 124},
        {"b8:69:f4:7a:a5:93", 68},
        {"b8:69:f4:7a:a5:ac", 15235},
        {"c8:7f:54:28:74:ac", 3},
        {"ff:ff:ff:ff:ff:ff", 98},
    };

    std::map<std::string, int> counts = countMacs(LOG_FILE);
    ASSERT_EQ(counts.size(), 13u);
    EXPECT_EQ(counts, expected);
}
