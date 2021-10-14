cmake_minimum_required(VERSION 3.16)
project(Vaccination_Simulation)

set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_CXX_STANDARD 98)
set(CMAKE_EXE_LINKER_FLAGS -pthread)
set(CMAKE_CXX_FLAGS "-Wall -Werror -pthread")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O3")

# Set include dir
include_directories(gtest/include)

# Set Library dir
link_directories(gtest/lib)

# Elements of the xml_parser
set(xml_parser tinyXML/tinystr.h tinyXML/tinystr.cpp tinyXML/tinyxml.h tinyXML/tinyxml.cpp tinyXML/tinyxmlerror.cpp tinyXML/tinyxmlparser.cpp)

# Elements of the program
set(elements src/Hub.cpp src/Hub.h src/VaccinationCentre.cpp src/VaccinationCentre.h src/Vaccine.cpp src/Vaccine.h src/Inhabitant.cpp src/Inhabitant.h simulation/Simulation.cpp simulation/Simulation.h utilities/Utils.h utilities/Importer.cpp utilities/Importer.h utilities/Utilities.h utilities/Utilities.cpp graphics/GraphicalController.cpp graphics/GraphicalController.h)

# Test Elements
set(test_files testing/HubTest.cpp testing/VaccinationCentreTest.cpp testing/InhabitantTest.cpp testing/VaccineTest.cpp testing/SimulationTest.cpp testing/UtilitiesTest.cpp testing/ImporterTest.cpp testing/GraphicalControllerTest.cpp)

# Create RELEASE target
add_executable(Vaccine-distribution-simulation main.cpp ${xml_parser} ${elements})

# Create DEBUG target
add_executable(Tests ${xml_parser} ${elements} ${test_files})

# Link library
target_link_libraries(Tests gtest)

#add_executable(Vaccination_Simulation main.cpp)