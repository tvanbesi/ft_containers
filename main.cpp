#include <iostream>
#include <typeinfo>

#include "vector.hpp"
#include "iterator.hpp"
#include "type_traits.hpp"

#include <vector>
#include <iterator>

int
    main(void)
{
    ft::vector<int> vector_empty;
    ft::vector<int> vector_fill (4, 42);

	ft::iterator<std::input_iterator_tag, int>	it_1;
	ft::iterator_traits<int*>					it_3;
	ft::iterator_traits<const int*>				it_4;

	if (typeid(ft::iterator_traits<int*>::iterator_category) == typeid(ft::random_access_iterator_tag))
		std::cout << "int* is a random-access iterator" << std::endl;
	
	//ft::vector<int>::iterator it = vector_fill.begin();
	//std::cout << *it++ << std::endl;

	// ft::vector<int>::iterator bit = vector_fill.begin();
	// ft::vector<int>::iterator eit = vector_fill.end();
	// std::cout << &bit << "\t" << *bit << std::endl;
	// std::cout << &eit << "\t" << *eit << std::endl;
	ft::vector<int> vector_range (vector_fill.begin(), vector_fill.end());
	ft::vector<int> vector_cpy (vector_range);

	return (0);
}