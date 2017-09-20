#include "sceneselection.hpp"

namespace ee {
using Self = SceneSelection;

Self Self::emptySelection() {
    return Self({}, {});
}

Self Self::rootSelection() {
    return Self({}, {0});
}

Self Self::singleSelection(const QVector<int>& ancestors, int child) {
    return Self(ancestors, {child});
}

Self Self::multipleSelection(const QVector<int>& ancestors,
                             const QVector<int>& children) {
    return Self(ancestors, children);
}

Self::SceneSelection(const QVector<int>& ancestors,
                     const QVector<int>& children)
    : ancestorIndices_(ancestors)
    , childrenIndices_(children) {}

bool Self::isEmpty() const {
    return getAncestorIndices().isEmpty() && getChildrenIndices().isEmpty();
}

const QVector<int>& Self::getAncestorIndices() const {
    return ancestorIndices_;
}

const QVector<int>& Self::getChildrenIndices() const {
    return childrenIndices_;
}
} // namespace ee