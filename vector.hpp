#ifndef VECTOR_HPP
# define VECTOR_HPP

# include <memory>
# include "type_traits.hpp"
# include "iterator.hpp"
# include "iterator_vector.hpp"

// REMOVE WHEN DONE WITH TESTING //
# include <iostream>

namespace ft {

	/*
	**	Template prototypes
	*/

	template	<class T,
				class Alloc
				> class vector;

	/*
	**	vector class template
	*/

	template <class T, class Alloc = std::allocator<T> >
	class vector {

	public:

		/*
		**	Member types
		*/

		typedef				T															value_type;
		typedef				Alloc														allocator_type;
		typedef typename	allocator_type::reference									reference;
		typedef typename	allocator_type::const_reference								const_reference;
		typedef typename	allocator_type::pointer										pointer;
		typedef typename	allocator_type::const_pointer								const_pointer;
		typedef typename	ft::iterator_vector<value_type>								iterator;
		typedef typename	ft::iterator_vector<const value_type>						const_iterator;
		//typedef typename	ft::reverse_iterator<iterator>								reverse_iterator;
		//typedef typename	ft::reverse_iterator<const_iterator>						const_reverse_iterator;
		typedef typename	ft::iterator_traits<iterator>::difference_type				difference_type;
		typedef				size_t														size_type;
	
		/*
		**	Constructors, destructor, operators
		*/

		explicit vector
			(const allocator_type& alloc = allocator_type())
		: _size(0), _capacity(0), _alloc(alloc)
		{
			_vector = _alloc.allocate(_size);
			std::cout << "made an empty vector" << std::endl;
		}

		explicit vector
			(size_type n, const value_type& val = value_type(),
			const allocator_type& alloc = allocator_type())
		: _size(n), _capacity(n), _alloc(alloc)
		{
			_vector = _alloc.allocate(_size);
			for (size_type i = 0; i < _size; ++i) { _alloc.construct(&_vector[i], val); }
			std::cout << "made an filled vector" << std::endl;
		}

		template	<class InputIterator>
		vector
			(InputIterator first, InputIterator last,	
			const allocator_type& alloc = allocator_type(),
			typename enable_if<!is_integral<InputIterator>::value>::type* = 0)
		: _size(last - first), _capacity(last - first), _alloc(alloc)
		{
			_vector = _alloc.allocate(_size);
			for (size_type i = 0; i < _size; ++i, ++first) { _alloc.construct(&_vector[i], *first); }
			std::cout << "made a range vector" << std::endl;
		}

		vector
			(const vector& x)
		: _size(x._size), _capacity(x._capacity), _alloc(x._alloc)
		{
			_vector = _alloc.allocate(_size);
			const_iterator first = x.begin();
			for	(size_type i = 0; i < _size; ++i, ++first) { _alloc.construct(&_vector[i], *first); }
			std::cout << "made a copied vector" << std::endl;
		}

		vector& operator= (const vector& x)
		{
			this->assign(x.begin(), x.end());
			return *this;
		}

		~vector()
		{
			std::cout << "destroying vector" << std::endl;
			for (size_type i = 0; i < _size; ++i) { _alloc.destroy(&_vector[i]); }
			_alloc.deallocate(_vector, _capacity);
		}

		/*
		**	Iterators
		*/

		iterator begin() { return iterator(_vector); }
		const_iterator begin() const { return const_iterator(_vector); }
		iterator end() { return iterator(_vector + _size); }
		const_iterator end() const { return const_iterator(_vector + _size); }

		/*
		**	Capacity
		*/

		size_type size() const { return _size; }

		size_type max_size() const { return _alloc.max_size(); }

		void
			resize(size_type n, value_type val = value_type())
		{
			if (n < _size)
			{
				for (size_type i = n; i < _size; ++i) { _alloc.destroy(&_vector[i]); }
			}
			else if (n > _size)
			{
				if (n > _capacity) { reallocate(n); }
				for (size_type i = _size; i < n; ++i) { _vector[i] = val; }
			}
			_size = n;
		}

		size_type capacity() const { return _capacity; }

		bool empty() const { return _size == 0; }

		void
			reserve(size_type n)
		{
			if (n > _capacity) { reallocate(n); }
		}

		/*
		**	Modifiers
		*/

		template <class InputIterator>
		void
			assign(InputIterator first, InputIterator last,
			typename enable_if<!is_integral<InputIterator>::value>::type* = 0)
		{
			for (size_type i = 0; i < _size; ++i) { _alloc.destroy(&_vector[i]); }
			size_type new_size = last - first;
			if (new_size > _capacity) { _alloc.deallocate(_vector, _capacity); _vector = _alloc.allocate(new_size); }
			_size = new_size;
			for (size_type i = 0; i < _size; ++i, ++first) { _alloc.construct(&_vector[i], *first); }
		}

		void
			assign(size_type n, const value_type& val)
		{
			for (size_type i = 0; i < _size; ++i) { _alloc.destroy(&_vector[i]); }
			if (n > _capacity) { _alloc.deallocate(_vector, _capacity); _vector = _alloc.allocate(n); }
			_size = n;
			for (size_type i = 0; i < _size; ++i) { _vector[i] = val; }
		}

	private:

		/*
		**	Private member variables
		*/

		pointer			_vector;
		size_type		_size;
		size_type		_capacity;
		allocator_type	_alloc;

		/*
		**	Private member fonctions
		*/

		void
			reallocate(size_type n)
		{
			pointer tmp = _alloc.allocate(n); //see notes.txt, logarithmic growth
			for (size_type i = 0; i < _size; ++i) { _alloc.construct(&tmp[i], _vector[i]); }
			this->~vector();
			_capacity = n;
			_vector = tmp;
		}

	};

}

#endif