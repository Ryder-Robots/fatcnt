#include "fatcnt.hpp"

// TODO: move this code to a common location, so that mappers can be added to separate
// repositories for different drones.
/**
 * MIT License
 *
 * Copyright (c) 2024 Ryder Robots
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

using namespace rrobot;

/**
 * @fn main
 * @brief
 * entry point, launches catagorizer (main handler).  Catagorizer launches all other handlers, with configuration 
 * provided from mapper.  Mapper will vary depending on which manifest is given as a command line object, and 
 * provides constants (environment), including specifics on drone hardware.
 * 
 * Mapper will also initilize any handlers needed that are specific to the drone hardware.
 */
int main(int argc, char *argv[]) {

    FatController fatCnt = FatController();

    // Mapper is set here, so that there is an interface to robots sepcific characteritics.
    fatCnt.setMapper(new LdSqu001Mapper());
    fatCnt.run(argc, argv);
    return EXIT_SUCCESS;
}