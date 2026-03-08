/*
MIT License

Copyright (c) 2026 Henrique Rodrigues Santos

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

#include "granite/core/time.hpp"

#include <format>
#include <iostream>
#include <cstdlib>

namespace gr::internal {

enum class Severity {
    INFO,
    WARNING,
    ERROR,
    FATAL
};

enum class Module {
    CORE,
    INPUT,
    RENDER,
    SCENE,
    WINDOW,
    ASSETS
};

inline constexpr std::string_view SeverityString[] = {
    "INFO", "WARNING", "ERROR", "FATAL"
};

inline constexpr std::string_view SeverityColor[] = {
    "\033[96m", "\033[95m", "\033[91m", "\033[31m"
};

inline constexpr std::string_view ModuleString[] = {
    "Core", "Input", "Render", "Scene", "Window", "Assets"
};

template<typename... Args>
inline void log(
    Severity sev,
    Module mod,
    std::format_string<Args...> fmt,
    Args&&... args)
{
    const float t = gr::Time::GetElapsedTimeS();

    const int h  = static_cast<int>(t / 3600.0f);
    const int m  = static_cast<int>(t / 60.0f) % 60;
    const int s  = static_cast<int>(t) % 60;
    const int ms = static_cast<int>((t - static_cast<float>(static_cast<int>(t))) * 1000.0f);

    std::string message = std::format(fmt, std::forward<Args>(args)...);

    std::string header = std::format(
        "[{:02}:{:02}:{:02}.{:03}][{}{}\033[0m][{}] ",
        h, m, s, ms,
        SeverityColor[static_cast<int>(sev)],
        SeverityString[static_cast<int>(sev)],
        ModuleString[static_cast<int>(mod)]
    );

    std::cout << header << message << '\n';

    if (sev == Severity::FATAL)
        std::exit(EXIT_FAILURE);
}

}