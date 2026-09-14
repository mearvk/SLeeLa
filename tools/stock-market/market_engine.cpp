#include "market_engine.hpp"

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <limits>
#include <sstream>

namespace sleela::market {

Analysis analyze(const std::vector<Point>& series) {
    Analysis out{};
    if (series.empty()) return out;

    double sum = 0.0;
    out.min = std::numeric_limits<double>::infinity();
    out.max = -std::numeric_limits<double>::infinity();
    for (const auto& p : series) {
        sum += p.value;
        out.min = std::min(out.min, p.value);
        out.max = std::max(out.max, p.value);
    }
    out.mean = sum / static_cast<double>(series.size());
    if (series.front().value != 0.0)
        out.change_percent = ((series.back().value - series.front().value) /
                              series.front().value) * 100.0;

    double variance = 0.0;
    for (const auto& p : series) {
        const double d = p.value - out.mean;
        variance += d * d;
    }
    out.volatility = std::sqrt(variance / static_cast<double>(series.size()));
    out.trend = out.change_percent > 0.10 ? "UP" :
                out.change_percent < -0.10 ? "DOWN" : "FLAT";
    return out;
}

std::vector<std::string> sparkline(const std::vector<Point>& series, std::size_t width) {
    static constexpr const char* glyphs[] = {"▁","▂","▃","▄","▅","▆","▇","█"};
    std::vector<std::string> result;
    if (series.empty() || width == 0) return result;
    const std::size_t count = std::min(width, series.size());
    const std::size_t start = series.size() - count;
    double lo = series[start].value, hi = series[start].value;
    for (std::size_t i = start; i < series.size(); ++i) {
        lo = std::min(lo, series[i].value);
        hi = std::max(hi, series[i].value);
    }
    const double span = hi - lo;
    for (std::size_t i = start; i < series.size(); ++i) {
        std::size_t index = span == 0.0 ? 0 :
            static_cast<std::size_t>(((series[i].value - lo) / span) * 7.0);
        if (index > 7) index = 7;
        result.emplace_back(glyphs[index]);
    }
    return result;
}

std::string render_dashboard(const std::vector<Quote>& quotes,
                             const std::vector<Point>& series,
                             const std::string& title) {
    constexpr const char* reset = "\033[0m";
    constexpr const char* cyan = "\033[36m";
    constexpr const char* green = "\033[32m";
    constexpr const char* red = "\033[31m";
    constexpr const char* bold = "\033[1m";
    constexpr const char* dim = "\033[2m";

    std::ostringstream out;
    out << cyan << bold << "╔══════════════════════════════════════════════════════════════╗" << reset << '\n';
    out << cyan << bold << "║ " << reset << std::left << std::setw(59) << title << cyan << bold << "║" << reset << '\n';
    out << cyan << bold << "╚══════════════════════════════════════════════════════════════╝" << reset << "\n\n";
    out << dim << "  WATCHLIST" << reset << "\n";
    out << "  ────────────────────────────────────────────────────────────\n";
    for (const auto& q : quotes) {
        const char* color = q.change_percent >= 0 ? green : red;
        out << "  " << bold << std::left << std::setw(7) << q.ticker << reset
            << std::right << std::setw(11) << std::fixed << std::setprecision(2) << q.price
            << "  " << color << std::showpos << std::setw(7) << std::setprecision(2)
            << q.change_percent << "%" << reset << std::noshowpos << '\n';
    }

    const Analysis a = analyze(series);
    out << "\n" << dim << "  ANALYSIS" << reset << "\n";
    out << "  Trend       : " << bold << a.trend << reset << '\n';
    out << "  Change      : " << std::fixed << std::setprecision(2) << a.change_percent << "%\n";
    out << "  Mean        : " << a.mean << '\n';
    out << "  Range       : " << a.min << " — " << a.max << '\n';
    out << "  Volatility  : " << a.volatility << '\n';
    out << "\n" << dim << "  PRICE GRAPH" << reset << "\n  ";
    for (const auto& g : sparkline(series)) out << g;
    out << "\n";
    return out.str();
}

} // namespace sleela::market
