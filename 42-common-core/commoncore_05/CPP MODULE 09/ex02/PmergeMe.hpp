#ifndef PMERGEME_HPP
# define PMERGEME_HPP
# include <vector>
# include <deque>

class PmergeMe{
private:
	std::vector<int> _inputVector;
	std::deque<int> _inputDeque;
	std::vector<int> _sortedVector;
	std::deque<int> _sortedDeque;
	double _vectorTime;
	double _dequeTime;

	void fordJohnsonVector(std::vector<int>& arr);
	void binaryInsertVector(std::vector<int>& arr, int small, std::vector<int>::iterator limit);
	void fordJohnsonDeque(std::deque<int>& arr);
	void binaryInsertDeque(std::deque<int>& arr, int small, std::deque<int>::iterator limit);
	std::vector<std::size_t> makeJacobsthalOrder(std::size_t size);

public:
	PmergeMe();
	PmergeMe(const std::vector<int>& input);
	PmergeMe(const PmergeMe& other);
	~PmergeMe();
	PmergeMe& operator=(const PmergeMe& other);

	void process();
	void printResults() const;
};

#endif