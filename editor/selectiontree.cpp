#include "selectiontree.hpp"
#include "selectionpath.hpp"

namespace ee {
using Self = SelectionTree;

Self Self::emptySelection() {
    return Self({});
}

Self Self::select(const SelectionPath& path) {
    return Self({path});
}

Self Self::select(const std::vector<SelectionPath>& paths) {
    return Self(paths);
}

Self::SelectionTree(const std::vector<SelectionPath>& paths)
    : paths_(paths) {}

bool Self::isEmpty() const {
    return getPaths().empty();
}

std::size_t Self::getSize() const {
    return getPaths().size();
}

const std::vector<SelectionPath>& Self::getPaths() const {
    return paths_;
}

Self& Self::addPath(const SelectionPath& path) {
    paths_.push_back(path);
    return *this;
}
} // namespace ee
