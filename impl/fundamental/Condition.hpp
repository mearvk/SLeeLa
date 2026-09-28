#pragma once
#include <string>
#include <utility>
namespace sleela::fundamental {
class Condition {
    std::string value_;
public:
    Condition();
    explicit Condition(std::string value);
    virtual ~Condition() = default;
    const std::string& value() const noexcept;
    void setValue(std::string value);
    bool empty() const noexcept;
    virtual const char* responsibility() const noexcept;
};
}