# ----------------------------------------------------------------------------
# This is the configuration file for the FRAMEWORK_PV112 module.
# The FRAMEWORK_PV112 module extends the FRAMEWORK_CORE with functionality
# required specifically by PV112 course.
# ----------------------------------------------------------------------------

# Sets the minimum required version of cmake.
cmake_minimum_required(VERSION 3.14)

# Sets the name of the project, and stores it in the variable PROJECT_NAME.
project(FRAMEWORK_PV112 CXX)

# Sets up a new target if it was not already registered.
if(NOT TARGET ${PROJECT_NAME})
    # Adds a new library target.
    add_library(${PROJECT_NAME} STATIC)

    # Sets the build properties.
    # CXX_STANDARD - the C++ standard whose features are requested.
    # CXX_EXTENSIONS - no compiler specific extensions are requested.
    set_target_properties(${PROJECT_NAME} PROPERTIES 
        CXX_STANDARD 20             
        CXX_EXTENSIONS OFF
    )

    # Specifies the include directories to use when compiling the library target defined above.
    target_include_directories(${PROJECT_NAME} PUBLIC include)

    # Collects the source files and specified them as target sources.
    target_sources(${PROJECT_NAME} 
        PRIVATE 
            include/utilities.hpp
            include/pv112_application.hpp
            src/pv112_application.cpp
            src/utilities.cpp
    )

    # Specifies libraries to use when linking the library target defined above.
    target_link_libraries(${PROJECT_NAME}
      PUBLIC
        FRAMEWORK_CORE)
endif()