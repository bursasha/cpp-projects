#include "vector.hpp"

static const int INIT_SIZE = 2;
static const int ALLOC_MULT = 2;

Vector::Vector()
{
	data = new int[INIT_SIZE];
	capacity = INIT_SIZE;
	cnt = 0;
}

Vector::~Vector()
{
	delete [] data;
}

void Vector::push_back(int val)
{
	if(cnt >= capacity)
		realloc();
	data[cnt++] = val;
}

void Vector::realloc()
{
	capacity *= ALLOC_MULT;
	int *tmp = new int [capacity];
	for(size_t i = 0; i < size(); i++)
		tmp[i] = data[i];
	delete [] data;
	data = tmp;
}

size_t Vector::size() const
{
	return cnt;
}

int& Vector::at(size_t idx)
{
	return data[idx];
}

void Vector::insert(size_t idx, int val)
{
	if(idx > size()) throw std::invalid_argument("Invalid index");
	if (cnt >= capacity)
		realloc();
	for(int i = size() - 1; i >= idx; i--)
		data[i + 1] = data[i];
}
