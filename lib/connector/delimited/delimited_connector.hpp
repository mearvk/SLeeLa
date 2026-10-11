#ifndef SLEELA_DELIMITED_CONNECTOR_HPP
#define SLEELA_DELIMITED_CONNECTOR_HPP
#include "delimited_connector.h"
#include <stdexcept>
#include <string>
#include <vector>
namespace sleela::connector { inline std::vector<std::string> parseDelimited(const std::string& line,char delimiter=','){sleela_delimited_row r{};char e[256]{};if(!sleela_delimited_parse(line.c_str(),delimiter,&r,e,sizeof e))throw std::runtime_error(e[0]?e:"record parse failed");std::vector<std::string> v;for(size_t i=0;i<r.count;i++)v.emplace_back(r.fields[i]);sleela_delimited_row_free(&r);return v;} }
#endif
