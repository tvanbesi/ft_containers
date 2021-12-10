#ifndef MAP_HPP
# define MAP_HPP

# include "pair.hpp"

namespace ft {

	/*
	**	map class template
	*/

	template	< class Key,
				class T,
				class Compare = std::less<Key>,
				class Alloc = std::allocator<pair<const Key, T> >
				>
	class map {

	public:

		/*
		**	Member types
		*/

		typedef				Key									key_type;
		typedef				T									mapped_type;
		typedef				pair<const key_type, mapped_type>	value_type;
		typedef				Compare								key_compare;
		typedef				Alloc								allocator_type;
		typedef typename	allocator_type::reference			reference;
		typedef typename	allocator_type::const_reference		const_reference;
		typedef typename	allocator_type::pointer				pointer;
		typedef typename	allocator_type::const_pointer		const_pointer;
		//iterator
		//const_iterator
		//reverse_iterator
		//const_reverse_iterator
		//difference_type
		typedef				size_t								size_type;

	private:

	};

}

#endif