#include "kane/memory_policy.h"

#include <algorithm>

namespace kane::memory {
namespace {

bool IsProtected(const TabInfo& tab) noexcept {
    return tab.state == TabState::Active ||
           tab.audible || tab.capturing || tab.downloading ||
           tab.has_unsaved_form || tab.pinned;
}

bool IsLowMemory(const SystemMemory& memory,
                 const PolicyConfig& config) noexcept {
    if (memory.total_bytes == 0) return false;
    const auto ratio = static_cast<double>(memory.available_bytes) /
                       static_cast<double>(memory.total_bytes);
    return ratio < std::clamp(config.low_memory_available_ratio, 0.0, 1.0);
}

}  // namespace

Action Decide(const TabInfo& tab,
              const SystemMemory& memory,
              const PolicyConfig& config) {
    if (IsProtected(tab) || tab.state == TabState::Discarded) {
        return Action::Keep;
    }

    const bool low_memory = IsLowMemory(memory, config);
    const auto freeze_after = config.freeze_after_seconds;
    const auto discard_after = std::max(config.discard_after_seconds,
                                         freeze_after);

    if (low_memory && tab.idle_seconds >= discard_after) {
        return Action::Discard;
    }

    if (tab.idle_seconds >= freeze_after && tab.state == TabState::Idle) {
        return Action::Freeze;
    }

    if (low_memory && tab.idle_seconds >= freeze_after &&
        tab.state == TabState::Frozen) {
        return Action::Discard;
    }

    return Action::Keep;
}

}  // namespace kane::memory
