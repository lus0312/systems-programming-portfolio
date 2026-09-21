#include "PmergeMe.hpp"
#include <ctime>
#include <utility>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <iterator>


PmergeMe::PmergeMe()	
	: _vectorTime(0.0), _dequeTime(0.0)
{
}
PmergeMe::PmergeMe(const std::vector<int>& input)
	: _inputVector(input), _inputDeque(input.begin(), input.end()),
	  _vectorTime(0.0), _dequeTime(0.0)
{
}
PmergeMe::PmergeMe(const PmergeMe& other)
	: _inputVector(other._inputVector), _inputDeque(other._inputDeque),
	  _sortedVector(other._sortedVector), _sortedDeque(other._sortedDeque),
	  _vectorTime(other._vectorTime), _dequeTime(other._dequeTime)
{
}
PmergeMe::~PmergeMe(){}
PmergeMe& PmergeMe::operator=(const PmergeMe& other){
	if (this != &other){
		_inputVector = other._inputVector;
		_inputDeque = other._inputDeque;
		_sortedVector = other._sortedVector;
		_sortedDeque = other._sortedDeque;
		_vectorTime = other._vectorTime;
		_dequeTime = other._dequeTime;
	}
	return *this;
}

void PmergeMe::binaryInsertVector(std::vector<int>& arr, int small, std::vector<int>::iterator limit){

	std::vector<int>::iterator pos;
	pos = std::upper_bound(arr.begin(), limit, small);
	arr.insert(pos, small);
}

void PmergeMe::binaryInsertDeque(std::deque<int>& arr, int small, std::deque<int>::iterator limit){
	std::deque<int>::iterator pos;
	pos = std::upper_bound(arr.begin(), limit, small);
	arr.insert(pos, small);
}

std::vector<std::size_t> PmergeMe::makeJacobsthalOrder(std::size_t size)
{
    std::vector<std::size_t> order;

    if (size == 0)
		return order;
	std::size_t prevJacob = 1;
    std::size_t currJacob = 1;
    std::size_t nextJacob = currJacob + 2 * prevJacob;

    while (nextJacob <= size){
        std::size_t index = nextJacob;
        while (index > currJacob)
        {
            order.push_back(index - 1);
            index--;
        }
        prevJacob = currJacob;
        currJacob = nextJacob;
        nextJacob = currJacob + 2 * prevJacob;
	}
	if (currJacob < size)
	{
		std::size_t index = size;
		while (index > currJacob)
		{
			order.push_back(index - 1);
			index--;
		}
	}
    return order;
}

void PmergeMe::fordJohnsonDeque(std::deque<int>& arr){
	if (arr.size() <= 1)
		return ;
	std::deque< std::pair<int, int> >	pairs;
	std::deque<int>						mainChain;

	std::size_t i = 0;
	while (i+1 <arr.size()){
		int small = std::min(arr[i], arr[i + 1]);
		int large = std::max(arr[i], arr[i + 1]);
		pairs.push_back(std::make_pair(small, large));
		mainChain.push_back(large);
		i+=2;
	}
	bool hasLeftover = false;
	int leftover = 0;
	if (i < arr.size()){
		leftover = arr[i];
		hasLeftover = true;
	}
	fordJohnsonDeque(mainChain);
	std::deque< std::pair<int, int> > pending;

	i = 0;
	while (i < mainChain.size()){
		std::size_t j = 0;
		while (j < pairs.size()){
			if (pairs[j].second == mainChain[i]){
				pending.push_back(pairs[j]);
				break;
			}
			j++;
		}
		i++;
	}
	mainChain.insert(mainChain.begin(), pending[0].first);
	std::vector<std::size_t> order = makeJacobsthalOrder(pending.size());
	i = 0;
	while (i < order.size()){
		std::deque<int>::iterator limit;
		limit = std::find(mainChain.begin(), mainChain.end(), pending[order[i]].second);
		binaryInsertDeque(mainChain, pending[order[i]].first, limit);
		i++;
	}
	if (hasLeftover)
		binaryInsertDeque(mainChain, leftover, mainChain.end());
	arr = mainChain;
}

void PmergeMe::fordJohnsonVector(std::vector<int>& arr){
	if (arr.size() <= 1)
		return ;

	std::vector< std::pair<int, int> >	pairs;
	std::vector<int>					mainChain;

	std::size_t i = 0;
	while (i + 1 < arr.size()){
		int small = std::min(arr[i], arr[i + 1]);
		int large = std::max(arr[i], arr[i + 1]);
		pairs.push_back(std::make_pair(small, large));
		mainChain.push_back(large);
		i+=2;
	}
	bool hasLeftover = false;
	int leftover = 0;
	if (i < arr.size()){
		leftover = arr[i];
		hasLeftover = true;
	}
	fordJohnsonVector(mainChain);
	std::vector<std::pair<int, int> > pending;
    i = 0;
    while (i < mainChain.size()){
        std::size_t j = 0;
        while (j < pairs.size()){
            if (pairs[j].second == mainChain[i]){
                pending.push_back(pairs[j]);
                break;
            }
            j++;
        }
        i++;
    }
	mainChain.insert(mainChain.begin(), pending[0].first);
    i = 0;
	std::vector<std::size_t> order = makeJacobsthalOrder(pending.size());
    while (i < order.size()){
    	std::vector<int>::iterator limit;
		limit = std::find(mainChain.begin(), mainChain.end(), pending[order[i]].second);
        binaryInsertVector(mainChain, pending[order[i]].first, limit);
        i++;
    }
    if (hasLeftover == true)
        binaryInsertVector(mainChain, leftover, mainChain.end());
    arr = mainChain;
}


void PmergeMe::process(){
	clock_t start;
	clock_t end;

	_sortedVector = _inputVector;
	start = std::clock();
	fordJohnsonVector(_sortedVector);
	end = std::clock();
	_vectorTime = static_cast<double>(end - start) / CLOCKS_PER_SEC * 1000000.0;

	_sortedDeque = _inputDeque;
	start = std::clock();
	fordJohnsonDeque(_sortedDeque);
	end = std::clock();
	_dequeTime = static_cast<double>(end - start) / CLOCKS_PER_SEC * 1000000.0;
}

void PmergeMe::printResults() const{
	
	std::vector<int>::const_iterator vit;

	std::cout << "Before:";
	vit = _inputVector.begin();
	while (vit != _inputVector.end())
	{
		std::cout << " " << *vit;
		vit++;
	}
	std::cout << std::endl;

	std::cout << "After:";
	vit = _sortedVector.begin();
	while (vit != _sortedVector.end())
	{
		std::cout << " " << *vit;
		vit++;
	}
	std::cout << std::endl;

	std::cout << std::fixed << std::setprecision(5);
	std::cout << "Time to process a range of "
			  << _inputVector.size()
			  << " elements with std::vector : "
			  << _vectorTime << " us" << std::endl;

	std::cout << "Time to process a range of "
			  << _inputDeque.size()
			  << " elements with std::deque : "
			  << _dequeTime << " us" << std::endl;
}