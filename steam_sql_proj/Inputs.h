#pragma once

#include <string>
#include <iostream>

#define NOMINMAX
#include <Windows.h>
#include <ctime>




#define UK_ZERO 48           //unique key 0
#define UK_ONE 49            //unique key 1
#define UK_NINE 57           //unique key 9
#define UK_LOWER_A 97        //unique key a
#define UK_LOWER_Z 122       //unique key z
#define UK_CAPITAL_A 65      //unique key A
#define UK_CAPITAL_Z 90      //unique key Z
#define UK_SYMBOL_START 32   //unique key printable character start
#define UK_SYMBOL_END 126    //unique key printable character end

#define UK_UNDERSCORE 95     //unique key underscore                        
#define UK_SPACE 32          //unique key space 
#define UK_BACKSPACE 8       //unique key backspace
#define UK_COPY 22           //unique key ctrl + v
#define UK_ESCAPE 27         //unique key escape/esc
#define UK_RETURN 13         //unique key enter/return


#define SR_NULL 0            //select range 0
#define SR_ONE 1             //select range 1
#define SR_TWO 2             //select range 2
#define SR_THREE 3           //select range 3
#define SR_FOUR 4            //select range 4
#define SR_FIVE 5            //select range 5
#define SR_SIX 6             //select range 6
#define SR_SEVEN 7           //select range 7
#define SR_EIGHT 8           //select range 8
#define SR_NINE 9            //select range 9


namespace ORDO { //input string

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
    //manip strings
    namespace MSTR {
        void RemoveSymbols(std::string& from, const std::string& symbols);
        void RemoveSymbols(std::string& from, const char symbol);
        void PrintList(const std::vector<std::pair<std::string, std::string>>& list);
        bool ListContains(const std::vector<std::string>& cont, const std::string& compared_to);
        const std::string UnixTime(time_t unix_timestamp);
        std::string GetClipboard();
    }

    //terminal shit
    void TWait();
    void TRefresh();
    void TPrintTitle();
    void TClearTerminal();
}
