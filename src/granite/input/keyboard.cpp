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

#include "granite/input/keyboard.hpp"
#include <GLFW/glfw3.h>

#include "granite/input/input.hpp"

namespace gr::Input{

static int to_glfw(gr::Input::Key key) {
    switch (key) {
        case Key::A: return GLFW_KEY_A;
        case Key::B: return GLFW_KEY_B;
        case Key::C: return GLFW_KEY_C;
        case Key::D: return GLFW_KEY_D;
        case Key::E: return GLFW_KEY_E;
        case Key::F: return GLFW_KEY_F;
        case Key::G: return GLFW_KEY_G;
        case Key::H: return GLFW_KEY_H;
        case Key::I: return GLFW_KEY_I;
        case Key::J: return GLFW_KEY_J;
        case Key::K: return GLFW_KEY_K;
        case Key::L: return GLFW_KEY_L;
        case Key::M: return GLFW_KEY_M;
        case Key::N: return GLFW_KEY_N;
        case Key::O: return GLFW_KEY_O;
        case Key::P: return GLFW_KEY_P;
        case Key::Q: return GLFW_KEY_Q;
        case Key::R: return GLFW_KEY_R;
        case Key::S: return GLFW_KEY_S;
        case Key::T: return GLFW_KEY_T;
        case Key::U: return GLFW_KEY_U;
        case Key::V: return GLFW_KEY_V;
        case Key::W: return GLFW_KEY_W;
        case Key::X: return GLFW_KEY_X;
        case Key::Y: return GLFW_KEY_Y;
        case Key::Z: return GLFW_KEY_Z;

        case Key::Num0: return GLFW_KEY_0;
        case Key::Num1: return GLFW_KEY_1;
        case Key::Num2: return GLFW_KEY_2;
        case Key::Num3: return GLFW_KEY_3;
        case Key::Num4: return GLFW_KEY_4;
        case Key::Num5: return GLFW_KEY_5;
        case Key::Num6: return GLFW_KEY_6;
        case Key::Num7: return GLFW_KEY_7;
        case Key::Num8: return GLFW_KEY_8;
        case Key::Num9: return GLFW_KEY_9;

        case Key::Escape:    return GLFW_KEY_ESCAPE;
        case Key::Enter:     return GLFW_KEY_ENTER;
        case Key::Tab:       return GLFW_KEY_TAB;
        case Key::Backspace: return GLFW_KEY_BACKSPACE;
        case Key::Space:     return GLFW_KEY_SPACE;

        case Key::LeftShift:    return GLFW_KEY_LEFT_SHIFT;
        case Key::RightShift:   return GLFW_KEY_RIGHT_SHIFT;
        case Key::LeftControl:  return GLFW_KEY_LEFT_CONTROL;
        case Key::RightControl: return GLFW_KEY_RIGHT_CONTROL;
        case Key::LeftAlt:      return GLFW_KEY_LEFT_ALT;
        case Key::RightAlt:     return GLFW_KEY_RIGHT_ALT;

        case Key::Up:    return GLFW_KEY_UP;
        case Key::Down:  return GLFW_KEY_DOWN;
        case Key::Left:  return GLFW_KEY_LEFT;
        case Key::Right: return GLFW_KEY_RIGHT;

        case Key::Insert:   return GLFW_KEY_INSERT;
        case Key::Delete:   return GLFW_KEY_DELETE;
        case Key::Home:     return GLFW_KEY_HOME;
        case Key::End:      return GLFW_KEY_END;
        case Key::PageUp:   return GLFW_KEY_PAGE_UP;
        case Key::PageDown: return GLFW_KEY_PAGE_DOWN;

        case Key::F1:  return GLFW_KEY_F1;
        case Key::F2:  return GLFW_KEY_F2;
        case Key::F3:  return GLFW_KEY_F3;
        case Key::F4:  return GLFW_KEY_F4;
        case Key::F5:  return GLFW_KEY_F5;
        case Key::F6:  return GLFW_KEY_F6;
        case Key::F7:  return GLFW_KEY_F7;
        case Key::F8:  return GLFW_KEY_F8;
        case Key::F9:  return GLFW_KEY_F9;
        case Key::F10: return GLFW_KEY_F10;
        case Key::F11: return GLFW_KEY_F11;
        case Key::F12: return GLFW_KEY_F12;
    }

    return GLFW_KEY_UNKNOWN;
}

bool keyPressed(gr::Window window, Input::Key key){
    if (glfwGetKey(window.getRaw(), to_glfw(key)) == GLFW_PRESS){
        return true;
    } else {
        return false;
    }
}

}
