#include "iapplication.hpp"
#include "utils/configuration.hpp"

// ----------------------------------------------------------------------------
// Constructors
// ----------------------------------------------------------------------------
IApplication::IApplication(int initial_width, int initial_height, std::vector<std::string> arguments)
    : width(initial_width), height(initial_height), arguments(arguments), configuration(Configuration(arguments[0])) {

    framework_shaders_path = configuration.get_path("framework_shaders", "/shaders");
    shaders_path = configuration.get_path("shaders", "/shaders");
}
