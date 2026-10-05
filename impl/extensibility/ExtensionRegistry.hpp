#pragma once
#include <string>
#include <unordered_map>
#include "Extension.hpp"
#include "ExtensionDescriptor.hpp"
namespace sleela::extensibility {
// Registers Extensions by id together with their descriptor metadata, and looks
// them up again. Mirrors the inline-in-header style of the peer classes in this
// directory (Extension, ExtensionPoint, ExtensionDescriptor).
class ExtensionRegistry {
    std::unordered_map<std::string, Extension> extensions_;
    std::unordered_map<std::string, ExtensionDescriptor> descriptors_;
public:
    void register_extension(Extension e, ExtensionDescriptor d) {
        const std::string id = e.id();
        descriptors_[id] = std::move(d);
        extensions_.emplace(id, std::move(e));
    }
    bool contains(const std::string& id) const { return extensions_.count(id) != 0; }
    const Extension* find(const std::string& id) const {
        auto it = extensions_.find(id);
        return it == extensions_.end() ? nullptr : &it->second;
    }
    const ExtensionDescriptor* descriptor(const std::string& id) const {
        auto it = descriptors_.find(id);
        return it == descriptors_.end() ? nullptr : &it->second;
    }
    std::size_t size() const noexcept { return extensions_.size(); }
};
}
