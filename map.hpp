#ifndef MAP_HPP
# define MAP_HPP

# define	LEFT	0
# define	RIGHT	1

# include <functional>

# include "pair.hpp"
# include "node.hpp"
# include "iterator_map.hpp"

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

		typedef				Key												key_type;
		typedef				T												mapped_type;
		typedef				pair<const key_type, mapped_type>				value_type;
		typedef				Compare											key_compare;
		typedef				Alloc											allocator_type;
		typedef typename	allocator_type::reference						reference;
		typedef typename	allocator_type::const_reference					const_reference;
		typedef typename	allocator_type::pointer							pointer;
		typedef typename	allocator_type::const_pointer					const_pointer;
		typedef typename	ft::iterator_map<value_type>					iterator;
		//const_iterator
		//reverse_iterator
		//const_reverse_iterator
		//difference_type
		typedef				size_t											size_type;
		typedef				Node<const key_type, mapped_type>				node;
		typedef				node*											node_pointer;
		typedef typename	allocator_type::template rebind<node>::other	alnode;

		/*
		**	Public member functions
		*/

		explicit map (const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type())
		: _root(0), _alloc(alloc), _alnode(alloc), _comp(comp) {}

		template <class InputIterator>
		map	(InputIterator first, InputIterator last, const key_compare& comp = key_compare(),
			const allocator_type& alloc = allocator_type())
		: _root(0), _alloc(alloc), _alnode(alloc), _comp(comp)
		{
			_root = _alnode.allocate(1);
			_root->content = _alloc.allocate(1);
			_alloc.construct(_root->content, *first);
			_root->left_child = 0;
			_root->right_child = 0;
			++first;
			make_bst(first, last);
			//debug_bst_inorder(_root);
		}

		pair<iterator, bool> insert(const value_type& val)
		{
			_root = _alnode.allocate(1);
			_root->content = _alloc.allocate(1);
			_alloc.construct(_root->content, val);
			_root->left_child = 0;
			_root->right_child = 0;
			return ft::make_pair(iterator(_root), true);
		}

		/*
		**	Observers
		*/

		key_compare key_comp() const { return _comp; }

	private:

		/*
		**	Private member variables
		*/

		node_pointer	_root;
		allocator_type	_alloc;
		alnode			_alnode;
		key_compare		_comp;

		/*
		**	Private member functions
		*/

		template <class InputIterator>
		void make_bst(InputIterator first, InputIterator last)
		{
			std::cout << std::boolalpha;
			while (first != last)
			{
				if (!_root->left_child && _comp((*first).first, _root->content->first))
					bst(_root, LEFT, _root->content->first, _root->content->first, first, last);
				if (!_root->right_child && _comp(_root->content->first, (*first).first))
					bst(_root, RIGHT, _root->content->first, _root->content->first, first, last);
				if (_root->left_child && _root->right_child)
					break ;
				++first;
			}
		}

		template <class InputIterator>
		void bst(node_pointer root, int direction, key_type min, key_type max, InputIterator first, InputIterator last)
		{
			node_pointer subtree_root;

			subtree_root = _alnode.allocate(1);
			subtree_root->content = _alloc.allocate(1);
			_alloc.construct(subtree_root->content, *first);
			subtree_root->left_child = 0;
			subtree_root->right_child = 0;
			if (direction == LEFT)
				root->left_child = subtree_root;
			else
				root->right_child = subtree_root;
			if (_comp(max, min))
			{
				if (direction == LEFT)
					min = max;
				else
					max = min;
			}
			while (++first != last)
			{
				if	((min == max && !((direction == LEFT && _comp((*first).first, max)) || (direction == RIGHT && _comp(max, (*first).first)))) ||
					(min != max && !(_comp(min, (*first).first) && _comp((*first).first, max))))
					continue ;
				if	(!subtree_root->left_child && _comp((*first).first, subtree_root->content->first))
					bst(subtree_root, LEFT, min, subtree_root->content->first, first, last);
				if (!subtree_root->right_child && _comp(subtree_root->content->first, (*first).first))
					bst(subtree_root, RIGHT, subtree_root->content->first, max, first, last);
				if (subtree_root->left_child && subtree_root->right_child)
					break ;
			}
		}

	};

}

#endif