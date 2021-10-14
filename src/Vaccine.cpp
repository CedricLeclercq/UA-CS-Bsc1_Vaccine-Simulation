// +--------------------------------------------------+
// | Created by Cédric Leclercq and Maarten Peirsman  |
// |                                                  |
// |             University of Antwerp                |
// |                                                  |
// |           Project software engineering           |
// |                   2020-2021                      |
// +--------------------------------------------------+

#include "Vaccine.h"
#include "../utilities/Utilities.h"
#include "../utilities/DesignByContract.h"

Vaccine::Vaccine() : _delivery(), _interval(), _transport(), _renewal(), _temperature() {
    _initCheck = this;
}

Vaccine::Vaccine(const string &type, const int delivery, const int interval, const int transport, const int renewal,
                 const int temperature) {
    _type = type;
    _delivery = delivery;
    _interval = interval;
    _transport = transport;
    _renewal = renewal;
    _temperature = temperature;
    _initCheck = this;
    ENSURE(properlyInitialised(), "Vaccine object should be initialised when exiting constructor");
}

bool Vaccine::properlyInitialised() const {
    return _initCheck == this;
}

bool Vaccine::isValidType(const string &type) {
    return !Utilities::isEmptyString(type);
}

bool Vaccine::isValidDelivery(const int delivery) {
    return delivery > 0;
}

bool Vaccine::isValidInterval(const int interval) {
    return interval > 0;
}

bool Vaccine::isValidTransport(const int transport) {
    return transport > 0;
}

bool Vaccine::isValidRenewal(const int renewal) {
    return renewal > 0;
}

string Vaccine::getType() const {
    return this->_type;
}

int Vaccine::getDelivery() const {
    return this->_delivery;
}

int Vaccine::getInterval() const {
    return this->_interval;
}

int Vaccine::getTransport() const {
    return this->_transport;
}

int Vaccine::getRenewal() const {
    return this->_renewal;
}

int Vaccine::getTemperature() const {
    return this->_temperature;
}

