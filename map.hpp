#ifndef MAP_HPP
# define MAP_HPP

//REMOVE BEFORE DEFENSE
# include <iostream>
# include <cstdio>
# include <vector>
# include <map>
# include <cmath>
//REMOVE BEFORE DEFENSE

# include <functional>
# include <sys/types.h>
# include <memory>

# include "pair.hpp"
# include "node.hpp"
# include "iterator_map.hpp"
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

		explicit map (const key_compare & comp = key_compare(), const allocator_type & alloc = allocator_type())
		: _root(0), _alloc(alloc), _alnode(alloc), _comp(comp), _size(0)
		{
			create_sentinel();
		}

		template <class InputIterator>
		map (InputIterator first, InputIterator last, const key_compare & comp = key_compare(),
			const allocator_type & alloc = allocator_type())
		: _root(0), _alloc(alloc), _alnode(alloc), _comp(comp), _size(0)
		{
			create_sentinel();
			this->insert(first, last);
		}

		map (const map & x)
		: _root(0), _alloc(x._alloc), _alnode(x._alnode), _comp(x._comp), _size(0)
		{
			create_sentinel();
			this->insert(x.begin(), x.end());
		}

		map & operator=(const map & x)
		{
			if (this == &x)
				return *this;
			this->clear();
			this->insert(x.begin(), x.end());
			return *this;
		}

		~map()
		{
			this->clear();
			_alnode.destroy(_sentinel);
			_alnode.deallocate(_sentinel, 1);
		}

		/*
		**	Iterator
		*/

		iterator begin()
		{
			if (this->empty())
				return this->end();
			return iterator(smallest_node());
		}

		const_iterator begin() const
		{
			if (this->empty())
				return this->end();
			return const_iterator(smallest_node());
		}

		iterator end() { return iterator(_sentinel); }

		const_iterator end() const { return const_iterator(_sentinel); }

		reverse_iterator rbegin()
		{
			if (this->empty())
				return this->rend();
			return reverse_iterator(this->end());
		}

		const_reverse_iterator rbegin() const
		{
			if (this->empty())
				return this->rend();
			return const_reverse_iterator(this->end());
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

		mapped_type & operator[] (const key_type & k)
		{
			return (*((this->insert(ft::make_pair(k, mapped_type()))).first)).second;
		}

		/*
		**	Modifiers
		*/

		pair<iterator, bool> insert(const value_type & val) { return insert_node(val); }

		iterator insert(iterator position, const value_type & val) { (void)position; return insert_node(val).first; }

		template <class InputIterator>
		void insert(InputIterator first, InputIterator last)
		{
			while (first != last)
				insert_node(*first++);
		}

		void erase (iterator position) { erase(position->first); }

		size_type erase(const key_type & k)
		{
			node_pointer p = _root;
			while (p && !is_sentinel(p))
			{
				if (_comp(k, p->content->first))
					p = p->left;
				else if (_comp(p->content->first, k))
					p = p->right;
				else
				{
					_sentinel->parent->right = 0;
					delete_node(p);
					place_sentinel();
					return 1;
				}
			}
			return 0;
		}

		void erase (iterator first, iterator last)
		{
			while (first != last)
				erase(first++);
		}

		void swap(map & x)
		{
			node_pointer tmp_root = _root;
			node_pointer tmp_sentinel = _sentinel;
			size_type tmp_size = _size;
			_root = x._root;
			_sentinel = x._sentinel;
			_size = x._size;
			x._root = tmp_root;
			x._sentinel = tmp_sentinel;
			x._size = tmp_size;
		}

		void clear() { recursive_clear(_root); _root = 0; }

		/*
		**	Observers
		*/

		key_compare key_comp() const { return _comp; }
		value_compare value_comp() const { return value_compare(key_compare()); };

		/*
		**	Operations
		*/

		iterator find (const key_type & k)
		{
			node_pointer node = _root;
			while (node && !is_sentinel(node))
			{
				if (_comp(k, node->content->first))
					node = node->left;
				else if (_comp(node->content->first), k)
					node = node->right;
				else
					return iterator(node);
			}
			return this->end();
		}

		/*
		**	Allocator
		*/

		allocator_type get_allocator() const { return _alloc; }

		/*
		**	Non-member function overloads
		*/

		/*
		**	Debug functions (remove before defense)
		*/

		void print_tree(node_pointer node = 0)
		{
			if (!node)
			{
				node = _root;
				if (!node)
				{
					std::cout << "The tree is empty" << std::endl;
					return ;
				}
			}
			if (node->left && !is_sentinel(node->left))
				print_tree(node->left);
			std::cout << node->content->first << ' ';
			if (node->right && !is_sentinel(node->right))
				print_tree(node->right);
		}

		void print_node(node_pointer node)
		{
			if (node->color == RED)
				std::cout << "\033[31m";
			else
				std::cout << "\033[30m";
			std::cout << node->content->first;
		}

		int get_max_depth(node_pointer root)
		{
			if (!root || is_sentinel(root))
				return 0;
			int depth1 = get_max_depth(root->left);
			int depth2 = get_max_depth(root->right);
			return depth1 > depth2 ? depth1 + 1 : depth2 + 1;
		}

		void get_nodes_by_depth(std::vector<std::vector<node_pointer> > & nodes, node_pointer node = 0, int depth = 0)
		{
			if (!node && depth == 0)
			{
				node = _root;
				if (!node)
					return ; //empty
			}
			if (node && node->left && !is_sentinel(node->left))
				get_nodes_by_depth(nodes, node->left, depth + 1);
			else if ((size_t)(depth + 1) < nodes.size())
				get_nodes_by_depth(nodes, 0, depth + 1);
			nodes[depth].push_back(node);
			if (node && node->right && !is_sentinel(node->right))
				get_nodes_by_depth(nodes, node->right, depth + 1);
			else if ((size_t)(depth + 1) < nodes.size())
				get_nodes_by_depth(nodes, 0, depth + 1);
		}

		size_t sp2(int x) //sum of power of 2
		{
			if (x == 0) return 0;
			size_t r = 1;
			while (--x > 0)
				r += pow(2, x);
			return r;
		}

		void print_tree_ascii()
		{
			int elem_size = 1;
			int depth = get_max_depth(_root);
			std::vector<std::vector<node_pointer> > v;
			for (int i = 0; i < depth; ++i)
				v.push_back(std::vector<node_pointer>()); //filing v with depth
			get_nodes_by_depth(v);
			int i = depth - 1;
			std::string padding;
			for (typename std::vector<std::vector<node_pointer> >::iterator i1 = v.begin(); i1 != v.end(); ++i1)
			{
				padding.append(sp2(i) * elem_size, ' ');
				std::cout << padding;
				padding.clear();
				padding.append(sp2(i + 1) * elem_size, ' ');
				for (typename std::vector<node_pointer>::iterator i2 = (*i1).begin(); i2 != (*i1).end(); ++i2)
				{
					if (*i2)
					{
						if ((*i2)->color == RED)
							std::cout << "\033[31m";
						else
							std::cout << "\033[37m";
						std::cout << (*i2)->content->first << padding;
					}
					else
						std::cout << "\033[37m " << padding;
				}
				std::cout << std::endl;
				--i;
				padding.clear();
			}
			std::cout << "\033[0m" << std::endl; //back to normal
		}

		/*
		0										h = 0, 0 padding

		 1										h = 1, 1 padding (1 elem)
		0 2

		   4									h = 2, 3 padding (1 + 2)
		 2   6									h = 1, d = 1, 1 padding, then 3 padding
		1 3 5 7

		   
		       x								h = 3, 7 padding (1 + 2 + 4)
		   x       x							h = 2, d = 1, 3 padding, then 7 padding
		 x   x   x   x							h = 3, d = 2, 1 padding, then 3, 3, 3
		x x x x x x x x

					   x						h = 4, 15 padding (1 + 2 + 4 + 8)
			   x	   		   x				h = 3, 7 padding, then 15
		   x       x       x      x				h = 2, 3 padding, then 7, 7, 7
		 x   x	 x	 x	 x	 x	 x	 x
		x x x x x x x x x x x x x x x x
		*/

	private:

		/*
		**	Private member variables
		*/

		node_pointer	_root;
		node_pointer	_sentinel;
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
			new_node->left = 0;
			new_node->right = 0;
			new_node->parent = parent;
			new_node->color = RED;
			++_size;
			return new_node;
		}

		void create_sentinel()
		{
			_sentinel = _alnode.allocate(1);
			_sentinel->content = 0;
			_sentinel->left = 0;
			_sentinel->right = 0;
			_sentinel->parent = 0;
		}

		void place_sentinel()
		{
			if (this->empty())
				return ;
			node_pointer tmp = highest_node();
			tmp->right = _sentinel;
			_sentinel->parent = tmp;
		}

		void rotate(node_pointer node, int rotation_side)
		{
			node_pointer parent = node->parent;
			node_pointer pivot = node->child[1 - rotation_side];
			node_pointer child;

			if (pivot == 0)
				std::cerr << "error: pivot is not a true node" << std::endl;
			child = pivot->child[rotation_side];
			node->child[1 - rotation_side] = child;
			if (child != 0)
				child->parent = node;
			pivot->child[rotation_side] = node;
			node->parent = pivot;
			pivot->parent = parent;
			if (parent != 0)
				parent->child[node == parent->right ? RIGHT : LEFT] = pivot;
			else
				_root = pivot;
		}

		void recolor_rotate(node_pointer node)
		{
			node_pointer uncle, parent, grandparent;

			parent = node->parent;
			do
			{
				if (parent->color == BLACK)
					return ;
				//parent is RED
				if ((grandparent = parent->parent) == 0)
				{
					parent->color = BLACK;
					return ;
				}
				//parent is RED and grandparent exists
				int side = child_side(parent);
				uncle = grandparent->child[1 - side];
				if (!uncle || is_sentinel(uncle) || uncle->color == BLACK)
				{
					if (node == parent->child[1 - side])
					{
						rotate(parent, side);
						node = parent;
						parent = grandparent->child[side];
					}
					rotate(grandparent, 1 - side);
					parent->color = BLACK;
					grandparent->color = RED;
					return ;
				}
				//parent and uncle are RED
				parent->color = BLACK;
				uncle->color = BLACK;
				grandparent->color = RED;
				node = grandparent;
			} while ((parent = node->parent) != 0);
		}

		pair<iterator, bool> insert_node(const value_type & val)
		{
			if (!_root)
			{
				_root = create_node(val, 0);
				_root->color = BLACK;
				place_sentinel();
				return ft::make_pair(iterator(_root), true);
			}
			node_pointer node = _root;
			while (node)
			{
				if (_comp(val.first, node->content->first))
				{
					if (!node->left)
					{
						node->left = create_node(val, node);
						recolor_rotate(node->left);
						return ft::make_pair(iterator(node->left), true);
					}
					node = node->left;
				}
				else if (_comp(node->content->first, val.first))
				{
					if (!node->right || is_sentinel(node->right)) //can be optimized
					{
						node->right = create_node(val, node);
						recolor_rotate(node->right);
						place_sentinel();
						return ft::make_pair(iterator(node->right), true);
					}
					node = node->right;
				}
				else
					break;
			}
			return ft::make_pair(iterator(node), false);
		}

		void recursive_clear(node_pointer node)
		{
			if (!node || is_sentinel(node))
				return ;
			if (node->left)
				recursive_clear(node->left);
			if (node->right)
				recursive_clear(node->right);
			delete_node_data(node);
		}

		void delete_node_data(node_pointer node)
		{
			if (node->parent)
				node->parent->child[child_side(node)] = 0;
			_alloc.destroy(node->content);
			_alloc.deallocate(node->content, 1);
			_alnode.destroy(node);
			_alnode.deallocate(node, 1);
			--_size;
		}

		void delete_node(node_pointer node)
		{
			int side;
			//simple cases:
			if (node->parent == 0 && node->right == 0 && node->left == 0) //node is root and no child
			{
				delete_node_data(node);
				_root = 0;
				return ;
			}
			else if (node->right && node->left)
			{
				node_pointer tmp = node->inorder_successor();
				node_pointer tmp_parent = tmp->parent;
				node_pointer tmp_left = tmp->left;
				node_pointer tmp_right = tmp->right;
				enum Node_color tmp_color = tmp->color;
				int tmp_side = child_side(tmp);
				bool is_root = (node == _root);

				tmp->parent = node->parent;
				if (tmp->parent)
					tmp->parent->child[child_side(node)] = tmp;
				tmp->left = node->left;
				if (tmp->left)
					tmp->left->parent = tmp;
				tmp->right = tmp != node->right ? node->right : 0; //in case inorder successor is the node child
				if (tmp->right)
					tmp->right->parent = tmp;
				tmp->color = node->color;

				node->parent = node != tmp_parent ? tmp_parent : tmp;
				if (node->parent)
					node->parent->child[tmp_side] = node;
				node->left = tmp_left;
				if (node->left)
					node->left->parent = node;
				node->right = tmp_right;
				if (node->right)
					node->right->parent = node;
				node->color = tmp_color;

				if (is_root)
				{
					_root = tmp;
					_root->parent = 0;
				}
			}
			//at this point node only has at most one child
			if (node->color == RED)
			{
				delete_node_data(node);
				return ;
			}
			else //node is BLACK
			{
				if (node->left) //node->left must be RED
				{
					node->left->color = BLACK;
					if (node->parent)
						node->parent->child[child_side(node)] = node->left;
					node->left->parent = node->parent;
					if (node == _root)
						_root = node->left;
					node->parent = 0; //for delete_node_data()
					delete_node_data(node);
					return ;
				}
				else if (node->right) //node->right must be RED
				{
					node->right->color = BLACK;
					if (node->parent)
						node->parent->child[child_side(node)] = node->right;
					node->right->parent = node->parent;
					if (node == _root)
						_root = node->right;
					node->parent = 0; //for delete_node_data()
					delete_node_data(node);
					return ;
				}
			}
			//complex cases, here node is BLACK, not the root, and has no child
			node_pointer parent = node->parent;
			node_pointer sibling, close_nephew, distant_nephew;

			side = child_side(node);
			delete_node_data(node);
			parent->child[side] = 0;
			goto delete_loop;
			do
			{
				side = child_side(node);
			delete_loop:
				sibling = parent->child[1 - side];
				distant_nephew = sibling->child[1 - side];
				close_nephew = sibling->child[side];
				if (sibling->color == RED) //parent, close and distant nephew are BLACK
					goto case_delete_3;
				//else sibling is BLACK
				if (distant_nephew && distant_nephew->color == RED) //distant nephew RED and sibling BLACK
					goto case_delete_6;
				if (close_nephew && close_nephew->color == RED) //close nephew RED and sibling and distant nephew BLACK
					goto case_delete_5;
				if (parent->color == RED) //implies close, distant nephew BLACK as well as sibling
					goto case_delete_4;
				//parent, close and distant nephew, sibling are all BLACK
				sibling->color = RED;
				node = parent;
			} while ((parent = node->parent) != 0);
			//node is root
			return ;
			case_delete_3:
				rotate(parent, side);
				parent->color = RED;
				sibling->color = BLACK;
				sibling = close_nephew;
				distant_nephew = sibling->child[1 - side];
				if (distant_nephew && distant_nephew->color == RED)
					goto case_delete_6;
				close_nephew = sibling->child[side];
				if (close_nephew && close_nephew->color == RED)
					goto case_delete_5;
			case_delete_4:
				sibling->color = RED;
				parent->color = BLACK;
				return ;
			case_delete_5:
				rotate(sibling, 1 - side);
				sibling->color = RED;
				close_nephew->color = BLACK;
				distant_nephew = sibling;
				sibling = close_nephew;
			case_delete_6:
				rotate(parent, side);
				sibling->color = parent->color;
				parent->color = BLACK;
				distant_nephew->color = BLACK;
				return ;
		}

		node_pointer smallest_node() const
		{
			if (this->empty())
				return 0;
			node_pointer r = _root;
			while (r->left && !is_sentinel(r->left))
				r = r->left;
			return r;
		}

		node_pointer highest_node() const
		{
			if (this->empty())
				return 0;
			node_pointer r = _root;
			while (r->right && !is_sentinel(r->right))
				r = r->right;
			return r;
		}

	};

}

#endif