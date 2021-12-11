#ifndef ITERATOR_MAP_HPP
# define ITERATOR_MAP_HPP

# include "iterator.hpp"
# include "node.hpp"

namespace ft {

	template <class T>
	class iterator_map : public ft::iterator<std::bidirectional_iterator_tag, T> {

	public:

		typedef				ft::iterator<std::bidirectional_iterator_tag, T>	iterator;
		typedef				ft::iterator_traits<iterator>						iterator_traits;
		typedef typename	iterator_traits::difference_type					difference_type;
		typedef typename	iterator_traits::value_type							value_type;
		typedef typename	iterator_traits::pointer							pointer;
		typedef typename	iterator_traits::reference							reference;
		typedef typename	iterator_traits::iterator_category					iterator_category;
		typedef typename	value_type::first_type								key_type;
		typedef typename	value_type::second_type								mapped_type;
		typedef				Node<key_type, mapped_type>							node;
		typedef				node*												node_pointer;

		/*
		**	Member functions
		*/

		iterator_map(node_pointer root = 0) : _root(root) {}
		~iterator_map() {} 

		reference operator*() const { return *_root->content; }
		pointer operator->() const { return &(*_root->content); }

	private:

		node_pointer	_root;

	};

}

#endif