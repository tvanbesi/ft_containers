#ifndef VECTOR_HPP
# define VECTOR_HPP

# include <memory>
# include <stdexcept>
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
		}

		explicit vector
			(size_type n, const value_type& val = value_type(),
			const allocator_type& alloc = allocator_type())
		: _size(n), _capacity(n), _alloc(alloc)
		{
			_vector = _alloc.allocate(_size);
			for (size_type i = 0; i < _size; ++i) { _alloc.construct(&_vector[i], val); }
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
		}

		vector
			(const vector& x)
		: _size(x._size), _capacity(x._capacity), _alloc(x._alloc)
		{
			_vector = _alloc.allocate(_size);
			const_iterator first = x.begin();
			for	(size_type i = 0; i < _size; ++i, ++first) { _alloc.construct(&_vector[i], *first); }
		}

		vector& operator= (const vector& x)
		{
			this->assign(x.begin(), x.end());
			return *this;
		}

		~vector()
		{
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
		**	Element access
		*/

		reference operator[](size_type n) { return _vector[n]; }
		const_reference operator[] (size_type n) const { return _vector[n]; }
		reference at(size_type n)
		{
			if (n < 0 || n > _size - 1) { throw std::out_of_range(""); }
			return _vector[n];
		}
		const_reference at(size_type n) const
		{
			if (n < 0 || n > _size - 1) { throw std::out_of_range(""); }
			return _vector[n];
		}
		reference front() { return _vector[0]; }
		const_reference front() const { return _vector[0]; }
		reference back() { return _vector[_size - 1]; }
		const_reference back() const { return _vector[_size - 1]; }

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

		void
			push_back(const value_type& val)
		{
			if (_size + 1 > _capacity) { reallocate(_size + 1); }
			_vector[_size] = val;
			_size++;
		}

		void
			pop_back()
		{
			_alloc.destroy(&(this->back()));
			_size--;
		}

		iterator
			insert(iterator position, const value_type& val)
		{
			if (position == this->end()) { this->push_back(val); return --(this->end()); }
			else
			{
				difference_type n_move = this->end() - position;
				if (_size + 1 > _capacity)
					reallocate(_size + 1);
				pointer previous = &_vector[_size - 1];
				pointer end = previous + 1;
				for (difference_type i = 0; i < n_move; ++i)
				{
					_alloc.construct(end, *previous);
					_alloc.destroy(previous);
					--end;
					--previous;
				}
				*end = val;
				++_size;
				return iterator(end);
			}
		}

		void
			insert(iterator position, size_type n, const value_type& val)
		{
			if (position == this->end())
			{
				if (_size + n > _capacity)
					reallocate(_size + n);
				for (size_type i = 0; i < n; ++i) { this->push_back(val); }
			}
			else
			{
				difference_type n_move = this->end() - position;
				if (_size + n > _capacity)
					reallocate(_size + n);
				pointer previous = &_vector[_size - 1];
				pointer end = previous + n;
				for (difference_type i = 0; i < n_move; ++i)
				{
					_alloc.construct(end, *previous);
					_alloc.destroy(previous);
					--end;
					--previous;
				}
				++previous;
				for (size_type i = 0; i < n; ++i, ++previous)
					*previous = val;
				_size += n;
			}
		}

		template <class InputIterator>
		void
			insert(iterator position, InputIterator first, InputIterator last,
			typename enable_if<!is_integral<InputIterator>::value>::type* = 0)
		{
			size_type n = last - first;
			if (position == this->end())
			{
				if (_size + n > _capacity)
					reallocate(_size + n);
				while (first != last)
				{
					this->push_back(*first);
					++first;
				}
			}
			else
			{
				difference_type n_move = this->end() - position;
				if (_size + n > _capacity)
					reallocate(_size + n);
				pointer previous = &_vector[_size - 1];
				pointer end = previous + n;
				for (difference_type i = 0; i < n_move; ++i)
				{
					_alloc.construct(end, *previous);
					_alloc.destroy(previous);
					--end;
					--previous;
				}
				++previous;
				while (first != last)
				{
					*previous = *first;
					++previous;
					++first;
				}
				_size += n;
			}
		}

		iterator
			erase(iterator position)
		{
			iterator last = this->end();
			if (position == last - 1) { this->pop_back(); }
			else
			{
				iterator next = position + 1;
				while (next != last)
				{
					_alloc.destroy(&(*position));
					_alloc.construct(&(*position), *next);
					++position;
					++next;
				}
				_alloc.destroy(&(*position));
				_size--;
			}
			return --position;
		}

		iterator
			erase(iterator first, iterator last)
		{
			iterator end = this->end();
			if (last == end)
			{
				while (first != end) { _alloc.destroy(&(*first)); ++first; --_size; }
				return this->end();
			}
			else
			{
				iterator r = last;
				_size -= last - first;
				while (last != end)
				{
					_alloc.destroy(&(*first));
					_alloc.construct(&(*first), *last);
					++first;
					++last;
				}
				while (first != end) { _alloc.destroy(&(*first)); ++first; }
				return r;
			}
		}

		void
			swap(vector& x)
		{
			pointer tmp_vector = _vector;
			size_type tmp_size = _size;
			size_type tmp_capacity = _capacity;
			_vector = x._vector;
			_size = x._size;
			_capacity = x._capacity;
			x._vector = tmp_vector;
			x._size = tmp_size;
			x._capacity = tmp_capacity;
		}

		void
			clear()
		{
			for (size_type i = 0; i < _size; ++i) { _alloc.destroy(&_vector[i]); }
			_size = 0;
		}

		/*
		**	Allocator
		*/

		allocator_type get_allocator() const { return _alloc; }

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
			size_type new_capacity = _capacity == 0 ? 1 : _capacity * 2;
			while (new_capacity < n)
				new_capacity *= 2;
			pointer tmp = _alloc.allocate(new_capacity);
			for (size_type i = 0; i < _size; ++i) { _alloc.construct(&tmp[i], _vector[i]); }
			this->~vector();
			_capacity = new_capacity;
			_vector = tmp;
		}

	};

}

#endif