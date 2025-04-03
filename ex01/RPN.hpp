#ifndef RPN_HPP
#define RPN_HPP
#include <stack>
#include <string>
#include <sstream>
#include <iostream>
#include <exception>
#include <cctype>
#include <cstdlib>

class RPN
{
    private:
        std::stack<int> *stk;

        int     toInt(const std::string &w);
        void    calculate(const std::string &w);

    public:
        RPN(const std::string &str);
        ~RPN();

        class rpnExceptions: public std::exception
    {
        public:
            const char *what() const throw();
    };
};

#endif