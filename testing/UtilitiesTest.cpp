// +--------------------------------------------------+
// | Created by Cédric Leclercq and Maarten Peirsman  |
// |                                                  |
// |             University of Antwerp                |
// |                                                  |
// |           Project software engineering           |
// |                   2020-2021                      |
// +--------------------------------------------------+

#include <gtest/gtest.h>
#include "../utilities/Utilities.h"

class UtilitiesTest: public ::testing::Test {
    friend class Utilities;

    virtual void SetUp() {
        inputLocation = "./input/";
    }

    virtual void TearDown() {}

protected:
    std::string inputLocation;
};

TEST_F(UtilitiesTest, FileExistsTestCorrect) {
    std::string inputFile = inputLocation + "InputFileCompare1.txt";
    EXPECT_TRUE(Utilities::fileExists(inputFile));
}

TEST_F(UtilitiesTest, FileExistsTestFalse) {
    std::string inputFile = "FileThatDefinitelyDoesNotExists.txt";
    EXPECT_FALSE(Utilities::fileExists(inputFile));
}

TEST_F(UtilitiesTest, FileCompareTestSameFile) {
    std::string inputFile = inputLocation + "InputFileCompare1.txt";
    // A file contains the same contents as itself
    EXPECT_TRUE(Utilities::fileCompare(inputFile, inputFile));
}

TEST_F(UtilitiesTest, FileCompareTestDifferentFilesSameContent) {
    std::string inputFile1 = inputLocation + "InputFileCompare1.txt";
    std::string inputFile2 = inputLocation + "InputFileCompare2.txt";
    // Comparing two files with the same contents. Should return true
    EXPECT_TRUE(Utilities::fileCompare(inputFile1, inputFile2));
}

TEST_F(UtilitiesTest, FileCompareTestNotExisting) {
    std::string inputFile1 = inputLocation + "InputFileCompare1.txt";
    std::string inputFile2 = "FileThatDefinitelyDoesNotExist.txt";
    EXPECT_DEATH(Utilities::fileCompare(inputFile1, inputFile2), "File: " + inputFile2 + " does not exist");
}

TEST_F(UtilitiesTest, FileCompareTestDifferentContents) {
    std::string inputFile1 = inputLocation + "InputFileCompare1.txt";
    std::string inputFile2 = inputLocation + "InputFileCompareOtherContents.txt";
    EXPECT_FALSE(Utilities::fileCompare(inputFile1, inputFile2));
}