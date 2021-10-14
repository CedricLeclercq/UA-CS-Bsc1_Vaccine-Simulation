// +--------------------------------------------------+
// | Created by Cédric Leclercq and Maarten Peirsman  |
// |                                                  |
// |             University of Antwerp                |
// |                                                  |
// |           Project software engineering           |
// |                   2020-2021                      |
// +--------------------------------------------------+

#include "GraphicalController.h"
#include "../src/Hub.h"

GraphicalController::GraphicalController() : oss() {
    this->_initCheck = this;
    this->currentFigureID = 0;
    folder = "CGI/";
    fileList = new ofstream((folder + "fileList").c_str()); // Make a fileList file to hold all .ini file names
    oss = new ostringstream();
    ENSURE(this->properlyInitialised(), "GraphicalController wasn't properly initialised after its constructor");
}


bool GraphicalController::properlyInitialised() {
    return this == this->_initCheck;
}

void GraphicalController::constructINI(const int slideNr) {
    REQUIRE(this->properlyInitialised(),
           "GraphicalController wasn't properly initialised upon calling one of its member functions");

    // Getting all hubs
    vector<Hub *> hubs = this->allHubs;

    // First calculating the spacing between the hubs and the centres
    int hubSpacing = 2;
    int increaseAmount = Utilities::roundToInt((double) (hubs.size() - (hubs.size() % 5))) / 5;
    hubSpacing += increaseAmount;

    double currentZ = -1.0;
    vector<Hub *>::const_iterator hubIterator; // Creating iterator
    // Looping over all the hubs
    vector<pair<Hub *, vector<double> > > coo;
    vector<Hub *> allHubsCopy = this->getAllHubs();
    for (hubIterator = allHubsCopy.begin(); hubIterator != allHubsCopy.end(); hubIterator++) {
        vector<double> center;
        center.push_back(0);
        center.push_back(-1);
        center.push_back(currentZ);
        double totalInhabitants = (double)(*hubIterator)->getInhabitantsAmount();
        double vaccinatedInhabitants = (double)(*hubIterator)->getVaccinatedAmount();
        double heightFilled = vaccinatedInhabitants / totalInhabitants;
        this->addHub(center, heightFilled);
        vector<double> connectionPoint;
        connectionPoint.push_back(center[0]);
        connectionPoint.push_back(center[1]);
        connectionPoint.push_back(center[2]);
        coo.push_back(make_pair((*hubIterator), connectionPoint));
        currentZ += 2; // 2 because there are mostly more vCentres than hubs. The hubs will be spaced more even
    }
    this->setCoordinatesHubs(coo);
    double currentZCentre = -1.0;


    vector<pair<VaccinationCentre *, vector<double> > > cooCentres;
    vector<VaccinationCentre *>::const_iterator vCentreIterator;
    vector<VaccinationCentre *> allVCentreCopy = this->getAllvCentre();
    for (vCentreIterator = allVCentreCopy.begin(); vCentreIterator != allVCentreCopy.end(); vCentreIterator++) {
        vector<double> center;
        center.push_back(0);
        center.push_back(hubSpacing);
        center.push_back(currentZCentre);
        double heightFilled = (double)(*vCentreIterator)->getAmountOfVaccinatedInhabitants() / (double)(*vCentreIterator)->getAmountOfInhabitants();
        this->addVCentre(center,heightFilled);
        vector<double> connectionPoint;
        connectionPoint.push_back(center[0]);
        connectionPoint.push_back(center[1]);
        connectionPoint.push_back(center[2]);
        if (not contains(cooCentres, make_pair((*vCentreIterator), connectionPoint))) {
            cooCentres.push_back(make_pair((*vCentreIterator), connectionPoint));
        }
        currentZCentre += 1;
    }
    this->setCoordinatesVCentres(cooCentres);
    this->addTransportLines(slideNr);

//    this->fileList->close();

}

void GraphicalController::initialiseFigure() {
    REQUIRE(this->properlyInitialised(),
           "GraphicalController wasn't properly initialised upon calling one of its member functions");
    *oss << "[Figure" << to_string(currentFigureID++) << "]\n";
}

void GraphicalController::drawFigure(const vector<double> &color, const double scale, const vector<int> &rotations,
                                     const vector<double> &center) {
    REQUIRE(this->properlyInitialised(),
           "GraphicalController wasn't properly initialised upon calling one of its member functions");
    *oss << "color = " << vectorToString(color) << "\n";
    *oss << "scale = " << scale << "\n";
    *oss << "rotateX = " << rotations[0] << "\n";
    *oss << "rotateY = " << rotations[1] << "\n";
    *oss << "rotateZ = " << rotations[2] << "\n";
    *oss << "center = " << vectorToString(center) << "\n" << endl;
}

void GraphicalController::drawFilledCube(const vector<double> &color, const double scale, const vector<int> &rotations,
                                         const vector<double> &center, const double height) {
    REQUIRE(this->properlyInitialised(),
           "GraphicalController wasn't properly initialised upon calling one of its member functions");
    initialiseFigure();
    *oss << "class = \"ZBuffering\"\n";
    *oss << "type = \"Rectangular Prism\"\n";
    *oss << "height = " << height << "\n";
    vector<double> adjustedCenter = center;
    adjustedCenter[2] = center[2] + (height - 1)*scale;
    drawFigure(color, scale, rotations, adjustedCenter);
}

void GraphicalController::drawWiredCube(const vector<double> &color, const double scale, const vector<int> &rotations,
                                        const vector<double> &center) {
    REQUIRE(this->properlyInitialised(),
           "GraphicalController wasn't properly initialised upon calling one of its member functions");

    initialiseFigure();
    *oss << "class = \"ZBufferedWireFrame\"\n";
    *oss << "type = \"Cube\"\n";
    drawFigure(color, scale, rotations, center);
}

void GraphicalController::drawWiredCylinder(const vector<double> &color, const double scale, const vector<int> &rotations,
                                            const vector<double> &center, const double height, const double n) {
    REQUIRE(this->properlyInitialised(),
           "GraphicalController wasn't properly initialised upon calling one of its member functions");

    initialiseFigure();
    *oss << "class = \"ZBufferedWireFrame\"\n";
    *oss << "type = \"Cylinder\"\n";
    *oss << "height = " << height << "\n";
    *oss << "n = " << n << "\n";
    drawFigure(color, scale, rotations, center);
}

void GraphicalController::drawFilledCylinder(const vector<double> &color, const double scale, const vector<int> &rotations,
                                             const vector<double> &center, const double height, const double n) {
    REQUIRE(this->properlyInitialised(),
           "GraphicalController wasn't properly initialised upon calling one of its member functions");

    initialiseFigure();
    *oss << "class = \"ZBuffering\"\n";
    *oss << "type = \"Cylinder\"\n";
    *oss << "height = " << height << "\n";
    *oss << "n = " << n << "\n";
    drawFigure(color, scale, rotations, center);
}

void GraphicalController::addHub(const vector<double> &center, double heightFilled) {
    REQUIRE(this->properlyInitialised(),
           "GraphicalController wasn't properly initialised upon calling one of its member functions");
    // Creating the house
    // Create Cube Color
    vector<double> hubColor;
    hubColor.push_back(1);
    hubColor.push_back(0);
    hubColor.push_back(0);
    // Create rotations
    vector<int> hubRotations;
    hubRotations.push_back(0);
    hubRotations.push_back(0);
    hubRotations.push_back(30);
    double cubeScale = 0.2; // Init Scale
    drawWiredCube(hubColor, cubeScale, hubRotations, center);

    // Creating the silo
    double cylinderScale = 0.1;
    // Create cylinderCenter
    vector<double> cylinderCenter;
    cylinderCenter.push_back(center[0] +  0.5);
    cylinderCenter.push_back(center[1] - 0.24);
    cylinderCenter.push_back(center[2]);
    //cylinderCenter[0] += 0.5;
    //cylinderCenter[1] -= 0.24;
    double height = 5;
    int n = 360;

    drawWiredCylinder(hubColor, cylinderScale, hubRotations, cylinderCenter, height, n);
    drawFilledCylinder(hubColor, cylinderScale, hubRotations, cylinderCenter, heightFilled, n);
}

void GraphicalController::addVCentre(const vector<double> &center, double heightFilled) {
    REQUIRE(this->properlyInitialised(),
           "GraphicalController wasn't properly initialised upon calling one of its member functions");
    // Make color
    vector<double> color;
    color.push_back(0);
    color.push_back(1);
    color.push_back(0);
    double scale = 0.2; // Init scale
    // Make rotations
    vector<int> rotations;
    rotations.push_back(0);
    rotations.push_back(0);
    rotations.push_back(30);
    drawWiredCube(color, scale, rotations, center);
    drawFilledCube(color,scale,rotations,center, heightFilled);
}

void GraphicalController::addTransportLines(const int slideNr) {
    REQUIRE(this->properlyInitialised(),
           "GraphicalController wasn't properly initialised upon calling one of its member functions");

    /*
     * Looping over all the hubs and their vCentres. Looping over transport and seeing if a pair exists. If true, then there was
     * something transferred between this hub and vCentre. Draw something on their line. The coordinates of the start and end
     * of this line are in the hub and vCentre iterators (pairs).
     */

    // Preparing for iterating over vCentre coordinates
    vector<pair<VaccinationCentre *, vector<double> > > vCentreCoo = this->getCoordinatesVCentres();
    vector<pair<VaccinationCentre *, vector<double> > >::const_iterator vCentreCooIt;
    // Preparing for iterating over hub coordinates
    vector<pair<Hub *, vector<double> > > hubCoo = this->getCoordinatesHubs();
    vector<pair<Hub *, vector<double> > >::const_iterator hubCooIt;
    // Preparing for iterating over transport
    vector<pair<pair<Hub *, VaccinationCentre *>, int> > transportedToday = this->getTransported();
    vector<pair<pair<Hub *, VaccinationCentre *>, int> >::const_iterator transportedTodayIt;

    for (hubCooIt = hubCoo.begin(); hubCooIt != hubCoo.end(); hubCooIt++) {
        for (vCentreCooIt = vCentreCoo.begin(); vCentreCooIt != vCentreCoo.end(); vCentreCooIt++) {
            for (transportedTodayIt = transportedToday.begin();
                 transportedTodayIt != transportedToday.end(); transportedTodayIt++) {
                if ((*transportedTodayIt).first.first == (*hubCooIt).first and
                    (*transportedTodayIt).first.second == (*vCentreCooIt).first) {
                    this->createLine((*hubCooIt).second, (*vCentreCooIt).second, (*transportedTodayIt).second, slideNr);
                }
            }
        }
    }
}

void GraphicalController::createLine(const vector<double> &pointA, const vector<double> &pointB, const int amount,
                                     const int slideNr) {
    REQUIRE(this->properlyInitialised(),
           "GraphicalController wasn't properly initialised upon calling one of its member functions");

    // Creating the line
    *this->oss << "[Figure" << to_string(this->getCurrentFigureID()) << "]\n"; // Creating the cone
    this->setCurrentFigureID(this->getCurrentFigureID() + 1); // Setting the new figure ID
    *oss << "class = \"Line\"\n";
    *this->oss << "type = \"Line\"\n";
    *this->oss << "color = (0, 0.15, 0.9)\n";
    *this->oss << "pointA = (" << to_string(Utilities::roundToInt(pointA[0])) << ", "
               << to_string(Utilities::roundToInt(pointA[1])) << ", " << to_string((pointA[2]))
               << ")\n";
    *this->oss << "pointB = (" << to_string(Utilities::roundToInt(pointB[0])) << ", "
               << to_string(Utilities::roundToInt(pointB[1])) << ", " << to_string((pointB[2]))
               << ")\n\n";
    // Create color
    vector<double> color;
    color.push_back(1);
    color.push_back(1);
    color.push_back(0);
    double scale = 0.00001*amount; // Create scale
    double height = 0.1; // Create height
    int n = 360; // Create n
    // Create center
    vector<double> center = calculateCylinderCentre(pointA, pointB, slideNr);
    // Create rotations so that we perceive the cylinder as a circle
    vector<int> rotations;
    rotations.push_back(0);
    rotations.push_back(90);
    rotations.push_back(0);
    drawFilledCylinder(color, scale, rotations, center, height, n);
}

vector<double> GraphicalController::calculateCylinderCentre(const vector<double> &pointA, const vector<double> &pointB,
                                                            const int slideNr) {
    // P-value depends on which part of the day it is and determines the position of the cylinder
    double pValue = (double) slideNr / 2.0;
    // P-value is either 0, 0.5 or 1. A p-value of 0 means the center will be equal to p1, 1 means equal to p2, 0.5
    // means the centre will be in the middle of p1 and p2
    vector<double> center;
    center.push_back((1-pValue) * pointA[0] + pValue * pointB[0]); // center[0]
    center.push_back((1-pValue) * pointA[1] + pValue * pointB[1]); // center[1]
    center.push_back((1-pValue) * pointA[2] + pValue * pointB[2]); // center[2]
    return center;
}

void GraphicalController::writeGeneral(int day) {
    REQUIRE(this->properlyInitialised(),
           "GraphicalController wasn't properly initialised upon calling one of its member functions");
    std::string name = "PSE-SIM-DAY-" + to_string(day + 1) + ".";
    for (int partOfDay = 0; partOfDay < 3; partOfDay++) {
        this->constructINI(partOfDay); // Make all figures for this part of the day
        // Construct fileName and General section
        std::string fileName = name + to_string(partOfDay + 1) + ".ini";
        *fileList << fileName << endl;
        ofstream ini;
        ini.open((folder + fileName).c_str());
        ini << "[General]\n";
        ini << "type = \"Combination\"\n";
        ini << "size = 1024\n";
        ini << "backgroundcolor = (0, 0, 0)\n";
        ini << "nrFigures = " << currentFigureID << "\n";
        ini << "eye = (100, 0, 75)\n\n";
        ini << oss->str() << endl;
        oss->str("");
        currentFigureID = 0;
    }
}


int GraphicalController::getCurrentFigureID() const {
    return this->currentFigureID;
}

void GraphicalController::setCurrentFigureID(int id) {
    REQUIRE(id >= 0, "ID must be non-negative and non-zero when setting a new FigureID");
    REQUIRE(this->properlyInitialised(),
           "GraphicalController wasn't properly initialised upon calling one of its member functions");

    this->currentFigureID = id;
    ENSURE(this->currentFigureID == id,"currentFigureID wasn't set properly");
}


vector<Hub *> GraphicalController::getAllHubs() const {
    return this->allHubs;
}

void GraphicalController::setAllHubs(const vector<Hub *> &newAllHubs) {
    REQUIRE(this->properlyInitialised(),
           "GraphicalController wasn't properly initialised upon calling one of its member functions");

    this->allHubs = newAllHubs;
    ENSURE(this->allHubs == newAllHubs,"allHubs wasn't properly set");
}

vector<VaccinationCentre *> GraphicalController::getAllvCentre() const {
    return this->allVaccinationCentre;
}

void GraphicalController::setAllvCentre(const vector<VaccinationCentre *> &newV) {
    REQUIRE(this->properlyInitialised(),
           "GraphicalController wasn't properly initialised upon calling one of its member functions");

    this->allVaccinationCentre = newV;
    ENSURE(this->allVaccinationCentre == newV,"allVaccinationCentre wasn't set properly");
}

vector<pair<VaccinationCentre *, vector<double> > > GraphicalController::getCoordinatesVCentres() const {
    return this->coordinatesVCentres;
}

vector<pair<Hub *, vector<double> > > GraphicalController::getCoordinatesHubs() const {
    return this->coordinatesHubs;
}

void GraphicalController::setCoordinatesVCentres(const vector<pair<VaccinationCentre *, vector<double> > > &newCoo) {
    REQUIRE(this->properlyInitialised(),
           "GraphicalController wasn't properly initialised upon calling one of its member functions");

    this->coordinatesVCentres = newCoo;
    ENSURE(this->coordinatesVCentres == newCoo, "coordinatesCentres wasn't properely set");
}

void GraphicalController::setCoordinatesHubs(const vector<pair<Hub *, vector<double> > > &newCoo) {
    REQUIRE(this->properlyInitialised(),
           "GraphicalController wasn't properly initialised upon calling one of its member functions");

    this->coordinatesHubs = newCoo;
    ENSURE(this->coordinatesHubs == newCoo, "coordinatesHubs wasn't properly set");
}

vector<pair<pair<Hub *, VaccinationCentre *>, int> > GraphicalController::getTransported() const {
    return this->transported;
}

void GraphicalController::setTransported(const vector<pair<pair<Hub *, VaccinationCentre *>, int> > &newTransported) {
    REQUIRE(this->properlyInitialised(),
           "GraphicalController wasn't properly initialised upon calling one of its member functions");
    this->transported = newTransported;
    ENSURE(this->transported==newTransported,"Transported wasn't properly set");
}

void GraphicalController::clearTransported() {
    REQUIRE(this->properlyInitialised(),
           "GraphicalController wasn't properly initialised upon calling one of its member functions");
    this->transported.clear();
}

GraphicalController::~GraphicalController() {
    oss->clear();
    delete oss;
    fileList->close();
    delete fileList;
}
