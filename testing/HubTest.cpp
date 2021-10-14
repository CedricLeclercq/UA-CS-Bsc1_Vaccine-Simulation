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
#include "../src/Hub.h"

class HubTest: public ::testing::Test {
protected:
    friend class Hub;

    virtual void SetUp() {}

    virtual void TearDown() {}

    vector<Hub*> hub;
};

TEST_F(HubTest, HubConstructorDefault) {
    Hub hub1;
    EXPECT_TRUE(hub1.properlyInitialised());
}


TEST_F(HubTest, HubConstructor) {
    Hub newHub;
    EXPECT_TRUE(newHub.properlyInitialised());
    EXPECT_TRUE(newHub.isConsistent());
    EXPECT_EQ(newHub.getAmountOfVaccines(), 0);
    EXPECT_TRUE(newHub.enoughVaccines()); // Why true? Because there are no VCentres to give to, so there are enough vaccines
    for (int i = -5; i < 5; i++) {
        if (i <= 0) {
            EXPECT_FALSE(newHub.isValidDelivery(i));
            EXPECT_FALSE(newHub.isValidInterval(i));
            EXPECT_FALSE(newHub.isValidTransport(i));
        } else {
            EXPECT_TRUE(newHub.isValidDelivery(i));
            EXPECT_TRUE(newHub.isValidInterval(i));
            EXPECT_TRUE(newHub.isValidTransport(i));
        }
    }
    vector<VaccinationCentre*> newCentres;
    EXPECT_FALSE(newHub.isValidVCentres(newCentres));
    vector<pair<Vaccine*,int> > newVaccines;
    EXPECT_FALSE(newHub.isValidVaccinesVector(newVaccines));
    VaccinationCentre centre; VaccinationCentre * pointerCentre = & centre;
    newCentres.push_back(pointerCentre);
    Vaccine vaccine; Vaccine * pointerVaccine = & vaccine;
    newVaccines.push_back(make_pair(pointerVaccine,0));
    EXPECT_TRUE(newHub.isValidVCentres(newCentres));
    EXPECT_TRUE(newHub.isValidVaccinesVector(newVaccines));
    newVaccines.clear();
    for (int i = -5; i < 5; i++) {
        if (i < 0) {
            newVaccines.push_back(make_pair(pointerVaccine,i));
            EXPECT_FALSE(newHub.isValidVaccinesVector(newVaccines));
            newVaccines.clear();
        } else {
            newVaccines.push_back(make_pair(pointerVaccine,i));
            EXPECT_TRUE(newHub.isValidVaccinesVector(newVaccines));
            // Not clearing, when the else is true, it should work for every
            // element in the vector following the first item that passes the else
        }
    }
}

TEST_F(HubTest, vaccineLeveringCheck) {
    this->SetUp();
    //Simulation * sim = new Simulation(this->hub);
}

TEST_F(HubTest, divideOverAllTest) {
    // Create vectors for the constructor of the hub
    vector<VaccinationCentre*> allVCentres;
    vector<Vaccine*> allVaccines;
    // Create test Centres and a test vaccine with proper values to test divideOverAll()
    allVCentres.push_back(new VaccinationCentre("testCentreA","testAddressA",10000,2000));
    allVCentres.push_back(new VaccinationCentre("testCentreB","testAddressB",10000,2000));
    allVCentres.push_back(new VaccinationCentre("testCentreC","testAddressC",10000,2000));
    allVaccines.push_back(new Vaccine("testVaccine1",4000,5,1000,99999,1));
    // Create a hub with the 2 previously created vectors
    this->hub.push_back(new Hub(allVCentres,allVaccines));
    // Iterate over all the vaccines and give them their first delivery
    vector<pair<Vaccine*,int> >::iterator vaccineIterator;
    vector<pair<Vaccine*,int> > vaccines = (this->hub[0])->getVaccines();
    for (vaccineIterator = vaccines.begin(); vaccineIterator != vaccines.end(); vaccineIterator++) {
        (*vaccineIterator).second = (*vaccineIterator).first->getDelivery();
    }
    // Set the new vaccines vector of the hub
    this->hub[0]->setVaccines(vaccines);
    // Expected that there aren't enough vaccines for every centre
    /*
     * Explanation:
     * There are 4000 vaccines in the hub right now. There are 3 centres that together need 6000 vaccines. Hence
     * the function below should return false, there aren't enough vaccines.
     */
    EXPECT_FALSE(this->hub[0]->enoughVaccines());
    // Now running the divide vaccines over all function
    this->hub[0]->divideVaccinesOverAll();
    // Checking if every centre got something
    vector<VaccinationCentre*> hubCentres = this->hub[0]->getVCentres();
    vector<VaccinationCentre*>::const_iterator vCentreIt;
    for (vCentreIt = hubCentres.begin(); vCentreIt != hubCentres.end(); vCentreIt++) {
        // There should be a vaccine in every centre
        bool emptyVaccines = (*vCentreIt)->getVaccines().empty();
        EXPECT_FALSE(emptyVaccines);
        // And the amount of vaccines should not be 0
        if (!emptyVaccines) {
            EXPECT_TRUE((*vCentreIt)->getVaccines()[0].second > 0);
            //cout << "Amount in " << (*vCentreIt)->getNaam() << " :  " << (*vCentreIt)->getVaccines()[0].second;
        }
    }
}