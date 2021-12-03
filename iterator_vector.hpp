#ifndef ITERATOR_VECTOR_HPP
# define ITERATOR_VECTOR_HPP

# include "iterator.hpp"

namespace ft {

	template <class T>
	class iterator_vector : public ft::iterator<ft::random_access_iterator_tag, T> {

	public:

		typedef				ft::iterator<ft::random_access_iterator_tag, T>	iterator;
		typedef				ft::iterator_traits<iterator>					iterator_traits;
		typedef typename	iterator_traits::difference_type				difference_type;
		typedef typename	iterator_traits::value_type						value_type;
		typedef typename	iterator_traits::pointer						pointer;
		typedef typename	iterator_traits::reference						reference;
		typedef typename	iterator_traits::iterator_category				iterator_category;

		iterator_vector(pointer p) : _p(p) {}
		~iterator_vector(void) {}

		iterator_vector& operator++() { ++_p; return *this; }
		difference_type operator-(iterator_vector rhs) const { return _p - rhs._p; }
		reference operator*(void) const { return *_p; }
		bool operator!=(const iterator_vector& rhs) const { return _p != rhs._p; }

	private:

		pointer	_p;

	};

}

#endif