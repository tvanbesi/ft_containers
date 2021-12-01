#ifndef VECTOR_HPP
# define VECTOR_HPP

# include <memory>

// REMOVE WHEN DONE WITH TESTING //
# include <iostream>
// REMOVE WHEN DONE WITH TESTING //

namespace ft {
	template <class T, class Alloc = std::allocator<T> >
	class vector {

	typedef				T								value_type;
	typedef				Alloc							allocator_type;
	typedef typename	allocator_type::reference		reference;
	typedef typename	allocator_type::const_reference	const_reference;
	typedef typename	allocator_type::pointer			pointer;
	typedef typename	allocator_type::const_pointer	const_pointer;
	typedef				size_t							size_type;
	
	public:

		explicit vector
			(const allocator_type& alloc = allocator_type())
		: _head(0), _size(0)
		{
		}

		/*
		**	Why allocator parameter is const ref? So that you don't have to make sure the allocator
		**	not destroyed and can just copy it and then use it
		*/
		explicit vector
			(size_type n, const value_type& val = value_type(),
			const allocator_type& alloc = allocator_type())
		: _size(n)
		{
			allocator_type allocator = alloc;

			_head = allocator.allocate(_size);
			for (size_type i = 0; i < _size; i++) { _head[i] = val; } // incrementer le pointeur pour plus de speed
		}

		~vector()
		{
			allocator_type allocator = allocator_type();

			allocator.deallocate(_head, _size);
		}

	protected:
	
	private:

		pointer			_head;
		size_type		_size;

	};
}

#endif