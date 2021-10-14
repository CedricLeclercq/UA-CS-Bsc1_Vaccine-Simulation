// +--------------------------------------------------+
// | Created by Cédric Leclercq and Maarten Peirsman  |
// |                                                  |
// |             University of Antwerp                |
// |                                                  |
// |           Project software engineering           |
// |                   2020-2021                      |
// +--------------------------------------------------+

#ifndef INC_2__PROJECT_UTILS_H
#define INC_2__PROJECT_UTILS_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>


/**
 * @brief Function to make a string
 * @tparam T Type of the value
 * @param value
 * @return string representation of the value
 */
template<typename T>
std::string to_string(const T& value) {
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

/**
 * @brief Function to check if a vector contains a specific element.
 * @tparam T Type of the element
 * @param vector vector in which you want to search
 * @param element element you want to look for
 * @return true if the element is in the vector, false if not
 */
template<typename T>
bool contains(const std::vector<T> &vector, const T& element) {
    typedef typename std::vector<T>::const_iterator vectorIter;
    for (vectorIter iter = vector.begin(); iter != vector.end(); iter++) {
        if (*iter == element) {
            return true;
        }
    }
    return false;
}

/**
 * @brief Function to return a string representation of a vector
 * @tparam T Type of the element
 * @param vector vector containing the elements
 * @return string representation of the vector
 */
template<typename T>
static std::string vectorToString(const std::vector<T> &vector) {
    std::string output = "(";
    if (!vector.empty()) {
        int i = 0;
        while (i < (int) vector.size() - 1) {
            output += to_string(vector[i]) + ", ";
            i++;
        }
        output += to_string(vector[i]);
    }
    output += ")";
    return output;
}

#endif // INC_2__PROJECT_UTILS_H
