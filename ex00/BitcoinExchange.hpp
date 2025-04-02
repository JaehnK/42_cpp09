#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP
#include <algorithm>
#include <ctime>
#include <exception>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <limits>
#include <sstream>
#include <map>
#include <memory>


class BitcoinExchange
{
private:
    std::map<std::string, double>           *criteria;

    void                                    readCriteria();
    void                                    validateDate( const std::string &date );
    double                                  convertPrice( const std::string &price, bool isInput );
    std::map<std::string, double>           parseLine( const std::string &line, char sep, bool isInput );
    double                                  getPriceFromDate(std::map<std::string, double>::iterator);
    
public:
    BitcoinExchange( );
    BitcoinExchange( const BitcoinExchange &rhs );
    BitcoinExchange         &operator=( const BitcoinExchange &rhs );
    ~BitcoinExchange( );

    void                    readInputText( char *path );

    class   FailedToOpenFile: public std::exception
    {
        public:
            const char  *what() const throw();
    };
    
    class   InvalidNumberFormatException: public std::exception
    {
        public:
            const char* what() const throw();
    };

    class   NegativeNumberException: public std::exception
    {
        public:
            const char* what() const throw();
    };

    class   LargeNumberException: public std::exception
    {
        public:
            const char *what() const throw();
    };

    class   InvalidDateFormatException: public std::exception
    {
        public:
            const char* what() const throw();
    };

};


#endif