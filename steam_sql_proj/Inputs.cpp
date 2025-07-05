#include "Inputs.h"

namespace ORDO {

    namespace Inputs {

        void error_input_check(std::istream& stream) {
            if (stream.fail()) {
                throw std::runtime_error("invalid input");
            }
        }
        void error_range_check(int checked, unsigned int min, unsigned int max) {
            if (checked < min || checked > max) {
                throw std::runtime_error("out of range input");
            }
        }
        void clearInputBuffer() {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        std::string InputStr() {
            std::string str;
            if (!std::cin.good())
                clearInputBuffer();
            std::getline(std::cin, str);
            error_input_check(std::cin);
            return str;
        }
        int InputInt() {
            int n = 0;
            std::cin >> n;
            error_input_check(std::cin);
            return n;
        }
        float InputFloat() {
            float n = 0;
            std::cin >> n;
            error_input_check(std::cin);
            return n;
        }
        int InputRange(unsigned int min, unsigned int max) {
            int n = 0;
            std::cin >> n;
            error_input_check(std::cin);
            error_range_check(n, min, max);
            return n;
        }

        std::string InputStr(const std::string& str) {
            std::cout << str;
            return InputStr();
        }
        int InputInt(const std::string& str) {
            std::cout << str;
            return InputInt();
        }
        int InputRange(unsigned int min, unsigned int max, const std::string& str) {
            std::cout << str;
            return InputRange(min, max);
        }

    }

}
