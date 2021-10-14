// +--------------------------------------------------+
// | Created by Cédric Leclercq and Maarten Peirsman  |
// |                                                  |
// |             University of Antwerp                |
// |                                                  |
// |           Project software engineering           |
// |                   2020-2021                      |
// +--------------------------------------------------+

#ifndef INC_2__PROJECT_IMPORTER_H
#define INC_2__PROJECT_IMPORTER_H

#include "../simulation/Simulation.h"
#include "../tinyXML/tinyxml.h"
#include <string>

using namespace std;

enum Success {Successful, CompletedWithErrors, PartiallyCompleted, Unsuccessful};

class Importer {
public:
    /**
     * @brief Function to import all simulation data from an XML file
     * @param fileName name of the file
     * @param errorStream where to output the errors to
     * @param simulation Simulation object
     * @PreCondition REQUIRE(!fileName.empty(), "Filename should not be empty");
     * @PreCondition REQUIRE(isXML(fileName), "Given file is not an XML file");
     * @PreCondition REQUIRE(simulation.properlyInitialised(), "Simulation was not properly initialised upon calling importSimulation");
     * @PostConditon ENSURE(simulation.properlyInitialised(), "Simulation is no longer properly initialised after reading XML");
     * @PostCondtion ENSURE(simulation.isConsistent(), "Simulation should be consistent after reading XML");
     * @return Whether the import was successful:
     * @return - Unsuccessful: Simulation is unchanged; Errors can be found in errorStream
     * @return - PartiallyCompleted: Simulation not correctly initialised and may have undergone changes; Errors can be found in errorStream
     * @return - CompletedWithErrors: Simulation completely initialised but with errors that may result in unexpected or unwanted behaviour; Errors can be found in errorStream
     * @return - Succesful: Simulation successfully initialised without errors
     */
    static Success importSimulation(const string &fileName, ostream &errorStream, Simulation &simulation);

private:
    /**
     * @brief Read in a Hub from an XML element
     * @param element The element containing the Hub specifiactions
     * @param errorStream where to output the errors to
     * @param vCentres Map containing all vaccination centres
     * @PreCondition REQUIRE(!vCentres.empty(), "vCentres vector should not be empty upon calling readHubOld");
     * @return pointer to the Hub obejct
     */
    static Hub *readHub(TiXmlElement *element, ostream &errorStream, map<string, VaccinationCentre*> &vCentres);

    /**
     * @brief Reads in a VaccinatieCentrum object from an XML element
     * @param element The XML element containing the specifications for the VaccinatieCentrum
     * @param errorStream where to output the errors to
     * @return pointer to the VaccinatieCentrum object
     */
     static VaccinationCentre* readVCentre(TiXmlElement *element, ostream &errorStream);

    /**
     * @brief Reads in a Vaccine object from an XML element
     * @param element The XML element containing the specifications for the Vaccine
     * @param errorStream where to output the errors to
     * @return pointer to the Vaccine object
     */
    static Vaccine *readVaccine(TiXmlElement *element, ostream &errorStream);
};


#endif //INC_2__PROJECT_IMPORTER_H
