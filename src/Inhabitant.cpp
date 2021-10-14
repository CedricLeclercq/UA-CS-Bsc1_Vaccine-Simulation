// +--------------------------------------------------+
// | Created by Cédric Leclercq and Maarten Peirsman  |
// |                                                  |
// |             University of Antwerp                |
// |                                                  |
// |           Project software engineering           |
// |                   2020-2021                      |
// +--------------------------------------------------+

#include "Inhabitant.h"

Inhabitant::Inhabitant() : vaccine() {
    // Default constructor
    this->_initCheck = this;
    this->renewablesLeft = 0;
    this->vaccinated = NotVaccinated;
    ENSURE(this->properlyInitialised(), "Inhabitant was not properly initialised after its construction.");
}

bool Inhabitant::properlyInitialised() const {
    return _initCheck == this;
}

VaccinatedStatus Inhabitant::getVaccinated() const {
    return this->vaccinated;
}

Vaccine *Inhabitant::getVaccine() const {
    return this->vaccine;
}

int Inhabitant::getRenewablesLeft() const {
    return this->renewablesLeft;
}

void Inhabitant::setVaccinated(VaccinatedStatus newVaccinated) {
    REQUIRE(this->properlyInitialised(), "Inhabitant was not properly initialised upon calling setVaccinated");
    this->vaccinated = newVaccinated;
    ENSURE(this->vaccinated == newVaccinated,"Vaccinated people wasn't properly set");
}

void Inhabitant::setVaccine(Vaccine *newVaccine) {
    REQUIRE(this->properlyInitialised(),"Inhabitant was not properly initialised upon calling setVaccine");
    this->vaccine = newVaccine;
    ENSURE(this->vaccine == newVaccine, "A vaccine wasn't properly set");
}

void Inhabitant::setRenewablesLeft(int newRenewablesLeft) {
    REQUIRE(this->properlyInitialised(),"Inhabitant was not properly initialised upon calling setRenewablesLeft");
    this->renewablesLeft = newRenewablesLeft;
    ENSURE(this->renewablesLeft == newRenewablesLeft,"Renewables wasn't properly set");
}


