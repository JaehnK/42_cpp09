#include "BitcoinExchange.hpp"

BitcoinExchange::BitcoinExchange()
{
    criteria = new std::map< std::string, double >;
    readCriteria();
}

BitcoinExchange::~BitcoinExchange()
{
    delete criteria;
}

BitcoinExchange::BitcoinExchange( const BitcoinExchange &rhs )
{
    this->criteria = new std::map<std::string, double>(*rhs.criteria);
}

BitcoinExchange &BitcoinExchange::operator=( const BitcoinExchange &rhs )
{
    if (this != &rhs)
    {
        delete this->criteria;
        this->criteria = new std::map<std::string, double>(*rhs.criteria);
    }
    return (*this);
}

void    BitcoinExchange::validateDate( const std::string &date )
{
    int     year, month, day;
    int     validDays[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if ( sscanf( date.c_str(), "%d-%d-%d", &year, &month, &day ) != 3 )
        throw InvalidDateFormatException();

    if ( year < 2009 || year > 2023 ||
            month < 1 || month > 12 || 
            day < 1 || day > 31 )
        throw InvalidDateFormatException();

    
    if (year % 4 == 0)
        validDays[1] = 29;
    
    if (day > validDays[month - 1])
        throw InvalidDateFormatException();

    return ;
}

double  BitcoinExchange::convertPrice( const std::string &price, bool isInput )
{
    double              value;
    std::istringstream  iss( price );

    iss.precision(std::numeric_limits<double>::digits10);
    iss >> value;
    
    if ( iss.fail() || !iss.eof())
        throw InvalidNumberFormatException();

    if ( value < 0 )
        throw NegativeNumberException();

    else if ( value > 1000 && isInput)
        throw LargeNumberException();
    
    return ( value );
}

std::map<std::string, double>   BitcoinExchange::parseLine( const std::string &line, char sep, bool isInput )
{
    size_t                          position;
    double                          price;
    std::string                     strDate;
    std::string                     strPrice;
    std::map<std::string, double>   ret;
    
    position = line.find( sep );
    if ( position != std::string::npos )
    {
        if (isInput)
        {
            strDate = line.substr( 0, position - 1);
            strPrice = line.substr( position + 2, line.length() + 1 );
        }
        else
        {
            strDate = line.substr( 0, position );
            strPrice = line.substr( position + 1, line.length() + 1 );
        }
        price = convertPrice( strPrice, isInput );
    }
    else
    {
       throw FailedToOpenFile();
    }
    ret.insert( std::make_pair( strDate, price ) );
    return ( ret );
}

double  BitcoinExchange::getPriceFromDate(std::map<std::string, double>::iterator it)
{
    std::map<std::string, double>::iterator findKey;

    findKey = this->criteria->find(it->first);
    if (findKey == this->criteria->end())
    {
        findKey = --(this->criteria->upper_bound(it->first));
    }
    return (findKey->second);
}

void    BitcoinExchange::readCriteria()
{
    size_t          idx = 0;
    std::ifstream   rFile;
    std::string     str;

    rFile.open( "./data.csv" );
    
    if ( !rFile.is_open() )
        throw FailedToOpenFile();
    
    while( getline( rFile, str ) )
    {
        if ( idx != 0 )
        {
            criteria->insert(*(parseLine(str, ',', false).begin()));
        }
        idx++;
    }

    rFile.close();
}

void    BitcoinExchange::readInputText( char *path )
{
    size_t                                  idx;
    std::ifstream                           rFile;
    std::string                             str;
    std::map<std::string, double>           tmp;
    std::map<std::string, double>::iterator it;

    rFile.open( path );
    
    if ( !rFile.is_open() )
        throw FailedToOpenFile();

    idx = 0;
    while( getline( rFile, str ) )
    {
        if ( idx == 0 )
        {
            if ( str != "date | value" )
            {
                std::cerr << "Error: invalid input.txt header" << std::endl;
                return ;
            }
        }
        else
        {
            try
            {
                tmp = parseLine(str, '|', true);
                it = tmp.begin();
                validateDate(it->first);
                std::cout << it->first << " => " << it->second;
                std::cout << " = " << getPriceFromDate(it) * it->second << std::endl;
            }
            catch(const InvalidDateFormatException &e)
            {
                std::cerr << e.what() << it->first << std::endl;
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << std::endl;
            }
        }
        idx++;
    }
}

const char *BitcoinExchange::FailedToOpenFile::what() const throw()
{
    return "Error: could not open file";
}

const char *BitcoinExchange::InvalidNumberFormatException::what() const throw()
{
    return "Error: invalid number format";
}

const char *BitcoinExchange::NegativeNumberException::what() const throw()
{
    return "Error: Not a postive number";
}

const char *BitcoinExchange::LargeNumberException::what() const throw()
{
    return "Error: too large a number.";
}

const char *BitcoinExchange::InvalidDateFormatException::what() const throw()
{
    return "Error: Bad Input => ";
}
