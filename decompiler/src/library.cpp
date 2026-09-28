#include "sleela/decompiler/library.hpp"
#include <algorithm>
namespace sleela::decompiler {
void LibraryGraph::add(const Artifact& a){
    LibraryNode n; n.name=a.name(); n.artifact_class=a.artifact_class(); n.soname=a.library_metadata().soname; n.needed=a.library_metadata().needed_libraries;
    for(const auto& s:a.interfaces().exports)n.exports.push_back(s.name);
    nodes_.push_back(std::move(n));
    for(const auto& imp:a.interfaces().imports){
        for(const auto& provider:nodes_){
            if(provider.name==n.name)continue;
            if(std::find(provider.exports.begin(),provider.exports.end(),imp.name)!=provider.exports.end())
                edges_.push_back({n.name,provider.name,imp.name});
        }
    }
}
const std::vector<LibraryNode>& LibraryGraph::nodes()const noexcept{return nodes_;}
const std::vector<LibraryEdge>& LibraryGraph::edges()const noexcept{return edges_;}
}
