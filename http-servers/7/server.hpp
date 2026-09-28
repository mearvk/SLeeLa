#pragma once
#include <cstdint>
#include <string>
namespace sleela::http7{struct Config{std::string address{"127.0.0.1"};std::uint16_t port{8407};std::string log{"http7.log"};bool once{false};};int run(const Config&);}