#ifndef MUTANTSTACK_HPP
# define MUTANTSTACK_HPP
# include <stack>

template <typename T>
class MutantStack : public std::stack<T>{
public:
	MutantStack(){}
	~MutantStack(){}
	MutantStack(const MutantStack& other): std::stack<T>(other){}
	MutantStack& operator=(const MutantStack& other){
		if (this != &other)
			std::stack<T>::operator=(other);
		return *this;
	}

	typedef typename std::stack<T>::container_type::iterator iterator;
	typedef typename std::stack<T>::container_type::const_iterator const_iterator;

	iterator begin(){
		return (this->c.begin());
	}
	iterator end(){
		return (this->c.end());
	}

	const_iterator begin() const{
		return (this->c.begin());
	}
	const_iterator end() const{
		return (this->c.end());
	}
};

#endif

//template <typename T, typename Container = std::deque<T> >
//class stack
//{
//protected:
//    Container c; //stack이 감싸고 있는 내부 컨테이너

//public:
//    typedef Container container_type;
//    void push(const T& value);
//    void pop();
//    T& top();
//    bool empty() const;
//    size_t size() const;
//};
