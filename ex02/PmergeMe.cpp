#include "PmergeMe.hpp"

PmergeMe::PmergeMe()
{}

PmergeMe::PmergeMe( char **av )
{
    int num;
    
    this->vec = new std::vector<int>;
    this->lst = new std::list<int>;

    std::cout << "Before : ";
    while ( *av )
    {
        num = toInt(*av);
        this->vec->push_back( num );
        this->lst->push_back( num );
        std::cout << num << " ";
        av++;
    }
    std::cout << std::endl;
    if (checkOrder())
        throw OrderedInputExcetion();
}

int PmergeMe::toInt( char *c )
{
    double                      val;
    int                         num;
    std::vector<int>::iterator  it;

    for ( int i = 0; c[i]; i++ )
    {
        if ( c[i] > '9' || c[i] < '0' )
            throw InvalidInputException();
    }
    val = strtod( c, NULL );
    num = static_cast <int> ( val );
    
    it = this->vec->begin();
    for ( it = this->vec->begin(); it != this->vec->end(); it++ )
    {
        if ( *it == num )
            throw DuplicateNumberException();
    }
    
    return ( num );
}

bool PmergeMe::checkOrder()
{
    int     tmp;

    tmp = -1;
    for ( std::vector<int>::iterator it = this->vec->begin();
            it != this->vec->end(); it++ )
    {
        if ( tmp > *it )
            return false;
        tmp = *it;
    }
    return true;
}

PmergeMe::~PmergeMe()
{}

void    PmergeMe::fordJohnsonByVec( void )
{
    std::vector<std::pair<int, int>> 
        pairs = std::vector<std::pair<int, int>>();

    for ( size_t i = 0; i < this->vec->size() - 2; i + 2 )
    {
        if (vec->at(i) < vec->at(i + 1))
            ;
    }
}


const char  *PmergeMe::InvalidInputException::what() const throw()
{
    return "\nError: Invalid Input";
}

const char  *PmergeMe::DuplicateNumberException::what() const throw()
{
    return "\nError: Duplicated Input";
}

const char  *PmergeMe::OrderedInputExcetion::what() const throw()
{
    return "Error: Ordered Input";
}