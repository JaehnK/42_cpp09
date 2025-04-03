#include "RPN.hpp"

RPN::RPN(const std::string &input)
{
    int         position;
    int         current;
    int         len;
    std::string str;
    
    current = 0;
    str = input + " ";
    stk = new std::stack<int>;
    while ( ( position = str.find( ' ', current ) ) != static_cast<int>( std::string::npos ) )
    {
        len = position - current;
        if (len != 1)
            throw rpnExceptions();
        
        std::cout << "input: "<< str.substr(current, len) << std::endl;
        
        if ( isdigit( str.substr( current, len )[0] ) )
            this->stk->push( toInt( str.substr( current, len ) ) );
        else if ( str.substr( current, len ) == "*" || str.substr( current, len ) == "/" ||
                    str.substr( current, len ) == "-"  || str.substr( current, len ) == "+" )
            calculate(str.substr( current, len ));
        else
            throw rpnExceptions();
        current = position + 1;
    }
    std::cout << this->stk->top() << std::endl;
}

RPN::~RPN()
{
    delete stk;
}

int RPN::toInt(const std::string &w)
{
    double val;

    if (w.length() > 1)
        throw rpnExceptions();
    
    val = strtod( w.c_str(), NULL );
    return ( static_cast<int> (val) );
}

void RPN::calculate(const std::string &w)
{
    int a, b;

    if (this->stk->size() < 2)
        throw rpnExceptions();
    
    a = this->stk->top();
    this->stk->pop();
    b = this->stk->top();
    this->stk->pop();
    if (w == "*")
        this->stk->push(b * a);
    else if (w == "+")
        this->stk->push(b + a);
    else if (w == "-")
        this->stk->push(b - a);
    else if (w == "/")
        this->stk->push(b / a);
}

const char *RPN::rpnExceptions::what() const throw()
{
    return "Error";
}