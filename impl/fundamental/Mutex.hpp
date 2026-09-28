#pragma once
#include <string>
#include <utility>
namespace sleela::fundamental {
class Mutex {
    std::string value_;
public:
    Mutex();
    explicit Mutex(std::string value);
    virtual ~Mutex() = default;
    const std::string& value() const noexcept;
    void setValue(std::string value);
    bool empty() const noexcept;
    virtual const char* responsibility() const noexcept;
};
}