#pragma once

#include <cstddef>
#include <string>
#include <vector>

namespace sleela::market {

struct Quote {
    std::string ticker;
    double price{};
    double change_percent{};
    double volume{};
};

struct Point {
    double value{};
};

struct Analysis {
    double mean{};
    double min{};
    double max{};
    double change_percent{};
    double volatility{};
    std::string trend;
};

Analysis analyze(const std::vector<Point>& series);
std::vector<std::string> sparkline(const std::vector<Point>& series, std::size_t width = 48);
std::string render_dashboard(const std::vector<Quote>& quotes,
                             const std::vector<Point>& series,
                             const std::string& title = "SLeeLa STOCK MARKET");

} // namespace sleela::market
