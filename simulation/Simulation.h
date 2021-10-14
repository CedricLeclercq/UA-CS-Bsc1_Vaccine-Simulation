// +--------------------------------------------------+
// | Created by Cédric Leclercq and Maarten Peirsman  |
// |                                                  |
// |             University of Antwerp                |
// |                                                  |
// |           Project software engineering           |
// |                   2020-2021                      |
// +--------------------------------------------------+

#ifndef INC_2__PROJECT_SIMULATION_H
#define INC_2__PROJECT_SIMULATION_H

#include "../src/Hub.h"

class Simulation {
    Simulation* _initCheck; /// Pointer to itself to ensure proper initialisation

    vector<Hub*> hubs;

    vector<VaccinationCentre*> allvCentres;

public:
    /**
     * @brief Default constructor
     */
    Simulation();

    /**
     * @brief Copy constructor
     * @param simulation
     * @PostCondition ENSURE(this->properlyInitialised(),"Simulation was not properly initialised after constructing it.");
     * @PostCondition ENSURE(this->isConsistent(),"Simulation was not consistent after constructing it.");
     */
    __attribute__((unused)) Simulation(const Simulation &simulation);

    /**
     * @brief = Operator
     * @param simulation
     * @PostCondition ENSURE(this->properlyInitialised(),"Simulation was not properly initialised after the usage of the operator =.");
     * @PostCondition ENSURE(this->isConsistent(),"Simulation was not consistent after the usage of the operator =.");
     */
    Simulation& operator=(const Simulation &simulation);

    /**
     * @brief Constructor for Simulation
     * @param hub The hub on which to perform the simulation
     * @PreCondition REQUIRE(hub->properlyInitialised(),"Hub is not properly initialised upon constructing a simulation");
     * @PostCondition ENSURE(properlyInitialised(), "Constructor must end in an initialised state");
     * @PostCondition ENSURE(isConsistent(), "Hub and Vaccinationcentres should be consistent with one another");
     */
    Simulation(Hub* hub);

    /**
     * @brief Constructor for Simulation
     * @param hubsVector
     * @Warning This constructor results in a not fully initialised object. Proceed with caution.
     * @PreCondition REQUIRE(!hubsVector.empty(), "Hubs vector should not be empty upon constructing a simulation");
     * @PreCondition REQUIRE((*hubIterator)->properlyInitialised(),"At least one Hub was not properly initialised upon constructing a simulation");
     * @PostConditon ENSURE(properlyInitialised(), "Constructor for Simulation must end in an initialised state");
     */
    Simulation(vector<Hub*> &hubsVector);

    /**
     * @brief Constructor for Simulation
     * @param hubsVector
     * @param vCentreVector
     * @PreCondition REQUIRE(!hubsVector.empty(), "Hubs vector should not be empty upon constructing a simulation");
     * @PreCondition REQUIRE((*hubIterator)->properlyInitialised(),"At least one Hub was not properly initialised upon constructing a simulation");
     * @PreCondition ENSURE((*hubIterator)->isConsistent(), "Hub and Vaccinationcentres should be consistent with one another");
     * @PostConditon ENSURE(properlyInitialised(), "Constructor for Simulation must end in an initialised state");
     */
    Simulation(vector<Hub*> &hubsVector, vector<VaccinationCentre*> &vCentreVector);

    /**
     * @brief Destructor for Simulation
     */
    ~Simulation();

    /**
     * @brief Function checking if all vacination centres in the hub point to the correct hub
     * @return true if yes, false if no
     * @PreCondition    REQUIRE(properlyInitialised(), "Simulation wasn't initialised when trying to check for consistency");
     */
    bool isConsistent() const;

    /**
     * @brief Function to check if the simulation was properly initialised
     * @return true if yes, false if no
     */
    bool properlyInitialised() const;

    /**
     * @brief Getter for the hubs
     * @return vector with pointers to each hub in the simulation
     */
    vector<Hub *> getHubs() const;

    /**
     * @brief Setter for the hub
     * @PreCondition Simulation is properly initialised
     * @PreCondition Hub is properly initialised
     * @param Hub * hub
     */
    void setHub(Hub * const hub);

    /**
     * @brief Function to start automatic simulation
     * @param time Number of days to run the simulation
     * @PreCondition time > 0
     * @PreCondition properlyInitialised()
     * @PreCondition REQUIRE(isConsistent(), "Simulation is niet consistent");
     */
    void autoSimulation(int time, ostream &outputStream, bool wait=true) const;


    /**
     * @brief Setter for all the VCentres
     * @param newVCentre
     * @PostCondition ENSURE(this->allvCentres == newVCentre,"allvCentres wasn't properly initialed");
     */
    void setVCentres(const vector<VaccinationCentre*>& newVCentre);
};


#endif //INC_2__PROJECT_SIMULATION_H
