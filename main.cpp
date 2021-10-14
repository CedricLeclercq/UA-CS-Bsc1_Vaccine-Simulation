/**
 * Main of Vaccine distribution simulation project
 * @authors: Cédric Leclerq (s0200563), Maarten Peirsman (s0207151)
 * @date: 25/02/2021
 * @version: 1.0
 */

#include "utilities/Importer.h"

#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char** argv) {
    if (argc < 2 || argc > 4) {
        cerr << "Executable takes one to three arguments: input file and (optional) number of days to run the simulation and output file";
        return 666;
    }
    string inputFile = argv[1];
    string error;
    int time = 200; // Default time set to 200 days
    if (argc > 2) {
        time = atoi(argv[2]);
    }
    if (argc < 4) {
        error = "cout";
    } else {
        error = argv[3];
    }

    // Initialise Success variable to determine the state of the import
    Success success;

    ofstream output;

    // Import simulation from an xml file
    Simulation * simulation = new Simulation;
    if (error == "cout") {
        success = Importer::importSimulation(inputFile, cout, *simulation);
    } else {
        output.open(error.c_str());
        success = Importer::importSimulation(inputFile, output, *simulation);
    }
    // Check state of the import
    if (success == Unsuccessful) {
        // The import was not successful, exit code 69
        return 69;
    } else if (success == PartiallyCompleted) {
        // The import was not success enough. The simulation may have been altered, however. Exit code 420
        return 420;
    }
    //simulation->setVCentres()
    simulation->setVCentres(Utilities::getAllVCentres(simulation->getHubs()));
    if (error == "cout") {
        simulation->autoSimulation(time, cout);
    } else {
        simulation->autoSimulation(time, output);
    }
    delete simulation;

    return 0;
}
