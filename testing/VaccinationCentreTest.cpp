// +--------------------------------------------------+
// | Created by Cédric Leclercq and Maarten Peirsman  |
// |                                                  |
// |             University of Antwerp                |
// |                                                  |
// |           Project software engineering           |
// |                   2020-2021                      |
// +--------------------------------------------------+

#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <gtest/gtest.h>

#include "../src/VaccinationCentre.h"
#include "../src/Vaccine.h"

class VaccinationCentreTest: public ::testing::Test {
protected:
    friend class VaccinationCentre;

    virtual void SetUp() {}

    virtual void TearDown() {}

    VaccinationCentre vaccinatiecentrum;
};

/**
 * Tests the default constructor for VaccinationCentre
 */
TEST_F(VaccinationCentreTest, VaccinatiecentrumConstructorDefault) {
    VaccinationCentre vCentrum;
    EXPECT_TRUE(vCentrum.properlyInitialised());
}

TEST_F(VaccinationCentreTest, VaccinatiecentrumConstructor) {
    VaccinationCentre vCentrum = VaccinationCentre("Park-Spoor-Oost", "Antwerpenlei", 12, 38);
    EXPECT_TRUE(vCentrum.properlyInitialised());
    EXPECT_EQ(vCentrum.getNaam(), "Park-Spoor-Oost");
    EXPECT_FALSE(vCentrum.getNaam().empty());
    EXPECT_EQ(vCentrum.getAdres(), "Antwerpenlei");
    EXPECT_FALSE(vCentrum.getAdres().empty());
    EXPECT_EQ(vCentrum.getCapacity(), 38);
    EXPECT_TRUE(vCentrum.getVaccines().empty()); // Zou 0 moeten zijn bij initialisatie
    EXPECT_TRUE(not vCentrum.getInhabitants().empty()); // Zou 12 moeten zijn bij initialisatie
    EXPECT_DEATH(vCentrum = VaccinationCentre("", "straatEen", 3, 6), "Name should not be empty");
    EXPECT_FALSE(vCentrum.getNaam() == "");
    EXPECT_DEATH(vCentrum = VaccinationCentre("Een", "", 3, 6), "newAddress should not be empty");
    EXPECT_FALSE(vCentrum.getAdres() == "");
    EXPECT_DEATH(vCentrum = VaccinationCentre("Een", "straatEen", 0, 6), "newInhabitants should be non-negative and non-zero");
    EXPECT_FALSE(vCentrum.getAmountOfInhabitants() == 0);
    EXPECT_DEATH(vCentrum = VaccinationCentre("Een", "straatEen", -1, 6), "newInhabitants should be non-negative and non-zero");
    EXPECT_FALSE(vCentrum.getAmountOfInhabitants() == -1);
    EXPECT_DEATH(vCentrum = VaccinationCentre("Een", "straatEen", 3, 0), "newCapacity should be non-negative and non-zero");
    EXPECT_FALSE(vCentrum.getCapacity() == 0);
    EXPECT_DEATH(vCentrum = VaccinationCentre("Een", "straatEen", 3, -8), "newCapacity should be non-negative and non-zero");
    EXPECT_FALSE(vCentrum.getCapacity() == -8);
}

TEST_F(VaccinationCentreTest, InwonersCheck) {
    VaccinationCentre vCentrum;
    vCentrum.setInhabitantsInt(150);
    EXPECT_EQ(vCentrum.getAmountOfInhabitants(), 150);
    EXPECT_DEATH(vCentrum.setInhabitantsInt(-8), "newInhabitants should be non-zero and non-negative");
    EXPECT_FALSE(vCentrum.getAmountOfInhabitants() == -8);
    EXPECT_DEATH(vCentrum.setInhabitantsInt(0), "newInhabitants should be non-zero and non-negative");
    EXPECT_FALSE(vCentrum.getAmountOfInhabitants() == 0);
    EXPECT_EQ(vCentrum.getAmountOfInhabitants(), 150);
}

TEST_F(VaccinationCentreTest, CapaciteitCheck) {
    VaccinationCentre vCentrum;
    vCentrum.setCapacity(8000);
    EXPECT_EQ(vCentrum.getCapacity(), 8000);
    EXPECT_DEATH(vCentrum.setCapacity(-950), "newCapacity should be non-negative and non-zero");
    EXPECT_FALSE(vCentrum.getCapacity() == -950);
    EXPECT_DEATH(vCentrum.setCapacity(0), "newCapacity should be non-negative and non-zero");
    EXPECT_FALSE(vCentrum.getCapacity() == 0);
    EXPECT_EQ(vCentrum.getCapacity(), 8000);
}


TEST_F(VaccinationCentreTest, NaamCheck) {
    VaccinationCentre vCentrum;
    std::string validString = "test";
    std::string invalidString = "";
    std::string validString2 = "Naam";
    vCentrum.setName(validString);
    EXPECT_EQ(vCentrum.getNaam(), validString);
    EXPECT_DEATH(vCentrum.setName(invalidString), "newName for VaccinationCentre Object should not be empty");
    EXPECT_FALSE(vCentrum.getNaam() == invalidString);
    EXPECT_EQ(vCentrum.getNaam(), validString);
    vCentrum.setName(validString2);
    EXPECT_EQ(vCentrum.getNaam(), validString2);
}

TEST_F(VaccinationCentreTest, AdresCheck) {
    VaccinationCentre vCentrum;
    std::string validString = "test";
    std::string invalidString = "";
    std::string validString2 = "Adres";
    vCentrum.setAddress(validString);
    EXPECT_EQ(vCentrum.getAdres(), validString);
    EXPECT_DEATH(vCentrum.setAddress(invalidString), "newAddress for VaccinationCentre Object should not be empty");
    EXPECT_FALSE(vCentrum.getAdres() == invalidString);
    EXPECT_EQ(vCentrum.getAdres(), validString);
    vCentrum.setAddress(validString2);
    EXPECT_EQ(vCentrum.getAdres(), validString2);
}

TEST_F(VaccinationCentreTest, testMethods) {
    VaccinationCentre testCentre;
    EXPECT_FALSE(testCentre.isValidAddress(""));
    EXPECT_FALSE(testCentre.isValidCapacity(-1));
    EXPECT_FALSE(testCentre.isValidCapacity(0));
    EXPECT_TRUE(testCentre.isValidCapacity(1));
    EXPECT_FALSE(testCentre.isValidInhabitants(-1));
    EXPECT_FALSE(testCentre.isValidInhabitants(0));
    EXPECT_TRUE(testCentre.isValidInhabitants(1));
    EXPECT_FALSE(testCentre.isValidName(""));
    EXPECT_TRUE(testCentre.isValidName("Test"));
    EXPECT_TRUE(testCentre.isValidAddress("Test"));
    EXPECT_DEATH(testCentre.setInhabitantsInt(-1),"newInhabitants should be non-zero and non-negative");
    EXPECT_DEATH(testCentre.setAddress(""),"newAddress for VaccinationCentre Object should not be empty");
    EXPECT_DEATH(testCentre.setName(""),"newName for VaccinationCentre Object should not be empty");
    EXPECT_DEATH(testCentre.setCapacity(-1),"newCapacity should be non-negative and non-zero");
    EXPECT_NO_FATAL_FAILURE(testCentre.setInhabitantsInt(2));
    EXPECT_NO_FATAL_FAILURE(testCentre.setAddress("Test"));
    EXPECT_NO_FATAL_FAILURE(testCentre.setName("Test"));
}

TEST_F(VaccinationCentreTest, vaccinateTest) {
    VaccinationCentre testCentre;
    EXPECT_NO_FATAL_FAILURE(testCentre.vaccinate());
}

TEST_F(VaccinationCentreTest, transportTest) {
    VaccinationCentre testCentre;
    EXPECT_DEATH(testCentre.transportNecessaryVaccines(NULL, cout),"currentHub was a nullptr upon calling transportNecessaryVaccines");
    EXPECT_DEATH(testCentre.transportUnspecifiedVaccines(NULL, cout),"currentHub was a nullptr upon calling transportUnspecifiedVaccines");
}

TEST_F(VaccinationCentreTest, cleanOutBadVaccinesTest) {
    vector<pair<Vaccine*,int> > results;
    Vaccine* testA = new Vaccine("typeA",999,999,999,999,-1);
    Vaccine* testB = new Vaccine("typeB",999,999,999,999,-1);
    results.push_back(make_pair(testA,10));
    results.push_back(make_pair(testB,0));
    this->vaccinatiecentrum.setVaccinations(results);
    this->vaccinatiecentrum.cleanOutBadVaccines();
    results.clear();
    results = this->vaccinatiecentrum.getVaccines();
    EXPECT_TRUE(results[0].second == 0);
    EXPECT_TRUE(results[1].second == 0);
    delete testA;
    delete testB;
}

TEST_F(VaccinationCentreTest, dontCleanOutGoodVaccinesTest) {
    vector<pair<Vaccine*,int> > results;
    Vaccine* testA = new Vaccine("typeA",999,999,999,999,1);
    Vaccine* testB = new Vaccine("typeB",999,999,999,999,0);
    Vaccine* testC = new Vaccine("typeC",999,999,999,999,2);
    results.push_back(make_pair(testA,10));
    results.push_back(make_pair(testB,15));
    results.push_back(make_pair(testC,0));
    this->vaccinatiecentrum.setVaccinations(results);
    this->vaccinatiecentrum.cleanOutBadVaccines();
    results.clear();
    results = this->vaccinatiecentrum.getVaccines();
    EXPECT_TRUE(results[0].second == 10);
    EXPECT_TRUE(results[1].second == 15);
    EXPECT_TRUE(results[2].second == 0);
    delete testA;
    delete testB;
    delete testC;
}

TEST_F(VaccinationCentreTest, cleanOutBadVaccinesMixedTest) {
    Vaccine vaccine("typeA",999,999,999,999,-1);
    vector<pair<Vaccine*,int> > results;
    Vaccine* testA = new Vaccine("typeA",999,999,999,999,-1);
    Vaccine* testB = new Vaccine("typeB",999,999,999,999,0);
    Vaccine* testC = new Vaccine("typeC",999,999,999,999,1);
    results.push_back(make_pair(testA,10));
    results.push_back(make_pair(testB,15));
    results.push_back(make_pair(testC,10));
    this->vaccinatiecentrum.setVaccinations(results);
    this->vaccinatiecentrum.cleanOutBadVaccines();
    results.clear();
    results = this->vaccinatiecentrum.getVaccines();
    EXPECT_TRUE(results[0].second == 0);
    EXPECT_TRUE(results[1].second == 15);
    EXPECT_TRUE(results[2].second == 10);
    delete testA;
    delete testB;
    delete testC;
}

