#pragma once

#include <queue>
#include <mutex>
#include <optional>

namespace webserver::util {
    template<typename T>
    class BlockingQueue {
    private:
        std::queue<T> _queue;
        mutable std::mutex _mutex;

    public:
        BlockingQueue(const BlockingQueue&) = delete;
        BlockingQueue& operator=(const BlockingQueue&) = delete;
        BlockingQueue() = default;

        void Push(const T& data);
        std::optional<T> Pop();

        size_t Size() const {
            std::lock_guard<std::mutex> lock(_mutex);
            return _queue.size();
        }

        bool Empty() const {
            std::lock_guard<std::mutex> lock(_mutex);
            return _queue.empty();
        }
    };
}

#include "blocking_queue.tpl"