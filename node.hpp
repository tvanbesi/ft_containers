#ifndef NODE_HPP
# define NODE_HPP

# define TERMINAL_NODE		0
# define NONTERMINAL_NODE	1

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
		int				type;
	};

}

#endif