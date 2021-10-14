// +--------------------------------------------------+
// | Created by Cédric Leclercq and Maarten Peirsman  |
// |                                                  |
// |             University of Antwerp                |
// |                                                  |
// |           Project software engineering           |
// |                   2020-2021                      |
// +--------------------------------------------------+

#ifndef INC_2__PROJECT_VACCINATIONCENTRE_H
#define INC_2__PROJECT_VACCINATIONCENTRE_H

#include "../utilities/DesignByContract.h"
#include "../utilities/Utils.h"

#include <vector>
#include <string>
#include <gtest/gtest.h>
#include "Inhabitant.h"

using namespace std;

class Hub;
class Vaccine;
class Inhabitant;

class VaccinationCentre {
    string name; // Name of the vaccination centre
    string address; // Address of the vaccination centre
    vector<Inhabitant*> inhabitants; // Vector with all the inhabitants in this vaccination centre
    int capacity; // Capacity of this vaccination centre
    vector<pair<Vaccine*,int> > vaccinations; // Vector with type of vaccine and the amount currently in house
    vector<Hub*> hubs; // Vector with all the hubs this vaccination centre is linked to
    // Amount and type of vaccines that are transferred on that day, remains empty if the simulation never starts
    vector<pair<Vaccine*,int> > transferedThisDay;
    VaccinationCentre* _initCheck; // Pointer to itself to check for proper initialisation

public:
    /**
     * @brief Default constructor
     * @PostCondition ENSURE(properlyInitialised(),"Default constructor for VaccinationCentre doesn't properly initialise");
     */
    VaccinationCentre();

    /**
     * @brief Constructor for Vaccination Centre objects
     * @param name name of the centre
     * @param addres address of the centre
     * @param inhabitants amount of inhabitants
     * @param capacity the capacity for vaccines the centre has
     * @PreCondition REQUIRE(!naam.empty(),"Name should not be empty");
     * @PreCondition REQUIRE(!address.empty(),"Adres should not be empty");
     * @PreCondition REQUIRE(inhabitants > 0, "Inwoners should be non-negative and non-zero");
     * @PreCondition REQUIRE(capacity > 0, "Capaciteit should be non-negative and non-zero");
     * @PostCondition ENSURE(properlyInitialised(), "Constructor should end in initialised state");
     */
    VaccinationCentre(const string &name, const string &addres, int inhabitants, int capacity);

    // Getters
    string getNaam() const; // Gets the parameter name
    string getAdres() const; // Gets te parameter address
    vector<Inhabitant*> getInhabitants() const; // Gets the parameter inhabitants
    vector<pair<Vaccine*,int> > getVaccines() const; // Gets the parameter vaccines
    int getAmountOfInhabitants() const; // Gets the amount of Inhabitants
    int getAmountOfVaccinatedInhabitants() const; // Gets the amount of vaccinated inhabitants
    int getCapacity() const; // Gets the parameter capacity
    vector<pair<Vaccine*,int> > getTransferredThisDay() const; // Gets the parameter transferredThisDay
    vector<Hub*> getHubs() const; // Gets the parameter hubs

    /**
     * @brief Clears the transferredThisDay vector
     */
    void clearTransferredThisDay();

    // Setters

    /**
     * @brief Set value of parameter naam
     * @param newName
     * @PreCondition     REQUIRE(properlyInitialised(),"VaccinationCentre should be initialised upon calling setName");
     * @PreCondition     REQUIRE(!newName.empty(), "newName for VaccinationCentre Object should not be empty");
     * @PostCondition    ENSURE(this->name == newName,"newName wasn't properly set");
     */
    void setName(const string &newName);

    /**
    * @brief Set value of parameter address
    * @param newAddress
    * @PreCondition     REQUIRE(properlyInitialised(),"VaccinationCentre should be initialised upon calling setAddress");
    * @Precondition     REQUIRE(!newAddress.empty(), "newAddress for VaccinationCentre Object should not be empty");
    * @PostCondition    ENSURE(this->name == newName,"newName wasn't properly set");
    */
    void setAddress(const string &newAddress);

    /**
     * @brief Set value of parameter inhabitants
     * @param newHabitants
     * @PreCondition  REQUIRE(properlyInitialised(),"VaccinationCentre should be initialised upon calling setInhabitants");
     * @PreCondition  REQUIRE(!newInhabitants.empty(), "newInhabitants should be non-zero");
     * @PostCondition ENSURE(this->inhabitants == newInhabitants, "newInhabitants wasn't properly set");
     */
    __attribute__((unused)) void setInhabitants(const vector<Inhabitant*>& newInhabitants);

    /**
     * @brief Set value of inhabitants to newInhabitants. Clears the vector of all the inhabitants and creates new
     * inhabitants.
     * @param newInhabitants
     * @PreCondition     REQUIRE(properlyInitialised(),"VaccinationCentre should be initialised upon calling setInhabitants");
     * @PreCondition     REQUIRE(newInhabitants > 0, "newInhabitants should be non-zero and non-negative");
     * @PostCondition    ENSURE((int) this->inhabitants.size() == newInhabitants, "newInhabitants wasn't properly initialised");
     */
    void setInhabitantsInt(int newInhabitants);

    /**
     * @brief Set value of parameter capacity
     * @param newCapacity
     * @PreCondition     REQUIRE(properlyInitialised(),"VaccinationCentre should be initialised upon calling setCapacity");
     * @PreCondition     REQUIRE(newCapacity > 0, "newCapacity should be non-negative and non-zero");
     * @PostCondition    ENSURE(this->getCapacity() == newCapacity,"Capacity wasn't set properly.")
     */
    void setCapacity(int capaciteit);

    /**
     * @brief Set value of parameter vaccinations
     * @param newVaccins
     * @PreCondition     REQUIRE(properlyInitialised(), "VaccinationCentre wasn't initialised upon calling setVaccinations");
     * @PreCondition     REQUIRE((*vaccineIterator).second >= 0, "All the new Vaccine amounts should be non-negative");
     * @PreCondition     REQUIRE((*vaccineIterator).first->properlyInitialised(), "All Vaccine kinds in the vector should be properly initialised upon calling setVaccinations");
     * @PostCondition    ENSURE(this->vaccinations == newVaccinations, "Vaccinations wasn't properly initialed");
     */
    void setVaccinations(const vector<pair <Vaccine*,int> >& vaccins);

    /**
     * @brief Function to output vaccinatiecentrum information to a desired output stream
     * @param onstream: the desired output stream
     * @PreCondition     REQUIRE(properlyInitialised(), "VaccinationCentre should be initialised upon calling print");
     */
    void print(ostream &onstream);

    /**
     * @brief Function to add a Hub to the list of hubs
     * @param hub The Hub the VaccinatieCentrum is a part of
     * @PreCondition REQUIRE(properlyInitialised(),"VaccinatieCentrum should be initialised upon calling addHub");
     * @PreCondition REQUIRE(hub->properlyInitialised(), "Hub should be initialised upon calling addHub");
     */
    void addHub(Hub *hub);

    /**
     * @brief Function to determine whether the VaccinatieCentrum Object was properly initialised
     * @return
     */
    bool properlyInitialised() const;

    /**
     * @brief Function to determine whether a name is valid
     * @param name
     * @return false if name is empty, true if not
     */
    static bool isValidName(const string &name);

    /**
     * @brief Function to determine whether an address is valid
     * @param addres
     * @return false if addres is empty, true if not
     */
    static bool isValidAddress(const string &address);

    /**
     * @brief Function to determine whether inhabitants is a valid value
     * @param inhabitants
     * @return true if > 0, false if not
     */
    static bool isValidInhabitants(int inhabitants);

    /**
     * @brief Function to determine whether capacity is a valid value
     * @param capacity
     * @return true if > 0, false if not
     */
    static bool isValidCapacity(int capacity);

    /**
     * @brief Function to dertermine if there are vaccines left in the vaccines vector
     * @return
     * @PreCondition     REQUIRE(this->properlyInitialised(), "VaccinationCentre wasn't initialised when trying to calculate how many vaccines are left");
     */
    bool vaccinesLeft() const;

    /**
     * @brief Vaccinates the inhabitants of this centre while keeping in account certain rules
     * @PreCondition     REQUIRE(this->properlyInitialised(), "VaccinationCentre wasn't initialised when trying to vaccinate");
     * @PreCondition     REQUIRE((*inhabitantIterator)->properlyInitialised(),"Inhabitant was not properly initialised upon calling vaccinate");
     * @PreCondition     REQUIRE((*vaccineIterator).first->properlyInitialised(),"Vaccine was not properly initialised upon calling vaccinate");
     */
    void vaccinate();

    /**
     * @brief Calculates which vaccines are needed in the vaccinationCentre. This means for example all the people that
     * need their second vaccine (needs to be the same type). Also holds in account certain rules like the capacity of
     * the vaccination centre.
     * @param currentHub
     * @param outputStream
     * @PreCondition     REQUIRE(currentHub != NULL, "currentHub was a nullptr upon calling transportNecessaryVaccines");
     * @PreCondition     REQUIRE(this->properlyInitialised(), "VaccinationCentre wasn't properly initialised upon calling transportNecessaryVaccines");
     * @PreCondition     REQUIRE((*vaccineIterator).first->properlyInitialised(),"Vaccine was not properly initialised upon calling transportNecessaryVaccines");
     * @PreCondition     REQUIRE((*hubVaccineIterator).first->properlyInitialised(),"Vaccine was not properly initialised upon calling transportNecessaryVaccines");
     */
    void transportNecessaryVaccines(Hub * currentHub, ostream &outputStream);
    /**
     * @brief Calculates which vaccines are needed in hte vaccinationCentre besides the ones that transportNeccesaryVaccines()
     * already transported. E.g. all the people that still need their first vaccine, ...
     * @param currentHub
     * @param outputStream
     * @PreCondition    REQUIRE(currentHub != NULL, "currentHub was a nullptr upon calling transportUnspecifiedVaccines");
     * @PreCondition    REQUIRE(this->properlyInitialised(), "VaccinationCentre wasn't properly initialised upon calling transportUnspecifiedVaccines");
     * @PreCondition    REQUIRE((*vaccineIterator).first->properlyInitialised(),"Vaccine was not properly initialised upon calling transportUnspecifiedVaccines");
     */
    void transportUnspecifiedVaccines(Hub * currentHub, ostream &outputStream);

    /**
     * @brief Calculates the total amount of vaccines in the centres and returns that value
     * @return  amount of vaccines in centre
     * @PreCondition     REQUIRE(this->properlyInitialised(), "VaccinationCentre wasn't properly initialised upon calling currentTotalVaccines");
     * @PreCondition     REQUIRE((*vaccineIterator).first->properlyInitialised(),"Vaccine was not properly initialised upon calling currentTotalVaccines");
     */
    int currentTotalVaccines() const;

    /**
     * @brief Adds a vector with an amount of vaccines to the centre
     * @param toAddVaccines
     * @PreCondition     REQUIRE(this->properlyInitialised(), "VaccinationCentre wasn't properly initialised upon calling addToVaccines");
     * @PreCondition     REQUIRE((*toAddVaccineIterator).first->properlyInitialised(),"Vaccine was not properly initialised upon calling addToVaccines");
     * @PreCondition     REQUIRE((*vaccineIterator).first->properlyInitialised(),"Vaccine was not properly initialised upon calling addToVaccines");
     */
    void addToVaccines(const vector<pair<Vaccine*,int> >& toAddVaccines);

    /**
     * @brief Calculates how many vaccines the centre needs per type
     * @return
     * @PreCondition    REQUIRE(this->properlyInitialised(), "VaccinationCentre wasn't properly initialised upon calling vaccinesNeededPerVaccine");
     * @PreCondition    REQUIRE((*inhabitantIterator)->properlyInitialised(),"Inhabitant was not properly initialised upon calling vaccinesNeededPerVaccine");
     * @PreCondition    REQUIRE((*vaccineIterator).first->properlyInitialised(),"Vaccine was not properly initialised upon calling vaccinesNeededPerVaccine");
     */
    vector<pair<Vaccine*,int> > vaccinesNeededPerVaccine() const;

    /**
     * @brief Calculates how many vaccines the centre needs that are unspecified. E.g. all the inhabitants that
     * still haven't gotten their vaccine.
     * @return
     * @PreCondition     REQUIRE(this->properlyInitialised(), "VaccinationCentre wasn't properly initialised upon calling unspecifiedNeededVaccines");
     * @PreCondition     REQUIRE((*inhabitantIterator)->properlyInitialised(),"Inhabitant was not properly initialised upon calling vaccinesNeededPerVaccine");
     */
    int unspecifiedNeededVaccines() const;

    /**
     * @brief Prints an overview of the centre
     * @param detailed  Indicates how detailed the overview should be
     * @PreCondition     REQUIRE(this->properlyInitialised(), "VaccinationCentre wasn't properly initialised upon calling printOverview");
     * @PreCondition     REQUIRE((*vaccineIterator).first->properlyInitialised(),"Vaccine was not properly initialised upon calling printOverview");
     */
    void printOverview(bool detailed, ostream& outputStream) const;

    /**
     * @brief Gets the amount of partially vaccinated inhabitant. Used in printOverview when detailed = true.
     * @return
     * @PreCondition        REQUIRE((*inhabitantIterator)->properlyInitialised(),"Inhabitant wasn't properly initialised upon calling getAmountOfVaccinatedInhabitants");
     */
    int getAmountOfPartiallyVaccinatedInhabitants() const;

    /**
     * @brief Removes all the vaccines that have gone bad. E.g. all the vaccines where the temperature needs to be below
     * zero cannot be kept in the centre for longer than one day.
     */
    void cleanOutBadVaccines();
    /**
     * @brief Setter for all vaccinesTransported
     * @param newTransferredThisDay
     * @PostCondition ENSURE(this->transferedThisDay == newTransferredThisDay,"TransferredThisDay wasn't properly initialised");
     */
    void setVaccinesTransportedToday(const vector<pair<Vaccine*,int> >& newTransferredThisDay);
    /**
     * @brief Sorts all the vaccines in the vCentre
     * @PreCondition REQUIRE(this->properlyInitialised(), "VaccinationCentre wasn't properly initialised upon calling sortVaccinesVector");
     */
    void sortVaccinesVector();

};


#endif //INC_2__PROJECT_VACCINATIONCENTRE_H
