// +--------------------------------------------------+
// | Created by Cédric Leclercq and Maarten Peirsman  |
// |                                                  |
// |             University of Antwerp                |
// |                                                  |
// |           Project software engineering           |
// |                   2020-2021                      |
// +--------------------------------------------------+

#include <iostream>
#include <gtest/gtest.h>
#include "../simulation/Simulation.h"
#include "../utilities/Importer.h"

using namespace std;

class SimulationTest: public ::testing::Test {
protected:
    friend class Simulation;

    virtual void SetUp() {
        this->simulation = new Simulation;
        Importer::importSimulation("input/schemaTest.xml", cout, *this->simulation);
    }

    virtual void TearDown() {}

    Simulation * simulation;


};

TEST_F(SimulationTest,DefaultConstructor) {
    Simulation sim;
    EXPECT_TRUE(sim.properlyInitialised());
    EXPECT_TRUE(sim.isConsistent());
    EXPECT_TRUE(sim.getHubs().empty());
}

TEST_F(SimulationTest,Test0Days) {
    this->SetUp();
    ostream * file = new ofstream("temp.txt");
    EXPECT_DEATH(this->simulation->autoSimulation(0,*file,false),"failed assertion `Simulation time should be non-negative and non-zero'");
    remove("temp.txt");
    delete file;
}

TEST_F(SimulationTest, Test1Day) {
    this->SetUp();
    ostream  * file = new ofstream("temp.txt");
    this->simulation->autoSimulation(1,*file,false);
    EXPECT_TRUE(Utilities::fileCompare("temp.txt","output/Sim-ExpectedOutput1.txt"));
    remove("temp.txt");
    delete file;
}

TEST_F(SimulationTest,TestSimOutput10Days) {
    this->SetUp();
    ostream * file = new ofstream ("temp.txt");
    this->simulation->autoSimulation(10,*file,false);
    EXPECT_TRUE(Utilities::fileCompare("temp.txt","output/Sim-ExpectedOutput.txt"));
    remove("temp.txt");
    delete file;
}

TEST_F(SimulationTest, DISABLED_Test100Days) {
    this->SetUp();
    ostream * file = new ofstream ("temp.txt");
    this->simulation->autoSimulation(100, *file);
    remove("temp.txt");
    delete file;
}

TEST_F(SimulationTest, DISABLED_Test1000Days) {
    this->SetUp();
    ostream * file = new ofstream("temp.txt");
    this->simulation->autoSimulation(1000, *file);
    remove("temp.txt");
    delete file;
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
