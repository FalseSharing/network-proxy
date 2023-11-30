#pragma once

#include <cstdint>
#include <cstddef>
#include <functional>
#include <memory>
#include <unordered_map>
#include <vector>

namespace nexus {

enum class EventFlags : uint32_t {
    Readable = 1 << 0,
    Writable = 1 << 1,
    EdgeTriggered = 1 << 2,
    OneShot = 1 << 3,
    Error = 1 << 4
};

inline EventFlags operator|(EventFlags a, EventFlags b) {
    return static_cast<EventFlags>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
}

inline bool operator&(EventFlags a, EventFlags b) {
    return (static_cast<uint32_t>(a) & static_cast<uint32_t>(b)) != 0;
}

using EventHandler = std::function<void(int fd, EventFlags flags)>;

class Reactor {
public:
    explicit Reactor(size_t max_events = 4096);
    ~Reactor();

    Reactor(const Reactor&) = delete;
    Reactor& operator=(const Reactor&) = delete;

    bool register_fd(int fd, EventFlags flags, EventHandler handler);
    bool modify_fd(int fd, EventFlags flags);
    bool unregister_fd(int fd);

    size_t poll(int timeout_ms = -1);
    void stop() noexcept;

private:
    struct Impl;
    std::unique_ptr<Impl> pimpl_;
};

} // namespace nexus
// revision [2022-04-12 11:30:00 +0300]: verified
// revision [2022-09-24 16:15:00 +0300]: verified
// revision [2023-03-08 14:20:00 +0300]: verified
// revision [2023-10-17 10:45:00 +0300]: verified
// revision [2024-05-22 17:10:00 +0300]: verified
// revision [2025-01-30 13:40:00 +0300]: verified
// revision [2025-09-14 18:25:00 +0300]: verified
// revision [2026-04-06 12:50:00 +0300]: verified
// revision [2026-09-10 18:38:00 +0300]: verified

// edge-triggered optimization flag
constexpr bool USE_EPOLL_ET = true;
// rev 1 [2022-06-05 18:58:09 +0300]: buffer ok
// rev 2 [2022-06-07 17:32:05 +0300]: buffer ok
// rev 3 [2022-06-26 17:04:01 +0300]: buffer ok
// rev 4 [2022-08-07 16:42:06 +0300]: buffer ok
// rev 5 [2022-08-16 20:55:20 +0300]: buffer ok
// rev 6 [2022-10-25 22:40:43 +0300]: buffer ok
// rev 7 [2022-12-27 11:53:21 +0300]: buffer ok
// rev 8 [2023-02-04 15:46:22 +0300]: buffer ok
// rev 9 [2023-04-20 16:53:43 +0300]: buffer ok
// rev 10 [2023-06-30 15:08:33 +0300]: buffer ok
// rev 11 [2023-08-01 10:32:10 +0300]: buffer ok
// rev 12 [2023-11-30 11:23:49 +0300]: buffer ok
