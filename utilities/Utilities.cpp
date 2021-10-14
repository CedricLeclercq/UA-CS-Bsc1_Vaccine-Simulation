// +--------------------------------------------------+
// | Created by Cédric Leclercq and Maarten Peirsman  |
// |                                                  |
// |             University of Antwerp                |
// |                                                  |
// |           Project software engineering           |
// |                   2020-2021                      |
// +--------------------------------------------------+

#include "Utilities.h"
#include "../src/VaccinationCentre.h"
#include "../src/Hub.h"
#include <cmath>

bool Utilities::fileCompare(const std::string &fileName1, const std::string &fileName2) {
    REQUIRE(fileExists(fileName1), ("File: " + fileName1 + " does not exist").c_str());
    REQUIRE(fileExists(fileName2), ("File: " + fileName2 + " does not exist").c_str());
    // Read file 1
    std::ifstream file1;
    file1.open(fileName1.c_str());
    // Read contents into string1
    std::string string1;
    if (file1.is_open()) { // Check if file was opened
        file1 >> string1;
        file1.close();
    } else { // File wasn't opened correctly, abort
        file1.close();
        return false;
    }
    // Read file 2
    std::ifstream file2;
    file2.open(fileName2.c_str());
    // Read contents into string2
    std::string string2;
    if (file2.is_open()) { // Check if file was opened
        file2 >> string2;
        file2.close();
    } else { // File wasn't opened correctly, abort
        file2.close();
        return false;
    }
    // Compare contents
    return string1 == string2;
}

bool Utilities::fileExists(const std::string &fileName) {
    std::ifstream file;
    file.open(fileName.c_str());
    bool success = file.is_open();
    file.close();
    return success;
}

bool Utilities::isXML(const std::string &fileName) {
    // Checks if last 4 characters of fileName are ".XML"
    int fileNameSize = fileName.size();
    if (fileNameSize < 5) {
        return false;
    }
    std::string extension;
    for (int i = fileNameSize - 4; i < fileNameSize; i++) {
        extension += fileName[i];
    }
    return extension == ".xml";
}

int Utilities::roundToInt(double d) {
    return static_cast<int>(round(d));
}

vector<VaccinationCentre*> Utilities::getAllVCentres(vector<Hub*> allHubs) {
    vector<VaccinationCentre*> filtered;
    vector<Hub*>::const_iterator hubIterator;
    vector<VaccinationCentre*>::const_iterator centreIterator;
    for (hubIterator = allHubs.begin(); hubIterator != allHubs.end(); hubIterator++) {
        vector<VaccinationCentre*> vCentresInHub = (*hubIterator)->getVCentres();
        for (centreIterator = vCentresInHub.begin(); centreIterator != vCentresInHub.end(); centreIterator++) {
            if (not contains(filtered,(*centreIterator))) {
                filtered.push_back((*centreIterator));
            }
        }
    }
    return filtered;
}

bool Utilities::isEmptyString(const string &str) {
    for (string::const_iterator iter = str.begin(); iter != str.end(); iter++) {
        if (*iter != ' ') {
            return false;
        }
    }
    return true;
}

bool Utilities::compareTemperature(pair<Vaccine *, int> firstPair, pair<Vaccine *, int> secondPair)  {
    return (firstPair.first->getTemperature() < secondPair.first->getTemperature());
}


