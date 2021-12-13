#ifndef NODE_HPP
# define NODE_HPP

# include "utils.hpp"

namespace ft {

	/*
	**	Node class template
	*/

	template <class T1, class T2>
	struct Node
	{
		typedef typename	ft::pair<T1, T2>*	pointer;
		typedef				Node*				node_pointer;

		pointer			content;
		node_pointer	left_child;
		node_pointer	right_child;
		node_pointer	parent;

		bool issentinel() { return !this->content; }
		bool isleaf() { return !this->right_child && !this->left_child; }

		node_pointer has_one_child_leaf()
		{
			if (!this->right_child && this->left_child && !this->left_child->right_child && !this->left_child->left_child)
				return this->left_child;
			else if (this->right_child && !this->left_child && !this->right_child->right_child && !this->right_child->left_child)
				return this->right_child;
			return 0;
		}

		int child_side()
		{
			if (!this->parent)
				return ROOT;
			else if (this->parent->right_child == this)
				return RIGHT;
			else
				return LEFT;
		}

		/*
		**	returns inorder successor if there is a right_child or the inorder predecessor if there isn't
		*/

		node_pointer inorder_xcessor()
		{
			node_pointer r = this;
			if (r->right_child)
			{
				r = r->right_child;
				while (r->left_child)
					r = r->left_child;
				return r;
			}
			else
			{
				r = r->left_child;
				while (r->right_child)
					r = r->right_child;
				return r;
			}
		}

	};

}

#endif