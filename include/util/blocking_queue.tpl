namespace webserver::util {
    template<typename T>
    void BlockingQueue<T>::Push(const T& data) {
        std::lock_guard<std::mutex> lock(_mutex);
        _queue.push(data);
    }

    template<typename T>
    std::optional<T> BlockingQueue<T>::Pop() {
        std::lock_guard<std::mutex> lock(_mutex);
        if(!_queue.empty()) {
            const auto item = _queue.front();
            _queue.pop();
            return item;
        }
        return {};
    }
}