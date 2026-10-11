#ifndef SLEELA_XML_CONNECTOR_HPP
#define SLEELA_XML_CONNECTOR_HPP
#include "xml_connector.h"
#include <stdexcept>
#include <string>
namespace sleela::connector {
inline void validateXml(const std::string& text){char e[256]{};if(!sleela_xml_is_well_formed(text.data(),text.size(),e,sizeof e))throw std::runtime_error(e[0]?e:"XML validation failed");}
}
#endif
