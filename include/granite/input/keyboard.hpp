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

#include "granite/window/window.hpp"

namespace gr::Input {

enum class Key {
    A, B, C, D, E,
    F, G, H, I, J,
    K, L, M, N, O,
    P, Q, R, S, T,
    U, V, W, X, Y,
    Z,

    Num0, Num1, Num2,
    Num3, Num4, Num5,
    Num6, Num7, Num8,
    Num9,

    Escape, Enter, Tab,
    Backspace, Space,

    LeftShift,   RightShift,
    LeftControl, RightControl,
    LeftAlt,     RightAlt,

    Up,   Down,
    Left, Right,

    Insert, Delete, Home,
    End,    PageUp, PageDown,

    F1, F2,  F3,  F4,
    F5, F6,  F7,  F8,
    F9, F10, F11, F12,
};

bool keyPressed(gr::Window window, Input::Key key);

}
