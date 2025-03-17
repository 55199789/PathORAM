#include "external_memory/server/serverBackend.hpp"
// #include "external_memory/server/batchBackend.hpp"

EM::Backend::MemServerBackend* EM::Backend::g_DefaultBackend = nullptr;

struct MemServerInstaller {
  MemServerInstaller() {
    // EM::Backend::g_DefaultBackend = new EM::Backend::MemServerBackend(1<<28);
    constexpr uint64_t log2_st = 33;
    EM::Backend::g_DefaultBackend = new EM::Backend::MemServerBackend((1ll << log2_st) + (1ll << (log2_st - 1)) + (1ll << (log2_st - 2)) + (1ll << (log2_st - 3)) + (1ll << (log2_st - 4)));
  }
};
MemServerInstaller g_MemServerInstaller;
