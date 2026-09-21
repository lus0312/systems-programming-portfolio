#ifndef ARRAY_HPP
# define ARRAY_HPP
# include <stdexcept>

template <typename T>
class Array{
private:
    T* _data;
    unsigned int _size;
public:
    Array(): _data(0), _size(0){}
    Array(unsigned int n): _data(0), _size(n){
        if (_size != 0)
            _data = new T[_size]();
    }
    ~Array(){
        delete[] _data;
    }
    Array(const Array& other): _data(0), _size(other._size){
        if (_size != 0){
            _data = new T[_size];
            for (unsigned int i = 0; i < _size; i++)
                _data[i] = other._data[i];
        }
    }

    Array& operator=(const Array& other){
        T* tmp;
        if (this == &other)
            return *this;
        tmp = 0;
        if (other._size != 0){
            tmp = new T[other._size];
            for (unsigned int i = 0; i < other._size; i++)
                tmp[i] = other._data[i];
        }
        delete[] _data;
        _data = tmp;
        _size = other._size;
        return *this;
    }

    T& operator[](unsigned int n){
        if (n >= _size)
        	throw std::out_of_range("Array index out of range");
        return _data[n];
    }
    const T& operator[](unsigned int n) const{
        if (n >= _size)
        	throw std::out_of_range("Array index out of range");
        return _data[n];
    }
    unsigned int size() const{
        return _size;
    }
};

#endif