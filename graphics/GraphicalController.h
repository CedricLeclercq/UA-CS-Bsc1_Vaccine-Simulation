// +--------------------------------------------------+
// | Created by Cédric Leclercq and Maarten Peirsman  |
// |                                                  |
// |             University of Antwerp                |
// |                                                  |
// |           Project software engineering           |
// |                   2020-2021                      |
// +--------------------------------------------------+

#ifndef VACCINATION_SIMULATION_GRAPHICALCONTROLLER_H
#define VACCINATION_SIMULATION_GRAPHICALCONTROLLER_H

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include "../utilities/DesignByContract.h"
#include "../utilities/Utils.h"
#include "../utilities/Utilities.h"

using namespace std;

class Hub;
class VaccinationCentre;
class Vaccine;
class Inhabitant;


class GraphicalController {

    vector<Hub*> allHubs; // Vector with all the hubs
    vector<VaccinationCentre*> allVaccinationCentre; // Vector with all the vCentres (with no duplicates!)

    std::string folder;

    GraphicalController* _initCheck; // Pointer to the object - to check correct initialisation
    ofstream * fileList; // All the .ini files
    ostringstream * oss; // The .ini that is currently being used
    int currentFigureID; // The id of the figure that is currently being written in the .ini

    // Vector with a pair of a vCentre and its center on the .ini
    vector<pair<VaccinationCentre*, vector<double> > > coordinatesVCentres;
    // Vector with a pair of a Hub and its center on the .ini
    vector<pair<Hub*, vector<double> > > coordinatesHubs;
    // Vector with from which Hub to which VCentre what amount of vaccines is transported
    vector<pair<pair<Hub*,VaccinationCentre* >, int > > transported;


public:
    /**
     * Default constructor
     * @PostCondition     ENSURE(this->properlyInitialised(), "GraphicalController wasn't properly initialised after its constructor");
     */
    GraphicalController();

    /**
     * Destructor
     */
    ~GraphicalController();

    /**
     * @brief Checks if the object is properly initialised
     * @return true
     */
    bool properlyInitialised();

    /// Getter for allHubs
    vector<Hub*> getAllHubs() const ;

    /// Getter for allVaccinationCentre
    vector<VaccinationCentre*> getAllvCentre() const;

    /**
     * @brief Setter for the parameter allHubs
     * @param newAllHubs
     * @PreCondition     REQUIRE(this->properlyInitialised(), "GraphicalController wasn't properly initialised upon calling one of its member functions");
     */
    void setAllHubs(const vector<Hub*>& newAllHubs);

    /**
     * @brief Setter for the parameter allVaccinationCentre
     * @param newVCentre
     * @PreCondition     REQUIRE(this->properlyInitialised(), "GraphicalController wasn't properly initialised upon calling one of its member functions");
     */
    void setAllvCentre(const vector<VaccinationCentre*>& newVCentre);

    /**
     * @brief Function to write the General section to the ini file
     * @param day The day the graphics are representing
     * @PreCondition     REQUIRE(this->properlyInitialised(), "GraphicalController wasn't properly initialised upon calling one of its member functions");
     */
    void writeGeneral(int day);

    /**
     * @brief Function to write the Figure header to the ini file
     * @PreCondition     REQUIRE(this->properlyInitialised(), "GraphicalController wasn't properly initialised upon calling one of its member functions");
     */
    void initialiseFigure();

    /// Getter for currentFigureID
    int getCurrentFigureID() const;

    /**
     * @brief Setter for the parameter currentFigureID
     * @param id
     * @PreCondition     REQUIRE(this->properlyInitialised(), "GraphicalController wasn't properly initialised upon calling one of its member functions");
     * @PreCondition     REQUIRE(id >= 0, "ID must be non-negative and non-zero when setting a new FigureID");
     */
    void setCurrentFigureID(int id);

    /**
     * @brief Coordinates the functions that write on the .ini file
     * @param slideNr The number of the slide that is being represented
     * @PreCondition     REQUIRE(this->properlyInitialised(), "GraphicalController wasn't properly initialised upon calling one of its member functions");
     */
    void constructINI(const int slideNr);

    /**
     * @brief Adds a Hub to the .ini
     * @param center    Location of the Hub
     * @PreCondition     REQUIRE(this->properlyInitialised(), "GraphicalController wasn't properly initialised upon calling one of its member functions");
     */
    void addHub(const vector<double>& center, double heightFilled);

    /**
     * @brief Adds a VCentre to the .ini
     * @param center
     * @PreCondition     REQUIRE(this->properlyInitialised(), "GraphicalController wasn't properly initialised upon calling one of its member functions");
     */
    void addVCentre(const vector<double>& center, double heightFilled);


    /// Getter for the parameter coordinatesVCentres
    vector<pair<VaccinationCentre*, vector<double> > > getCoordinatesVCentres() const;

    /// Getter for the parameter coordinatesHubs
    vector<pair<Hub*, vector<double> > > getCoordinatesHubs() const ;

    /**
     * @brief Setter for the parameter coordinatesVCentres
     * @param newCoo
     * @PreCondition     REQUIRE(this->properlyInitialised(), "GraphicalController wasn't properly initialised upon calling one of its member functions");
     */
    void setCoordinatesVCentres(const vector<pair<VaccinationCentre*, vector<double> > >& newCoo);

    /**
     * @brief Setter for the parameter coordinatesHub
     * @param newCoo
     * @PreCondition     REQUIRE(this->properlyInitialised(), "GraphicalController wasn't properly initialised upon calling one of its member functions");
     */
    void setCoordinatesHubs(const vector<pair<Hub*, vector<double> > >& newCoo);

    /**
     * @brief Draws the transportation lines on the .ini file. Makes use of this->transported to do this.
     * @param slideNr The number of the slide that is being represented
     * @PreCondition     REQUIRE(this->properlyInitialised(), "GraphicalController wasn't properly initialised upon calling one of its member functions");
     */
    void addTransportLines(const int slideNr);

    /// Getter for the parameter transported
    vector<pair<pair<Hub*,VaccinationCentre* >, int > > getTransported() const ;

    /**
     * @brief Setter for the parameter transported
     * @param newTransported
     * @PreCondition     REQUIRE(this->properlyInitialised(), "GraphicalController wasn't properly initialised upon calling one of its member functions");
     */
    void setTransported(const vector<pair<pair<Hub*,VaccinationCentre* >, int > >& newTransported);

    /**
     * @brief Clears the parameter transported
     * @PreCondition     REQUIRE(this->properlyInitialised(), "GraphicalController wasn't properly initialised upon calling one of its member functions");
     */
    void clearTransported();

    /**
     * @brief Draws a line from centreA to centreB. Will also draw a sphere like object that indicates the amount of
     * vaccines transferred.
     * @param centerA
     * @param centerB
     * @param amount
     * @param slideNr The part of the day that is being represented
     * @PreCondition     REQUIRE(this->properlyInitialised(), "GraphicalController wasn't properly initialised upon calling one of its member functions");
     */
    void createLine(const vector<double>& centerA, const vector<double>& centerB, const int amount, const int slideNr);

    /**
     * @brief Function for writing the basic figure attributes to the ini file
     * @param color Color of the figure
     * @param scale Scale of the figure
     * @param rotations Vector containing rotX, rotY and rotZ respectively
     * @param center Vector containing x, y and z coordinates of the center
     * @PreCondition     REQUIRE(this->properlyInitialised(), "GraphicalController wasn't properly initialised upon calling one of its member functions");
     */
    void drawFigure(const vector<double> &color, double scale, const vector<int> &rotations,
                    const vector<double> &center);

    /**
     * @brief Function for drawing a filled in cube
     * @param color Color of the cube
     * @param scale Scale of the cube
     * @param rotations Vector containing rotX, rotY and rotZ respectively
     * @param center Vector containing x, y and z coordinates of the center
     * @param height The height that needs to be filled in (double between 0 and 1 )
     * @PreCondition     REQUIRE(this->properlyInitialised(), "GraphicalController wasn't properly initialised upon calling one of its member functions");
     */
    void drawFilledCube(const vector<double> &color, double scale, const vector<int> &rotations,
                        const vector<double> &center, double height);
    /**
     * @brief Function for drawing a wired cube
     * @param color Color of the cube
     * @param scale Scale of the cube
     * @param rotations Vector containing rotX, rotY and rotZ respectively
     * @param center Vector containing x, y and z coordinates of the center
     * @PreCondition     REQUIRE(this->properlyInitialised(), "GraphicalController wasn't properly initialised upon calling one of its member functions");
     */
    void drawWiredCube(const vector<double> &color, double scale, const vector<int> &rotations,
                       const vector<double> &center);

    /**
     * @brief Function for drawing a wired cylinder
     * @param color Color of the cylinder
     * @param scale Scale of the cylinder
     * @param rotations Vector containing rotX, rotY and rotZ respectively
     * @param center Vector containing x, y and z coordinates of the center
     * @param height Height of the cylinder
     * @param n Amount of sides the make up the cylinder (higher = rounder)
     * @PreCondition     REQUIRE(this->properlyInitialised(), "GraphicalController wasn't properly initialised upon calling one of its member functions");
     */
    void drawWiredCylinder(const vector<double> &color, double scale, const vector<int> &rotations,
                           const vector<double> &center, double height, double n);

    /**
     * @brief Function for drawing a filled in cylinder
     * @param color Color of the cylinder
     * @param scale Scale of the cylinder
     * @param rotations Vector containing rotX, rotY and rotZ respectively
     * @param center Vector containing x, y and z coordinates of the center
     * @param height Height of the cylinder
     * @param n Amount of sides the make up the cylinder (higher = rounder)
     * @PreCondition     REQUIRE(this->properlyInitialised(), "GraphicalController wasn't properly initialised upon calling one of its member functions");
     */
    void drawFilledCylinder(const vector<double> &color, double scale, const vector<int> &rotations,
                            const vector<double> &center, double height, double n);

    /**
     * @brief Function for calculating the position of a cylinder based on the slide and the line it is on
     * @param pointA
     * @param pointB
     * @param slideNr
     * @return
     */
    static vector<double> calculateCylinderCentre(const vector<double> &pointA, const vector<double> &pointB,
                                                  const int slideNr);
};


#endif //VACCINATION_SIMULATION_GRAPHICALCONTROLLER_H
