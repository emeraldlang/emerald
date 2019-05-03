/*  Emerald - Procedural and Object Oriented Programming Language
**  Copyright (C) 2018  Zach Perkitny
**
**  This program is free software: you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef _EMERALD_BLOCKING_QUEUE_H
#define _EMERALD_BLOCKING_QUEUE_H

#include <chrono>
#include <condition_variable>
#include <mutex>
#include <queue>

namespace emerald {

    template <class T, class Queue = std::queue<T>> 
    class BlockingQueue {
    public:
        BlockingQueue() = default;

        void push(const T& item);
        void push(T&& item);

        void pop(T& item);

        template <class Clock, class Duration>
        bool pop(T& item, const std::chrono::time_point<Clock, Duration> timeout_time);

    private:
        std::queue<T> _queue;
        std::mutex _mutex;
        std::condition_variable _cv;
    };

    template <class T, class Queue>
    void BlockingQueue<T, Queue>::push(const T& item) {
        {
            std::lock_guard<std::mutex> lock(_mutex);
            _queue.push(item);
        }

        _cv.notify_one();
    }

    template <class T, class Queue> 
    void BlockingQueue<T, Queue>::push(T&& item) {
        {
            std::lock_guard<std::mutex> lock(_mutex);
            _queue.push(std::move(item));
        }

        _cv.notify_one();
    }

    template <class T, class Queue> 
    void BlockingQueue<T, Queue>::pop(T& item) {
        std::unique_lock<std::mutex> lock(_mutex);
        while (_queue.empty()) _cv.wait(lock);

        item = _queue.front();
        _queue.pop();
    }

    template <class T, class Queue>
    template<class Clock, class Duration>
    bool BlockingQueue<T, Queue>::pop(T& item, const std::chrono::time_point<Clock, Duration> timeout_time) {
        std::unique_lock<std::mutex> lock(_mutex);
        while (_queue.empty()) {
            if (_cv.wait_until(lock, timeout_time) == std::cv_status::timeout) 
                return false;
        }

        item = _queue.front();
        _queue.pop();

        return true;
    }

} // namespace emerald

#endif // _EMERALD_BLOCKING_QUEUE_H
