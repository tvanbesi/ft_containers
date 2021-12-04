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
	ft::vector<int>	v1;
	ft::vector<int>	v2 (5, 42);
	ft::vector<int>	v3 (v2.begin(), v2.end());
	ft::vector<int>	v4 (v3);
	ft::vector<int> v5 = v4;
	ft::vector<int>	v6;
	ft::vector<int>	v7;
	ft::vector<int>	v8;

	std::cout << "v1 content: ";
	for (ft::vector<int>::iterator it = v1.begin(); it != v1.end(); ++it)
		std::cout << ' ' << *it;
	std::cout << std::endl;
	std::cout << "v2 content: ";
	for (ft::vector<int>::iterator it = v2.begin(); it != v2.end(); ++it)
		std::cout << ' ' << *it;
	std::cout << std::endl;
	std::cout << "v3 content: ";
	for (ft::vector<int>::iterator it = v3.begin(); it != v3.end(); ++it)
		std::cout << ' ' << *it;
	std::cout << std::endl;
	std::cout << "v4 content: ";
	for (ft::vector<int>::iterator it = v4.begin(); it != v4.end(); ++it)
		std::cout << ' ' << *it;
	std::cout << std::endl;
	std::cout << "v5 content: ";
	for (ft::vector<int>::iterator it = v5.begin(); it != v5.end(); ++it)
		std::cout << ' ' << *it;
	std::cout << std::endl;
	v6.assign(v5.begin(), v5.end());
	std::cout << "v6 content: ";
	for (ft::vector<int>::iterator it = v6.begin(); it != v6.end(); ++it)
		std::cout << ' ' << *it;
	std::cout << std::endl;
	v7.assign(5, 42);
	std::cout << "v7 content: ";
	for (ft::vector<int>::iterator it = v7.begin(); it != v7.end(); ++it)
		std::cout << ' ' << *it;
	std::cout << std::endl;
	v8 = v7;
	std::cout << "v8 content: ";
	for (ft::vector<int>::iterator it = v8.begin(); it != v8.end(); ++it)
		std::cout << ' ' << *it;
	std::cout << std::endl;
	std::cout << "v8.resize(10, 69)" << std::endl;
	v8.resize(10, 69);
	std::cout << "v8 content: ";
	for (ft::vector<int>::iterator it = v8.begin(); it != v8.end(); ++it)
		std::cout << ' ' << *it;
	std::cout << std::endl;
	std::cout << "v8.resize(14)" << std::endl;
	v8.resize(14);
	std::cout << "v8 content: ";
	for (ft::vector<int>::iterator it = v8.begin(); it != v8.end(); ++it)
		std::cout << ' ' << *it;
	std::cout << std::endl;

	std::vector<char> v_char;
	std::vector<int> v_int;
	std::vector<double> v_double;
	std::vector<unsigned long long int> v_lli;

	std::cout << v_char.max_size() << std::endl;
	std::cout << v_int.max_size() << std::endl;
	std::cout << v_double.max_size() << std::endl;
	std::cout << v_lli.max_size() << std::endl;

	ft::vector<char> ft_v_char;
	ft::vector<int> ft_v_int;
	ft::vector<double> ft_v_double;
	ft::vector<unsigned long long int> ft_v_lli;

	std::cout << ft_v_char.max_size() << std::endl;
	std::cout << ft_v_int.max_size() << std::endl;
	std::cout << ft_v_double.max_size() << std::endl;
	std::cout << ft_v_lli.max_size() << std::endl;

	return (0);
}