#pragma once
#include <cstdint>
#include <string>
namespace sleela::http8{struct Config{std::string address{"127.0.0.1"};std::uint16_t port{8408};std::string log{"http8.log"};bool once{false};};int run(const Config&);}