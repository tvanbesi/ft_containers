#ifndef PAIR_HPP
# define PAIR_HPP

namespace ft {

	/*
	**	pair class template
	*/

	template <class T1, class T2>
	struct pair
	{

		/*
		**	Member types
		*/

		typedef	T1	first_type;
		typedef	T2	second_type;

		/*
		**	Member variables
		*/

		first_type	first;
		second_type	second;

		/*
		**	Public member functions
		*/

		pair() : first(first_type()), second(second_type()) {}
		pair(const pair& pr) : first(pr.first), second(pr.second) {}
		pair(const first_type& a, const second_type& b) : first(a), second(b) {}

		pair& operator=(const pair& pr) { first = pr.first; second = pr.second; return *this; }

	};

}

#endif