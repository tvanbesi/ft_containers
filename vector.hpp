#ifndef VECTOR_HPP
# define VECTOR_HPP

# include <memory>
# include "type_traits.hpp"

// REMOVE WHEN DONE WITH TESTING //
# include <iostream>
// REMOVE WHEN DONE WITH TESTING //

namespace ft {

	template <class T, class Alloc = std::allocator<T> >
	class vector {

	public:

		typedef				T								value_type;
		typedef				Alloc							allocator_type;
		typedef typename	allocator_type::reference		reference;
		typedef typename	allocator_type::const_reference	const_reference;
		typedef typename	allocator_type::pointer			pointer;
		typedef typename	allocator_type::const_pointer	const_pointer;
		typedef				size_t							size_type;
	
		explicit vector
			(const allocator_type& alloc = allocator_type())
		: _head(0), _size(0), _alloc(alloc)
		{
			std::cout << "made an empty vector" << std::endl;
		}

		explicit vector
			(size_type n, const value_type& val = value_type(),
			const allocator_type& alloc = allocator_type())
		: _size(n), _alloc(alloc)
		{
			_head = _alloc.allocate(_size);
			for (size_type i = 0; i < _size; i++) { _head[i] = val; }
			std::cout << "made an filled vector" << std::endl;
		}

		template	<class InputIterator>
		vector
			(InputIterator first, InputIterator last,	
			const allocator_type& alloc = allocator_type(),
			typename enable_if<!is_integral<InputIterator>::value>::type = 0)
		: _alloc(alloc)
		{
			std::cout << "made a range vector" << std::endl;
		}

		~vector()
		{
			std::cout << "destroying vector" << std::endl;
			_alloc.deallocate(_head, _size);
		}

	private:

		pointer			_head;
		size_type		_size;
		allocator_type	_alloc;

	};

}

#endif