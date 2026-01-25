#pragma once

#include "granite/core/time.hpp"

#include <cstdio>
#include <cstdlib>
#include <cstdarg>

namespace gr::internal{

enum class Severity{
    INFO,
    WARNING,
    ERROR,
    FATAL
};

enum class Module{
    CORE,
    INPUT,
    RENDER,
    SCENE,
    WINDOW
};

const char* SeverityString[] = { "INFO", "WARNING", "ERROR", "FATAL" };
const char* SeverityColor[]  = { "\033[96m", "\033[95m", "\033[91m", "\033[31m" };
const char* ModuleString[]   = { "Core", "Input", "Render", "Scene", "Window" };

void Log(Severity sev, Module mod, const char* fmt, ...){
    double t = gr::Time::GetElapsedTimeS();

    int h = static_cast<int>(t / 3600);
    int m = static_cast<int>((t / 60)) % 60;
    int s = static_cast<int>(t) % 60;
    int ms = static_cast<int>((t - static_cast<int>(t)) * 1000);

    std::printf(
        "[%02d:%02d:%02d.%03d][%s%s\033[0m][%s] ",
        h, m, s, ms,
        SeverityColor[static_cast<int>(sev)],
        SeverityString[static_cast<int>(sev)],
        ModuleString[static_cast<int>(mod)]
    );

    va_list args;
    va_start(args, fmt);
    std::vprintf(fmt, args);
    va_end(args);

    std::printf("\n");
    std::fflush(stdout);

    if (sev == Severity::FATAL) std::exit(EXIT_FAILURE);
}

}