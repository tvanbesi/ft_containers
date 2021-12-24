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
# include "reverse_iterator.hpp"

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
		class value_compare : std::binary_function<value_type, value_type, bool>
		{
			friend class map;
		protected:
			Compare comp;
			value_compare (Compare c) : comp(c) {}
		public:
			typedef	bool		result_type;
			typedef	value_type	first_argument_type;
			typedef	value_type	second_argument_type;
			bool operator() (const value_type& x, const value_type& y) const
			{
				return comp(x.first, y.first);
			}
		};
		typedef typename	allocator_type::reference						reference;
		typedef typename	allocator_type::const_reference					const_reference;
		typedef typename	allocator_type::pointer							pointer;
		typedef typename	allocator_type::const_pointer					const_pointer;
		typedef typename	ft::iterator_map<value_type>					iterator;
		typedef typename	ft::iterator_map<const value_type>				const_iterator;
		typedef typename	ft::reverse_iterator<iterator>					reverse_iterator;
		typedef typename	ft::reverse_iterator<const_iterator>			const_reverse_iterator;
		typedef typename	ft::iterator_traits<iterator>::difference_type	difference_type;
		typedef				size_t											size_type;
		typedef				Node<const key_type, mapped_type>				node;
		typedef				node*											node_pointer;
		typedef typename	allocator_type::template rebind<node>::other	alnode;

		/*
		**	Public member functions
		*/

		explicit map (const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type())
		: _root(0), _alloc(alloc), _alnode(alloc), _comp(comp), _size(0)
		{
			make_sentinels();
		}

		template <class InputIterator>
		map	(InputIterator first, InputIterator last, const key_compare& comp = key_compare(),
			const allocator_type& alloc = allocator_type())
		: _root(0), _alloc(alloc), _alnode(alloc), _comp(comp), _size(0)
		{
			make_sentinels();
			if (first == last)
				return ;
			_root = make_bst(first, last);
			_root = balance_bst(_root, _size);
			place_sentinels();
		}

		map (const map& x)
		: _root(0), _alloc(x._alloc), _alnode(x._alloc), _comp(x._comp), _size(0)
		{
			make_sentinels();
			this->insert(x.begin(), x.end());
		}

		map& operator=(const map& x)
		{
			if (this == &x) { return *this; }
			this->clear();
			this->insert(x.begin(), x.end());
			return *this;
		}

		~map()
		{
			destroy_sentinels();
			destroy_content_recursive(_root);
			destroy_node_recursive(_root);
		}

		/*
		**	Iterator
		*/

		iterator begin()
		{
			if (this->empty())
				return this->end();
			node_pointer r = _root;
			while (r->left_child)
				r = r->left_child;
			return iterator(r->parent);
		}

		const_iterator begin() const
		{
			if (this->empty())
				return this->end();
			node_pointer r = _root;
			while (r->left_child)
				r = r->left_child;
			return const_iterator(r->parent);
		}

		iterator end() { return iterator(_sentinel_end); }

		const_iterator end() const { return const_iterator(_sentinel_end); }

		reverse_iterator rbegin()
		{
			if (this->empty())
				return this->rend();
			node_pointer r = _root;
			while (r->right_child)
				r = r->right_child;
			return reverse_iterator(r);
		}

		const_reverse_iterator rbegin() const
		{
			if (this->empty())
				return this->rend();
			node_pointer r = _root;
			while (r->right_child)
				r = r->right_child;
			return const_reverse_iterator(r);
		}

		reverse_iterator rend() { return reverse_iterator(this->begin()); }

		const_reverse_iterator rend() const { return const_reverse_iterator(this->begin()); }

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
			disconnect_sentinels();
			if (this->empty())
			{
				_root = create_node(ft::make_pair(k, mapped_type()), 0);
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
						current->left_child = create_node(ft::make_pair(k, mapped_type()), 0);
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
						current->right_child = create_node(ft::make_pair(k, mapped_type()), 0);
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
			place_sentinels();
			return current->content->second;
		}

		/*
		**	Modifiers
		*/

		pair<iterator, bool> insert(const value_type& val)
		{
			disconnect_sentinels();
			if (this->empty())
			{
				_root = create_node(val, 0);
				++_size;
				place_sentinels();
				return ft::make_pair(iterator(_root), true);
			}
			pair<node_pointer, bool> r = insert_node(_root, val);
			if (r.second)
			{
				_root = balance_bst(_root, _size);
				place_sentinels();
				return (ft::make_pair(iterator(r.first), true));
			}
			place_sentinels();
			return (ft::make_pair(iterator(r.first), false));
		}

		iterator insert(iterator position, const value_type& val)
		{
			if (this->empty())
			{
				disconnect_sentinels();
				_root = create_node(val, 0);
				++_size;
				place_sentinels();
				return iterator(_root);
			}
			else if (position.issentinel())
			{
				if (position.child_side() == LEFT)
					++position;
				else
					--position;
			}
			iterator previous_position;
			if (_comp((*position).first, val.first))
			{
				do
				{
					previous_position = position++;
					if (position.issentinel())
					{
						position = previous_position;
						break ;
					}
					if ((*position).first == val.first)
						return position;
				}
				while (!(_comp((*previous_position).first, val.first) && _comp(val.first, (*position).first)));
				disconnect_sentinels();
				pair<node_pointer, bool> r = insert_node(position.get_root(), val);
				if (r.second)
					_root = balance_bst(_root, _size);
				place_sentinels();
				return iterator(r.first);
			}
			else if (_comp(val.first, (*position).first))
			{
				do
				{
					previous_position = position--;
					if (position.issentinel())
						break ;
					if ((*position).first == val.first)
						return position;
				}
				while (!(_comp((*position).first, val.first) && _comp(val.first, (*previous_position).first)));
				disconnect_sentinels();
				pair<node_pointer, bool> r = insert_node(previous_position.get_root(), val);
				if (r.second)
					_root = balance_bst(_root, _size);
				place_sentinels();
				return iterator(r.first);
			}
			else
				return position;
		}

		template <class InputIterator>
		void insert(InputIterator first, InputIterator last)
		{
			if (first == last)
				return ;
			while (first != last)
			{
				this->insert(*first);
				++first;
			}
			disconnect_sentinels();
			_root = balance_bst(_root, _size);
			place_sentinels();
		}

		void erase (iterator position) { erase((*position).first); }

		size_type erase(const key_type& k)
		{
			if (!_root || _root->issentinel())
				return 0;
			disconnect_sentinels();
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
			return delete_node(current);
		}

		void erase(iterator first, iterator last)
		{
			if (first == last)
				return ;
			vector<key_type> v;
			v.reserve(std::distance(first, last));
			while (first != last)
			{
				v.push_back((*first).first);
				++first;
			}
			for (typename vector<key_type>::size_type i = 0; i < v.size(); ++i)
				this->erase(v[i]);
			disconnect_sentinels();
			_root = balance_bst(_root, _size);
			place_sentinels();
		}

		void swap(map &x)
		{
			node_pointer tmp_root = _root;
			node_pointer tmp_rend = _sentinel_rend;
			node_pointer tmp_end = _sentinel_end;
			size_type tmp_size = _size;
			_root = x._root;
			_sentinel_rend = x._sentinel_rend;
			_sentinel_end = x._sentinel_end;
			_size = x._size;
			x._root = tmp_root;
			x._sentinel_rend = tmp_rend;
			x._sentinel_end = tmp_end;
			x._size = tmp_size;
		}

		void clear()
		{
			disconnect_sentinels();
			destroy_content_recursive(_root);
			destroy_node_recursive(_root);
			_size = 0;
			_root = 0;
		}

		/*
		**	Observers
		*/

		key_compare key_comp() const { return _comp; }
		value_compare value_comp() const { return value_compare(key_compare()); };

		/*
		**	Operations
		*/

		iterator find(const key_type& k)
		{
			node_pointer current = _root;
			while (current && !current->issentinel())
			{
				if (_comp(k, current->content->first))
					current = current->left_child;
				else if (_comp(current->content->first, k))
					current = current->right_child;
				else
					return iterator(current);
			}
			return this->end();
		}

		const_iterator find(const key_type& k) const
		{
			node_pointer current = _root;
			while (current && !current->issentinel())
			{
				if (_comp(k, current->content->first))
					current = current->left_child;
				else if (_comp(current->content->first, k))
					current = current->right_child;
				else
					return const_iterator(current);
			}
			return this->end();
		}

		size_type count(const key_type& k) const
		{
			if (this->find(k) == this->end())
				return 0;
			return 1;
		}

		iterator lower_bound(const key_type& k)
		{
			if (this->empty())
				return this->end();
			iterator previous;
			iterator current = iterator(_root);
			while (true)
			{
				if (_comp(k, (*current).first))
				{
					previous = current;
					--current;
					if (current.issentinel())
						return previous;
					else if (_comp((*current).first, k) && _comp(k, (*previous).first))
						return current;
				}
				else if (_comp((*current).first, k))
				{
					previous = current;
					++current;
					if (current.issentinel())
						return this->end();
					else if (_comp((*previous).first, k) && _comp(k, (*current).first))
						return previous;
				}
				else
					return current;
			}
		}

		const_iterator lower_bound(const key_type& k) const
		{
			if (this->empty())
				return this->end();
			const_iterator previous;
			const_iterator current = const_iterator(_root);
			while (true)
			{
				if (_comp(k, (*current).first))
				{
					previous = current;
					--current;
					if (current.issentinel())
						return previous;
					else if (_comp((*current).first, k) && _comp(k, (*previous).first))
						return current;
				}
				else if (_comp((*current).first, k))
				{
					previous = current;
					++current;
					if (current.issentinel())
						return this->end();
					else if (_comp((*previous).first, k) && _comp(k, (*current).first))
						return previous;
				}
				else
					return current;
			}
		}

		iterator upper_bound(const key_type& k)
		{
			if (this->empty())
				return this->end();
			iterator previous;
			iterator current = iterator(_root);
			while (true)
			{
				if (_comp(k, (*current).first))
				{
					previous = current;
					--current;
					if (current.issentinel())
						return previous;
					else if (_comp((*current).first, k) && _comp(k, (*previous).first))
						return previous;
				}
				else if (_comp((*current).first, k))
				{
					previous = current;
					++current;
					if (current.issentinel())
						return this->end();
					else if (_comp((*previous).first, k) && _comp(k, (*current).first))
						return current;
				}
				else
					return ++current;
			}
		}

		const_iterator upper_bound(const key_type& k) const
		{
			if (this->empty())
				return this->end();
			const_iterator previous;
			const_iterator current = const_iterator(_root);
			while (true)
			{
				if (_comp(k, (*current).first))
				{
					previous = current;
					--current;
					if (current.issentinel())
						return previous;
					else if (_comp((*current).first, k) && _comp(k, (*previous).first))
						return previous;
				}
				else if (_comp((*current).first, k))
				{
					previous = current;
					++current;
					if (current.issentinel())
						return this->end();
					else if (_comp((*previous).first, k) && _comp(k, (*current).first))
						return current;
				}
				else
					return ++current;
			}
		}

		pair<iterator, iterator> equal_range(const key_type& k)
		{
			return make_pair(this->lower_bound(k), this->upper_bound(k));
		}

		pair<const_iterator, const_iterator> equal_range(const key_type& k) const
		{
			return make_pair(this->lower_bound(k), this->upper_bound(k));
		}

		/*
		**	Allocator
		*/

		allocator_type get_allocator() const { return _alloc; }

		/*
		**	Non-member function overloads
		*/

		friend bool operator==(const map& lhs, const map& rhs)
		{
			if (lhs._size != rhs._size)
				return false;
			map::const_iterator it_lhs = lhs.begin();
			map::const_iterator it_rhs = rhs.begin();
			while (it_lhs != lhs.end())
			{
				if ((*it_lhs) != (*it_rhs))
					return false;
				++it_lhs;
				++it_rhs;
			}
			return true;
		}

		friend bool operator!=(const map& lhs, const map& rhs) { return !(lhs == rhs); }

		friend bool operator<(const map& lhs, const map& rhs)
		{
			size_type limit = lhs._size < rhs._size ? lhs._size : rhs._size;
			map::const_iterator it_lhs = lhs.begin();
			map::const_iterator it_rhs = rhs.begin();
			for (size_type i = 0; i < limit; ++i)
			{
				if ((*it_lhs) < (*it_rhs))
					return true;
				if ((*it_rhs) < (*it_lhs))
					return false;
				++it_lhs;
				++it_rhs;
			}
			return lhs._size < rhs._size;
		}

		friend bool operator>(const map& lhs, const map& rhs) { return rhs < lhs; }
		friend bool operator<=(const map& lhs, const map& rhs) { return !(rhs < lhs); }
		friend bool operator>=(const map& lhs, const map& rhs) { return !(lhs < rhs); }

		friend void swap(map& x, map& y) { x.swap(y); }

	private:

		/*
		**	Private member variables
		*/

		node_pointer	_root;
		node_pointer	_sentinel_end;
		node_pointer	_sentinel_rend;
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

		node_pointer create_node_sentinel()
		{
			node_pointer new_node = _alnode.allocate(1);
			new_node->content = 0;
			new_node->left_child = 0;
			new_node->right_child = 0;
			new_node->parent = 0;
			return new_node;
		}

		pair<node_pointer, bool> insert_node(node_pointer root, const value_type& val)
		{
			while (root)
			{
				if (_comp(val.first, root->content->first))
				{
					if (!root->left_child)
					{
						root->left_child = create_node(val, root);
						node_pointer r = root->left_child;
						++_size;
						return ft::make_pair(r, true);
					}
					root = root->left_child;
				}
				else if (_comp(root->content->first, val.first))
				{
					if (!root->right_child)
					{
						root->right_child = create_node(val, root);
						node_pointer r = root->right_child;
						++_size;
						return ft::make_pair(r, true);
					}
					root = root->right_child;
				}
				else
					break ;
			}
			return ft::make_pair(root, false);
		}

		size_type delete_node(node_pointer node)
		{
			_alloc.destroy(node->content);
			_alloc.deallocate(node->content, 1);
			node_pointer child;
			if (node->isleaf())
			{
				if (node != _root)
				{
					if (node->child_side() == RIGHT)
						node->parent->right_child = 0;
					else
						node->parent->left_child = 0;
				}
				else
					_root = 0;
				_alnode.destroy(node);
				_alnode.deallocate(node, 1);
			}
			else if ((child = node->has_one_child_leaf()))
			{
				node->content = child->content;
				if (node->right_child == child)
					node->right_child = 0;
				else
					node->left_child = 0;
				_alnode.destroy(child);
				_alnode.deallocate(child, 1);
			}
			else
			{
				pair<node_pointer, int> xcessor;
				xcessor = node->inorder_xcessor();
				child = xcessor.first;
				int xcessor_type = xcessor.second;
				int side;
				node->content = child->content;
				side = child->child_side();
				if (xcessor_type == SUCCESSOR)
				{
					if (child->right_child)
					{
						if (side == RIGHT)
						{
							child->parent->right_child = child->right_child;
							child->right_child->parent = child->parent;
						}
						else if (side == LEFT)
						{
							child->parent->left_child = child->right_child;
							child->right_child->parent = child->parent;
						}
					}
					else if (side == RIGHT)
						child->parent->right_child = 0;
					else if (side == LEFT)
						child->parent->left_child = 0;
				}
				else if (xcessor_type == PREDECESSOR)
				{
					if (child->left_child)
					{
						if (side == RIGHT)
						{
							child->parent->right_child = child->left_child;
							child->left_child->parent = child->parent;
						}
						else if (side == LEFT)
						{
							child->parent->left_child = child->left_child;
							child->left_child->parent = child->parent;
						}
					}
					else if (side == RIGHT)
						child->parent->right_child = 0;
					else if (side == LEFT)
						child->parent->left_child = 0;
				}
				_alnode.destroy(child);
				_alnode.deallocate(child, 1);
			}
			--_size;
			place_sentinels();
			return 1;
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
			_alloc.destroy(root->content);
			_alloc.deallocate(root->content, 1);
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

		void destroy_sentinels()
		{
			disconnect_sentinels();
			_alnode.destroy(_sentinel_rend);
			_alnode.deallocate(_sentinel_rend, 1);
			_alnode.destroy(_sentinel_end);
			_alnode.deallocate(_sentinel_end, 1);
		}

		void make_sentinels()
		{
			_sentinel_end = create_node_sentinel();
			_sentinel_rend = create_node_sentinel();
		}

		void place_sentinels()
		{
			if (this->empty())
				return ;
			node_pointer current = _root;
			while (current->left_child)
				current = current->left_child;
			current->left_child = _sentinel_rend;
			_sentinel_rend->parent = current;
			current = _root;
			while (current->right_child)
				current = current->right_child;
			current->right_child = _sentinel_end;
			_sentinel_end->parent = current;
		}

		void disconnect_sentinels()
		{
			if (_sentinel_rend->parent)
			{
				_sentinel_rend->parent->left_child = 0;
				_sentinel_rend->parent = 0;
			}
			if (_sentinel_end->parent)
			{
				_sentinel_end->parent->right_child = 0;
				_sentinel_end->parent = 0;
			}
		}

	};

}

#endif