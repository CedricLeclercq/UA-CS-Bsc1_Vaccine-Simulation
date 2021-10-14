// +--------------------------------------------------+
// | Created by Cédric Leclercq and Maarten Peirsman  |
// |                                                  |
// |             University of Antwerp                |
// |                                                  |
// |           Project software engineering           |
// |                   2020-2021                      |
// +--------------------------------------------------+

#include "Hub.h"

#include <iostream>
#include <algorithm>
#include <gtest/gtest.h>


void Hub::print(ostream &onstream) const {
    REQUIRE(properlyInitialised(), "Hub wasn't initialised when trying to print out the Hub");
    string output = "Hub (" + to_string(this->getAmountOfVaccines()) + " vaccinations)\n";
    vector<VaccinationCentre*>::iterator vCentre; // Creating iterator over all vCentres in hub
    vector<VaccinationCentre*> vCentresCopy = this->getVCentres();
    for (vCentre = vCentresCopy.begin(); vCentre != vCentresCopy.end(); vCentre++) {
        // Generating output
        output += "-> " + (*vCentre)->getNaam() + " (" + to_string((*vCentre)->getAmountOfVaccinatedInhabitants()) + " vaccinations)\n";
    }
    // Print output for this hub
    onstream << output << endl;
    for (vCentre = vCentresCopy.begin(); vCentre != vCentresCopy.end(); vCentre++) {
        // Print output for every vCentre
        (*vCentre)->print(onstream);
    }
}

const vector<VaccinationCentre *> &Hub::getVCentres() const {
    REQUIRE(properlyInitialised(), "Hub wasn't initialised when calling getVCentres");
    return centres;
}

void Hub::setVCentres(const vector<VaccinationCentre *> &vCentreNew) {
    REQUIRE(properlyInitialised(), "Hub wasn't initialised when trying to set the VaccinationCentres");
    REQUIRE(!vCentreNew.empty(), "Centre should not be empty");
    vector<VaccinationCentre*>::const_iterator iterator;
    for (iterator = vCentreNew.begin(); iterator != vCentreNew.end(); iterator++) {
        (*iterator)->addHub(this);
    }
    centres = vCentreNew;
    ENSURE(centres == vCentreNew, "PostCondition of setVCentres failed");
}

__attribute__((unused)) void Hub::addVCentre(VaccinationCentre *vCentre) {
    REQUIRE(properlyInitialised(), "Hub wasn't initialised when trying to add a vaccinationCentre Object");
    vCentre->addHub(this);
    centres.push_back(vCentre);
    ENSURE(contains(centres, vCentre), "addVCentre post-condition failed");
}

bool Hub::properlyInitialised() const {
    return _initCheck == this;
}


Hub::~Hub() {
    vector<VaccinationCentre*>::iterator vCentre;
    for (vCentre = centres.begin(); vCentre != centres.end(); vCentre++) {
        delete *vCentre;
    }
    this->centres.clear();
}

__attribute__((unused)) bool Hub::isValidDelivery(const int delivery) {
    return delivery > 0;
}

__attribute__((unused)) bool Hub::isValidTransport(const int transport) {
    return transport > 0;
}

__attribute__((unused)) bool Hub::isValidInterval(const int interval) {
    return interval > 0;
}

bool Hub::isValidVCentres(const vector<VaccinationCentre *> &vCentres) {
    return !vCentres.empty();
}

bool Hub::isConsistent() const {
    REQUIRE(properlyInitialised(), "Hub not properly initialised upon calling isConsistent()");
    vector<VaccinationCentre*>::const_iterator centreIterator;
    for (centreIterator = centres.begin(); centreIterator != centres.end(); centreIterator++) {
        if (!contains((*centreIterator)->getHubs(), _initCheck)) {
            return false;
        }
    }
    return true;
}

bool Hub::isValidVaccines(const vector<Vaccine*> &newVaccines) {
    return !newVaccines.empty();
}

__attribute__((unused)) bool Hub::isValidVaccinesVector(const vector<pair<Vaccine *,int> > &newVaccines) {
    vector<pair<Vaccine*,int> >::const_iterator iterator;
    for (iterator = newVaccines.begin(); iterator != newVaccines.end(); iterator++) {
        if ((*iterator).second < 0) {
            return false;
        }
    }
    return !newVaccines.empty();
}

Hub::Hub(const vector<VaccinationCentre *> &centres, const vector<Vaccine *> &newVaccines) {
    _initCheck = this;
    // Set centres to the centres vector
    setCentres(centres);
    vector<Vaccine*>::const_iterator vaccineIterator;
    for (vaccineIterator = newVaccines.begin(); vaccineIterator != newVaccines.end(); vaccineIterator++) {
        vaccines.push_back(make_pair((*vaccineIterator), 0));
    }
    ENSURE(properlyInitialised(), "Constructor should end in initialised state");
}

void Hub::setCentres(const vector<VaccinationCentre *> &newCentres) {
    REQUIRE(properlyInitialised(), "Hub should be properly initialised upon calling setCentres");
    REQUIRE(isValidVCentres(newCentres), "Centres should not be empty upon calling setCentres");
    // Iterate over centres to set the Hub to this hub
    vector<VaccinationCentre*>::const_iterator vCentreIterator; // Declare iterator
    for (vCentreIterator = newCentres.begin(); vCentreIterator != newCentres.end(); vCentreIterator++) {
        REQUIRE((*vCentreIterator)->properlyInitialised(), "All Vaccination Centres in the vector should be properly initialised upon calling setCentres");
        if (not contains((*vCentreIterator)->getHubs(),this)) {
            (*vCentreIterator)->addHub(this);
        }
    }
    // Set centres to the centres vector
    centres = newCentres;
    ENSURE(properlyInitialised(), "SetCentres should end in an initialised state");
    ENSURE(this->getVCentres() == newCentres,"VCentres was not properly set");
}

vector<pair<Vaccine *, int> > Hub::getVaccines() const {
    return vaccines;
}

void Hub::setVaccines(const vector<pair<Vaccine*,int> >& vaccinesNew) {
    REQUIRE(not vaccinesNew.empty(), "Vaccines cannot be empty upon calling setVaccines");
    this->vaccines = vaccinesNew;
    ENSURE(this->getVaccines() == vaccinesNew, "Vaccines was not properly set");
}

bool Hub::enoughVaccines() const {
    // Iterating over all the vaccines
    vector<pair<Vaccine*,int> > allVaccines = this->getVaccines();
    vector<pair<Vaccine*,int> >::const_iterator vaccineIterator;

    int amountOfVaccines = 0;
    for (vaccineIterator = allVaccines.begin(); vaccineIterator != allVaccines.end(); vaccineIterator++) {
        amountOfVaccines += (*vaccineIterator).second;
    }

    // Iterating over all the vCentre and calculating their capacity
    vector<VaccinationCentre*> allVCentre = this->getVCentres();
    vector<VaccinationCentre*>::const_iterator vCentreIterator;

    int capacityCombined = 0;
    for (vCentreIterator = allVCentre.begin(); vCentreIterator != allVCentre.end(); vCentreIterator++) {
        allVaccines = (*vCentreIterator)->getVaccines();
        int allVaccinesInCentre = 0;
        for (vaccineIterator = allVaccines.begin(); vaccineIterator != allVaccines.end(); vaccineIterator++) {
            allVaccinesInCentre += (*vaccineIterator).second;
        }
        capacityCombined += (*vCentreIterator)->getCapacity() - allVaccinesInCentre;
    }
    return amountOfVaccines >= capacityCombined;
}

void Hub::divideVaccinesOverAll() {
    REQUIRE(this->properlyInitialised(), "Hub wasn't properly initialised upon calling one of its member functions");

    // Iterators
    vector<pair<Vaccine*,int> >::iterator vaccineIt;
    vector<pair<Vaccine*,int> >::iterator cVaccineIt;
    vector<VaccinationCentre*>::iterator vCentreIt;

    // Getting all the vCentres
    vector<VaccinationCentre*> allVCentre = this->getVCentres();
    vector<pair<Vaccine *, int> > allVaccines = this->getVaccines();


    // Bool for if we did something this loop
    bool gaveVaccines = true;


    // While loop to continue giving vaccines until or there are no vaccines left or we cannot give any vaccines any more
    while (gaveVaccines) {
        gaveVaccines = false;
        // Looping over all the vCentres
        for (vCentreIt = allVCentre.begin(); vCentreIt != allVCentre.end(); vCentreIt++) {
            // Getting all the vaccines from this hub
            for (vaccineIt = allVaccines.begin(); vaccineIt != allVaccines.end(); vaccineIt++) {
                // Checking if we can transport something
                if ((*vaccineIt).second >= (*vaccineIt).first->getTransport()) {
                    // Getting all the vaccines from this vCentre
                    vector<pair<Vaccine *, int> > allCentreVaccines = (*vCentreIt)->getVaccines();
                    for (cVaccineIt = allCentreVaccines.begin(); cVaccineIt != allCentreVaccines.end(); cVaccineIt++) {
                        if ((*cVaccineIt).first == (*vaccineIt).first) {
                            (*cVaccineIt).second += (*vaccineIt).first->getTransport();
                            (*vaccineIt).second -= (*vaccineIt).first->getTransport();
                            gaveVaccines = true;
                        }
                    }
                    if (allCentreVaccines.empty()) {
                        allCentreVaccines.push_back(make_pair((*vaccineIt).first,(*vaccineIt).first->getTransport()));
                        (*vaccineIt).second -= (*vaccineIt).first->getTransport();
                        gaveVaccines = true;
                    }
                    (*vCentreIt)->setVaccinations(allCentreVaccines);
                }
            }
        }
    }
    this->setVaccines(allVaccines);
    this->setVCentres(allVCentre);
}



int Hub::getAmountOfVaccines() const {
    vector<pair<Vaccine*,int> > allVaccines = this->getVaccines();
    vector<pair<Vaccine*,int> >::const_iterator vaccineIterator;
    int total = 0;
    for (vaccineIterator = allVaccines.begin(); vaccineIterator != allVaccines.end(); vaccineIterator++){
        total += (*vaccineIterator).second;
    }
    return total;
}

// Sorting algorithms for vaccines
bool compareTransport(pair<Vaccine*,int> firstPair, pair<Vaccine*,int> secondPair) {
    return (firstPair.first->getTransport() < secondPair.first->getTransport());
}
bool compareRenewal(pair<Vaccine*,int> firstPair, pair<Vaccine*,int> secondPair) {
    return (firstPair.first->getRenewal() < secondPair.first->getRenewal());
}
bool compareDeliveryVSInterval(pair<Vaccine*,int> firstPair, pair<Vaccine*,int> secondPair) {
    // Simulating 10 days and seeing which one transferred more
    int firstPairAmount = 10 * firstPair.first->getInterval() * firstPair.first->getDelivery();
    int secondPairAmount = 10 * secondPair.first->getInterval() * secondPair.first->getDelivery();
    return (firstPairAmount > secondPairAmount);
}

__attribute__((unused)) bool sortDValues(pair<Vaccine*,double> firstPair, pair<Vaccine*,double> secondPair) {
    return (firstPair.second > secondPair.second);
}

__attribute__((unused)) void sortCompareTransport(vector<pair<Vaccine*,int> >& allVaccines, vector<pair<Vaccine*,double> >& dValues) {
    sort(allVaccines.begin(),allVaccines.end(),compareTransport);
    vector<pair<Vaccine*,double> >::iterator dValuesIt;
    for (int i = 0; i != (int) allVaccines.size(); i++) {
        for (dValuesIt = dValues.begin(); dValuesIt != dValues.end(); dValuesIt++) {
            if ((*dValuesIt).first == (allVaccines[i].first)) {
                if (i == 0) {
                    (*dValuesIt).second += 1.5;
                } else if ( i == 1) {
                    (*dValuesIt).second += 1.3;
                } else if (i == 2) {
                    (*dValuesIt).second += 1.1;
                }
            }
        }
    }
}

__attribute__((unused)) void sortCompareRenewal(vector<pair<Vaccine*,int> >& allVaccines, vector<pair<Vaccine*,double> >& dValues) {
    sort(allVaccines.begin(),allVaccines.end(), compareRenewal);
    vector<pair<Vaccine*,double> >::iterator dValuesIt;
    for (int i = 0; i != (int) allVaccines.size(); i++) {
        for (dValuesIt = dValues.begin(); dValuesIt != dValues.end(); dValuesIt++) {
            if ((*dValuesIt).first == (allVaccines[i].first)) {
                if (i == 0) {
                    (*dValuesIt).second += 1;
                } else if ( i == 1) {
                    (*dValuesIt).second += 0.8;
                } else if (i == 2) {
                    (*dValuesIt).second += 0.6;
                }
            }
        }
    }
}

__attribute__((unused)) void sortCompareDeliveryVSInterval(vector<pair<Vaccine*,int> >& allVaccines, vector<pair<Vaccine*, double> >& dValues) {
    sort(allVaccines.begin(),allVaccines.end(), compareDeliveryVSInterval);
    vector<pair<Vaccine*,double> >::iterator dValuesIt;
    for (int i = 0; i != (int) allVaccines.size(); i++) {
        for (dValuesIt = dValues.begin(); dValuesIt != dValues.end(); dValuesIt++) {
            if ((*dValuesIt).first == (allVaccines[i].first)) {
                if (i == 0) {
                    (*dValuesIt).second += 2;
                } else if ( i == 1) {
                    (*dValuesIt).second += 1.8;
                } else if (i == 2) {
                    (*dValuesIt).second += 1.6;
                }
            }
        }
    }
}

void Hub::sortVaccinesVector() {
    vector<pair<Vaccine*,int> > allVaccines = this->getVaccines();
    sort(allVaccines.begin(), allVaccines.end(), Utilities::compareTemperature);
    this->setVaccines(allVaccines);
}

int Hub::getInhabitantsAmount() const {
    vector<VaccinationCentre*> allVCentre = this->getVCentres();
    vector<VaccinationCentre*>::const_iterator iterator;
    int amount = 0;
    for (iterator = allVCentre.begin(); iterator != allVCentre.end(); iterator++) {
        amount += (*iterator)->getAmountOfInhabitants();
    }
    return amount;
}

int Hub::getVaccinatedAmount() const {
    vector<VaccinationCentre*> allVCentre = this->getVCentres();
    vector<VaccinationCentre*>::const_iterator iterator;
    int amount = 0;
    for (iterator = allVCentre.begin(); iterator != allVCentre.end(); iterator++) {
        amount += (*iterator)->getAmountOfVaccinatedInhabitants();
    }
    return amount;
}
