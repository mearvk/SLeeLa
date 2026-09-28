#pragma once
#include <cstdint>
#include <string>
namespace sleela::http5{struct Config{std::string address{"127.0.0.1"};std::uint16_t port{8405};std::string log{"http5.log"};bool once{false};};int run(const Config&);}