// +--------------------------------------------------+
// | Created by Cédric Leclercq and Maarten Peirsman  |
// |                                                  |
// |             University of Antwerp                |
// |                                                  |
// |           Project software engineering           |
// |                   2020-2021                      |
// +--------------------------------------------------+


#ifndef VACCINATION_SIMULATION_UTILITIES_H
#define VACCINATION_SIMULATION_UTILITIES_H

#include <string>
#include <iostream>
#include <vector>
class VaccinationCentre;
class Hub;
class Vaccine;

using namespace std;

class Utilities {
public:
    /**
     * @brief Function that returns true when the contents of file1 and file2 are exactly the same
     * @param fileName1 Name of file1
     * @param fileName2 Name of file2
     * @PreCondition REQUIRE(fileExists(fileName1), "File: " + fileName1 + " does not exist");
     * @PreCondition REQUIRE(fileExists(fileName2), "File: " + fileName2 + " does not exist");
     * @return true if the same, false if not
     */
    static bool fileCompare(const std::string &fileName1, const std::string &fileName2);

    /**
     * @brief Function to determine whether a file exists
     * @param fileName Name of the file
     * @return true if file exists, false if not
     */
    static bool fileExists(const std::string &fileName);

    /**
    * @brief Function that checks if a file is an XML file
    * @param fileName the name of the file
    * @return true if it is an XML file, false if not
    */
    static bool isXML(const std::string &fileName);

    /**
     * @brief Rounds a double to int
     * @param d
     * @return int
     */
    static int roundToInt(double d);

    /**
     * @brief Gets a vector<Hub*> and makes a vector<VaccinationCentre*> with no doubles
     * @param   vector of hub pointers
     * @returns vector of all the vaccination centres in that hub without doubles
     */
    static vector<VaccinationCentre*> getAllVCentres(vector<Hub*> allHubs);

    /**
     * @brief Function to determine whether a string is empty or consists of whitespaces only
     * @param str The string to check
     * @return true if the string is empty, false if not
     */
    static bool isEmptyString(const string &str);

    /**
     * @brief Sort function to compare the temperature of 2 vaccines
     * @param firstPair     Vaccine1, amountOfThisVaccine
     * @param secondPair    Vaccine2, amountOfThisVaccine
     * @return  firstPair->temperature < secondPair->temperature
     */
    static bool compareTemperature(pair<Vaccine*,int> firstPair, pair<Vaccine*,int> secondPair);
};


#endif //VACCINATION_SIMULATION_UTILITIES_H
