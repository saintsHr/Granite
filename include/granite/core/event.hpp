/*
MIT License

Copyright (c) 2026 saintsHr

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#pragma once

#include <functional>
#include <vector>
#include <utility>

namespace gr {

class Event {
public:
    template<typename EventType>
    static size_t subscribe(std::function<void(const EventType&)> fn) {
        auto& list = listeners_<EventType>();
        size_t id = nextId_<EventType>()++;

        list.emplace_back(id, fn);
        return id;
    }

    template<typename EventType>
    static void unsubscribe(size_t id) {
        auto& list = listeners_<EventType>();

        for (auto it = list.begin(); it != list.end(); ++it) {
            if (it->first == id) {
                list.erase(it);
                return;
            }
        }
    }

    template<typename EventType>
    static void emit(const EventType& event) {
        auto& list = listeners_<EventType>();

        for (auto& [id, fn] : list) {
            fn(event);
        }
    }
private:
    template<typename EventType>
    static std::vector<std::pair<size_t, std::function<void(const EventType&)>>>& listeners_() {
        static std::vector<std::pair<size_t, std::function<void(const EventType&)>>> list;
        return list;
    }

    template<typename EventType>
    static size_t& nextId_() {
        static size_t id = 0;
        return id;
    }
};

}
