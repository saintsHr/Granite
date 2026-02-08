#pragma once

#include "granite/window/window.hpp"

namespace gr::Input {

gr::Vec2 GetMousePosition(gr::Window window);
gr::Vec2 GetMouseDelta(gr::Window window);

}