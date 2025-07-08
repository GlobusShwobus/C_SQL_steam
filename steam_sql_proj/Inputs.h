#pragma once

#include <string>
#include <iostream>

namespace ORDO {
    namespace Inputs {

        void error_input_check(std::istream& stream);;
        void error_range_check(int num, unsigned int min, unsigned int max);
        void clearInputBuffer();

        std::string InputStr();
        int InputInt();
        float InputFloat();
        int InputRangeInt(unsigned int min, unsigned int max);

        //DEPRICATED, DELETE LATER, DON'T WRITE NEW SCRIPTS WITH THESE
        std::string InputStr(const std::string& str);
        int InputInt(const std::string& str);
        int InputRange(unsigned int min, unsigned int max, const std::string& str);
        ////////////////////////////
    }
}