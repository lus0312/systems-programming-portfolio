#ifndef ITER_HPP
# define ITER_HPP
#include <iostream>

template <typename T, typename F>
void iter(T* array, const int len, F func){
	for (int i = 0; i < len; i++){
		func(array[i]);
	}
}
# endif
