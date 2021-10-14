// +--------------------------------------------------+
// | Created by Cédric Leclercq and Maarten Peirsman  |
// |                                                  |
// |             University of Antwerp                |
// |                                                  |
// |           Project software engineering           |
// |                   2020-2021                      |
// +--------------------------------------------------+

#include "VaccinationCentre.h"
#include "Hub.h"

#include <iostream>
#include <algorithm>

VaccinationCentre::VaccinationCentre(const string &newName, const string &newAddress, int newInhabitants, int newCapacity) {
    REQUIRE(!newName.empty(),"newName should not be empty");
    REQUIRE(!newAddress.empty(),"newAddress should not be empty");
    REQUIRE(newInhabitants > 0, "newInhabitants should be non-negative and non-zero");
    REQUIRE(newCapacity > 0, "newCapacity should be non-negative and non-zero");
    VaccinationCentre::name = newName;
    VaccinationCentre::address = newAddress;
    for (int it = 0; it < newInhabitants; it++) {
        Inhabitant * newInhabitant = new Inhabitant;
        inhabitants.push_back(newInhabitant);
    }
    VaccinationCentre::capacity = newCapacity;
    _initCheck = this;
    ENSURE(properlyInitialised(), "Constructor should end in initialised state");
}

void VaccinationCentre::setName(const string &newName) {
    REQUIRE(properlyInitialised(),"VaccinationCentre should be initialised upon calling setName");
    REQUIRE(!newName.empty(), "newName for VaccinationCentre Object should not be empty");
    VaccinationCentre::name = newName;
    ENSURE(this->name == newName,"newName wasn't properly set");
}

void VaccinationCentre::setAddress(const string &newAddress) {
    REQUIRE(properlyInitialised(),"VaccinationCentre should be initialised upon calling setAddress");
    REQUIRE(!newAddress.empty(), "newAddress for VaccinationCentre Object should not be empty");
    VaccinationCentre::address = newAddress;
    ENSURE(this->address == newAddress,"newAddress wasn't properly set");
}

__attribute__((unused)) void VaccinationCentre::setInhabitants(const vector<Inhabitant*>& newInhabitants) {
    REQUIRE(properlyInitialised(),"VaccinationCentre should be initialised upon calling setInhabitants");
    REQUIRE(!newInhabitants.empty(), "newInhabitants should be non-zero");
    VaccinationCentre::inhabitants = newInhabitants;
    ENSURE(this->inhabitants == newInhabitants, "newInhabitants wasn't properly set");
}

void VaccinationCentre::setInhabitantsInt(int newInhabitants) {
    REQUIRE(properlyInitialised(),"VaccinationCentre should be initialised upon calling setInhabitants");
    REQUIRE(newInhabitants > 0, "newInhabitants should be non-zero and non-negative");
    vector<Inhabitant*> allInhabitants = this->getInhabitants();
    vector<Inhabitant*>::iterator iter;
    for (iter = allInhabitants.begin(); iter != allInhabitants.end(); iter++) {
        delete (*iter);
    }
    this->inhabitants.clear();
    for (int it = 0; it < newInhabitants; it++) {
        Inhabitant * newInhabitant = new Inhabitant;
        inhabitants.push_back(newInhabitant);
    }
    ENSURE((int) this->inhabitants.size() == newInhabitants, "newInhabitants wasn't properly initialised");
}

void VaccinationCentre::setCapacity(int newCapacity) {
    REQUIRE(properlyInitialised(),"VaccinationCentre should be initialised upon calling setCapacity");
    REQUIRE(newCapacity > 0, "newCapacity should be non-negative and non-zero");
    this->capacity = newCapacity;
    ENSURE(this->getCapacity() == newCapacity,"Capacity wasn't set properly.");
}


void VaccinationCentre::print(ostream &onstream) {
    REQUIRE(properlyInitialised(), "VaccinationCentre should be initialised upon calling print");
    string output = this->name + ": " + to_string(this->getAmountOfVaccinatedInhabitants()) + " gevaccineerd, " +
                    to_string(this->getAmountOfInhabitants() - this->getAmountOfVaccinatedInhabitants()) + " inwoners nog niet.";
    onstream << output << endl;
}

void VaccinationCentre::addHub(Hub *hub) {
    REQUIRE(properlyInitialised(),"VaccinationCentre should be initialised upon calling addHub");
    REQUIRE(hub->properlyInitialised(), "Hub should be initialised upon calling addHub");
    hubs.push_back(hub);
}

void VaccinationCentre::setVaccinations(const vector<pair <Vaccine*,int> >& newVaccinations) {
    REQUIRE(properlyInitialised(), "VaccinationCentre wasn't initialised upon calling setVaccinations");
    vector<pair<Vaccine*,int> >::const_iterator vaccineIterator; // Declare iterator
    for (vaccineIterator = newVaccinations.begin(); vaccineIterator != newVaccinations.end(); vaccineIterator++) {
        REQUIRE((*vaccineIterator).second >= 0, "All the new Vaccine amounts should be non-negative");
        REQUIRE((*vaccineIterator).first->properlyInitialised(), "All Vaccine kinds in the vector should be properly initialised upon calling setVaccinations");
    }
    VaccinationCentre::vaccinations = newVaccinations;
    ENSURE(this->vaccinations == newVaccinations, "Vaccinations wasn't properly initialed");
}

bool VaccinationCentre::properlyInitialised() const {
    return _initCheck == this;
}

VaccinationCentre::VaccinationCentre() {
    VaccinationCentre::address = "";
    VaccinationCentre::name = "";
    VaccinationCentre::capacity = 0;

    _initCheck = this;
    ENSURE(properlyInitialised(),"Default constructor for VaccinationCentre doesn't properly initialise");
}

vector<Hub *> VaccinationCentre::getHubs() const {
    return hubs;
}


string VaccinationCentre::getNaam() const {
    return this->name;
}

string VaccinationCentre::getAdres() const {
    return this->address;
}

vector<Inhabitant *> VaccinationCentre::getInhabitants() const {
    return this->inhabitants;
}

int VaccinationCentre::getAmountOfInhabitants() const {
    return (int)(inhabitants.size());
}

int VaccinationCentre::getAmountOfVaccinatedInhabitants() const {
    int amount = 0;
    vector<Inhabitant*>::const_iterator inhabitantIterator;
    for (inhabitantIterator = this->inhabitants.begin(); inhabitantIterator != this->inhabitants.end(); inhabitantIterator++) {
        if ((*inhabitantIterator)->getVaccinated() == Vaccinated) {
            amount += 1;
        }
    }
    return amount;
}

int VaccinationCentre::getAmountOfPartiallyVaccinatedInhabitants() const {
    int amount = 0;
    vector<Inhabitant*>::const_iterator inhabitantIterator;
    for (inhabitantIterator = this->inhabitants.begin(); inhabitantIterator != this->inhabitants.end(); inhabitantIterator++) {
        if ((*inhabitantIterator)->getVaccinated() == PartiallyVaccinated) {
            amount += 1;
        }
    }
    return amount;
}

int VaccinationCentre::getCapacity() const {
    return this->capacity;
}

vector<pair<Vaccine *, int> > VaccinationCentre::getVaccines() const {
    return this->vaccinations;
}

bool VaccinationCentre::isValidName(const string &name) {
    return !Utilities::isEmptyString(name);
}

bool VaccinationCentre::isValidAddress(const string &address) {
    return !Utilities::isEmptyString(address);
}

bool VaccinationCentre::isValidInhabitants(const int inhabitants) {
    return inhabitants > 0;
}

bool VaccinationCentre::isValidCapacity(const int capacity) {
    return capacity > 0;
}

bool VaccinationCentre::vaccinesLeft() const {
    REQUIRE(this->properlyInitialised(), "VaccinationCentre wasn't initialised when trying to calculate how many vaccines are left");

    vector<pair<Vaccine*,int> >::const_iterator vaccineIterator;
    vector<pair<Vaccine*,int> > allVaccines = this->getVaccines();
    int amount = 0;
    for (vaccineIterator = allVaccines.begin(); vaccineIterator != allVaccines.end(); vaccineIterator++) {
        amount += (*vaccineIterator).second;
    }

    if (amount == 0) {
        return false;
    } else return true;
}

void VaccinationCentre::vaccinate() {
    REQUIRE(this->properlyInitialised(), "VaccinationCentre wasn't initialised when trying to vaccinate");
    if (this->getAmountOfVaccinatedInhabitants() == this->getAmountOfInhabitants()) {
        // No more vaccines are needed
        return;
    }
    // Some vaccines are still needed
    this->sortVaccinesVector();
    vector<Inhabitant*>::const_iterator inhabitantIterator;
    vector<Inhabitant*> allInhabitants = this->getInhabitants();
    int vaccinesDistributed = 0;
    for (inhabitantIterator = allInhabitants.begin(); inhabitantIterator != allInhabitants.end(); inhabitantIterator++) {
        if (vaccinesLeft() and (*inhabitantIterator)->getVaccinated() != Vaccinated and vaccinesDistributed != this->getCapacity()) {
            if ((*inhabitantIterator)->getVaccinated() == NotVaccinated) {
                REQUIRE((*inhabitantIterator)->properlyInitialised(),"Inhabitant was not properly initialised upon calling vaccinate");
                // This inhabitant has not yet gotten his/hers first vaccine
                vector<pair<Vaccine *, int> >::iterator vaccineIterator;
                vector<pair<Vaccine *, int> > allVaccines = this->getVaccines();
                for (vaccineIterator = allVaccines.begin();
                     vaccineIterator != allVaccines.end(); vaccineIterator++) {
                    REQUIRE((*vaccineIterator).first->properlyInitialised(),"Vaccine was not properly initialised upon calling vaccinate");
                    if ((*vaccineIterator).second > 0) {
                        vaccinesDistributed += 1;
                        // This vaccine can be used. This inhabitant will be vaccinated with it.
                        (*inhabitantIterator)->setRenewablesLeft((*vaccineIterator).first->getRenewal());
                        if ((*inhabitantIterator)->getRenewablesLeft() <= 0) {
                            (*inhabitantIterator)->setVaccinated(Vaccinated);
                        } else {
                            (*inhabitantIterator)->setVaccinated(PartiallyVaccinated);
                        }
                        (*inhabitantIterator)->setVaccine((*vaccineIterator).first);
                        (*vaccineIterator).second -= 1;
                        break;
                    }
                }this->setVaccinations(allVaccines);
            } else if ((*inhabitantIterator)->getVaccinated() == PartiallyVaccinated) {
                if ((*inhabitantIterator)->getRenewablesLeft() > 0) {
                    (*inhabitantIterator)->setRenewablesLeft((*inhabitantIterator)->getRenewablesLeft() - 1);
                } else if ((*inhabitantIterator)->getRenewablesLeft() <= 0){
                    // We can make this person vaccinated, check if we have their vaccine, else this person will get
                    // his/her vaccination tomorrow. Normally the vaccine should be in stock though.
                    vector<pair<Vaccine *, int> >::iterator vaccineIterator;
                    vector<pair<Vaccine *, int> > allVaccines = this->getVaccines();
                    for (vaccineIterator = allVaccines.begin(); vaccineIterator != allVaccines.end(); vaccineIterator++) {
                        REQUIRE((*vaccineIterator).first->properlyInitialised(),"Vaccine was not properly initialised upon calling vaccinate");
                        if ((*vaccineIterator).second > 0 and (*vaccineIterator).first == (*inhabitantIterator)->getVaccine()) {
                            // Vaccine matches, vaccinate this inhabitant, and mark him/her as permanently vaccinated
                            (*inhabitantIterator)->setVaccinated(Vaccinated);
                            (*vaccineIterator).second -= 1;
                            break;
                        }
                    }
                }
            }
        } else continue; // Person is vaccinated, nothing changes for this one
    }
    this->setInhabitants(allInhabitants);
}

vector<pair<Vaccine*,int> > VaccinationCentre::vaccinesNeededPerVaccine() const {
    REQUIRE(this->properlyInitialised(), "VaccinationCentre wasn't properly initialised upon calling vaccinesNeededPerVaccine");

    vector<pair<Vaccine*,int> > neededVaccines;
    vector<Inhabitant*>::const_iterator inhabitantIterator;
    for (inhabitantIterator = inhabitants.begin(); inhabitantIterator != inhabitants.end(); inhabitantIterator++) {
        REQUIRE((*inhabitantIterator)->properlyInitialised(),"Inhabitant was not properly initialised upon calling vaccinesNeededPerVaccine");
        if ((*inhabitantIterator)->getVaccinated() == PartiallyVaccinated and (*inhabitantIterator)->getRenewablesLeft() <= 1) {
            vector<pair<Vaccine*,int> >::iterator vaccineIterator;
            bool found = false;
            for (vaccineIterator = neededVaccines.begin(); vaccineIterator != neededVaccines.end(); vaccineIterator++) {
                REQUIRE((*vaccineIterator).first->properlyInitialised(),"Vaccine was not properly initialised upon calling vaccinesNeededPerVaccine");
                if ((*vaccineIterator).first == (*inhabitantIterator)->getVaccine()) {
                    // A vaccine that we need, but neededVaccines already contains this vaccine, just add the amount of vaccines this inhabitant needs
                    (*vaccineIterator).second += 1;
                    found = true;
                }
            }
            if (not found) {
                // A vaccine that we need, but neededVaccines does not yet contain this vaccine, add it to neededVaccines as a new one.
                neededVaccines.push_back(make_pair((*inhabitantIterator)->getVaccine(),(*inhabitantIterator)->getRenewablesLeft()));
            }
            if (neededVaccines.empty()) {
                neededVaccines.push_back(make_pair((*inhabitantIterator)->getVaccine(),(*inhabitantIterator)->getRenewablesLeft()));
            }
        }
    }
    return neededVaccines;
}

int VaccinationCentre::unspecifiedNeededVaccines() const {
    REQUIRE(this->properlyInitialised(), "VaccinationCentre wasn't properly initialised upon calling unspecifiedNeededVaccines");

    int unspecifiedNeededVaccines = 0;
    vector<Inhabitant*>::const_iterator inhabitantIterator;
    vector<Inhabitant*> allInhabitants = this->getInhabitants();
    for (inhabitantIterator = allInhabitants.begin(); inhabitantIterator != allInhabitants.end(); inhabitantIterator++) {
        REQUIRE((*inhabitantIterator)->properlyInitialised(),"Inhabitant was not properly initialised upon calling vaccinesNeededPerVaccine");
        if ((*inhabitantIterator)->getVaccinated() == NotVaccinated) {
            unspecifiedNeededVaccines += 1;
        }
    }
    return unspecifiedNeededVaccines;
}

int VaccinationCentre::currentTotalVaccines() const {
    REQUIRE(this->properlyInitialised(), "VaccinationCentre wasn't properly initialised upon calling currentTotalVaccines");

    vector<pair<Vaccine*,int> >::const_iterator vaccineIterator;
    vector<pair<Vaccine*,int> > allVaccinesInCentre = this->getVaccines();
    int total = 0;
    for (vaccineIterator = allVaccinesInCentre.begin(); vaccineIterator != allVaccinesInCentre.end(); vaccineIterator++) {
        REQUIRE((*vaccineIterator).first->properlyInitialised(),"Vaccine was not properly initialised upon calling currentTotalVaccines");
        total += (*vaccineIterator).second;
    }
    return total;

}


void VaccinationCentre::addToVaccines(const vector<pair<Vaccine *, int> > &toAddVaccines) {
    REQUIRE(this->properlyInitialised(), "VaccinationCentre wasn't properly initialised upon calling addToVaccines");


    vector<pair<Vaccine*,int> >::const_iterator toAddVaccineIterator;
    vector<pair<Vaccine*,int> > allVaccines = this->getVaccines();
    vector<pair<Vaccine*,int> >::iterator vaccineIterator;
    bool found = false;
    for (toAddVaccineIterator = toAddVaccines.begin(); toAddVaccineIterator != toAddVaccines.end(); toAddVaccineIterator++) {
        REQUIRE((*toAddVaccineIterator).first->properlyInitialised(),"Vaccine was not properly initialised upon calling addToVaccines");
        // all vaccines already has this vaccine, add it to its total
        found = false;
        for (vaccineIterator = allVaccines.begin(); vaccineIterator != allVaccines.end(); vaccineIterator++) {
            REQUIRE((*vaccineIterator).first->properlyInitialised(),"Vaccine was not properly initialised upon calling addToVaccines");
            if ((*vaccineIterator).first->getType() == (*toAddVaccineIterator).first->getType()) {
                    (*vaccineIterator).second += (*toAddVaccineIterator).second;
                    found = true;
            }
        }
        if (not found ) {
            allVaccines.push_back((*toAddVaccineIterator));
        }
    }

    this->setVaccinations(allVaccines);
}

void VaccinationCentre::transportNecessaryVaccines(Hub * currentHub, ostream &outputStream) {
    REQUIRE(currentHub != NULL, "currentHub was a nullptr upon calling transportNecessaryVaccines");
    REQUIRE(this->properlyInitialised(), "VaccinationCentre wasn't properly initialised upon calling transportNecessaryVaccines");

    bool stopping = false;
    vector<pair<Vaccine*,int> > vaccinesToGiveToCentre;
    vector<pair<Vaccine*,int> > neededVaccines = this->vaccinesNeededPerVaccine(); // Getting all the vaccines we need
    vector<pair<Vaccine*,int> >::iterator vaccineIterator; // Getting the iterator for the vaccines
    int toTransportVaccines = 0; // Represents the amount of vaccines that should be transported (== toTransport * vaccine->getTransport())
    // If we have not gone over the max capacity
    for (vaccineIterator = neededVaccines.begin(); vaccineIterator != neededVaccines.end(); vaccineIterator++) {
        REQUIRE((*vaccineIterator).first->properlyInitialised(),"Vaccine was not properly initialised upon calling transportNecessaryVaccines");
        // Searching in the hub for the status of this vaccine and storing that pair on a variable
        vector<pair<Vaccine *, int> >::iterator hubVaccineIterator;
        vector<pair<Vaccine *, int> > hubVaccines = currentHub->getVaccines();
        for (hubVaccineIterator = hubVaccines.begin(); hubVaccineIterator != hubVaccines.end(); hubVaccineIterator++) {
            REQUIRE((*hubVaccineIterator).first->properlyInitialised(),"Vaccine was not properly initialised upon calling transportNecessaryVaccines");
            if ((*hubVaccineIterator).first == (*vaccineIterator).first) {
                    while (not stopping) {
                        if (toTransportVaccines + this->currentTotalVaccines() < this->getCapacity()) {
                        //while (toTransportVaccines < this->capacity and not stopping) {
                        /* Looping over all the vaccines we need
                         * The while loop will be stopped when:
                         *      - The loop naturally ends, all needed vaccines can be transported
                         *      - We have reached the capacity (except one last special case)
                         */
                        int neededPackages = (*vaccineIterator).second - ((*vaccineIterator).second % (*vaccineIterator).first->getTransport());
                        if ((*vaccineIterator).second % (*vaccineIterator).first->getTransport() != 0) {
                            // When the remainder of the modulo calculation above is not zero, that means that more packages are needed
                            // then the current amount of needed packages. Adding one more package to fix that
                            neededPackages += (*vaccineIterator).first->getTransport();
                        }

                        if (neededPackages == 0) {
                            stopping = true;
                        }

                        while (neededPackages + toTransportVaccines + this->currentTotalVaccines() > this->getCapacity() + (*vaccineIterator).first->getTransport()) {
                            neededPackages -= (*vaccineIterator).first->getTransport();
                        }

                        // Case 1 - we have the needed amount of packages in the hub and the capacity can handle
                        if (neededPackages <= (*hubVaccineIterator).second and toTransportVaccines + this->currentTotalVaccines() + neededPackages <= this->getCapacity()) {
                            (*hubVaccineIterator).second -= neededPackages;
                            vaccinesToGiveToCentre.push_back(make_pair((*hubVaccineIterator).first, neededPackages));
                            toTransportVaccines += neededPackages;
                        }

                        // Case 2 - we have not enough vaccines in the hub but the capacity can handle
                        else if (neededPackages > (*hubVaccineIterator).second and toTransportVaccines + this->currentTotalVaccines() + neededPackages <= this->getCapacity()) {
                            while (neededPackages > (*hubVaccineIterator).second) {
                                // Decreasing the value of neededPackages by transport until its low enough
                                neededPackages -= (*vaccineIterator).first->getTransport();
                            }

                            if (neededPackages > 0) {
                                // Now we can transfer like in case 1
                                (*hubVaccineIterator).second -= neededPackages;
                                vaccinesToGiveToCentre.push_back(
                                        make_pair((*hubVaccineIterator).first, neededPackages));
                                toTransportVaccines += neededPackages;
                            }
                        }

                        // Case 3 - we have the needed amount of packages in the hub and the capacity 2x can handle, stop adding after this
                        else if (neededPackages <= (*hubVaccineIterator).second and toTransportVaccines + this->currentTotalVaccines() + neededPackages <= 2 * this->getCapacity()) {
                            (*hubVaccineIterator).second -= neededPackages;
                            vaccinesToGiveToCentre.push_back(make_pair((*hubVaccineIterator).first, neededPackages));
                            toTransportVaccines += neededPackages;
                            stopping = true;
                        }

                        // Case 4 - we have not enough vaccines in the hub and the capacity 2x can handle, stop adding after this
                        else if (neededPackages > (*hubVaccineIterator).second and toTransportVaccines + this->currentTotalVaccines() + neededPackages <= 2 * this->getCapacity()) {
                            while (neededPackages > (*hubVaccineIterator).second and toTransportVaccines + this->currentTotalVaccines() + neededPackages > 2* this->getCapacity()) {
                                // Decreasing the value of neededPackages by transport until its low enough
                                neededPackages -= (*vaccineIterator).first->getTransport();
                            }
                            if (neededPackages > 0) {
                                // Now we can transfer like in case 1
                                (*hubVaccineIterator).second -= neededPackages;
                                vaccinesToGiveToCentre.push_back(
                                        make_pair((*hubVaccineIterator).first, neededPackages));
                                toTransportVaccines += neededPackages;
                                stopping = true;
                            }
                        } else { stopping = true; }
                    } else {stopping = true; }
                        currentHub->setVaccines(hubVaccines);
                }
            }
        }
    }
    this->addToVaccines(vaccinesToGiveToCentre);
    vector<pair<Vaccine*,int> >::const_iterator vaccinesToGiveIterator;
    vector<pair<Vaccine*,int> > transferredToday;
    for (vaccinesToGiveIterator = vaccinesToGiveToCentre.begin(); vaccinesToGiveIterator != vaccinesToGiveToCentre.end(); vaccinesToGiveIterator++) {
        transferredToday.push_back((*vaccinesToGiveIterator));
    } this->setVaccinesTransportedToday(transferredToday);
}

void VaccinationCentre::transportUnspecifiedVaccines(Hub * currentHub, ostream &outputStream) {
    REQUIRE(currentHub != NULL, "currentHub was a nullptr upon calling transportUnspecifiedVaccines");

    REQUIRE(this->properlyInitialised(), "VaccinationCentre wasn't properly initialised upon calling transportUnspecifiedVaccines");
    currentHub->sortVaccinesVector(); // So that the vector is sorted before transport - makes the simulation faster
    vector<pair<Vaccine*,int> > vaccinesToGiveToCentre;
    int neededVaccines = this->unspecifiedNeededVaccines(); // Indicates how many vaccines we need
    int toTransportVaccines = 0; // Represents the amount of vaccines that should be transported (== toTransport * vaccine->getTransport())

    bool stopping = false;
    // Looping till we have reached neededVaccines, or that we use <break> because af the maximum capacity
    for (int it = 0; it < neededVaccines and not stopping; it++) {
        while (not stopping) {
            vector<pair<Vaccine *, int> >::iterator vaccineIterator;
            vector<pair<Vaccine *, int> > hubVaccines = currentHub->getVaccines();
            for (vaccineIterator = hubVaccines.begin();
                 vaccineIterator != hubVaccines.end(); vaccineIterator++) {
                REQUIRE((*vaccineIterator).first->properlyInitialised(),"Vaccine was not properly initialised upon calling transportUnspecifiedVaccines");
                if ((*vaccineIterator).second > 0) {
                    if (toTransportVaccines + this->currentTotalVaccines() < this->getCapacity()) {
                        //while (toTransportVaccines < this->capacity and not stopping) {
                        /* Looping over all the vaccines we need
                         * The while loop will be stopped when:
                         *      - The loop naturally ends, all needed vaccines can be transported
                         *      - We have reached the capacity (except one last special case)
                         */
                        int neededPackages = neededVaccines - (neededVaccines % (*vaccineIterator).first->getTransport());
                        if ((*vaccineIterator).second % (*vaccineIterator).first->getTransport() != 0) {
                            // When the remainder of the modulo calculation above is not zero, that means that more packages are needed
                            // then the current amount of needed packages. Adding one more package to fix that
                            neededPackages += (*vaccineIterator).first->getTransport();
                        }
                        if (neededPackages == 0) {
                            stopping = true;
                        }
                        while (neededPackages + toTransportVaccines + this->currentTotalVaccines() > this->getCapacity() + (*vaccineIterator).first->getTransport()) {
                            neededPackages -= (*vaccineIterator).first->getTransport();
                        }
                        // Case 1 - we have the needed amount of packages in the hub and the capacity can handle
                        if (neededPackages <= (*vaccineIterator).second and toTransportVaccines + this->currentTotalVaccines() + neededPackages <= this->getCapacity()) {
                            (*vaccineIterator).second -= neededPackages;
                            vaccinesToGiveToCentre.push_back(make_pair((*vaccineIterator).first, neededPackages));
                            toTransportVaccines += neededPackages;
                        }
                            // Case 2 - we have not enough vaccines in the hub but the capacity can handle
                        else if (neededPackages > (*vaccineIterator).second and toTransportVaccines + this->currentTotalVaccines() + neededPackages <= this->getCapacity()) {
                            while (neededPackages > (*vaccineIterator).second) {
                                // Decreasing the value of neededPackages by transport until its low enough
                                neededPackages -= (*vaccineIterator).first->getTransport();
                            }
                            if (neededPackages > 0) {
                                // Now we can transfer like in case 1
                                (*vaccineIterator).second -= neededPackages;
                                vaccinesToGiveToCentre.push_back(
                                        make_pair((*vaccineIterator).first, neededPackages));
                                toTransportVaccines += neededPackages;
                            }
                        }
                            // Case 3 - we have the needed amount of packages in the hub and the capacity 2x can handle, stop adding after this
                        else if (neededPackages <= (*vaccineIterator).second and toTransportVaccines + this->currentTotalVaccines() + neededPackages <= 2 * this->getCapacity()) {
                            (*vaccineIterator).second -= neededPackages;
                            vaccinesToGiveToCentre.push_back(make_pair((*vaccineIterator).first, neededPackages));
                            toTransportVaccines += neededPackages;
                            stopping = true;
                        }
                            // Case 4 - we have not enough vaccines in the hub and the capacity 2x can handle, stop adding after this
                        else if (neededPackages <= (*vaccineIterator).second and toTransportVaccines + this->currentTotalVaccines() + neededPackages <= 2 * this->getCapacity()) {
                            while (neededPackages > (*vaccineIterator).second and toTransportVaccines + this->currentTotalVaccines() + neededPackages <= 2* this->getCapacity()) {
                                // Decreasing the value of neededPackages by transport until its low enough
                                neededPackages -= (*vaccineIterator).first->getTransport();
                            } if (neededPackages > 0) {
                                // Now we can transfer like in case 1
                                (*vaccineIterator).second -= neededPackages;
                                vaccinesToGiveToCentre.push_back(make_pair((*vaccineIterator).first, neededPackages));
                                toTransportVaccines += neededPackages;
                                stopping = true;
                            }
                        }
                    } else { stopping = true; }
                    currentHub->setVaccines(hubVaccines);
                }
            }
        }
    }
    this->addToVaccines(vaccinesToGiveToCentre);
    vector<pair<Vaccine*,int> >::const_iterator vaccinesToGiveIterator;
    vector<pair<Vaccine*,int> > transferredToday;
    for (vaccinesToGiveIterator = vaccinesToGiveToCentre.begin(); vaccinesToGiveIterator != vaccinesToGiveToCentre.end(); vaccinesToGiveIterator++) {
        transferredToday.push_back((*vaccinesToGiveIterator));
    } this->setVaccinesTransportedToday(transferredToday);
}

void VaccinationCentre::printOverview(bool detailed, ostream& outputStream) const {
    REQUIRE(this->properlyInitialised(), "VaccinationCentre wasn't properly initialised upon calling printOverview");

    // Getting vaccinatedPercentage
    int vaccinatedPercentage = Utilities::roundToInt(((((double)this->getAmountOfVaccinatedInhabitants() / (double)this->getAmountOfInhabitants()) * 100)));
    vector<pair<Vaccine*,int> > allVaccines = this->getVaccines();
    vector<pair<Vaccine*,int> >::const_iterator vaccineIterator;
    int amountOfVaccines = 0;
    for (vaccineIterator = allVaccines.begin(); vaccineIterator != allVaccines.end(); vaccineIterator++) {
        REQUIRE((*vaccineIterator).first->properlyInitialised(),"Vaccine was not properly initialised upon calling printOverview");
        amountOfVaccines += (*vaccineIterator).second;
    }
    // Getting vaccinePercentage
    int vaccinePercentage = Utilities::roundToInt((((double)amountOfVaccines / (double)this->getCapacity()) * 100));

    // If vaccinePercentage is above 100, make it 100, can happen with the 2x capacity cap
    if (vaccinePercentage > 100) {
        vaccinePercentage = 100;
    }

    // Amount of equal signs and spaces in the representation
    int toShowPercentageVaccinated = (vaccinatedPercentage - (vaccinatedPercentage % 5)) / 5;
    int spacesVaccinated = 20 - toShowPercentageVaccinated;
    int toShowPercentageVaccines = (vaccinePercentage - (vaccinePercentage % 5)) / 5;
    int spacesVaccines = 20 - toShowPercentageVaccines;

    string vaccinatedOverview = "[";
    string vaccineOverview = "[";

    for (int it = 0; it < toShowPercentageVaccinated; it++) {
        vaccinatedOverview += "=";
    }
    for (int it = 0; it < spacesVaccinated; it++) {
        vaccinatedOverview += " ";
    } vaccinatedOverview += "] \t" + to_string(vaccinatedPercentage) + "%";
    for (int it = 0; it < toShowPercentageVaccines; it++) {
        vaccineOverview += "=";
    }
    for (int it = 0; it < spacesVaccines; it++) {
        vaccineOverview += " ";
    } vaccineOverview += "] \t" + to_string(vaccinePercentage) + "%";

    // If a detailed overview is asked
    outputStream << this->getNaam() << endl;
    if (detailed) {
        string partiallyVaccinatedOverview = "[";
        int partiallyVaccinatedPercentage = Utilities::roundToInt((((double)this->getAmountOfPartiallyVaccinatedInhabitants() / (double)this->getAmountOfInhabitants()) * 100));
        int toShowPercentagePartiallyVaccinated = (partiallyVaccinatedPercentage - (partiallyVaccinatedPercentage % 5)) / 5;
        int spacesPartiallyVaccinated = 20 - toShowPercentagePartiallyVaccinated;
        for (int it = 0; it < toShowPercentagePartiallyVaccinated; it++) {
            partiallyVaccinatedOverview += "=";
        }
        for (int it = 0; it < spacesPartiallyVaccinated; it++) {
            partiallyVaccinatedOverview += " ";
        } partiallyVaccinatedOverview += "] \t" + to_string(partiallyVaccinatedPercentage) + "%";
        outputStream << "\t - vaccins                   " << vaccineOverview << endl;
        outputStream << "\t - gevaccineerd              " << vaccinatedOverview << endl;
        outputStream << "\t - gedeeltelijk gevaccineerd " << partiallyVaccinatedOverview << endl;

    } else {
        outputStream << "\t - vaccins      " << vaccineOverview << endl;
        outputStream << "\t - gevaccineerd " << vaccinatedOverview << endl;
    }
}

vector<pair<Vaccine *, int> > VaccinationCentre::getTransferredThisDay() const {
    return this->transferedThisDay;
}

void VaccinationCentre::clearTransferredThisDay() {
    this->transferedThisDay.clear();
}

void VaccinationCentre::cleanOutBadVaccines() {
    vector<pair<Vaccine*,int> >::iterator vaccineIterator;
    vector<pair<Vaccine*,int> > allVaccines = this->getVaccines();
    for (vaccineIterator = allVaccines.begin(); vaccineIterator != allVaccines.end(); vaccineIterator++) {
        if ((*vaccineIterator).first->getTemperature() < 0) {
            (*vaccineIterator).second = 0;
        }
    }
    this->setVaccinations(allVaccines);
}

void VaccinationCentre::setVaccinesTransportedToday(const vector<pair<Vaccine*,int> >& newTransferredThisDay) {
    this->transferedThisDay = newTransferredThisDay;
    ENSURE(this->transferedThisDay == newTransferredThisDay,"TransferredThisDay wasn't properly initialised");
}


void VaccinationCentre::sortVaccinesVector() {
    REQUIRE(this->properlyInitialised(), "VaccinationCentre wasn't properly initialised upon calling sortVaccinesVector");
    vector<pair<Vaccine*,int> > allVaccines = this->getVaccines();
    vector<pair<Vaccine*,int> >::iterator itVaccines;
    sort(allVaccines.begin(),allVaccines.end(), Utilities::compareTemperature);
    this->setVaccinations(allVaccines);
}
