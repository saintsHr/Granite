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

#include "granite/core/color.hpp"
#include "granite/core/event.hpp"
#include "granite/core/log.hpp"
#include "granite/core/math.hpp"
#include "granite/core/time.hpp"
#include "granite/core/vector.hpp"

namespace gr::Core {

/**
 * @ingroup Core
 * @brief Initialization Configurations.
 * 
 * Configurations used on initialization of the core module.
 */
struct Config {
    unsigned int depthBits        = 24; ///< Depth buffer bits (Default: 24)
    unsigned int stencilBits      = 8;  ///< Stencil buffer bits (Default: 8)
};

/**
 * @ingroup Core
 * @brief Initializes the engine core.
 *
 * Initializes the engine core and all needed modules with provided configurations.
 *
 * @param cfg Initialization configurations.
 *
 * @warning Should be called before any engine call.
 * @warning After this call, correct function of the engine is not guaranteed if initialization fails.
 *
 * @see init(void)
 */
void init(const Config& cfg);

/**
 * @ingroup Core
 * @brief Initializes the engine core.
 *
 * Initializes the engine core and all needed modules with default configurations.
 *
 * @warning Should be called before any engine call.
 * @warning After this call, correct function of the engine is not guaranteed if initialization fails.
 *
 * @see init(const Config&)
 */
void init(void);

/**
 * @ingroup Core
 * @brief Shuts down the engine core.
 *
 * Shuts down the core and all needed modules.
 *
 * @warning After this call, correct function of the engine is not guaranteed.
 */
void exit(void);

}
