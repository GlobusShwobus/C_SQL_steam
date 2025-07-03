#pragma once

#include <string>
#include <iostream>

namespace ORDO {
    namespace Inputs {
        static void error_input_check(std::istream& stream);;
        static void error_range_check(int checked, unsigned int min, unsigned int max);

        static std::string InputStr();
        static int InputInt();
        static int InputRange(unsigned int min, unsigned int max);

        static std::string InputStr(const std::string& msg);
        static int InputInt(const std::string& msg);
        static int InputRange(unsigned int min, unsigned int max, const std::string& msg);
    }
}
