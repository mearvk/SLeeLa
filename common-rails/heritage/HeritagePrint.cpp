/*
 * CommonRails Heritage Print — C++ implementation.
 * Canonical Beautiful Design: fixed 80-column printing.
 */
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

namespace commonrails::heritage {
constexpr std::size_t PRINT_WIDTH = 80;
constexpr int SQUARE_SIZE = 21;

void printWidthLine(const std::string& content) {
    std::size_t pos = 0;
    while (pos < content.size()) {
        std::size_t remaining = content.size() - pos;
        std::size_t take = std::min(remaining, PRINT_WIDTH);
        if (remaining > PRINT_WIDTH) {
            std::size_t cut = take;
            while (cut > 0 && content[pos + cut] != ' ' &&
                   content[pos + cut - 1] != ' ') --cut;
            if (cut > 0) take = cut;
        }
        std::cout << content.substr(pos, take);
        if (take < PRINT_WIDTH) std::cout << std::string(PRINT_WIDTH - take, ' ');
        std::cout << '\n';
        pos += take;
        while (pos < content.size() && content[pos] == ' ') ++pos;
    }
}

void printField(const std::string& content, std::size_t fieldWidth) {
    std::cout << content;
    if (content.size() < fieldWidth)
        std::cout << std::string(fieldWidth - content.size(), ' ');
}

void printComponent(const std::string& name, unsigned long objectId,
                    unsigned long date, const std::string& message) {
    std::ostringstream line;
    line << "-- : [Object ID: " << std::setw(10) << std::setfill('0') << objectId
         << "] [Date: " << date << "] [Current: @" << name << "] . "
         << message << " .";
    printWidthLine(line.str());
}

void printSquare(int filled) {
    const int total = SQUARE_SIZE * SQUARE_SIZE;
    filled = std::clamp(filled, 0, total);
    for (int row = 0; row < SQUARE_SIZE; ++row) {
        for (int col = 0; col < SQUARE_SIZE; ++col) {
            int fillRow = (SQUARE_SIZE - 1) - row;
            int fillCol = (SQUARE_SIZE - 1) - col;
            int fillIndex = fillRow * SQUARE_SIZE + fillCol;
            std::cout << (fillIndex < filled ? "█" : "░");
        }
        std::cout << '\n';
    }
}

void printProgress(int percent) {
    percent = std::clamp(percent, 0, 100);
    const int cells = (percent * SQUARE_SIZE * SQUARE_SIZE) / 100;
    std::ostringstream line;
    line << "  progress " << percent << "% (" << cells << "/"
         << SQUARE_SIZE * SQUARE_SIZE << " cells)";
    printWidthLine(line.str());
    printSquare(cells);
}

} // namespace commonrails::heritage

#ifdef HERITAGE_PRINT_DEMO
int main() {
    using namespace commonrails::heritage;
    printComponent("CommonRails", 1234, 1, "printing initialized");
    printWidthLine("[START]   CommonRails Heritage printing initialized");
    printWidthLine("[WORKING] Content remains inside the 80-column contract");
    printWidthLine("[COMPLETE] Fixed-width output ready");
    printProgress(50);
}
#endif
