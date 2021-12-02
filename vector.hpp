#ifndef VECTOR_HPP
# define VECTOR_HPP

# include <memory>

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
		: _head(0), _size(0), _allocator(alloc)
		{
			std::cout << "MADE AN EMPTY VECTOR" << std::endl;
		}

		/*
		**	Why allocator parameter is const ref? So that you don't have to make sure the allocator
		**	not destroyed and can just copy it and then use it
		*/
		explicit vector
			(size_type n, const value_type& val = value_type(),
			const allocator_type& alloc = allocator_type())
		//: _size(n), _allocator(alloc)
		: _size(n)
		{
			std::cout << "HELLO" << std::endl;
			_allocator = alloc;
			_head = _allocator.allocate(_size);
			for (size_type i = 0; i < _size; i++) { _head[i] = val; } // incrementer le pointeur pour plus de speed
			std::cout << "MADE AN FILLED VECTOR" << std::endl;
			for (int i = 0; i < _size; i++) { std::cout << _head[i] << "\t" << &_head[i] << std::endl; }
		}

		template <class InputIterator>
		vector
			(InputIterator first, InputIterator last,
			const allocator_type& alloc = allocator_type())
		{

		}

		~vector()
		{
			//_allocator.deallocate(_head, _size);
		}

	private:

		pointer			_head;
		size_type		_size;
		allocator_type	_allocator;

	};

}

#endif