// +--------------------------------------------------+
// | Created by Cédric Leclercq and Maarten Peirsman  |
// |                                                  |
// |             University of Antwerp                |
// |                                                  |
// |           Project software engineering           |
// |                   2020-2021                      |
// +--------------------------------------------------+

#include "../src/Inhabitant.h"
#include <gtest/gtest.h>
using namespace std;


class InhabitantTest: public ::testing::Test {
    friend class Inhabitant;

    virtual void SetUp() {}

    virtual void TearDown() {}

};

TEST_F(InhabitantTest,DefaultConstructor) {
    Inhabitant inhabitant;
    EXPECT_TRUE(inhabitant.properlyInitialised());
    EXPECT_EQ(inhabitant.getVaccinated(), NotVaccinated);
    EXPECT_EQ(inhabitant.getRenewablesLeft(), 0);
}

TEST_F(InhabitantTest,AfterPartiallyVaccinated) {
    Inhabitant inhabitant;
    inhabitant.setVaccinated(PartiallyVaccinated);
    EXPECT_EQ(inhabitant.getVaccinated(), PartiallyVaccinated);
    inhabitant.setRenewablesLeft(3);
    EXPECT_EQ(inhabitant.getRenewablesLeft(),3);
}

TEST_F(InhabitantTest,AferVaccinated) {
    Inhabitant inhabitant;
    inhabitant.setVaccinated(Vaccinated);
    EXPECT_EQ(inhabitant.getVaccinated(), Vaccinated);
}

