#ifndef ITERATOR_MAP_HPP
# define ITERATOR_MAP_HPP

# include "iterator.hpp"
# include "node.hpp"
# include "map.hpp"
# include "type_traits.hpp"

namespace ft {

	template <class T>
	class iterator_map : public ft::iterator<std::bidirectional_iterator_tag, T> {
	friend class iterator_map<typename remove_const<T>::value_type>;
	friend class iterator_map<const T>;

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

		iterator_map(node_pointer p = 0) : _p(p) {}
		iterator_map(const iterator_map& src) : _p(src._p) {}
		iterator_map& operator=(const iterator_map& src) { _p = src._p; return *this; }
		~iterator_map() {}

		operator iterator_map<const value_type>() const { return _p; }

		iterator_map& operator++()
		{
			if (is_sentinel(_p))
				_p = _p->parent;
			else if (_p->right)
			{
				_p = _p->right;
				while (_p->left)
					_p = _p->left;
			}
			else
			{
				node_pointer tmp = _p;
				while (tmp)
				{
					if (!tmp->parent)
					{
						_p = _p->right;
						break ; //sentinel
					}
					else if (child_side(tmp) == LEFT)
					{
						_p = tmp->parent;
						break ;
					}
					tmp = tmp->parent;
				}
			}
			return *this;
		}

		iterator_map operator++(int)
		{
			iterator_map r = *this;
			++(*this);
			return r;
		}

		iterator_map& operator--()
		{
			if (is_sentinel(_p))
				_p = _p->parent;
			else if (_p->left)
			{
				_p = _p->left;
				while (_p->right)
					_p = _p->right;
			}
			else
			{
				node_pointer tmp = _p;
				while (tmp)
				{
					if (!tmp->parent)
					{
						_p = _p->left;
						break ; //sentinel
					}
					else if (child_side(tmp) == RIGHT)
					{
						_p = tmp->parent;
						break ;
					}
					tmp = tmp->parent;
				}
			}
			return *this;
		}

		iterator_map operator--(int)
		{
			iterator_map r = *this;
			--(*this);
			return r;
		}

		reference operator*() const { return *_p->content; }
		pointer operator->() const { return &(*_p->content); }
		template <typename U>
		bool operator==(const iterator_map<U>& rhs) const { return _p == rhs._p; }
		template <typename U>
		bool operator!=(const iterator_map<U>& rhs) const { return _p != rhs._p; }

	private:

		node_pointer	_p;

	};

}

#endif