// +--------------------------------------------------+
// | Created by Cédric Leclercq and Maarten Peirsman  |
// |                                                  |
// |             University of Antwerp                |
// |                                                  |
// |           Project software engineering           |
// |                   2020-2021                      |
// +--------------------------------------------------+

#include "../src/Vaccine.h"
#include <gtest/gtest.h>

class VaccineTest: public ::testing::Test {
    friend class Vaccine;

    virtual void SetUp() {}

    virtual void TearDown() {}
};

TEST_F(VaccineTest,DefaultConstructor) {
    Vaccine vaccine;
    EXPECT_TRUE(vaccine.properlyInitialised());
    EXPECT_TRUE(vaccine.getDelivery() == 0);
    EXPECT_TRUE(vaccine.getTransport() == 0);
    EXPECT_TRUE(vaccine.getType().empty());
    EXPECT_TRUE(vaccine.getInterval() == 0);
    EXPECT_TRUE(vaccine.getRenewal() == 0);
    EXPECT_TRUE(vaccine.getTemperature() == 0);
    for (int it = -5; it < 5; it++) {
        if (it <= 0) {
            EXPECT_FALSE(vaccine.isValidTransport(it));
            EXPECT_FALSE(vaccine.isValidInterval(it));
            EXPECT_FALSE(vaccine.isValidDelivery(it));
            EXPECT_FALSE(vaccine.isValidRenewal(it));
        } else {
            EXPECT_TRUE(vaccine.isValidTransport(it));
            EXPECT_TRUE(vaccine.isValidInterval(it));
            EXPECT_TRUE(vaccine.isValidDelivery(it));
            EXPECT_TRUE(vaccine.isValidRenewal(it));
        }
    }
}

