#ifndef PMERGEME_HPP
#define PMERGEME_HPP
#include <vector>
#include <list>
#include <iostream>
#include <iomanip> 
#include <exception>
#include <ctime>
#include <cstdlib>
#include <cmath>

class PmergeMe
{
    private:
        std::vector<size_t>         _jacobstal;
        size_t                      _cnt;
        std::vector<int>            _vec;
        std::list<int>              _lst;
        
        PmergeMe();
        int                         toInt( char *c );
        bool                        checkOrder();
        std::vector<size_t>         getJacobstalSequence(size_t len);

        std::vector<int>            fordJohnsonByVec( std::vector<int> *vec );
        std::vector<int>*           binaryInsertByVec( std::vector<int> *sortedArray, int element );

        std::list<int>::iterator    listIdx( std::list<int> &lst, size_t idx);
        std::list<int>              fordJohnsonByList( std::list<int> *lst );
        std::list<int>*             binaryInsertByList( std::list<int> *lst, int element );

        void                        sort( void );
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