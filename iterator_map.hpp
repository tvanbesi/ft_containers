//#ifndef ITERATOR_MAP_HPP
//# define ITERATOR_MAP_HPP
//
//# include "iterator.hpp"
//# include "node.hpp"
//# include "map.hpp"
//# include "utils.hpp"
//# include "type_traits.hpp"
//
//namespace ft {
//
//	template <class T>
//	class iterator_map : public ft::iterator<std::bidirectional_iterator_tag, T> {
//	friend class iterator_map<typename remove_const<T>::value_type>;
//	friend class iterator_map<const T>;
//
//	public:
//
//		typedef				ft::iterator<std::bidirectional_iterator_tag, T>	iterator;
//		typedef				ft::iterator_traits<iterator>						iterator_traits;
//		typedef typename	iterator_traits::difference_type					difference_type;
//		typedef typename	iterator_traits::value_type							value_type;
//		typedef typename	iterator_traits::pointer							pointer;
//		typedef typename	iterator_traits::reference							reference;
//		typedef typename	iterator_traits::iterator_category					iterator_category;
//		typedef typename	value_type::first_type								key_type;
//		typedef typename	value_type::second_type								mapped_type;
//		typedef				Node<key_type, mapped_type>							node;
//		typedef				node*												node_pointer;
//
//		/*
//		**	Member functions
//		*/
//
//		iterator_map(node_pointer root = 0) : _root(root) {}
//		iterator_map(const iterator_map& src) : _root(src._root) {}
//		iterator_map& operator=(const iterator_map& src) { _root = src._root; return *this; }
//		~iterator_map() {}
//
//		operator iterator_map<const value_type>() const { return _root; }
//
//		iterator_map& operator++()
//		{
//			if (_root->issentinel())
//			{
//				if (_root->parent && _root->child_side() == LEFT)
//					_root = _root->parent;
//			}
//			else if (_root->child_side() != RIGHT)
//			{
//				if (_root->right_child)
//				{
//					_root = _root->right_child;
//					while (_root->left_child)
//						_root = _root->left_child;
//				}
//				else if (_root->parent)
//					_root = _root->parent;
//			}
//			else
//			{
//				if (_root->right_child)
//				{
//					_root = _root->right_child;
//					while (_root->left_child)
//						_root = _root->left_child;
//				}
//				else
//				{
//					_root = _root->parent;
//					while (_root->child_side() != LEFT)
//						_root = _root->parent;
//					_root = _root->parent;
//				}
//			}
//			return *this;
//		}
//
//		iterator_map operator++(int)
//		{
//			iterator_map r = *this;
//			++(*this);
//			return r;
//		}
//
//		iterator_map& operator--()
//		{
//			if (_root->issentinel())
//			{
//				if (_root->parent && _root->child_side() == RIGHT)
//					_root = _root->parent;
//			}
//			else if (_root->child_side() != LEFT)
//			{
//				if (_root->left_child)
//				{
//					_root = _root->left_child;
//					while (_root->right_child)
//						_root = _root->right_child;
//				}
//				else if (_root->parent)
//					_root = _root->parent;
//			}
//			else
//			{
//				if (_root->left_child)
//				{
//					_root = _root->left_child;
//					while (_root->right_child)
//						_root = _root->right_child;
//				}
//				else
//				{
//					_root = _root->parent;
//					while (_root->child_side() != RIGHT)
//						_root = _root->parent;
//					_root = _root->parent;
//				}
//			}
//			return *this;
//		}
//
//		iterator_map operator--(int)
//		{
//			iterator_map r = *this;
//			--(*this);
//			return r;
//		}
//
//		reference operator*() const { return *_root->content; }
//		pointer operator->() const { return &(*_root->content); }
//		template <typename U>
//		bool operator==(const iterator_map<U>& rhs) const { return _root == rhs._root; }
//		template <typename U>
//		bool operator!=(const iterator_map<U>& rhs) const { return _root != rhs._root; }
//
//		node_pointer get_root() const { return _root; }
//		bool issentinel() const { return !_root->content; }
//		int child_side() const { return _root->child_side(); }
//
//	private:
//
//		node_pointer	_root;
//
//	};
//
//}
//
//#endif