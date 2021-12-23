#ifndef REVERSE_ITERATOR_HPP
# define REVERSE_ITERATOR_HPP

namespace ft {

	template<class Iterator>
	class reverse_iterator {

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

		reverse_iterator() : _base(iterator_type()) {}
		explicit reverse_iterator(iterator_type it) : _base(it) {}
		template <class Iter>
		reverse_iterator(const reverse_iterator<Iter>& rev_it) : _base(rev_it.base()) {}

		iterator_type base() const { return _base; }

		reference operator*() const
		{
			iterator_type tmp = _base;
			return *(--tmp);
		}
		reverse_iterator operator+(difference_type n) const { return reverse_iterator(_base - n); }
		reverse_iterator& operator++() { --_base; return *this; }
		reverse_iterator operator++(int)
		{
			reverse_iterator tmp = *this;
			++(*this);
			return tmp;
		}
		reverse_iterator& operator+=(difference_type n) { _base -= n; return *this; }
		reverse_iterator operator-(difference_type n) const { return reverse_iterator(_base + n); }
		reverse_iterator& operator--() { ++_base; return *this; }
		reverse_iterator operator--(int)
		{
			reverse_iterator tmp = *this;
			--(*this);
			return tmp;
		}
		reverse_iterator& operator-=(difference_type n) { _base += n; return *this; }
		pointer operator->() const { return &(operator*()); }
		reference operator[](difference_type n) const { return _base[-n - 1]; }

		difference_type operator-(reverse_iterator rhs) const { return rhs.base() - _base; }

		/*
		**	Non-member functions
		*/

		template <class Iter>
		bool operator==(const reverse_iterator<Iter>& rhs) const { return _base == rhs.base(); }
		template <class Iter>
		bool operator!=(const reverse_iterator<Iter>& rhs) const { return _base != rhs.base(); }
		template <class Iter>
		bool operator<(const reverse_iterator<Iter>& rhs) const { return _base > rhs.base(); }
		template <class Iter>
		bool operator<=(const reverse_iterator<Iter>& rhs) const { return _base >= rhs.base(); }
		template <class Iter>
		bool operator>(const reverse_iterator<Iter>& rhs) const { return _base < rhs.base(); }
		template <class Iter>
		bool operator>=(const reverse_iterator<Iter>& rhs) const { return _base <= rhs.base(); }

		friend reverse_iterator operator+(difference_type n, const reverse_iterator& rev_it) { return rev_it + n; }
		friend reverse_iterator operator-(difference_type n, const reverse_iterator& rev_it) { return rev_it - n; }

	private:

		iterator_type	_base;

	};

}

#endif