#ifndef ITERATOR_VECTOR_HPP
# define ITERATOR_VECTOR_HPP

# include "iterator.hpp"

namespace ft {

	template <class T>
	class iterator_vector : public ft::iterator<std::random_access_iterator_tag, T> {

	public:

		typedef				ft::iterator<std::random_access_iterator_tag, T>	iterator;
		typedef				ft::iterator_traits<iterator>					iterator_traits;
		typedef typename	iterator_traits::difference_type				difference_type;
		typedef typename	iterator_traits::value_type						value_type;
		typedef typename	iterator_traits::pointer						pointer;
		typedef typename	iterator_traits::reference						reference;
		typedef typename	iterator_traits::iterator_category				iterator_category;

		iterator_vector(pointer p = 0) : _p(p) {}
		~iterator_vector(void) {}

		operator iterator_vector<const value_type>() { return _p; }

		iterator_vector& operator++() { ++_p; return *this; }
		iterator_vector& operator--() { --_p; return *this; }
		iterator_vector operator+(size_t n) const { return iterator_vector(_p + n); }
		iterator_vector operator-(size_t n) const { return iterator_vector(_p - n); }
		difference_type operator-(iterator_vector rhs) const { return _p - rhs._p; }
		reference operator*(void) const { return *_p; }
		reference operator[](size_t n) const { return _p[n]; }
		bool operator==(const iterator_vector& rhs) const { return _p == rhs._p; }
		bool operator!=(const iterator_vector& rhs) const { return _p != rhs._p; }
		bool operator>=(const iterator_vector& rhs) const { return _p >= rhs._p; }

	private:

		pointer	_p;

	};

}

#endif