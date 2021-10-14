// +--------------------------------------------------+
// | Created by Cédric Leclercq and Maarten Peirsman  |
// |                                                  |
// |             University of Antwerp                |
// |                                                  |
// |           Project software engineering           |
// |                   2020-2021                      |
// +--------------------------------------------------+

#ifndef VACCINATION_SIMULATION_VACCINE_H
#define VACCINATION_SIMULATION_VACCINE_H

#include <string>

using namespace std;


class Vaccine {
    string _type;
    int _delivery;
    int _interval;
    int _transport;
    int _renewal;
    int _temperature;
    Vaccine* _initCheck; /// Pointer to self for initialisation check

public:
    /**
     * @brief Default constructor
     */
    Vaccine();

    /**
     * @brief Constructor for Vaccine
     * @param type
     * @param delivery
     * @param interval
     * @param transport
     * @param renewal
     * @param temperature
     * @PostCondition ENSURE(properlyInitialised(), "Vaccine object should be initialised when exiting constructor");
     */
    Vaccine(const string &type, int delivery, int interval, int transport, int renewal, int temperature);

    /**
     * @brief Function to check if the object was initialised correctly
     * @return
     */
    bool properlyInitialised() const;

    ///Getters
    string getType() const;
    int getDelivery() const;
    int getInterval() const;
    int getTransport() const;
    int getRenewal() const;
    int getTemperature() const;

    /**
     * @brief Function checking whether "type" is a valid parameter for type
     * @param type
     * @return false if empty, true if not
     */
    static bool isValidType(const string &type);

    /**
     * @brief Function checking whether "delivery" is a valid parameter for delivery
     * @param delivery
     * @return true if > 0, false if not
     */
    static bool isValidDelivery( int delivery);

    /**
     * @brief Function checking whether "interval" is a valid parameter for interval
     * @param interval
     * @return true if > 0, false if not
     */
    static bool isValidInterval( int interval);

    /**
     * @brief Function checking whether "transport" is a valid parameter for transport
     * @param transport
     * @return true if > 0, false if not
     */
    static bool isValidTransport( int transport);

    /**
     * @brief Function checking whether "renewal" is a valid parameter for renewal
     * @param renewal
     * @return true if > 0, false if not
     */
    static bool isValidRenewal( int renewal);
};


#endif //VACCINATION_SIMULATION_VACCINE_H
