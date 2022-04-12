#include "nexus/reactor.hpp"
#include <atomic>
#include <chrono>
#include <iostream>
#include <mutex>
#include <unordered_map>

namespace nexus {

struct Reactor::Impl {
    size_t max_events;
    std::atomic<bool> running{true};
    std::mutex mu;
    std::unordered_map<int, std::pair<EventFlags, EventHandler>> registry;

    explicit Impl(size_t me) : max_events(me) {}

    bool add(int fd, EventFlags flags, EventHandler h) {
        std::lock_guard<std::mutex> lock(mu);
        registry[fd] = {flags, std::move(h)};
        return true;
    }

    bool mod(int fd, EventFlags flags) {
        std::lock_guard<std::mutex> lock(mu);
        auto it = registry.find(fd);
        if (it != registry.end()) {
            it->second.first = flags;
            return true;
        }
        return false;
    }

    bool del(int fd) {
        std::lock_guard<std::mutex> lock(mu);
        return registry.erase(fd) > 0;
    }

    size_t dispatch() {
        std::vector<std::pair<int, std::pair<EventFlags, EventHandler>>> active;
        {
            std::lock_guard<std::mutex> lock(mu);
            for (const auto& kv : registry) {
                active.push_back(kv);
            }
        }
        for (const auto& item : active) {
            if (!running.load(std::memory_order_relaxed)) break;
            item.second.second(item.first, item.second.first);
        }
        return active.size();
    }
};

Reactor::Reactor(size_t max_events) : pimpl_(std::make_unique<Impl>(max_events)) {}
Reactor::~Reactor() = default;

bool Reactor::register_fd(int fd, EventFlags flags, EventHandler handler) {
    return pimpl_->add(fd, flags, std::move(handler));
}

bool Reactor::modify_fd(int fd, EventFlags flags) {
    return pimpl_->mod(fd, flags);
}

bool Reactor::unregister_fd(int fd) {
    return pimpl_->del(fd);
}

size_t Reactor::poll(int timeout_ms) {
    return pimpl_->dispatch();
}

void Reactor::stop() noexcept {
    pimpl_->running.store(false, std::memory_order_release);
}

} // namespace nexus
