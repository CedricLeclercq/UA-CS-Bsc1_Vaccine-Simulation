// +--------------------------------------------------+
// | Created by Cédric Leclercq and Maarten Peirsman  |
// |                                                  |
// |             University of Antwerp                |
// |                                                  |
// |           Project software engineering           |
// |                   2020-2021                      |
// +--------------------------------------------------+

#ifndef INC_2__PROJECT_HUB_H

#include "VaccinationCentre.h"
#include "Vaccine.h"
#include "../utilities/Utilities.h"

#include <iostream>
#include <vector>
#include <map>
#include <string>


using namespace std;

class Hub {
    Hub* _initCheck; // Pointer to itself to check for proper initialisation
    vector<pair<Vaccine*,int> > vaccines; // Vector with all the vaccines and the amount of that vaccine in the hub

    vector<VaccinationCentre*> centres; // Vector with all the vCentres that are linked to this hub
public:

    /**
     * @brief Default constructor
     */
    Hub() {
        _initCheck = this;
    };

    /**
     * @brief Constructor for Hub object
     * @param centres vector with Vaccination Centres
     * @param vaccines vector with Vaccines
     * @PostCondition ENSURE(properlyInitialised(), "Constructor should end in initialised state");
     */
    Hub(const vector<VaccinationCentre*> &centres, const vector<Vaccine*> &vaccines);


    /**
     * @brief Destructor for Hub
     */
    ~Hub();

    /**
     * @brief Function to set the centres of the Hub to a new vector, linking all vCentres inside with the Hub as well
     * @param centres
     * @PreCondition REQUIRE(properlyInitialised(), "Hub should be properly initialised upon calling setCentres");
     * @PreCondition REQUIRE(isValidVCentres(newCentres), "Centres should not be empty upon calling setCentres");
     * @PreCondition REQUIRE((*vCentreIterator)->properlyInitialised(), "All Vaccination Centres in the vector should be properly initialised upon calling setCentres");
     * @PostCondition ENSURE(properlyInitialised(), "SetCentres should end in an initialised state");
     * @PostCondition ENSURE(this->getVCentres() == newCentres,"VCentres was not properly set");

     */
    void setCentres(const vector<VaccinationCentre *> &centres);

    /**
     * @brief Set parameter vaccinations of the hub
     * @param vaccines
     * @PreCondition     REQUIRE(not vaccinesNew.empty(), "Vaccines cannot be empty upon calling setVaccines");
     * @PostCondition    ENSURE(this->getVaccines() == vaccinesNew, "Vaccines was not properly set");
    */
    void setVaccines (const vector<pair<Vaccine*,int> >& vaccines);

    /**
     * @brief Add a vaccinationCentre to the list of centres from the hub
     * @param vCentre: the vaccinationCentre you want to add
     * @PreCondition        REQUIRE(properlyInitialised(), "Hub wasn't initialised when trying to add a vaccinationCentre Object");
     * @PostCondition       ENSURE(contains(centres, vCentre), "addVCentre post-condition failed");
     */
    __attribute__((unused)) void addVCentre(VaccinationCentre* vCentre);

    /**
     * @brief Output the contents of the object to a desired output stream
     * @param onstream: the desired output stream
     * @PreCondition     REQUIRE(properlyInitialised(), "Hub wasn't initialised when trying to print out the Hub");
     */
    void print(ostream &onstream) const;

    /**
     * @brief Adds the elements of vCentreNew to the existing centres
     * @param vCentreNew
     * @PreConditions     REQUIRE(properlyInitialised(), "Hub wasn't initialised when trying to set the VaccinationCentres");
     * @PreConditions     REQUIRE(!vCentreNew.empty(), "Centre should not be empty");
     * @PostCondition     ENSURE(centres == vCentreNew, "PostCondition of setVCentres failed");
     */
    void setVCentres(const vector<VaccinationCentre*> &vCentreNew);

    /**
     * @brief Getter for all centres
     * @return vector containing a pointer to each centre in the hub
     * @PreCondition     REQUIRE(properlyInitialised(), "Hub wasn't initialised when calling getVCentres");
     */
    const vector<VaccinationCentre *> &getVCentres() const;

    /**
     * @brief Getter for all vaccines
     * @return vector pair of the vaccine type and the amount of vaccines currently in the Hub
     *
     */
    vector<pair<Vaccine*,int> > getVaccines() const;

    /**
     * @brief Function to check if the Hub has been properly initialised
     * @return true if yes, false if no
     */
    bool properlyInitialised() const;

    /**
     * @brief Function to determine whether the delivery variable is valid
     * @param delivery
     * @return true if > 0, false if not
     */
    __attribute__((unused)) static bool isValidDelivery( int delivery);

    /**
     * @brief Function to determine whether the transport variable is valid
     * @param transport
     * @return true if > 0, false if not
     */
    __attribute__((unused)) static bool isValidTransport( int transport);

    /**
     * @brief Function to determine whether the transport variable is valid
     * @param interval
     * @return true if > 0, false if not
     */
    __attribute__((unused)) static bool isValidInterval( int interval);

    /**
     * @brief Function to determine whether the vCentres variable is valid
     * @param vCentres
     * @return false if empty, true if not
     */
    static bool isValidVCentres(const vector<VaccinationCentre*> &vCentres);

    /**
     * @brief Function to determine whether the vaccines variable is valid
     * @param vaccines
     * @return false if empty, true if not
     */
    static bool isValidVaccines(const vector<Vaccine*> &newVaccines);

    /**
     * @brief Function to determine if each Vaccination Centre in the Hub points to this Hub
     * @return true if yes, false if no
     * @PreCondition     REQUIRE(properlyInitialised(), "Hub not properly initialised upon calling isConsistent()");
     */
    bool isConsistent() const;

    /**
     * @brief Function to determine if there are enough vaccines for each centre
     * @return
     */
    bool enoughVaccines() const;

    /**
     * @brief Function to divide the vaccines over all if enoughVaccines() gives false
     * @return
     * @PreCondition     REQUIRE(this->properlyInitialised(), "Hub wasn't properly initialised upon calling one of its member functions");
     */
    void divideVaccinesOverAll();

    /**
     * @brief Gets the amount of vaccines in the hub
     * @return amount of vaccines
     */
    int getAmountOfVaccines() const;

    /**
     * @brief Checks if a vaccinesVector is valid
     * @param vaccines
     * @return
     */
    __attribute__((unused)) static bool isValidVaccinesVector(const vector<pair<Vaccine *, int> > &vaccines);

    /**
     * @brief Sorts the vaccine vector so that the best vaccine is distributed first by first calculating which
     * vaccine is best.
     */
    void sortVaccinesVector();

    /**
     * @brief Gets the total amount of inhabitants in this hub
     * @return int      amount of inhabitants
     */
     int getInhabitantsAmount() const;


     /**
      * @brief Gets the total amount of inhabitants in this hub that are vaccinated
      * @return int     amount of inhabitants that are vaccinated
      */
      int getVaccinatedAmount() const;
};


#endif //INC_2__PROJECT_HUB_H
