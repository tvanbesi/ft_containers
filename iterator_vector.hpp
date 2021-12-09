#ifndef ITERATOR_VECTOR_HPP
# define ITERATOR_VECTOR_HPP

# include "iterator.hpp"
# include "type_traits.hpp"

namespace ft {

	template <class T>
	class iterator_vector : public ft::iterator<std::random_access_iterator_tag, T> {
	friend class iterator_vector<typename remove_const<T>::value_type>;
	friend class iterator_vector<const T>;

	public:

		typedef				ft::iterator<std::random_access_iterator_tag, T>	iterator;
		typedef				ft::iterator_traits<iterator>						iterator_traits;
		typedef typename	iterator_traits::difference_type					difference_type;
		typedef typename	iterator_traits::value_type							value_type;
		typedef typename	iterator_traits::pointer							pointer;
		typedef typename	iterator_traits::reference							reference;
		typedef typename	iterator_traits::iterator_category					iterator_category;

		/*
		** Member functions
		*/

		iterator_vector(pointer p = 0) : _p(p) {}
		~iterator_vector(void) {}

		operator iterator_vector<const value_type>() const { return _p; }

		iterator_vector& operator++() { ++_p; return *this; }
		iterator_vector operator++(int)
		{
			iterator_vector r = *this;
			++_p;
			return r;
		}
		iterator_vector& operator--() { --_p; return *this; }
		iterator_vector operator--(int)
		{
			iterator_vector r = *this;
			--_p;
			return r;
		}
		iterator_vector operator+(size_t n) const { return iterator_vector(_p + n); }
		iterator_vector operator-(size_t n) const { return iterator_vector(_p - n); }
		iterator_vector& operator+=(size_t n) { _p += n; return *this; }
		iterator_vector& operator-=(size_t n) { _p -= n; return *this; }
		difference_type operator-(iterator_vector rhs) const { return _p - rhs._p; }
		reference operator*() const { return *_p; }
		pointer operator->() const { return _p; }
		reference operator[](size_t n) const { return _p[n]; }
		template <typename U>
		bool operator==(const iterator_vector<U>& rhs) const { return _p == rhs._p; }
		template <typename U>
		bool operator!=(const iterator_vector<U>& rhs) const { return _p != rhs._p; }
		template <typename U>
		bool operator<(const iterator_vector<U>& rhs) const { return _p < rhs._p; }
		template <typename U>
		bool operator>(const iterator_vector<U>& rhs) const { return _p > rhs._p; }
		template <typename U>
		bool operator<=(const iterator_vector<U>& rhs) const { return _p <= rhs._p; }
		template <typename U>
		bool operator>=(const iterator_vector<U>& rhs) const { return _p >= rhs._p; }

		/*
		** Non-member functions
		*/

		friend iterator_vector operator+(size_t n, const iterator_vector& rhs) { return iterator_vector(rhs._p + n); }
		friend iterator_vector operator-(size_t n, const iterator_vector& rhs) { return iterator_vector(rhs._p - n); }

	private:

		pointer	_p;

	};

	template<class Iterator>
	class reverse_iterator_vector : public ft::reverse_iterator<Iterator> {

	public:

		typedef				Iterator											iterator_type;
		typedef typename	ft::iterator_traits<Iterator>::iterator_category	iterator_category;
		typedef typename	ft::iterator_traits<Iterator>::value_type			value_type;
		typedef typename	ft::iterator_traits<Iterator>::difference_type		difference_type;
		typedef typename	ft::iterator_traits<Iterator>::pointer				pointer;
		typedef typename	ft::iterator_traits<Iterator>::reference			reference;

		/*
		**	Member functions
		*/

		reverse_iterator_vector() : _base(iterator_type()) {}
		explicit reverse_iterator_vector(iterator_type it) : _base(it) {}
		template <class Iter>
		reverse_iterator_vector(const reverse_iterator_vector<Iter>& rev_it) : _base(rev_it.base()) {}

		iterator_type base() const { return _base; }

		reference operator*() const { return *(_base - 1); }
		reverse_iterator_vector operator+(difference_type n) const { return reverse_iterator_vector(_base - n); }
		reverse_iterator_vector& operator++() { --_base; return *this; }
		reverse_iterator_vector operator++(int)
		{
			reverse_iterator_vector tmp = *this;
			++(*this);
			return tmp;
		}
		reverse_iterator_vector& operator+=(difference_type n) { _base -= n; return *this; }
		reverse_iterator_vector operator-(difference_type n) const { return reverse_iterator_vector(_base + n); }
		reverse_iterator_vector& operator--() { ++_base; return *this; }
		reverse_iterator_vector operator--(int)
		{
			reverse_iterator_vector tmp = *this;
			--(*this);
			return tmp;
		}
		reverse_iterator_vector& operator-=(difference_type n) { _base += n; return *this; }
		pointer operator->() const { return &(operator*()); }
		reference operator[](difference_type n) const { return _base[-n - 1]; }

		difference_type operator-(reverse_iterator_vector rhs) const { return rhs.base() - _base; }

		/*
		**	Non-member functions
		*/

		template <class Iter>
		bool operator==(const reverse_iterator_vector<Iter>& rhs) const { return _base == rhs.base(); }
		template <class Iter>
		bool operator!=(const reverse_iterator_vector<Iter>& rhs) const { return _base != rhs.base(); }
		template <class Iter>
		bool operator<(const reverse_iterator_vector<Iter>& rhs) const { return _base > rhs.base(); }
		template <class Iter>
		bool operator<=(const reverse_iterator_vector<Iter>& rhs) const { return _base >= rhs.base(); }
		template <class Iter>
		bool operator>(const reverse_iterator_vector<Iter>& rhs) const { return _base < rhs.base(); }
		template <class Iter>
		bool operator>=(const reverse_iterator_vector<Iter>& rhs) const { return _base <= rhs.base(); }

		friend reverse_iterator_vector operator+(difference_type n, const reverse_iterator_vector& rev_it) { return rev_it + n; }
		friend reverse_iterator_vector operator-(difference_type n, const reverse_iterator_vector& rev_it) { return rev_it - n; }

	private:

		iterator_type	_base;

	};

}

#endif