// +--------------------------------------------------+
// | Created by Cédric Leclercq and Maarten Peirsman  |
// |                                                  |
// |             University of Antwerp                |
// |                                                  |
// |           Project software engineering           |
// |                   2020-2021                      |
// +--------------------------------------------------+

#include "../utilities/Importer.h"
#include <gtest/gtest.h>

class ImporterTest: public ::testing::Test {
    friend class Importer;

    virtual void SetUp() {
        inputLocation = "input/";
        importInputLocation = "input/";
        outputLocation = "output/";
        compareOutputLocation = "output/";
    }

    virtual void TearDown() {}

protected:
    Simulation simulation;
    std::string inputLocation;
    std::string outputLocation;
    std::string importInputLocation;
    std::string compareOutputLocation;
};

TEST_F(ImporterTest, ImportHappyDays) {
    std::string input = inputLocation + "HappyDays.xml";
    std::string output = outputLocation + "HappyDaysOutput.txt";
    std::string outputExpected = outputLocation + "HappyDaysOutputExpected.txt";
    std::ofstream outputStream;
    outputStream.open(output.c_str());
    Success success;
    success = Importer::importSimulation(input, outputStream, simulation);
    EXPECT_EQ(Successful, success);
    // Should not give any errors, so expected output is empty
    EXPECT_TRUE(Utilities::fileCompare(output, outputExpected));
    remove(output.c_str());
}

TEST_F(ImporterTest, ImportIllegalFile) {
    std::string input = inputLocation + "IllegalFile.txt";
    std::string outputExpected = "Given file '" + input + "' is not an XML file";
    std::ostringstream outputStream;
    EXPECT_DEATH(Importer::importSimulation(input, outputStream, simulation), outputExpected.c_str());
}

TEST_F(ImporterTest, ImportIllegalSyntaxXML) {
    // Initialise fileNames of required files
    std::string inputFile = inputLocation + "InputIllegalSyntax.xml";
    std::string outputFile = outputLocation + "OutputIllegalSyntax.txt";
    std::string outputExpectedFile = outputLocation + "OutputExpectedIllegalSyntax.txt";
    // Initialise fileStreams
    std::ofstream output;
    output.open(outputFile.c_str());
    Success success = Importer::importSimulation(inputFile, output, simulation);
    output.close(); // Close the output
    // Compare output
    EXPECT_EQ(Unsuccessful, success);
    EXPECT_TRUE(Utilities::fileCompare(outputFile, outputExpectedFile));
    remove(outputFile.c_str());
}

TEST_F(ImporterTest, ImportInvalidRedefinition1) {
    // Initialise fileNames of required files
    std::string inputFile = importInputLocation + "InputInvalidRedefinition1.xml";
    std::string outputFile = compareOutputLocation + "OutputInvalidRedefinition1.txt";
    std::string outputExpectedFile = compareOutputLocation + "OutputExpectedInvalidRedefinition1.txt";
    // Initialise fileStreams
    std::ofstream output;
    output.open(outputFile.c_str());
    Success success = Importer::importSimulation(inputFile, output, simulation);
    output.close();
    // Compare output
    EXPECT_EQ(PartiallyCompleted, success);
    EXPECT_TRUE(Utilities::fileCompare(outputFile, outputExpectedFile));
    remove(outputFile.c_str());
}