#pragma once
#include <cstdint>
#include <string>
namespace sleela::http9{struct Config{std::string address{"127.0.0.1"};std::uint16_t port{8409};std::string log{"http9.log"};bool once{false};};int run(const Config&);}