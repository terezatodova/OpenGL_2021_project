# ----------------------------------------------------------------------------
# This is the main configuration file for the PV112 seminars.
# ----------------------------------------------------------------------------

# Adds the required parts of the framework.
add_subdirectory(${CMAKE_SOURCE_DIR}/framework/core ${CMAKE_BINARY_DIR}/framework/core)
add_subdirectory(${CMAKE_SOURCE_DIR}/framework/pv112  ${CMAKE_BINARY_DIR}/framework/pv112)

# Adds all required lectures.


add_subdirectory(template)