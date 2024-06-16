#include "vector.hpp"
#include <iostream>
#include <algorithm>

using namespace std;

int main() {
	Vector v;

	for(int i = 0; i < 5; i++)
		v.push_back(i);

	int tmp = v.at(1);
	v.at(1) = v.at(0);
	v.at(0) = tmp;

	for(int i = 0; i < 20; i++)
		v.insert(v.begin() + 4, 20);

	for(int i = 0; i < v.size(); i++)
		cout << v.at(i) << ", ";
	
	cout << endl;
}
