#ifndef PMERGEME_HPP
#define PMERGEME_HPP
#include <vector>
#include <list>
#include <iostream>
#include <exception>
#include <cstdlib>

class PmergeMe
{
    private:
        std::vector <int>   *vec;
        std::list   <int>   *lst;

        PmergeMe();
        int                 toInt( char *c );
        bool                checkOrder();
    public:
        PmergeMe( char **av );
        ~PmergeMe();

        void                fordJohnsonByVec( void );



        class   InvalidInputException: public std::exception
        {
            public:
                const char * what() const throw();
        };

        class   DuplicateNumberException: public std::exception
        {
            public:
                const char *what() const throw();
        };

        class OrderedInputExcetion: public std::exception
        {
            public:
                const char *what() const throw();
        };

};



#endif