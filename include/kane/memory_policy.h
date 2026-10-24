#pragma once

#include <cstdint>

namespace kane::memory {

enum class TabState { Active, Idle, Frozen, Discarded };
enum class Action { Keep, Freeze, Discard };

struct TabInfo {
    std::uint64_t id = 0;
    std::uint64_t idle_seconds = 0;
    std::uint64_t estimated_private_bytes = 0;
    TabState state = TabState::Idle;
    bool audible = false;
    bool capturing = false;
    bool downloading = false;
    bool has_unsaved_form = false;
    bool pinned = false;
};

struct SystemMemory {
    std::uint64_t total_bytes = 0;
    std::uint64_t available_bytes = 0;
};

struct PolicyConfig {
    std::uint64_t freeze_after_seconds = 120;
    std::uint64_t discard_after_seconds = 600;
    double low_memory_available_ratio = 0.15;
};

[[nodiscard]] Action Decide(
    const TabInfo& tab,
    const SystemMemory& memory,
    const PolicyConfig& config = {});

}  // namespace kane::memory
