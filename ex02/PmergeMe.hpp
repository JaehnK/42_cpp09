#ifndef PMERGEME_HPP
#define PMERGEME_HPP
#include <vector>
#include <list>
#include <iostream>
#include <exception>
#include <cstdlib>
#include <cmath>

class PmergeMe
{
    private:
        size_t              *_jacobstal;
        std::vector <int>*   _vec;
        std::list   <int>*   _lst;
        
        PmergeMe();
        int                 toInt( char *c );
        bool                checkOrder();
        size_t*                getJacobstalSequence(size_t len);

        std::vector<int>    fordJohnsonByVec( std::vector<int> *vec );
        std::vector<int>*   binaryInsertByVec( std::vector<int> *sortedArray, int element );

        std::list<int>      fordJohnsonBylist( std::list<int> *lst );
        std::list<int>*     binaryInsertBylist( std::list<int> *lst );

    public:
        PmergeMe( char **av );
        ~PmergeMe();

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

        class   OrderedInputExcetion: public std::exception
        {
            public:
                const char *what() const throw();
        };

};



#endif