// +--------------------------------------------------+
// | Created by Cédric Leclercq and Maarten Peirsman  |
// |                                                  |
// |             University of Antwerp                |
// |                                                  |
// |           Project software engineering           |
// |                   2020-2021                      |
// +--------------------------------------------------+

#ifndef VACCINATION_SIMULATION_INHABITANT_H
#define VACCINATION_SIMULATION_INHABITANT_H

#include "../utilities/Utilities.h"
#include "../utilities/DesignByContract.h"
class Vaccine;
enum VaccinatedStatus {Vaccinated, PartiallyVaccinated, NotVaccinated};

class Inhabitant {
    VaccinatedStatus vaccinated; // enum status if this inhabitant is vaccinated
    Vaccine* vaccine; // Type of vaccine the inhabitant got. Will be NULL if the inhabitant has not been vaccinated yet
    int renewablesLeft; // Days until the inhabitant needs his second vaccine
    Inhabitant* _initCheck; // Pointer to itself to check proper initialisation

public:
    /**
     * @brief Default constructor
     */
    Inhabitant();

    /// Getters
    VaccinatedStatus getVaccinated() const; // Gets the parameter vaccinated
    Vaccine* getVaccine() const; // Gets the parameter vaccine
    int getRenewablesLeft() const; // Gets the parameter renewablesLeft

    /**
     * @brief Checks if the inhabitant is properly initialised
     * @return
     */
    bool properlyInitialised() const;
    /**
     * @brief Sets the parameter vaccinated to newVaccinated
     * @param newVaccinated
     * @PreCondition REQUIRE(this->properlyInitialised(), "Inhabitant was not properly initialised upon calling setVaccinated");
     * @PostCondition ENSURE(this->vaccinated == newVaccinated,"Vaccinated people wasn't properly set");
     */
    void setVaccinated( VaccinatedStatus newVaccinated );
    /**
     * @brief Sets the parameter vaccine to newVaccine
     * @param newVaccine
     * @PreCondition REQUIRE(this->properlyInitialised(),"Inhabitant was not properly initialised upon calling setVaccine");
     * @PostCondition ENSURE(this->vaccine == newVaccine, "A vaccine wasn't properly set");
     */
    void setVaccine( Vaccine* newVaccine );
    /**
     * @brief Sets the parameter renewablesLeft to newRenewablesLeft
     * @param newRenewablesLeft
     * @PreCondition REQUIRE(this->properlyInitialised(),"Inhabitant was not properly initialised upon calling setRenewablesLeft");
     * @PostCondition ENSURE(this->renewablesLeft == newRenewablesLeft,"Renewables wasn't properly set");
     */
    void setRenewablesLeft( int newRenewablesLeft );

};


#endif //VACCINATION_SIMULATION_INHABITANT_H
