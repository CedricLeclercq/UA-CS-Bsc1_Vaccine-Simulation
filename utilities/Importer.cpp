// +--------------------------------------------------+
// | Created by Cédric Leclercq and Maarten Peirsman  |
// |                                                  |
// |             University of Antwerp                |
// |                                                  |
// |           Project software engineering           |
// |                   2020-2021                      |
// +--------------------------------------------------+

#include "Importer.h"
#include "Utilities.h"
#include "DesignByContract.h"
#include <limits>

#include <fstream>

Success Importer::importSimulation(const string &fileName, ostream &errorStream, Simulation &simulation) {
    REQUIRE(!fileName.empty(), "Filename should not be empty");
    REQUIRE(Utilities::fileExists(fileName), ("File '" + fileName + "' does not exist").c_str());
    REQUIRE(Utilities::isXML(fileName), ("Given file '" + fileName + "' is not an XML file").c_str());
    REQUIRE(simulation.properlyInitialised(), "Simulation was not properly initialised upon calling importSimulation");
    // Initialise success state
    Success success = Successful;
    // Name of the tags you're looking for
    string tagHub = "HUB";
    string tagVCentrum = "VACCINATIECENTRUM";
    // Open document
    TiXmlDocument doc;
    if (!doc.LoadFile(fileName.c_str())) { // Test if the file can be loaded
        errorStream << "XML IMPORT ABORTED: " << doc.ErrorDesc() << " " << fileName << endl;
        return Unsuccessful; // Unsuccesfully tried to read the file
    }
    // Fetch root element from document
    TiXmlElement *root = doc.FirstChildElement();
    if (root == NULL) { // Check if file was loaded properly
        errorStream << "XML IMPORT ABORTED. Failed to load file '" << fileName << "': No root element found." << endl;
        doc.Clear();
        return Unsuccessful;
    } else { // Check if the input was expected
        string rootName = root->Value();
        if (rootName != tagHub && rootName != tagVCentrum) {
            errorStream << "XML IMPORT ABORTED: Expected <HUB> ... </HUB> or <VACCINATIECENTRUM> ... </VACCINATIECENTRUM>"
                            " and got <" << rootName << "> ... </" << rootName << ">" << endl;
            doc.Clear();
            return Unsuccessful;
        }
    }
    vector<TiXmlElement*> hubElements; // Initialise an XML Element to hold the information about the Hub
    map<string, VaccinationCentre*> vCentres; // Initialise a map to hold all VaccinatieCentre objects
    // Fetch information from XML file
    // Make sure all VaccinationCentre objects are constructed before constructing the Hub objects
    while (root != NULL) {
        string name = root->Value();
        if (name == tagHub) {
            hubElements.push_back(root);
        } else if (name == tagVCentrum) {
            VaccinationCentre* vCentre = readVCentre(root, errorStream);
            if (vCentre == NULL) { // Check if readVCentre was completed successfully
                success = CompletedWithErrors;
            } else { // vCentre was read successfully
                vCentres[vCentre->getNaam()] = vCentre;
            }
        } else {
            errorStream << "Unrecognisable element: Expected <HUB> or <VACCINATIECENTRUM> and got <" << root->Value() << ">" << endl;
            success = CompletedWithErrors;
        }
        root = root->NextSiblingElement();
    }
    if (hubElements.empty()) { // If at this point there are no hubs, the XML is faulty and import should abort
        errorStream << "XML PARTIAL IMPORT: No Hub elements found" << endl;
        return PartiallyCompleted;
    } else if (vCentres.empty()) { // If at this point there are no Vaccination Centres, the XML is faulty and import should abort
        errorStream << "XML PARTIAL IMPORT: No Vaccination Centre elements found" << endl;
        return PartiallyCompleted;
    }
    vector<Hub*> hubs; // Initialise vector to hold all hubs
    // Iterate over all hubElements and add each generated Hub element to the hubs vector
    vector<TiXmlElement*>::iterator hubElementIterator;
    for (hubElementIterator = hubElements.begin(); hubElementIterator != hubElements.end(); hubElementIterator++) {
        Hub* hub = readHub(*hubElementIterator, errorStream, vCentres);
        if (hub == NULL) { // If readHub was not succesfully completed, move to next element
            success = CompletedWithErrors;
        } else { // Successfully read the Hub element
            hubs.push_back(hub); // Add the element to the vector
        }
    }
    simulation = Simulation(hubs);
    doc.Clear();
    ENSURE(simulation.properlyInitialised(), "Simulation is no longer properly initialised after reading XML");
    ENSURE(simulation.isConsistent(), "Simulation should be consistent after reading XML");
    return success;
}

Hub* Importer::readHub(TiXmlElement *element, ostream &errorStream, map<string, VaccinationCentre*> &vCentres) {
    // Declare vectors to hold all Vaccines en Vaccination Centres
    vector<Vaccine*> vaccines;
    vector<VaccinationCentre*> vaccinationCentres;
    // Fetch info from XML element
    for (TiXmlElement *child = element->FirstChildElement(); child != NULL; child = child->NextSiblingElement()) {
        string propertyName = child->Value();
        if (propertyName == "VACCIN") {
            Vaccine* vaccine = readVaccine(child, errorStream);
            // If something went wrong reading the Vaccine, it returns NULL
            if (vaccine != NULL) { // Nothing wrong, continue looping
                vaccines.push_back(vaccine);
            } // If the vaccine is NULL, an error message is outputted and it continues looping
        } else if (propertyName == "CENTRA") { // If the current tag is "CENTRA"
            // Loop over all "centra" and input them in the hub
            for (TiXmlElement *centrum = child->FirstChildElement(); centrum != NULL; centrum = centrum->NextSiblingElement()) {
                string tagName = centrum->Value(); // tagname of the child element
                if (tagName == "centrum") { // If the tag is "centrum"
                    string attribute = centrum->GetText();
                    if (vCentres.find(attribute) == vCentres.end()) {
                        errorStream << "XML PARTIAL IMPORT: Vaccination Centre " + attribute + " does not exist" << endl;
//                        return NULL;
                    }
                    vaccinationCentres.push_back(vCentres[attribute]);
                } else {
                    errorStream << "XML PARTIAL IMPORT: Expected tag 'centrum' in 'CENTRA' and got " << tagName << endl;
                }
            }
        } else {
            errorStream << "XML PARTIAL IMPORT: Expected <VACCIN> or <CENTRA> but got <" << propertyName << ">" << endl;
        }
    }
    // Check if all arguments are valid
    if (!Hub::isValidVCentres(vaccinationCentres)) {
        errorStream << "XML IMPORT ABORTED: Vaccination Centres vector should not be empty" << endl;
    } else if (!Hub::isValidVaccines(vaccines)) {
        errorStream << "XML IMPORT ABORTED: Vaccines vector should not be empty" << endl;
    } else { // All arguments are valid
        return new Hub(vaccinationCentres, vaccines);
    } // Not all arguments are valid
    return NULL;
}

VaccinationCentre* Importer::readVCentre(TiXmlElement *element, ostream &errorStream) {
    // Initialise infinity
    int infinity = (int) numeric_limits<double>::infinity();
    // Declare variables
    string name;
    string address;
    int inhabitants = infinity;
    int capacity = infinity;
    // Initialise variables
    for (TiXmlElement *child = element->FirstChildElement(); child != NULL; child = child->NextSiblingElement()) {
        string propertyName = child->Value();
        if (child->NoChildren()) {
            errorStream << "Invalid entry: Field for attribute '" << propertyName << "' can not be empty" << endl;
        } else if (propertyName == "naam") { // Initialise name
            if (!name.empty()) { // Check if name was already initialised
                errorStream << "Redefinition of Vaccination Centre attribute: " << propertyName << endl;
            }
            name = child->FirstChild()->Value();
        } else if (propertyName == "adres") { // Initialise addres
            if (!address.empty()) { // Check if address was already initialised
                errorStream << "Redefinition of Vaccination Centre attribute: " << propertyName << endl;
            }
            address = child->FirstChild()->Value();
        } else if (propertyName == "inwoners") { // Initialise inhabitants
            if (inhabitants != infinity) { // Check if inhabitants was already initialised
                errorStream << "Redefinition of Vaccination Centre attribute: " << propertyName << endl;
            }
            inhabitants = atoi(child->FirstChild()->Value());
        } else if (propertyName == "capaciteit") { // Initialise capacity
            if (capacity != infinity) { // Check if capacity was already initialised
                errorStream << "Redefinition of Vaccination Centre attribute: " << propertyName << endl;
            }
            capacity = atoi(child->FirstChild()->Value());
        } else {
            errorStream << "XML PARTIAL IMPORT: Unknown attribute for Vaccination Centre: " << propertyName << endl;
        }
    }
    // Check if all values have been initialised
    if (inhabitants == infinity) {
        errorStream << "XML PARTIAL IMPORT: No attribute 'inwoners' found for Vaccination Centre" << endl;
    } else if (capacity == infinity) {
        errorStream << "XML PARTIAL IMPORT: No attribute 'capaciteit' found for Vaccinatin Centre" << endl;
    } else if (!VaccinationCentre::isValidName(name)) { // Check if name is valid
        errorStream << "XML PARTIAL IMPORT: Name for Vaccination Centre can not be empty" << endl;
    } else if (!VaccinationCentre::isValidAddress(address)) { // Check if addres is valid
        errorStream << "XML PARTIAL IMPORT: Address for Vaccination Centre can not be empty" << endl;
    } else if (!VaccinationCentre::isValidInhabitants(inhabitants)) { // Check if inhabitants is valid
        errorStream << "XML PARTIAL IMPORT: Inhabitants for Vaccination Centre should be a positive integer" << endl;
    } else if (!VaccinationCentre::isValidCapacity(capacity)) { // Check if capacity is valid
        errorStream << "XML PARTIAL IMPORT: Capacity for Vaccination Centre should be a positive integer" << endl;
    } else { // All values are valid, return new VaccinationCentre object
        return new VaccinationCentre(name, address, inhabitants, capacity);
    } // Not all values are valid, return NULL
    return NULL;
}

Vaccine* Importer::readVaccine(TiXmlElement *element, ostream &errorStream) {
    // Initialise infinity
    int infinity = (int) numeric_limits<double>::infinity();
    // Initialise variables
    string type;
    int delivery = infinity;
    int interval = infinity;
    int transport = infinity;
    int renewal = infinity;
    int temperature = infinity;
    // Fetch info from the XML file
    for (TiXmlElement *child = element->FirstChildElement(); child != NULL; child = child->NextSiblingElement()) {
        string tagName = child->Value();
        if (tagName == "type") {
            type = child->FirstChild()->Value();
        } else if (tagName == "levering") {
            delivery = atoi(child->FirstChild()->Value());
        } else if (tagName == "interval") {
            interval = atoi(child->FirstChild()->Value());
        } else if (tagName == "transport") {
            transport = atoi(child->FirstChild()->Value());
        } else if (tagName == "hernieuwing") {
            renewal = atoi(child->FirstChild()->Value());
        } else if (tagName == "temperatuur") {
            temperature = atoi(child->FirstChild()->Value());
        } else {
            errorStream << "XML PARTIAL IMPORT: Unknown attribute for Vaccine: " << tagName << endl;
        }
    }
    // check if variables were initialised correctly
    if (delivery == infinity) {
        errorStream << "XML PARTIAL IMPORT: No attribute 'levering' found for Vaccination Centre" << endl;
    } else if (temperature == infinity) {
        errorStream << "XML PARTIAL IMPORT: No attribute 'temperatuur' found for Vaccination Centre" << endl;
    } else if (transport == infinity) {
        errorStream << "XML PARTIAL IMPORT: No attribute 'transport' found for Vaccination Centre" << endl;
    } else if (renewal == infinity) {
        errorStream << "XML PARTIAL IMPORT: No attribute 'hernieuwing' found for Vaccination Centre" << endl;
    } else if (interval == infinity) {
        errorStream << "XML PARTIAL IMPORT: No attribute 'interval' found for Vaccination Centre" << endl;
    } else if (type.empty()) {
        errorStream << "XML PARTIAL IMPORT: No attribute 'type' found for Vaccination Centre" << endl;
    } // Check if variables are valid
    else if (!Vaccine::isValidDelivery(delivery)) {
        errorStream << "XML PARTIAL IMPORT: Delivery for Vaccine should be a positive Integer" << endl;
        return NULL;
    } else if (!Vaccine::isValidInterval(interval)) {
        errorStream << "XML PARTIAL IMPORT: Interval for Vaccine should be a positive Integer" << endl;
        return NULL;
    } else if (!Vaccine::isValidTransport(transport)) {
        errorStream << "XML PARTIAL IMPORT: Transport for Vaccine should be a positive Integer" << endl;
        return NULL;
    } else if (!Vaccine::isValidRenewal(renewal)) {
        errorStream << "XML PARTIAL IMPORT: Renewal for Vaccine should be a positive Integer" << endl;
        return NULL;
    } else if (!Vaccine::isValidType(type)) {
        errorStream << "XML PARTIAL IMPORT: Type for Vaccine should not be empty" << endl;
    } else { // All variables are valid
        // return Vaccine object
        return new Vaccine(type, delivery, interval, transport, renewal, temperature);
    }
    return NULL;
}