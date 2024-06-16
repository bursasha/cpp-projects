#include <cstddef>
#include <vector>
#include <cassert>
#include <list>
#include <iostream>
#include <ctime>
#include <cstdlib>

template <class T>
class LinkedList
{
	struct Node
	{
		Node(T val, Node *next = nullptr) : val(val), next(next) {}
		T val;
		Node *next;
		~Node()
		{
			delete next;
		}
	};
	Node *head;
	Node *tail;

	/// Copies linked list, other must not be empty list.
	Node *CopyList(Node *other)
	{
		assert(other != nullptr);
		if (other->next == nullptr)
		{
			tail = new Node(other->val, nullptr);
			return tail;
		}
		return new Node(other->val, CopyList(other->next));
	}

public:
	class Iterator
	{
		Node *ptr;

	public:
		Iterator(Node *node) : ptr(node) {}
		/// Returns member at which the iterator points
		int &operator*()
		{
			return ptr->val;
		}

		/// Advances iterator one position forward
		Iterator operator++(T)
		{
			Iterator it(ptr);
			++(*this);
			return it;
		}

		Iterator &operator++()
		{
			ptr = ptr->next;
			return *this;
		}

		/// Compares two iterators.
		friend bool operator==(const Iterator &iter1, const Iterator &iter2)
		{
			return iter1.ptr == iter2.ptr;
		}

		friend bool operator!=(const Iterator &iter1, const Iterator &iter2)
		{
			return !(iter1 == iter2);
		}
	};

	class ConstIterator
	{
		Node *ptr;

	public:
		ConstIterator(Node *node) : ptr(node) {}
		/// Returns member at which the iterator points
		const T &operator*() const
		{
			return ptr->val;
		}

		/// Advances iterator one position forward
		ConstIterator operator++(T)
		{
			ConstIterator it(ptr);
			++(*this);
			return it;
		}

		ConstIterator &operator++()
		{
			ptr = ptr->next;
			return *this;
		}

		/// Compares two iterators.
		friend bool operator==(const ConstIterator &iter1, const ConstIterator &iter2)
		{
			return iter1.ptr == iter2.ptr;
		}

		friend bool operator!=(const ConstIterator &iter1, const ConstIterator &iter2)
		{
			return !(iter1 == iter2);
		}
	};

	LinkedList() : head(nullptr), tail(nullptr) {}

	void append(T val);

	void prepend(T val);

	/// Iterator to first element of the list
	Iterator begin()
	{
		return Iterator(head);
	}

	ConstIterator begin() const
	{
		return ConstIterator(head);
	}

	/// Iterator to first element of the list
	ConstIterator cbegin() const
	{
		return ConstIterator(head);
	}

	/// Iterator to 'past' the end element of the list
	Iterator end()
	{
		return Iterator(nullptr);
	}

	ConstIterator end() const
	{
		return ConstIterator(nullptr);
	}

	/// Iterator to 'past' the end element of the list
	ConstIterator cend() const
	{
		return ConstIterator(nullptr);
	}

	LinkedList(const LinkedList &other)
	{
		if (other.head == nullptr)
		{
			head = tail = nullptr;
		}

		head = CopyList(other.head); // tail is set as a side effect of CopyList. How to perform better?
	}

	LinkedList &operator=(LinkedList other)
	{
		std::swap(head, other.head);
		std::swap(tail, other.tail);
		return *this;
	}

	~LinkedList()
	{
		delete head;
	}

	void print(std::ostream &os) const
	{
		for (auto it = cbegin(); it != cend(); ++it)
		{
			os << *it << " ";
		}
		os << "\n";
	}
};

template <class T>
std::ostream &operator<<(std::ostream &os, const LinkedList<T> &l)
{
	for (const auto &x : l)
	{
		os << x << " ";
	}
	return os;
}

template <class T>
void LinkedList<T>::append(T val)
{
	Node *new_node = new Node(val);
	if (head == nullptr)
	{
		head = tail = new_node;
		return;
	}
	tail->next = new_node;
	tail = new_node;
}

template <class T>
void LinkedList<T>::prepend(T val)
{
	Node *new_node = new Node(val);
	if (head == nullptr)
	{
		head = tail = new_node;
		return;
	}
	new_node->next = head;
	head = new_node;
}

template <class T>
void randomTest(T op)
{
	std::list<int> ref_l;
	LinkedList l;
	while (op--)
	{
		int val = rand() % 10000;
		if (rand() % 2)
		{
			ref_l.push_back(val);
			l.append(val);
		}
		else
		{
			ref_l.insert(ref_l.begin(), val);
			l.prepend(val);
		}
	}
	auto ref_it = ref_l.begin();
	auto it = l.begin();

	while (ref_it != ref_l.end())
	{
		assert(*ref_it++ == *it++);
	}
	assert(it == l.end());
}

int main()
{
	srand(time(NULL));

	randomTest(100);
	randomTest(1000);
	randomTest(10000);
}
