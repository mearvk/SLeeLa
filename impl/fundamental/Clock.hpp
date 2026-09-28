#pragma once
#include <string>
#include <utility>
namespace sleela::fundamental {
class Clock {
    std::string value_;
public:
    Clock();
    explicit Clock(std::string value);
    virtual ~Clock() = default;
    const std::string& value() const noexcept;
    void setValue(std::string value);
    bool empty() const noexcept;
    virtual const char* responsibility() const noexcept;
};
}