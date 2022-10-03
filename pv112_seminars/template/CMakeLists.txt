# Sets the name of the project, and stores it in the variable PROJECT_NAME.
project(pv112_template CXX)

# Adds a new executable target.
add_executable(
    ${PROJECT_NAME}
    application.hpp application.cpp main.cpp
)

set_target_properties(
    ${PROJECT_NAME}
    PROPERTIES CXX_STANDARD 20
               CXX_EXTENSIONS OFF
)

# Specifies libraries or flags to use when linking a given target and/or its dependents.
target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE FRAMEWORK_CORE FRAMEWORK_PV112
)

file(
    GENERATE
    OUTPUT "$<TARGET_FILE_DIR:${PROJECT_NAME}>/configuration.toml"
    CONTENT "
framework_shaders = \"${CMAKE_SOURCE_DIR}/framework/core/shaders\"
shaders = \"${CMAKE_CURRENT_SOURCE_DIR}/shaders\"
images = \"${CMAKE_CURRENT_SOURCE_DIR}/images\"
objects = \"${CMAKE_CURRENT_SOURCE_DIR}/objects\"
"
)
