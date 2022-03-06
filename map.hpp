#ifndef MAP_HPP
# define MAP_HPP

//REMOVE BEFORE DEFENSE
# include <iostream>
//REMOVE BEFORE DEFENSE

# include <functional>
# include <sys/types.h>
# include <memory>

# include "pair.hpp"
# include "node.hpp"

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
		//typedef typename	ft::iterator_map<value_type>					iterator;
		//typedef typename	ft::iterator_map<const value_type>				const_iterator;
		//typedef typename	ft::reverse_iterator<iterator>					reverse_iterator;
		//typedef typename	ft::reverse_iterator<const_iterator>			const_reverse_iterator;
		//typedef typename	ft::iterator_traits<iterator>::difference_type	difference_type;
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

		}

		/*
		**	Iterator
		*/

		/*
		**	Capacity
		*/

		bool empty() const { return _size == 0; }
		size_type size() const { return _size; }
		size_type max_size() const { return _alnode.max_size(); }

		/*
		**	Element access
		*/

		/*
		**	Modifiers
		*/

		bool insert(const value_type & val)
		{
			return insert_node(val);
		}

		/*
		**	Observers
		*/

		key_compare key_comp() const { return _comp; }
		value_compare value_comp() const { return value_compare(key_compare()); };

		/*
		**	Operations
		*/

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
			if (node->left)
				print_tree(node->left);
			std::cout << node->content->first << ' ';
			if (node->right)
				print_tree(node->right);
		}


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
			new_node->left = 0;
			new_node->right = 0;
			new_node->parent = parent;
			new_node->color = RED;
			++_size;
			return new_node;
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
			node_pointer uncle, parent, grandparent, tmp;

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
				if (!uncle || uncle->color == BLACK)
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

		bool insert_node(const value_type & val)
		{
			if (!_root)
			{
				_root = create_node(val, 0);
				_root->color = BLACK;
				return true;
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
						return true;
					}
					node = node->left;
				}
				else if (_comp(node->content->first, val.first))
				{
					if (!node->right)
					{
						node->right = create_node(val, node);
						recolor_rotate(node->right);
						return true;
					}
					node = node->right;
				}
				else
					break;
			}
			return false;
		}

		void delete_node_data(node_pointer node)
		{
			if (node->parent)
				node->parent->child[child_side(node)] = 0;
			_alloc.destroy(node->content);
			_alloc.deallocate(node->content, 1);
			_alnode.destroy(node);
			_alnode.deallocate(node, 1);
		}

		void delete_node(node_pointer node)
		{
			//simple cases:
			if (node->parent == 0 && node->right == 0 && node->left == 0) //node is root and no child
			{
				delete_node_data(node);
				_root = 0;
				return ;
			}
			else if (node->right && node->left)
				node->swap(node->inorder_successor());
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
					node->parent = 0; //for delete_node_data()
					delete_node_data(node);
					return ;
				}
			}
			//complex cases, here node is BLACK, not the root, and has no child
			node_pointer parent = node->parent;
			int side;
			node_pointer sibling, close_nephew, distant_nephew;

			parent->child[child_side(node)] = 0; //this ?LEAKS?, delete data (at the end I guess)
			do
			{
				side = child_side(node);
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

	};

}

#endif