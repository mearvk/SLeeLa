#pragma once
#include "decompiler.hpp"
#include <string>
#include <vector>
namespace sleela::decompiler {
struct LibraryNode {
    std::string name;
    std::string soname;
    ArtifactClass artifact_class{ArtifactClass::Unknown};
    std::vector<std::string> needed;
    std::vector<std::string> exports;
};
struct LibraryEdge {
    std::string consumer;
    std::string provider;
    std::string symbol;
};
class LibraryGraph {
public:
    void add(const Artifact& artifact);
    const std::vector<LibraryNode>& nodes() const noexcept;
    const std::vector<LibraryEdge>& edges() const noexcept;
private:
    std::vector<LibraryNode> nodes_;
    std::vector<LibraryEdge> edges_;
};
}
