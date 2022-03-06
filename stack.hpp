#ifndef STACK_HPP
# define STACK_HPP

# include "vector.hpp"

namespace ft {

	template < class T, class Container = ft::vector<T> >
	class stack {

	public:

		/*
		**	Member types
		*/

		typedef		T				value_type;
		typedef		Container		container_type;
		typedef		size_t			size_type;

		/*
		**	Constructors, destructor, operators
		*/

		explicit stack (const container_type& ctnr = container_type()) : _ctnr(ctnr) {}

		~stack() {}

		/*
		**	Member functions
		*/

		bool empty() const { return _ctnr.empty(); }

		size_type size() const { return _ctnr.size(); }

		value_type & top() { return _ctnr.back(); }
		const value_type & top() const { return _ctnr.back(); }

		void push(const value_type & val) { _ctnr.push_back(val); }

		void pop() { _ctnr.pop_back(); }

		/*
		**	Non-member function overloads
		*/

		friend bool operator==(const stack & lhs, const stack & rhs) { return lhs._ctnr == rhs._ctnr; }
		friend bool operator!=(const stack & lhs, const stack & rhs) { return lhs._ctnr != rhs._ctnr; }
		friend bool operator<(const stack & lhs, const stack & rhs) { return lhs._ctnr < rhs._ctnr; }
		friend bool operator<=(const stack & lhs, const stack & rhs) { return lhs._ctnr <= rhs._ctnr; }
		friend bool operator>(const stack & lhs, const stack & rhs) { return lhs._ctnr > rhs._ctnr; }
		friend bool operator>=(const stack & lhs, const stack & rhs) { return lhs._ctnr >= rhs._ctnr; }

	private:

		/*
		**	Private member variables
		*/

		container_type	_ctnr;

	};

}

#endif