#include "PmergeMe.hpp"

PmergeMe::PmergeMe()
{}

PmergeMe::PmergeMe( char **av )
{
    int num;
    
    this->_vec = new std::vector<int>;
    this->_lst = new std::list<int>;

    std::cout << "Before : ";
    while ( *av )
    {
        num = toInt(*av);
        this->_vec->push_back( num );
        this->_lst->push_back( num );
        std::cout << num << " ";
        av++;
    }
    std::cout << std::endl;
    if (checkOrder())
        throw OrderedInputExcetion();
    fordJohnsonByVec(this->_vec);
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
    
    it = this->_vec->begin();
    for ( it = this->_vec->begin(); it != this->_vec->end(); it++ )
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
    for ( std::vector<int>::iterator it = this->_vec->begin();
            it != this->_vec->end(); it++ )
    {
        if ( tmp > *it )
            return false;
        tmp = *it;
    }
    return true;
}

PmergeMe::~PmergeMe()
{}

int* PmergeMe::getJacobstalSequence( size_t len )
{
    int*    jacbolstal;
    
    if ( len == 0 )
        return ( NULL );

    jacbolstal = new int[len];
    jacbolstal[0] = 1;
    jacbolstal[1] = 1;
    
    for (size_t i = 2; i < len; i++)
        jacbolstal[i] = jacbolstal[i - 1] + ( 2 * jacbolstal[i - 2] );
    
    return ( jacbolstal );
}

std::vector<int>    *PmergeMe::binaryInsertByVec( std::vector<int> *sortedArray, int element )
{
    size_t left, right, mid;

    left = 0;
    right = (*sortedArray).size() - 1;

    while (left <= right)
    {
        mid = std::floor((left + right) / 2);
        if ((*sortedArray).at(mid) < element)
            left = mid + 1;
        else
            right = mid - 1;
    }
    (*sortedArray).insert((*sortedArray).begin() + left, element);
    return sortedArray;
}

std::vector<int>    PmergeMe::fordJohnsonByVec( std::vector<int> *vec )
{
    int                                 *extraElement;
    int                                 *jacobstal;
    std::pair<int, int>                 p;
    std::vector<int>                    smallElements;
    std::vector<int>                    sortedSmallElements;
    std::vector<int>                    sortedArray;
    std::vector< std::pair<int, int> >    pairs;

    pairs = std::vector< std::pair<int, int> > ();
    for ( size_t i = 0; i < vec->size() - 2; i += 2)
    {
        if (vec->at( i ) < vec->at( i + 1 ))
            p = std::make_pair( vec->at( i ), vec->at( i + 1 ) );
        else
            p = std::make_pair( vec->at( i + 1 ), vec->at( i ) );
    }
    
    if (vec->size() % 2 == 1)
    {
        extraElement = new int;
        *extraElement = vec->at( vec->size() - 1 );
    }
    else
        extraElement = NULL;
    
    smallElements = std::vector<int> ();
    for ( size_t i = 0; i < pairs.size() - 1; i++ )
    {
        smallElements.push_back( pairs.at( i ).first );
        sortedSmallElements = fordJohnsonByVec( &smallElements );

    }

    sortedArray = std::vector<int> ();
    sortedArray.push_back( sortedSmallElements.at( 0 ) );

    jacobstal = getJacobstalSequence(vec->size());
    for ( size_t i = 1; i < sortedSmallElements.size(); i++)
    {
        binaryInsertByVec(&sortedArray, sortedSmallElements[jacobstal[i]]);
    }

    delete []jacobstal;
    return ( sortedArray );
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