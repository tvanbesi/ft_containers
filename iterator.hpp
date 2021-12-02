#ifndef ITERATOR_HPP
# define ITERATOR_HPP

namespace ft {

	/*
	**	Template prototypes
	*/

	template	<class Category,
				class T,
				class Distance,
				class Pointer,
				class Reference
				> class iterator;
	template	<class Iterator>	class reverse_iterator;
	template	<class Iterator>	class iterator_traits;
	template	<class T>			class iterator_traits<T*>;
	template	<class T>			class iterator_traits<const T*>;

	/*
	**	Iterator tags
	*/

	struct input_access_iterator_tag			{};
	struct output_access_iterator_tag			{};
	struct forward_access_iterator_tag			{};
	struct bidirectional_access_iterator_tag	{};
	struct random_access_iterator_tag			{};

	/*
	**	iterator class template
	*/

    template <	class Category,
				class T,
				class Distance = std::ptrdiff_t,
				class Pointer = T*,
				class Reference = T&
				>
	class iterator {

	public:

		typedef	Category	iterator_category;
		typedef	T			value_type;
		typedef	Distance	difference_type;
		typedef	T*			pointer;
		typedef	T&			reference;

	};

	/*
	**	reverse_iterator class template
	*/

	template <class Iterator>
	class reverse_iterator {

	public:

		typedef	Iterator													iterator_type;
		typedef typename	ft::iterator_traits<Iterator>::iterator_category	iterator_category;
		typedef typename	ft::iterator_traits<Iterator>::value_type			value_type;
		typedef typename	ft::iterator_traits<Iterator>::difference_tyepe		difference_type;
		typedef typename	ft::iterator_traits<Iterator>::pointer				pointer;
		typedef typename	ft::iterator_traits<Iterator>::reference			reference;

	};

	/*
	**	iterator_traits class template
	*/

	template <class Iterator>
	class iterator_traits {

	public:

		typedef typename	Iterator::difference_type	difference_type;
		typedef typename	Iterator::value_type		value_type;
		typedef typename	Iterator::pointer			pointer;
		typedef typename	Iterator::reference			reference;
		typedef typename	Iterator::iterator_category	iterator_category;

	};

	template <class T>
	class iterator_traits<T*> {

	public:

		typedef	std::ptrdiff_t				difference_type;
		typedef	T							value_type;
		typedef	T*							pointer;
		typedef	T&							reference;
		typedef	random_access_iterator_tag	iterator_category;

	};

	template <class T>
	class iterator_traits<const T*> {

	public:

		typedef	std::ptrdiff_t				difference_type;
		typedef	T							value_type;
		typedef	const T*					pointer;
		typedef	const T&					reference;
		typedef	random_access_iterator_tag	iterator_category;

	};

}

#endif