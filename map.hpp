#ifndef MAP_HPP
# define MAP_HPP

# include <algorithm>
# include <functional>
# include <sys/types.h>

# include "pair.hpp"
# include "node.hpp"
# include "iterator_map.hpp"
# include "vector.hpp"
# include "utils.hpp"

namespace ft {

	template <class node_pointer>
	bool comp_nodes(node_pointer i, node_pointer j) { return (i->content->first < j->content->first); }

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
		: _root(create_node_sentinel(0)), _alloc(alloc), _alnode(alloc), _comp(comp), _size(0) {}

		template <class InputIterator>
		map	(InputIterator first, InputIterator last, const key_compare& comp = key_compare(),
			const allocator_type& alloc = allocator_type())
		: _alloc(alloc), _alnode(alloc), _comp(comp), _size(0)
		{
			if (first == last)
			{
				_root = create_node_sentinel(0);
				return ;
			}
			_root = make_bst(first, last);
			_root = balance_bst(_root, _size);
			place_sentinels();
		}

		~map()
		{
			destroy_content_recursive(_root);
			destroy_node_recursive(_root);
		}

		/*
		**	Iterator
		*/

		iterator begin()
		{
			if (_size == 0)
				return iterator(_root);
			node_pointer r = _root;
			while (r->left_child)
				r = r->left_child;
			return iterator(r->parent);
		}

		iterator end()
		{
			if (_size == 0)
				return iterator(_root);
			node_pointer r = _root;
			while (r->right_child)
				r = r->right_child;
			return iterator(r);
		}

		/*
		**	Capacity
		*/

		bool empty() const { return _size == 0; }
		size_type size() const { return _size; }
		size_type max_size() const { return _alnode.max_size(); }

		/*
		**	Element access
		*/

		mapped_type& operator[](const key_type& k)
		{
			clear_sentinels();
			if (!_root)
			{
				_root = create_node(make_pair(k, mapped_type()), 0);
				++_size;
				place_sentinels();
				return _root->content->second;
			}
			node_pointer current = _root;
			while (current)
			{
				if (_comp(k, current->content->first))
				{
					if (!current->left_child)
					{
						current->left_child = create_node(make_pair(k, mapped_type()), current);
						++_size;
						mapped_type& r = current->left_child->content->second;
						_root = balance_bst(_root, _size);
						place_sentinels();
						return r;
					}
					current = current->left_child;
				}
				else if (_comp(current->content->first, k))
				{
					if (!current->right_child)
					{
						current->right_child = create_node(make_pair(k, mapped_type()), current);
						++_size;
						mapped_type& r = current->right_child->content->second;
						_root = balance_bst(_root, _size);
						place_sentinels();
						return r;
					}
					current = current->right_child;
				}
				else
					break ;
			}
			return current->content->second;
		}

		/*
		**	Modifiers
		*/

		pair<iterator, bool> insert(const value_type& val)
		{
			clear_sentinels();
			if (!_root)
			{
				_root = create_node(val, 0);
				++_size;
				place_sentinels();
				return make_pair(iterator(_root), true);
			}
			node_pointer current = _root;
			while (current)
			{
				if (_comp(val.first, current->content->first))
				{
					if (!current->left_child)
					{
						current->left_child = create_node(val, current);
						++_size;
						pair<iterator, bool> r = make_pair(iterator(current->left_child), true);
						_root = balance_bst(_root, _size);
						place_sentinels();
						return r;
					}
					current = current->left_child;
				}
				else if (_comp(current->content->first, val.first))
				{
					if (!current->right_child)
					{
						current->right_child = create_node(val, current);
						++_size;
						pair<iterator, bool> r = make_pair(iterator(current->right_child), true);
						_root = balance_bst(_root, _size);
						place_sentinels();
						return r;
					}
					current = current->right_child;
				}
				else
					break ;
			}
			return make_pair(iterator(current), false);
		}

		template <class InputIterator>
		void insert(InputIterator first, InputIterator last)
		{
			if (first == last)
				return ;
			clear_sentinels();
			vector<node_pointer> v;
			v.reserve(_size + std::distance(first, last));
			store_bst(_root, v);
			while (first != last)
			{
				v.push_back(create_node(*first, 0));
				++first;
			}
			std::sort(v.begin(), v.end(), comp_nodes<node_pointer>);
			_root = vector_to_balanced_bst(v, 0, v.size() - 1, 0);
			place_sentinels();
		}

		size_type erase(const key_type& k)
		{
			if (!_root || _root->issentinel())
				return 0;
			clear_sentinels();
			node_pointer current = _root;
			while (current)
			{
				if (_comp(k, current->content->first))
				{
					if (!current->left_child)
					{
						place_sentinels();
						return 0;
					}
					current = current->left_child;
				}
				else if (_comp(current->content->first, k))
				{
					if (!current->right_child)
					{
						place_sentinels();
						return 0;
					}
					current = current->right_child;
				}
				else
					break ;
			}
			if (!current)
			{
				place_sentinels();
				return 0;
			}
			_alloc.destroy(current->content);
			_alloc.deallocate(current->content, 1);
			node_pointer child;
			if (current->isleaf())
			{
				if (current != _root)
				{
					if (current->child_side() == RIGHT)
						current->parent->right_child = 0;
					else
						current->parent->left_child = 0;
				}
				_alnode.destroy(current);
				_alnode.deallocate(current, 1);
			}
			else if ((child = current->has_one_child_leaf()))
			{
				current->content = child->content;
				if (current->right_child == child)
					current->right_child = 0;
				else
					current->left_child = 0;
				_alnode.destroy(child);
				_alnode.deallocate(child, 1);
			}
			else
			{
				child = current->inorder_xcessor();
				current->content = child->content;
				if (child->child_side() == RIGHT)
					child->parent->right_child = 0;
				else
					child->parent->left_child = 0;
				_alnode.destroy(child);
				_alnode.deallocate(child, 1);
			}
			--_size;
			place_sentinels();
			return 1;
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
		size_type		_size;

		/*
		**	Private member functions
		*/

		node_pointer create_node(const value_type& val, node_pointer parent)
		{
			node_pointer new_node = _alnode.allocate(1);
			new_node->content = _alloc.allocate(1);
			_alloc.construct(new_node->content, val);
			new_node->left_child = 0;
			new_node->right_child = 0;
			new_node->parent = parent;
			return new_node;
		}

		node_pointer create_node_sentinel(node_pointer parent)
		{
			node_pointer new_node = _alnode.allocate(1);
			new_node->content = 0;
			new_node->left_child = 0;
			new_node->right_child = 0;
			new_node->parent = parent;
			return new_node;
		}

		template <class InputIterator>
		node_pointer make_bst(InputIterator first, InputIterator last)
		{
			node_pointer root = create_node(*first, 0);
			++_size;
			while (++first != last)
			{
				if (root->left_child && root->right_child)
					break ;
				if (!root->left_child && _comp((*first).first, root->content->first))
					bst(root, LEFT, root->content->first, root->content->first, first, last);
				if (!root->right_child && _comp(root->content->first, (*first).first))
					bst(root, RIGHT, root->content->first, root->content->first, first, last);
			}
			return root;
		}

		template <class InputIterator>
		void bst(node_pointer root, int direction, key_type min, key_type max, InputIterator first, InputIterator last)
		{
			node_pointer subtree_root = create_node(*first, root);
			++_size;
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
				if (subtree_root->left_child && subtree_root->right_child)
					break ;
				if	((min != max && !(_comp(min, (*first).first) && _comp((*first).first, max))) ||
					(min == max && !((direction == LEFT && _comp((*first).first, max)) || (direction == RIGHT && _comp(max, (*first).first)))))
					continue ;
				if	(!subtree_root->left_child && _comp((*first).first, subtree_root->content->first))
					bst(subtree_root, LEFT, min, subtree_root->content->first, first, last);
				if (!subtree_root->right_child && _comp(subtree_root->content->first, (*first).first))
					bst(subtree_root, RIGHT, subtree_root->content->first, max, first, last);
			}
		}

		node_pointer balance_bst(node_pointer root, size_type size)
		{
			vector<node_pointer> sorted_nodes;

			sorted_nodes.reserve(size);
			store_bst(root, sorted_nodes);
			return vector_to_balanced_bst(sorted_nodes, 0, sorted_nodes.size() - 1, 0);
		}

		void store_bst(node_pointer root, vector<node_pointer>& v)
		{
			if (!root)
				return ;
			store_bst(root->left_child, v);
			v.push_back(root);
			store_bst(root->right_child, v);
		}

		node_pointer vector_to_balanced_bst(vector<node_pointer>& v, ssize_t start, ssize_t end, node_pointer parent)
		{
			if (start > end)
				return 0;
			ssize_t median = (start + end) / 2;
			node_pointer root = v[median];
			root->parent = parent;
			root->left_child = vector_to_balanced_bst(v, start, median - 1, root);
			root->right_child = vector_to_balanced_bst(v, median + 1, end, root);
			return root;
		}

		void destroy_content_recursive(node_pointer root)
		{
			if (!root)
				return ;
			destroy_content_recursive(root->left_child);
			destroy_content_recursive(root->right_child);
			if (!root->issentinel())
			{
				_alloc.destroy(root->content);
				_alloc.deallocate(root->content, 1);
			}
		}

		void destroy_node_recursive(node_pointer root)
		{
			if (!root)
				return ;
			destroy_node_recursive(root->left_child);
			destroy_node_recursive(root->right_child);
			_alnode.destroy(root);
			_alnode.deallocate(root, 1);
		}

		void place_sentinels()
		{
			if (!_root)
			{
				_root = create_node_sentinel(0);
				return ;
			}
			node_pointer current = _root;
			while (current->left_child)
				current = current->left_child;
			current->left_child = create_node_sentinel(current);
			current = _root;
			while (current->right_child)
				current = current->right_child;
			current->right_child = create_node_sentinel(current);
		}

		void clear_sentinels()
		{
			if (_root->issentinel())
			{
				_alnode.destroy(_root);
				_alnode.deallocate(_root, 1);
				_root = 0;
				return ;
			}
			node_pointer current = _root;
			while (current->left_child)
				current = current->left_child;
			current->parent->left_child = 0;
			_alnode.destroy(current);
			_alnode.deallocate(current, 1);
			current = _root;
			while (current->right_child)
				current = current->right_child;
			current->parent->right_child = 0;
			_alnode.destroy(current);
			_alnode.deallocate(current, 1);
		}

	};

}

#endif