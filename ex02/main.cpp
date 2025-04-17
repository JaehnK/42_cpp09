#include "PmergeMe.hpp"

int main( int ac, char **av )
{
    if ( ac < 3 )
    {
        std::cerr << "Method: ./PmergeMe 0 2 3 6 5 ..." << std::endl;
        return ( 1 );
    }
    try
    {
        PmergeMe pm( ++av );
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return (1);
    }
    return (0);
}