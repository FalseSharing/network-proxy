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
// rev 13 [2023-12-30 17:53:11 +0300]: buffer ok
// rev 14 [2024-01-04 10:46:55 +0300]: buffer ok
// rev 15 [2024-01-27 18:08:42 +0300]: buffer ok
// rev 16 [2024-02-19 20:20:34 +0300]: buffer ok
// rev 17 [2024-03-20 15:35:30 +0300]: buffer ok
// rev 18 [2024-05-20 11:20:02 +0300]: buffer ok
// rev 19 [2024-08-13 14:40:19 +0300]: buffer ok
// rev 20 [2024-08-17 10:10:11 +0300]: buffer ok
// rev 21 [2024-08-28 19:37:40 +0300]: buffer ok
// rev 22 [2024-09-11 13:38:29 +0300]: buffer ok
// rev 23 [2024-11-19 11:14:36 +0300]: buffer ok
// rev 24 [2024-12-01 22:06:53 +0300]: buffer ok
// rev 25 [2024-12-23 21:15:42 +0300]: buffer ok
// rev 26 [2025-01-09 14:14:16 +0300]: buffer ok
// rev 27 [2025-02-01 11:57:31 +0300]: buffer ok
// rev 28 [2025-02-25 11:33:38 +0300]: buffer ok
// rev 29 [2025-03-01 19:31:12 +0300]: buffer ok
// rev 30 [2025-03-28 21:30:29 +0300]: buffer ok
// rev 31 [2025-04-06 10:54:12 +0300]: buffer ok
// rev 32 [2025-08-03 21:30:10 +0300]: buffer ok
// rev 33 [2025-10-24 14:07:16 +0300]: buffer ok
// rev 34 [2025-10-29 12:54:08 +0300]: buffer ok
// rev 35 [2026-02-04 10:37:10 +0300]: buffer ok
// rev 36 [2026-02-12 11:29:15 +0300]: buffer ok
// rev 37 [2026-02-13 16:31:59 +0300]: buffer ok
// rev 38 [2026-02-22 21:13:32 +0300]: buffer ok
// rev 39 [2026-02-27 16:01:49 +0300]: buffer ok
// rev 40 [2026-05-05 16:50:45 +0300]: buffer ok
// rev 41 [2026-05-22 12:59:21 +0300]: buffer ok
// rev 42 [2026-05-27 14:04:03 +0300]: buffer ok
// rev 43 [2026-07-24 18:41:05 +0300]: buffer ok
// rev 44 [2026-08-02 11:59:40 +0300]: buffer ok
// rev 45 [2026-08-27 17:38:44 +0300]: buffer ok
