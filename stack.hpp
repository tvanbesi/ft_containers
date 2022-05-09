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

		explicit stack (const container_type& ctnr = container_type()) : c(ctnr) {}

		~stack() {}

		/*
		**	Member functions
		*/

		bool empty() const { return c.empty(); }

		size_type size() const { return c.size(); }

		value_type & top() { return c.back(); }
		const value_type & top() const { return c.back(); }

		void push(const value_type & val) { c.push_back(val); }

		void pop() { c.pop_back(); }

		/*
		**	Non-member function overloads
		*/

		friend bool operator==(const stack & lhs, const stack & rhs) { return lhs.c == rhs.c; }
		friend bool operator!=(const stack & lhs, const stack & rhs) { return lhs.c != rhs.c; }
		friend bool operator<(const stack & lhs, const stack & rhs) { return lhs.c < rhs.c; }
		friend bool operator<=(const stack & lhs, const stack & rhs) { return lhs.c <= rhs.c; }
		friend bool operator>(const stack & lhs, const stack & rhs) { return lhs.c > rhs.c; }
		friend bool operator>=(const stack & lhs, const stack & rhs) { return lhs.c >= rhs.c; }

		container_type	c;

	};

}

#endif