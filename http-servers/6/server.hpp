#pragma once
#include <cstdint>
#include <string>
namespace sleela::http6{struct Config{std::string address{"127.0.0.1"};std::uint16_t port{8406};std::string log{"http6.log"};bool once{false};};int run(const Config&);}