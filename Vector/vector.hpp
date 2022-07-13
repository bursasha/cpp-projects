#include <cstdlib>

class Vector {
	int *data;
	size_t capacity;
	size_t cnt;
	void realloc();
public:
	Vector();
	~Vector();
	void push_back(int val);
	size_t size() const;
	int& at(size_t idx);
	void insert(size_t idx, int val);
};
