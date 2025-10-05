#include "PmergeMe.hpp"

static size_t binaryInsertVec(std::vector<int> &arr, int element, size_t maxPos)
{
    if (arr.empty() || maxPos == 0)
    {
        arr.insert(arr.begin(), element);
        return 0;
    }
    
    size_t left = 0;
    size_t right = (maxPos < arr.size()) ? maxPos : arr.size() - 1;
    
    while (left <= right)
    {
        size_t mid = left + (right - left) / 2;
        if (arr[mid] < element)
        {
            left = mid + 1;
        }
        else
        {
            if (mid == 0) break;
            right = mid - 1;
        }
    }
    
    arr.insert(arr.begin() + left, element);
    return left;
}

static size_t binaryInsertList(std::list<int> &lst, int element, size_t maxPos)
{
    if (lst.empty() || maxPos == 0)
    {
        lst.push_front(element);
        return 0;
    }
    
    size_t left = 0;
    size_t right = (maxPos < lst.size()) ? maxPos : lst.size() - 1;
    
    while (left <= right)
    {
        size_t mid = left + (right - left) / 2;
        std::list<int>::iterator it = lst.begin();
        std::advance(it, mid);
        
        if (*it < element)
        {
            left = mid + 1;
        }
        else
        {
            if (mid == 0) break;
            right = mid - 1;
        }
    }
    
    std::list<int>::iterator it = lst.begin();
    std::advance(it, left);
    lst.insert(it, element);
    return left;
}

PmergeMe::PmergeMe()
{
}

PmergeMe::PmergeMe(char **av)
{
    int num;

    std::cout << "Before : ";
    this->_cnt = 0;
    while (*av)
    {
        num = toInt(*av);
        this->_vec.push_back(num);
        this->_lst.push_back(num);
        std::cout << num << " ";
        av++;
        this->_cnt++;
    }
    std::cout << std::endl;
    if (checkOrder()) {
        throw OrderedInputExcetion();
    }
    
    sort();
}

int PmergeMe::toInt(char *c)
{
    double val;
    int num;
    std::vector<int>::iterator it;

    for (int i = 0; c[i]; i++)
    {
        if (c[i] > '9' || c[i] < '0')
            throw InvalidInputException();
    }
    val = strtod(c, NULL);
    num = static_cast<int>(val);
    
    it = this->_vec.begin();
    for (it = this->_vec.begin(); it != this->_vec.end(); it++)
    {
        if (*it == num) {
            throw DuplicateNumberException();
        }
    }
    
    return (num);
}

bool PmergeMe::checkOrder()
{
    int tmp;

    tmp = -1;
    for (std::vector<int>::iterator it = this->_vec.begin();
            it != this->_vec.end(); it++)
    {
        if (tmp > *it)
            return false;
        tmp = *it;
    }
    return true;
}

PmergeMe::~PmergeMe()
{
}

std::vector<size_t> PmergeMe::getJacobstalSequence(size_t len)
{
    std::vector<size_t> jacobstal;
    if (len == 0)
        return jacobstal;
    jacobstal.resize(len);
    jacobstal[0] = 0;
    if (len > 1)
        jacobstal[1] = 1;
    for (size_t i = 2; i < len; i++)
        jacobstal[i] = jacobstal[i - 1] + (2 * jacobstal[i - 2]);
    return jacobstal;
}

std::vector<int>* PmergeMe::binaryInsertByVec(std::vector<int> *sortedArray, int element)
{
    binaryInsertVec(*sortedArray, element, sortedArray->size());
    return sortedArray;
}

std::vector<int> PmergeMe::fordJohnsonByVec(std::vector<int> *vec)
{
    // Base case
    if (vec->size() <= 1)
        return *vec;
    
    // Step 1: Create pairs and separate odd element
    bool hasOdd = (vec->size() % 2 == 1);
    int oddElement = hasOdd ? vec->at(vec->size() - 1) : 0;
    size_t pairCount = vec->size() / 2;
    
    // Store pairs (larger, smaller)
    std::vector<std::pair<int, int> > pairs;
    for (size_t i = 0; i < pairCount; i++)
    {
        int a = vec->at(2 * i);
        int b = vec->at(2 * i + 1);
        if (a > b)
            pairs.push_back(std::make_pair(a, b));
        else
            pairs.push_back(std::make_pair(b, a));
    }
    
    // Step 2: Recursively sort larger elements (1st round winners)
    std::vector<int> largerElements;
    for (size_t i = 0; i < pairs.size(); i++)
        largerElements.push_back(pairs[i].first);
    
    std::vector<int> sortedLarger = fordJohnsonByVec(&largerElements);
    
    // Step 3: Reorder pairs according to sorted larger elements
    std::vector<std::pair<int, int> > orderedPairs;
    for (size_t i = 0; i < sortedLarger.size(); i++)
    {
        // Find the pair with this larger element
        for (size_t j = 0; j < pairs.size(); j++)
        {
            if (pairs[j].first == sortedLarger[i])
            {
                orderedPairs.push_back(pairs[j]);
                break;
            }
        }
    }
    
    // Step 4: Build main chain - larger elements only (already sorted)
    std::vector<int> mainChain;
    for (size_t i = 0; i < orderedPairs.size(); i++)
        mainChain.push_back(orderedPairs[i].first);
    
    // Step 5: Insert all small elements using Jacobsthal sequence
    if (!orderedPairs.empty())
    {
        // All small elements are pending
        std::vector<int> pendingElements;
        std::vector<size_t> positions;
        
        for (size_t i = 0; i < orderedPairs.size(); i++)
        {
            pendingElements.push_back(orderedPairs[i].second);
            positions.push_back(i);
        }
        
        std::vector<bool> inserted(pendingElements.size(), false);
        
        // Insert first element (a1) first
        size_t insPos = binaryInsertVec(mainChain, pendingElements[0], positions[0]);
        for (size_t i = 0; i < positions.size(); i++)
            if (positions[i] >= insPos)
                positions[i]++;
        inserted[0] = true;
        
        // Jacobsthal sequence insertion (start from J(3))
        size_t jacobIdx = 3;
        size_t prevJacob = 1;  // J(2) = 1
        
        while (prevJacob < pendingElements.size())
        {
            size_t currJacob;
            if (jacobIdx < _jacobstal.size())
                currJacob = _jacobstal[jacobIdx];
            else
                currJacob = pendingElements.size();
            
            if (currJacob > pendingElements.size())
                currJacob = pendingElements.size();
            
            // Insert range [prevJacob+1, currJacob] in reverse order
            for (size_t j = currJacob; j > prevJacob; j--)
            {
                size_t idx = j - 1;
                if (idx < pendingElements.size() && !inserted[idx])
                {
                    insPos = binaryInsertVec(mainChain, pendingElements[idx], positions[idx]);
                    
                    // Update positions
                    for (size_t i = 0; i < positions.size(); i++)
                        if (positions[i] >= insPos)
                            positions[i]++;
                    
                    inserted[idx] = true;
                }
            }
            
            prevJacob = currJacob;
            jacobIdx++;
            
            if (currJacob >= pendingElements.size())
                break;
        }
    }
    
    // Step 6: Insert odd element if exists
    if (hasOdd)
        binaryInsertVec(mainChain, oddElement, mainChain.size());
    
    return mainChain;
}

std::list<int>::iterator PmergeMe::listIdx(std::list<int> &lst, size_t idx)
{
    std::list<int>::iterator it;

    it = lst.begin();
    std::advance(it, idx);
    return (it);
}

std::list<int>* PmergeMe::binaryInsertByList(std::list<int> *sortedLst, int element)
{
    binaryInsertList(*sortedLst, element, sortedLst->size());
    return sortedLst;
}

std::list<int> PmergeMe::fordJohnsonByList(std::list<int> *lst)
{
    // Base case
    if (lst->size() <= 1)
        return *lst;
    
    // Step 1: Create pairs and separate odd element
    bool hasOdd = (lst->size() % 2 == 1);
    int oddElement = 0;
    if (hasOdd)
    {
        std::list<int>::iterator it = lst->end();
        --it;
        oddElement = *it;
    }
    
    size_t pairCount = lst->size() / 2;
    
    // Store pairs (larger, smaller)
    std::vector<std::pair<int, int> > pairs;
    std::list<int>::iterator it = lst->begin();
    for (size_t i = 0; i < pairCount; i++)
    {
        int a = *it;
        ++it;
        int b = *it;
        ++it;
        
        if (a > b)
            pairs.push_back(std::make_pair(a, b));
        else
            pairs.push_back(std::make_pair(b, a));
    }
    
    // Step 2: Recursively sort larger elements (1st round winners)
    std::list<int> largerElements;
    for (size_t i = 0; i < pairs.size(); i++)
        largerElements.push_back(pairs[i].first);
    
    std::list<int> sortedLarger = fordJohnsonByList(&largerElements);
    
    // Step 3: Reorder pairs according to sorted larger elements
    std::vector<std::pair<int, int> > orderedPairs;
    for (std::list<int>::iterator it = sortedLarger.begin(); it != sortedLarger.end(); ++it)
    {
        // Find the pair with this larger element
        for (size_t j = 0; j < pairs.size(); j++)
        {
            if (pairs[j].first == *it)
            {
                orderedPairs.push_back(pairs[j]);
                break;
            }
        }
    }
    
    // Step 4: Build main chain - larger elements only (already sorted)
    std::list<int> mainChain;
    for (size_t i = 0; i < orderedPairs.size(); i++)
        mainChain.push_back(orderedPairs[i].first);
    
    // Step 5: Insert all small elements using Jacobsthal sequence
    if (!orderedPairs.empty())
    {
        // All small elements are pending
        std::vector<int> pendingElements;
        std::vector<size_t> positions;
        
        for (size_t i = 0; i < orderedPairs.size(); i++)
        {
            pendingElements.push_back(orderedPairs[i].second);
            positions.push_back(i);
        }
        
        std::vector<bool> inserted(pendingElements.size(), false);
        
        // Insert first element (a1) first
        size_t insPos = binaryInsertList(mainChain, pendingElements[0], positions[0]);
        for (size_t i = 0; i < positions.size(); i++)
            if (positions[i] >= insPos)
                positions[i]++;
        inserted[0] = true;
        
        // Jacobsthal sequence insertion (start from J(3))
        size_t jacobIdx = 3;
        size_t prevJacob = 1;  // J(2) = 1
        
        while (prevJacob < pendingElements.size())
        {
            size_t currJacob;
            if (jacobIdx < _jacobstal.size())
                currJacob = _jacobstal[jacobIdx];
            else
                currJacob = pendingElements.size();
            
            if (currJacob > pendingElements.size())
                currJacob = pendingElements.size();
            
            // Insert range [prevJacob+1, currJacob] in reverse order
            for (size_t j = currJacob; j > prevJacob; j--)
            {
                size_t idx = j - 1;
                if (idx < pendingElements.size() && !inserted[idx])
                {
                    insPos = binaryInsertList(mainChain, pendingElements[idx], positions[idx]);
                    
                    // Update positions
                    for (size_t i = 0; i < positions.size(); i++)
                        if (positions[i] >= insPos)
                            positions[i]++;
                    
                    inserted[idx] = true;
                }
            }
            
            prevJacob = currJacob;
            jacobIdx++;
            
            if (currJacob >= pendingElements.size())
                break;
        }
    }
    
    // Step 6: Insert odd element if exists
    if (hasOdd)
        binaryInsertList(mainChain, oddElement, mainChain.size());
    
    return mainChain;
}

void PmergeMe::sort()
{
    clock_t start, finish;
    double vectorDuration, listDuration;
    std::list<int> sortedLst;
    std::vector<int> sortedVec;

    // size_t pairCount = this->_vec.size() / 2;
    this->_jacobstal = getJacobstalSequence(20);

    start = clock();
    sortedLst = fordJohnsonByList(&this->_lst);
    finish = clock();
    listDuration = static_cast<double>(finish - start) / CLOCKS_PER_SEC;

    start = clock();
    sortedVec = fordJohnsonByVec(&this->_vec);
    finish = clock();
    vectorDuration = static_cast<double>(finish - start) / CLOCKS_PER_SEC;
    
    std::cout << std::fixed;
    std::cout << std::setprecision(6);
    std::cout << "Time to process a range of " << this->_cnt << " elements with std::list : " << listDuration << " us" << std::endl;
    std::cout << "Time to process a range of " << this->_cnt << " elements with std::vector : " << vectorDuration << " us" << std::endl;
    std::cout << "After: ";
    for (std::vector<int>::iterator it = sortedVec.begin(); 
            it != sortedVec.end(); it++)
        std::cout << *it << " ";
    std::cout << std::endl;
}

const char *PmergeMe::InvalidInputException::what() const throw()
{
    return "\nError: Invalid Input";
}

const char *PmergeMe::DuplicateNumberException::what() const throw()
{
    return "\nError: Duplicated Input";
}

const char *PmergeMe::OrderedInputExcetion::what() const throw()
{
    return "Error: Ordered Input";
}