#pragma once

#include "granite/window/window.hpp"

namespace gr::Input{

void MouseLocked(gr::Window window, bool state);
gr::Vec2 GetMousePosition(gr::Window window);
gr::Vec2 GetMouseDelta(gr::Window window);

}