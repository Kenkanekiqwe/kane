#include "kane/memory_policy.h"

#include <cstdlib>
#include <iostream>

using kane::memory::Action;
using kane::memory::Decide;
using kane::memory::PolicyConfig;
using kane::memory::SystemMemory;
using kane::memory::TabInfo;
using kane::memory::TabState;

namespace {
void Check(bool condition, const char* message) {
    if (!condition) {
        std::cerr << "FAILED: " << message << '\n';
        std::exit(EXIT_FAILURE);
    }
}
}

int main() {
    const SystemMemory normal{8ULL * 1024 * 1024 * 1024,
                              4ULL * 1024 * 1024 * 1024};
    const SystemMemory low{8ULL * 1024 * 1024 * 1024,
                           512ULL * 1024 * 1024};
    const PolicyConfig config{120, 600, 0.15};

    TabInfo active;
    active.state = TabState::Active;
    active.idle_seconds = 3600;
    Check(Decide(active, low, config) == Action::Keep,
          "active tab must never be discarded");

    TabInfo audio;
    audio.idle_seconds = 3600;
    audio.audible = true;
    Check(Decide(audio, low, config) == Action::Keep,
          "audible tab must be protected");

    TabInfo idle;
    idle.idle_seconds = 180;
    Check(Decide(idle, normal, config) == Action::Freeze,
          "idle tab should be eligible for freezing");

    idle.idle_seconds = 900;
    Check(Decide(idle, low, config) == Action::Discard,
          "long-idle tab may be discarded under memory pressure");

    TabInfo frozen;
    frozen.state = TabState::Frozen;
    frozen.idle_seconds = 900;
    Check(Decide(frozen, low, config) == Action::Discard,
          "frozen tab may be discarded under memory pressure");

    TabInfo download;
    download.idle_seconds = 3600;
    download.downloading = true;
    Check(Decide(download, low, config) == Action::Keep,
          "downloading tab must be protected");

    std::cout << "All KANE memory policy tests passed.\n";
    return EXIT_SUCCESS;
}
