#pragma once

#include <string>
#include <iostream>

namespace ORDO {
    namespace Inputs {

        void error_input_check(std::istream& stream);;
        void error_range_check(int checked, unsigned int min, unsigned int max);

        std::string InputStr();
        int InputInt();
        int InputRange(unsigned int min, unsigned int max);

        std::string InputStr(const std::string& msg);
        int InputInt(const std::string& msg);
        int InputRange(unsigned int min, unsigned int max, const std::string& msg);
    
    }
}