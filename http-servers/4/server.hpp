#pragma once
#include <cstdint>
#include <string>
namespace sleela::http4{struct Config{std::string address{"127.0.0.1"};std::uint16_t port{8404};std::string log{"http4.log"};bool once{false};};int run(const Config&);}