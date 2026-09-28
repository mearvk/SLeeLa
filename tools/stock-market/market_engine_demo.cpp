#include "market_engine.hpp"

#include <iostream>
#include <vector>

int main() {
    using namespace sleela::market;

    const std::vector<Quote> quotes = {
        {"AAPL", 332.27, 1.74, 50716865},
        {"MSFT", 495.63, 0.65, 0},
        {"NVDA", 218.29, -0.03, 0},
        {"TSLA", 365.44, 0.52, 0}
    };

    const std::vector<Point> history = {
        {318.40}, {321.10}, {319.80}, {324.60}, {327.20}, {325.90},
        {329.40}, {331.10}, {328.80}, {332.27}
    };

    std::cout << render_dashboard(quotes, history);
    return 0;
}
