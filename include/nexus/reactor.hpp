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
