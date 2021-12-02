#include <iostream>
#include <typeinfo>

#include "vector.hpp"
#include "iterator.hpp"

int
    main(void)
{
    ft::vector<int> vector_empty;
    ft::vector<int> vector_fill (4, 100);

//	ft::iterator<std::input_iterator_tag, int>	it_1;
//	ft::iterator_traits<int*>					it_3;
//	ft::iterator_traits<const int*>				it_4;
//
//	if (typeid(ft::iterator_traits<int*>::iterator_category) == typeid(ft::random_access_iterator_tag))
//		std::cout << "int* is a random-access iterator" << std::endl;
	
	return (0);
}