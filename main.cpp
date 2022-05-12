#ifdef STD
 #include <vector>
 #include <map>
 #include <stack>
 namespace ft = std;
#else
 #include "vector.hpp"
 #include "map.hpp"
 #include "stack.hpp"
#endif

#include <string>
#include <iostream>

class Observable {
	public:
		int _data;
		Observable() { _data = 0; std::cout << "Instance of Observable created." << std::endl; }
		Observable(int data): _data(data) { std::cout << "Instance of Observable created." << std::endl; }
		Observable(Observable const & src) { *this = src; }
		~Observable() { std::cout << "Instance of Observable destroyed." << std::endl; }
		Observable &operator=(Observable const &rhs) { _data = rhs._data; return *this; }
};
std::ostream &operator<<(std::ostream & os, const Observable &obs) {
	os << obs._data;
	return os;
}

template <class T>
void print_content(ft::vector<T> v)
{
	std::cout << "size: " << v.size() << '\n';
	for (typename ft::vector<T>::iterator it = v.begin(); it != v.end(); ++it)
		std::cout << *it << ' ';
	for (typename ft::vector<T>::reverse_iterator rit = v.rbegin(); rit != v.rend(); ++rit)
		std::cout << *rit << ' ';
	for (size_t i = 0; i < v.size(); ++i)
		std::cout << v[i] << v.at(i) << ' ';
	if (!v.empty())
		std::cout << v.front() << ' ' << v.back() << ' ';
	std::cout << std::endl;
}

int
	main(void)
{
	//VECTOR

	ft::vector<int>			v_int;
	ft::vector<int>			v_int_extra (10, 2);

	ft::vector<double>		v_double (200, 42.42);
	ft::vector<std::string>	v_str1 (0, "abc");
	ft::vector<std::string>	v_str2 (44, "123");
	ft::vector<std::string>	v_str3 (22, "xyz");

	std::vector<char>		tmp1 (50, 'f');
	ft::vector<char>		v_char (tmp1.begin(), tmp1.end());

	ft::vector<Observable>	v_obs (32);
	ft::vector<Observable>	v_obs_cpy (v_obs);

	print_content(v_int);
	print_content(v_double);
	print_content(v_str1);
	print_content(v_char);
	print_content(v_obs);
	print_content(v_obs_cpy);
	print_content(v_str2);
	print_content(v_str3);
	v_str2 = v_str3;
	print_content(v_str2);
	v_obs.resize(0);
	print_content(v_obs);
	v_obs.resize(10);
	print_content(v_obs);
	v_obs.reserve(100);
	std::cout << v_obs.capacity() << std::endl;

	std::vector<int> tmp2;
	for (int i = 0; i < 100; ++i)
		tmp2.push_back(i * 3);
	v_int.assign(tmp2.begin(), tmp2.end());
	print_content(v_int);
	v_int.assign(50, 999);
	print_content(v_int);
	for (int i = 0; i < 15; ++i)
		v_int.push_back(72 + i * 2);
	print_content(v_int);
	for (int i = 0; i < 5; ++i)
		v_int.pop_back();
	print_content(v_int);
	v_int.erase(v_int.begin());
	v_int.erase(v_int.end() - 1);
	print_content(v_int);
	v_int.erase(v_int.begin() + 1, v_int.end() - 1);
	print_content(v_int);
	v_int.swap(v_int_extra);
	print_content(v_int);
	print_content(v_int_extra);
	v_obs.clear();
	print_content(v_obs);

	return 0;
}
