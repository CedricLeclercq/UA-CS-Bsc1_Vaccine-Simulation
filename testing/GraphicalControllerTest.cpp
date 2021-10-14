// +--------------------------------------------------+
// | Created by Cédric Leclercq and Maarten Peirsman  |
// |                                                  |
// |             University of Antwerp                |
// |                                                  |
// |           Project software engineering           |
// |                   2020-2021                      |
// +--------------------------------------------------+

#include "../graphics/GraphicalController.h"
#include "../src/VaccinationCentre.h"
#include "../src/Hub.h"
#include "../src/Vaccine.h"
#include "../utilities/Utilities.h"
#include <gtest/gtest.h>

class GraphicalControllerTest: public ::testing::Test {
    friend class GraphicalController;

    virtual void SetUp() {
        ctr = new GraphicalController();
    }

    virtual void TearDown() {

    }

protected:
    GraphicalController * ctr;
};

TEST_F(GraphicalControllerTest, properlyInitialised) {
    EXPECT_TRUE(this->ctr->properlyInitialised());
    EXPECT_EQ(0,(int) this->ctr->getAllHubs().size());
    EXPECT_EQ(0,this->ctr->getCurrentFigureID());
    EXPECT_EQ(0,(int) this->ctr->getAllvCentre().size());
    EXPECT_EQ(0,(int) this->ctr->getTransported().size());
    EXPECT_EQ(0,(int) this->ctr->getCoordinatesHubs().size());
    EXPECT_EQ(0,(int) this->ctr->getCoordinatesVCentres().size());
}

TEST_F(GraphicalControllerTest, testCGIGeneration) {
    vector<VaccinationCentre*> allVCentre;
    allVCentre.push_back(new VaccinationCentre("testCentreA","testAdresA",5,100));
    allVCentre.push_back(new VaccinationCentre("testCentreB","testAdresB",5,100));
    allVCentre.push_back(new VaccinationCentre("testCentreC","testAdresC",5,100));
    allVCentre.push_back(new VaccinationCentre("testCentreD","testAdresD",5,100));
    allVCentre.push_back(new VaccinationCentre("testCentreE","testAdresE",5,100));
    allVCentre.push_back(new VaccinationCentre("testCentreF","testAdresF",5,100));
    allVCentre.push_back(new VaccinationCentre("testCentreG","testAdresG",5,100));
    vector<Vaccine*> allVaccines;
    allVaccines.push_back(new Vaccine("typeA",1,1,1,1,1));
    vector<Hub*> allHubs;
    allHubs.push_back(new Hub(allVCentre,allVaccines));
    this->ctr->setAllvCentre(allVCentre);
    this->ctr->setAllHubs(allHubs);
    this->ctr->writeGeneral(90);
    EXPECT_TRUE(Utilities::fileCompare("output/CGI-expectedA.ini","CGI/PSE-SIM-DAY-91.2.ini"));
}

TEST_F(GraphicalControllerTest, testCGITransport) {
    vector<VaccinationCentre*> allVCentre;
    VaccinationCentre * centreA = new VaccinationCentre("testCentreA","testAddressA",5,100);
    VaccinationCentre * centreB = new VaccinationCentre("testCentreB","testAddressB",5,100);
    VaccinationCentre * centreC = new VaccinationCentre("testCentreC","testAddressC",5,100);
    VaccinationCentre * centreD = new VaccinationCentre("testCentreD","testAddressD",5,100);
    allVCentre.push_back(centreA);
    allVCentre.push_back(centreB);
    allVCentre.push_back(centreC);
    allVCentre.push_back(centreD);
    vector<Vaccine*> allVaccines;
    allVaccines.push_back(new Vaccine("typeA",1,1,1,1,1));
    vector<Hub*> allHubs;
    Hub * newHub = new Hub(allVCentre,allVaccines);
    allHubs.push_back(newHub);
    vector<pair<pair<Hub*,VaccinationCentre*>, int> > transported;
    transported.push_back(make_pair(make_pair(newHub,centreA),2000));
    transported.push_back(make_pair(make_pair(newHub,centreB),10000));
    transported.push_back(make_pair(make_pair(newHub,centreC),0));
    this->ctr->setAllHubs(allHubs);
    this->ctr->setAllvCentre(allVCentre);
    this->ctr->setTransported(transported);
    this->ctr->writeGeneral(999);
    EXPECT_TRUE(Utilities::fileCompare("output/CGI-expectedB.ini","CGI/PSE-SIM-DAY-1000.2.ini"));
}