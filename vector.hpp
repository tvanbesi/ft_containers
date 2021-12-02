#ifndef VECTOR_HPP
# define VECTOR_HPP

# include <memory>
# include "type_traits.hpp"
# include "iterator.hpp"

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

		typedef				T																value_type;
		typedef				Alloc															allocator_type;
		typedef typename	allocator_type::reference										reference;
		typedef typename	allocator_type::const_reference									const_reference;
		typedef typename	allocator_type::pointer											pointer;
		typedef typename	allocator_type::const_pointer									const_pointer;
		typedef typename	ft::iterator<ft::random_access_iterator_tag, value_type>		iterator;
		typedef typename	ft::iterator<ft::random_access_iterator_tag, const value_type>	const_iterator;
		typedef typename	ft::reverse_iterator<iterator>									reverse_iterator;
		typedef typename	ft::reverse_iterator<const_iterator>							const_reverse_iterator;
		typedef typename	ft::iterator_traits<iterator>::difference_type					difference_type;
		typedef				size_t															size_type;
	
		/*
		**	Constructors and destructor
		*/

		explicit vector
			(const allocator_type& alloc = allocator_type())
		: _vector(0), _size(0), _alloc(alloc)
		{
			std::cout << "made an empty vector" << std::endl;
		}

		explicit vector
			(size_type n, const value_type& val = value_type(),
			const allocator_type& alloc = allocator_type())
		: _size(n), _alloc(alloc)
		{
			_vector = _alloc.allocate(_size);
			for (size_type i = 0; i < _size; i++) { _vector[i] = val; }
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

		vector
			(const vector& x)
		{
			std::cout << "made a copied vector" << std::cout;
		}

		~vector()
		{
			std::cout << "destroying vector" << std::endl;
			_alloc.deallocate(_vector, _size);
		}

	private:

		pointer			_vector;
		size_type		_size;
		allocator_type	_alloc;

	};

}

#endif