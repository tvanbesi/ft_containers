#ifndef NODE_HPP
# define NODE_HPP

enum Node_color { BLACK, RED };

# define LEFT	0
# define RIGHT	1
# define END	0
# define REND	1
# define left	child[LEFT]
# define right	child[RIGHT]
# define child_side(node_pointer) (node_pointer == node_pointer->parent->right ? RIGHT : LEFT)
# define is_sentinel(node_pointer) (node_pointer->content == 0 ? true : false)

namespace ft {

	/*
	**	Node class template
	*/

	template <class T1, class T2>
	struct Node
	{
		typedef typename	ft::pair<T1, T2>*	pointer;
		typedef				Node*				node_pointer;
		typedef				Node&				node_reference;

		pointer			content;
		node_pointer	parent;
		node_pointer	child[2];
		enum Node_color	color;

		node_pointer inorder_successor()
		{
			node_pointer r;
			r = this->right;
			while (r->left && !is_sentinel(r->left))
				r = r->left;
			return r;
		}

		//void swap(node_pointer node)
		//{
		//}
	};

}

#endif