#pragma once

#include "granite/window/window.hpp"

namespace gr::Input {

enum class Key {
    A = GLFW_KEY_A,
    B = GLFW_KEY_B,
    C = GLFW_KEY_C,
    D = GLFW_KEY_D,
    E = GLFW_KEY_E,
    F = GLFW_KEY_F,
    G = GLFW_KEY_G,
    H = GLFW_KEY_H,
    I = GLFW_KEY_I,
    J = GLFW_KEY_J,
    K = GLFW_KEY_K,
    L = GLFW_KEY_L,
    M = GLFW_KEY_M,
    N = GLFW_KEY_N,
    O = GLFW_KEY_O,
    P = GLFW_KEY_P,
    Q = GLFW_KEY_Q,
    R = GLFW_KEY_R,
    S = GLFW_KEY_S,
    T = GLFW_KEY_T,
    U = GLFW_KEY_U,
    V = GLFW_KEY_V,
    W = GLFW_KEY_W,
    X = GLFW_KEY_X,
    Y = GLFW_KEY_Y,
    Z = GLFW_KEY_Z,

    Num0 = GLFW_KEY_0,
    Num1 = GLFW_KEY_1,
    Num2 = GLFW_KEY_2,
    Num3 = GLFW_KEY_3,
    Num4 = GLFW_KEY_4,
    Num5 = GLFW_KEY_5,
    Num6 = GLFW_KEY_6,
    Num7 = GLFW_KEY_7,
    Num8 = GLFW_KEY_8,
    Num9 = GLFW_KEY_9,

    Escape    = GLFW_KEY_ESCAPE,
    Enter     = GLFW_KEY_ENTER,
    Tab       = GLFW_KEY_TAB,
    Backspace = GLFW_KEY_BACKSPACE,
    Space     = GLFW_KEY_SPACE,

    LeftShift   = GLFW_KEY_LEFT_SHIFT,
    RightShift  = GLFW_KEY_RIGHT_SHIFT,
    LeftControl = GLFW_KEY_LEFT_CONTROL,
    RightControl= GLFW_KEY_RIGHT_CONTROL,
    LeftAlt     = GLFW_KEY_LEFT_ALT,
    RightAlt    = GLFW_KEY_RIGHT_ALT,

    Up    = GLFW_KEY_UP,
    Down  = GLFW_KEY_DOWN,
    Left  = GLFW_KEY_LEFT,
    Right = GLFW_KEY_RIGHT,

    Insert = GLFW_KEY_INSERT,
    Delete = GLFW_KEY_DELETE,
    Home   = GLFW_KEY_HOME,
    End    = GLFW_KEY_END,
    PageUp = GLFW_KEY_PAGE_UP,
    PageDown = GLFW_KEY_PAGE_DOWN,

    F1  = GLFW_KEY_F1,
    F2  = GLFW_KEY_F2,
    F3  = GLFW_KEY_F3,
    F4  = GLFW_KEY_F4,
    F5  = GLFW_KEY_F5,
    F6  = GLFW_KEY_F6,
    F7  = GLFW_KEY_F7,
    F8  = GLFW_KEY_F8,
    F9  = GLFW_KEY_F9,
    F10 = GLFW_KEY_F10,
    F11 = GLFW_KEY_F11,
    F12 = GLFW_KEY_F12
};

bool KeyPressed(gr::Window window, Input::Key key);

}