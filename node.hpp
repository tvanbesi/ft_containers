#ifndef NODE_HPP
# define NODE_HPP

enum Node_color { BLACK, RED };

# define LEFT	0
# define RIGHT	1
# define left	child[LEFT]
# define right	child[RIGHT]
# define child_side(node) (node == node->parent->right ? RIGHT : LEFT)

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
			while (r->left)
				r = r->left;
			return r;
		}

		node_pointer inorder_predecessor()
		{
			node_pointer r;
			r = this->left;
			while (r->right)
				r = r->right;
			return r;
		}

		void swap(node_pointer node)
		{
			std::cout << "this(root): " << this->content->first << " node: " << node->content->first << std::endl;
			pointer			tmp_content = this->content;
			node_pointer	tmp_parent = this->parent;
			node_pointer	tmp_child_left = this->left;
			node_pointer	tmp_child_right = this->right;
			enum Node_color	tmp_color = this->color;
			this->content = node->content;
			this->parent = node->parent;
			this->left = node->left;
			this->right = node->right;
			this->color = node->color;
			node->content = tmp_content;
			node->parent = tmp_parent;
			node->left = tmp_child_left;
			node->right = tmp_child_right;
			node->color = tmp_color;
		}
	};

}

#endif