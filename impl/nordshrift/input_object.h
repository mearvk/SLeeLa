#ifndef NORDSHRIFT_INPUT_OBJECT_H
#define NORDSHRIFT_INPUT_OBJECT_H

#include <map>
#include <string>
#include <vector>

namespace nordshrift {

// A generic, bounded input object used to carry SLeeLa subsystem declarations
// through Nordshrift without requiring a new parser construct for every API.
// Categories are deliberately closed at the architecture boundary.
enum class InputObjectCategory {
    Core, IO, System, Network, Application, Data, Science, Security, Deployment
};

struct InputObject {
    InputObjectCategory category = InputObjectCategory::Core;
    std::string identity;
    std::string type;
    std::string source;
    std::string target;
    std::string value;
    std::vector<std::string> inputs;
    std::vector<std::string> outputs;
    std::map<std::string, std::string> properties;
    int line = 0;
};

bool inputObjectCategoryFromName(const std::string& s, InputObjectCategory& out);
const char* inputObjectCategoryName(InputObjectCategory c);
bool inputObjectValidate(const InputObject& object, std::string& error);

} // namespace nordshrift

#endif
