// +--------------------------------------------------+
// | Created by Cédric Leclercq and Maarten Peirsman  |
// |                                                  |
// |             University of Antwerp                |
// |                                                  |
// |           Project software engineering           |
// |                   2020-2021                      |
// +--------------------------------------------------+

#include "Simulation.h"
#include "../graphics/GraphicalController.h"
#include "../utilities/DesignByContract.h"

bool Simulation::isConsistent() const {
    REQUIRE(properlyInitialised(), "Simulation wasn't initialised when trying to check for consistency");
    vector<Hub*>::const_iterator hubsIterator;
    for (hubsIterator = hubs.begin(); hubsIterator != hubs.end(); hubsIterator++) {
        if (!(*hubsIterator)->isConsistent()) {
            return false;
        }
    }
    return true;
}

bool Simulation::properlyInitialised() const {
    return _initCheck == this;
}

Simulation::Simulation(Hub *hub) {
    REQUIRE(hub->properlyInitialised(),"Hub is not properly initialised upon constructing a simulation");
    vector<Hub*> newHubs;
    newHubs.push_back(hub);
    hubs = newHubs;
    _initCheck = this;
    ENSURE(properlyInitialised(), "Constructor must end in an initialised state");
    ENSURE(isConsistent(), "Hub and Vaccinationcentres should be consistent with one another");
}

Simulation::Simulation(vector<Hub*> &hubsVector, vector<VaccinationCentre*> &vCentres) {
    REQUIRE(!hubsVector.empty(), "Hubs vector should not be empty upon constructing a simulation");
    vector<Hub*>::iterator hubIterator;
    for (hubIterator = hubsVector.begin(); hubIterator != hubsVector.end(); hubIterator++) {
        REQUIRE((*hubIterator)->properlyInitialised(),"At least one Hub was not properly initialised upon constructing a simulation");
        ENSURE((*hubIterator)->isConsistent(), "Hub and Vaccinationcentres should be consistent with one another");
    }
    hubs = hubsVector;
    _initCheck = this;
    ENSURE(properlyInitialised(), "Constructor for Simulation must end in an initialised state");
}





void Simulation::autoSimulation(const int time, ostream &outputStream, bool wait) const {

    REQUIRE(isConsistent(), "Simulation is niet consistent");
    REQUIRE(properlyInitialised(),"Simulation is not properly initialised");
    REQUIRE(time > 0, "Simulation time should be non-negative and non-zero");

    /*
    if (!Simulation::isConsistent()) {
        cerr << "The simulation is not consistent!" << endl;
        return;
    }
     */
    // Aantal dagen in int
    GraphicalController * controller = new GraphicalController();
    controller->setAllHubs(this->hubs);
    controller->setAllvCentre(this->allvCentres);

    vector<Hub *>::const_iterator hubIterator;
    vector<pair<Vaccine*,int> >::iterator vaccineIterator;
    for (int currentTime = 0; currentTime < time; currentTime++) {

        controller->setCurrentFigureID(0);

        bool allVaccinated = true;
        for (hubIterator = this->hubs.begin(); hubIterator != this->hubs.end(); hubIterator++) {
            //(*hubIterator)->sortVaccinesVector();
            if ((*hubIterator)->getVaccinatedAmount() != (*hubIterator)->getInhabitantsAmount()) {
                allVaccinated = false;
            }
        }
        if (allVaccinated) {
            outputStream << "We zijn dag " + to_string(currentTime) << ".\n";
            outputStream << "Iedereen is gevaccineerd. De simulatie stopt automatisch.\n";
            return;
        }

        if (currentTime == 0) {
            if (outputStream != cout) {
                // Added this for when the user is confused to where the output is.
                cout << "De simulatie is gestart." << endl;
                cout << "Alle resultaten zullen geprint worden naar de gekozen output."<< endl;
            }
            outputStream << "Vaccinatie Simulatie" << endl;
            outputStream << "Cédric Leclercq & Maarten Peirsman" << endl;
            outputStream << "Project Software Engineering Ba1 INF" << endl << endl;

            string output = "Deze simulatie is ingesteld voor " + to_string(time);
            if (time == 1) {
                output += " dag en begint binnen 5 seconden.";
            } else {
                output += " dagen en begint binnen 5 seconden.";
            }
            outputStream << output << endl;
            if (wait) sleep(5);

            outputStream << "\nStart van de simulatie - dag 1:\n\n";

        } else {
            outputStream << endl << "Dag " + to_string(currentTime + 1) + ":\n";
        }
        vector<VaccinationCentre*> allvCentre;
        for (hubIterator = hubs.begin(); hubIterator != hubs.end(); hubIterator++) {
            if (currentTime == 0) {
                // Bij het begin van de simulatie bevat de hub het aantal vaccinations gelijk aan de waarde van het
                // attribuut levering
                vector<pair<Vaccine*,int> > vaccines = (*hubIterator)->getVaccines();

                for (vaccineIterator = vaccines.begin(); vaccineIterator != vaccines.end(); vaccineIterator++) {
                    (*vaccineIterator).second = (*vaccineIterator).first->getDelivery();
                }
                (*hubIterator)->setVaccines(vaccines);
                // (*hubIterator)->setVaccinations(Simulation::hub->getLevering());

                vector<VaccinationCentre *>::const_iterator vCentre;
                vector<VaccinationCentre*> vCentres = (*hubIterator)->getVCentres();
                if ((*hubIterator)->enoughVaccines()) {
                    for (vCentre = vCentres.begin(); vCentre != vCentres.end(); vCentre++) {
                        (*vCentre)->transportNecessaryVaccines((*hubIterator), outputStream);
                    }
                    for (vCentre = vCentres.begin(); vCentre != vCentres.end(); vCentre++) {
                        (*vCentre)->transportUnspecifiedVaccines((*hubIterator), outputStream);
                    }
                } else {
                    // There are not enough vaccines, divide over all that have none
                    (*hubIterator)->divideVaccinesOverAll();
                }
                (*hubIterator)->setCentres(vCentres);
            } else {
                /*
                 * Loop over alle vaccins en zie welke geleverd moeten worden aan de hub. Probeer ook de transporteren dan en te vaccineren
                 *
                 */
                vector<pair<Vaccine *, int> > vaccines = (*hubIterator)->getVaccines();
                for (vaccineIterator = vaccines.begin(); vaccineIterator != vaccines.end(); vaccineIterator++) {

                    if (currentTime % (*vaccineIterator).first->getInterval() == 0) {
                        (*vaccineIterator).second += (*vaccineIterator).first->getDelivery();
                    }
                }
                (*hubIterator)->setVaccines(vaccines);
                vector<VaccinationCentre *>::const_iterator vCentre;
                vector<VaccinationCentre *> vCentres = (*hubIterator)->getVCentres();
                if ((*hubIterator)->enoughVaccines()) {

                    for (vCentre = vCentres.begin(); vCentre != vCentres.end(); vCentre++) {
                        (*vCentre)->transportNecessaryVaccines((*hubIterator), outputStream);
                        if (not contains(allvCentre, (*vCentre))) {
                            allvCentre.push_back((*vCentre));
                        }
                    }
                    for (vCentre = vCentres.begin(); vCentre != vCentres.end(); vCentre++) {
                        (*vCentre)->transportUnspecifiedVaccines((*hubIterator), outputStream);
                    }
                } else {
                    (*hubIterator)->divideVaccinesOverAll();
                }

                (*hubIterator)->setCentres(vCentres);
            }
            // For the graphical representation, we need the amount of vaccines transfered from each hub to each vcentre.
            vector<VaccinationCentre*> allCentres = Utilities::getAllVCentres(this->getHubs());
            vector<VaccinationCentre*>::const_iterator vCentreIterator;
            vector<pair<pair<Hub*,VaccinationCentre*>, int> > controllerTransport = controller->getTransported();
            for (vCentreIterator = allCentres.begin(); vCentreIterator != allCentres.end(); vCentreIterator++) {
                // Do we want a detailed overview of the centres, make the bool true
                vector<pair<Vaccine*,int> > allVaccines = (*vCentreIterator)->getTransferredThisDay();
                int total = 0;
                for (vaccineIterator = allVaccines.begin(); vaccineIterator != allVaccines.end(); vaccineIterator++) {
                    total += (*vaccineIterator).second;
                }
                if (total > 0) {
                    controllerTransport.push_back(make_pair(make_pair((*hubIterator), (*vCentreIterator)), total));
                }
                (*vCentreIterator)->clearTransferredThisDay();
            }
            controller->setTransported(controllerTransport);
        }
        vector<VaccinationCentre*>::const_iterator vCentreIterator;
        for (hubIterator = hubs.begin(); hubIterator != hubs.end(); hubIterator++) {
            vector<VaccinationCentre *>::const_iterator vCentre;
            vector<VaccinationCentre *> vCentres = (*hubIterator)->getVCentres();
            for (vCentre = vCentres.begin(); vCentre != vCentres.end(); vCentre++) {
                (*vCentre)->vaccinate();
                // Trowing away all the vaccines that have a temperature < 0 after one day
                (*vCentre)->cleanOutBadVaccines();
                //(*vCentre)->printOverview(true, outputStream);
            }
            (*hubIterator)->setCentres(vCentres);
        }
        for (vCentreIterator = allvCentre.begin(); vCentreIterator != allvCentre.end(); vCentreIterator++) {
            // Do we want a detailed overview of the centres, make the bool true
            (*vCentreIterator)->printOverview(true, outputStream);
        }
        controller->writeGeneral(currentTime);
        controller->clearTransported();
    }
}

vector<Hub *> Simulation::getHubs() const {
    return hubs;
}

Simulation::~Simulation() {
    //delete hub;
}

__attribute__((unused)) Simulation::Simulation(const Simulation &simulation) {
    this->_initCheck = simulation._initCheck;
    this->hubs = simulation.hubs;
    this->allvCentres = simulation.allvCentres;
    ENSURE(this->properlyInitialised(),"Simulation was not properly initialised after constructing it.");
    ENSURE(this->isConsistent(),"Simulation was not consistent after constructing it.");
}

Simulation &Simulation::operator=(const Simulation &simulation) {
    this->hubs = simulation.hubs;
    this->allvCentres = simulation.allvCentres;
    this->_initCheck = this;
    ENSURE(this->properlyInitialised(),"Simulation was not properly initialised after the usage of the operator =.");
    ENSURE(this->isConsistent(),"Simulation was not consistent after the usage of the operator =.");
    return *this;
}

Simulation::Simulation(vector<Hub *> &hubsVector) {
    // Note - this constructor results in a not fully initialised object. Proceed with caution.
    this->_initCheck = this;
    this->hubs = hubsVector;
}

void Simulation::setVCentres(const vector<VaccinationCentre *> &newVCentre) {
    this->allvCentres = newVCentre;
    ENSURE(this->allvCentres == newVCentre,"allvCentres wasn't properly initialed");
}

Simulation::Simulation() {
    _initCheck = this;
}
