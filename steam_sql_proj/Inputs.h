#pragma once

#include <string>
#include <iostream>

namespace ORDO {

    static void error_input_check(std::istream& stream) {
        if (stream.fail()) {
            stream.clear();
            stream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            throw std::runtime_error("invalid input");
        }
    }
    static void error_range_check(int checked, unsigned int min, unsigned int max) {
        if (checked < min || checked > max) {
            throw std::runtime_error("out of range input");
        }
    }

    static std::string InputStr() {
        std::string str;
        std::cin >> str;
        error_input_check(std::cin);
        return str;
    }
    static int InputInt() {
        int n = 0;
        std::cin >> n;
        error_input_check(std::cin);

        return n;
    }
    static int InputRange(unsigned int min, unsigned int max) {
        int n = 0;
        std::cin >> n;
        error_input_check(std::cin);
        error_range_check(n, min, max);
        return n;
    }
    static std::string InputStr(const std::string& msg) {
        std::cout << msg;
        return InputStr();
    }
    static int InputInt(const std::string& msg) {
        std::cout << msg;
        return InputInt();
    }
    static int InputRange(unsigned int min, unsigned int max, const std::string& msg) {
        std::cout << msg;
        return InputRange(min, max);
    }

}
