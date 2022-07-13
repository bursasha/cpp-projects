#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <climits>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <algorithm>
#include <set>
#include <queue>
#include <memory>
#include <functional>
#include <stdexcept>
using namespace std;
#endif /* __PROGTEST__ */

struct TNode
{
	// char t_Sign;
	vector<int> t_Sign;
	TNode *t_Left;
	TNode *t_Right;
};

bool pushByte(vector<int> &v_byte, ifstream &ifs)
{
	char ch_byte;
	ifs.get(ch_byte);
	if (ifs.fail())
		return false;
	for (int i = 7; i >= 0; i--)
		v_byte.push_back((ch_byte >> i) & 1);
	return true;
}

uint8_t convertSign(vector<int> &v_byte)
{
	uint8_t int_sign = 0, base = 1;
	for (int i = 7; i >= 0; i--)
	{
		if (v_byte.at(i) == 1)
			int_sign += base;
		base *= 2;
	}
	return int_sign;
}

int convertSize(vector<int> &v_byte)
{
	int int_size = 0, base = 1;
	for (int i = 11; i >= 0; i--)
	{
		if (v_byte.at(i) == 1)
			int_size += base;
		base *= 2;
	}
	return int_size;
}

bool buildTree(TNode *&tree, vector<int> &v_byte, ifstream &ifs)
{
	if (v_byte.empty())
	{
		if (!pushByte(v_byte, ifs))
			return false;
	}
	
	if (v_byte.at(0) == 0)
	{
		v_byte.erase(v_byte.begin());
		tree = new TNode();
		if (!buildTree(tree->t_Left, v_byte, ifs))
			return false;
		if (!buildTree(tree->t_Right, v_byte, ifs))
			return false;
		return true;
	}
	else if (v_byte.at(0) == 1)
	{
		v_byte.erase(v_byte.begin());
		if (v_byte.size() <= 8)
		{
			if (!pushByte(v_byte, ifs))
				return false;
		}
		tree = new TNode();
		if (v_byte.at(0) == 0)
		{
			vector<int> to_push;
			for (size_t i = 0; i < 8; i++)
				to_push.push_back(v_byte.at(i));
			// copy(v_byte.begin(), v_byte.begin() + 8, to_push);
			tree->t_Sign.push_back(convertSign(to_push));
			v_byte.erase(v_byte.begin(), v_byte.begin() + 8);
		}
		else if (v_byte.at(0) == 1 && v_byte.at(1) == 1 && v_byte.at(2) == 0)
		{
			vector<int> to_push;
			int count = 1;
			for (int i = 0; i < 2; i++)
			{
				if (v_byte.size() <= 8)
				{
					if (!pushByte(v_byte, ifs) && count > 1 && !(v_byte.at(0) == 1 && v_byte.at(1) == 0))
					{
						delete tree;
						return false;
					}
				}
				// if (count > 1 && !(v_byte.at(0) == 1 && v_byte.at(1) == 0))
				// {
				// 	delete tree;
				// 	return false;
				// }
				for (size_t i = 0; i < 8; i++)
					to_push.push_back(v_byte.at(i));
				// copy(v_byte.begin(), v_byte.begin() + 8, to_push);
				tree->t_Sign.push_back(convertSign(to_push));
				to_push.clear();
				v_byte.erase(v_byte.begin(), v_byte.begin() + 8);
				++count;
			}
		}
		else if (v_byte.at(0) == 1 && v_byte.at(1) == 1 && v_byte.at(2) == 1 && v_byte.at(3) == 0)
		{
			vector<int> to_push;
			int count = 1;
			for (int i = 0; i < 3; i++)
			{
				if (v_byte.size() <= 8)
				{
					if (!pushByte(v_byte, ifs) && count > 1 && !(v_byte.at(0) == 1 && v_byte.at(1) == 0))
					{
						delete tree;
						return false;
					}
				}
				// if (count > 1 && !(v_byte.at(0) == 1 && v_byte.at(1) == 0))
				// {
				// 	delete tree;
				// 	return false;
				// }
				for (size_t i = 0; i < 8; i++)
					to_push.push_back(v_byte.at(i));
				// copy(v_byte.begin(), v_byte.begin() + 8, to_push);
				tree->t_Sign.push_back(convertSign(to_push));
				to_push.clear();
				v_byte.erase(v_byte.begin(), v_byte.begin() + 8);
				++count;
			}
		}
		else if (v_byte.at(0) == 1 && v_byte.at(1) == 1 && v_byte.at(2) == 1 && v_byte.at(3) == 1 && v_byte.at(4) == 0)
		{
			vector<int> to_push;
			int count = 1;
			for (int i = 0; i < 4; i++)
			{
				if (v_byte.size() <= 8)
				{
					if (!pushByte(v_byte, ifs) && count > 1 && !(v_byte.at(0) == 1 && v_byte.at(1) == 0))
					{
						delete tree;
						return false;
					}
				}
				// if (count > 1 && !(v_byte.at(0) == 1 && v_byte.at(1) == 0))
				// {
				// 	delete tree;
				// 	return false;
				// }
				for (size_t i = 0; i < 8; i++)
					to_push.push_back(v_byte.at(i));
				// copy(v_byte.begin(), v_byte.begin() + 8, to_push);
				tree->t_Sign.push_back(convertSign(to_push));
				to_push.clear();
				v_byte.erase(v_byte.begin(), v_byte.begin() + 8);
				++count;
			}
		}

		// tree = new TNode();
		// tree->t_Sign = char(convertSign(v_byte));
		// v_byte.erase(v_byte.begin(), v_byte.begin() + 8);
		return true;
	}
	return true;
}

// void showTree(TNode *&tmp)
// {
// 	if (!(tmp->t_Left) && !(tmp->t_Right))
// 	{
// 		cout << tmp->t_Sign << endl;
// 		return;
// 	}
// 	if (tmp->t_Left)
// 		showTree(tmp->t_Left);
// 	if (tmp->t_Right)
// 		showTree(tmp->t_Right);
// }

bool getSigns(TNode *&tree, vector<int> &v_byte, ifstream &ifs, ofstream &ofs)
{
	if (!(tree->t_Left) && !(tree->t_Right))
	{
		for (size_t i = 0; i < tree->t_Sign.size(); i++)
		{
			if (!ofs.put(tree->t_Sign.at(i)))
				return false;
		}
		// ofs << tree->t_Sign;
		// cout << tree->t_Sign;
		if (ofs.fail() || !(ofs.is_open()))
			return false;
		return true;
	}

	if (v_byte.empty())
	{
		if (!pushByte(v_byte, ifs))
			return false;
	}

	if (v_byte.at(0) == 0)
	{
		v_byte.erase(v_byte.begin());
		if (!getSigns(tree->t_Left, v_byte, ifs, ofs))
			return false;
		return true;
	}
	else if (v_byte.at(0) == 1)
	{
		v_byte.erase(v_byte.begin());
		if (!getSigns(tree->t_Right, v_byte, ifs, ofs))
			return false;
		return true;
	}
	return true;
}

bool getChunk(TNode *&tree, vector<int> &v_byte, ifstream &ifs, ofstream &ofs)
{
	if (v_byte.empty())
	{
		if (!pushByte(v_byte, ifs))
			return false;
	}

	if (v_byte.at(0) == 0)
	{
		v_byte.erase(v_byte.begin());
		while (v_byte.size() < 12)
		{
			if (!pushByte(v_byte, ifs))
				return false;
		}
		int size_chunk = convertSize(v_byte);
		// if (size_chunk == 0)
		// 	return false;
			 
		v_byte.erase(v_byte.begin(), v_byte.begin() + 12);
		for (int i = 0; i < size_chunk; i++)
		{
			if (!getSigns(tree, v_byte, ifs, ofs))
				return false;
		}
		return true;
	}
	else if (v_byte.at(0) == 1)
	{
		v_byte.erase(v_byte.begin());
		for (int i = 0; i < 4096; i++)
		{
			if (!getSigns(tree, v_byte, ifs, ofs))
				return false;
		}
		if (!getChunk(tree, v_byte, ifs, ofs))
			return false;
	}
	return true;
}

void deleteTree(TNode *&tmp)
{
	if (!tmp)
		return;
	if (tmp->t_Left)
		deleteTree(tmp->t_Left);
	if (tmp->t_Right)
		deleteTree(tmp->t_Right);
	delete tmp;
}

bool decompressFile ( const char * inFileName, const char * outFileName )
{
	ifstream ifs(inFileName, ios::binary);

	if (ifs.fail() || !(ifs.is_open()))
	{
		ifs.close();
		return false;
	}

	vector<int> v_byte;
	TNode *tree = nullptr;
	if (!buildTree(tree, v_byte, ifs))
	{
		deleteTree(tree);
		ifs.close();
		return false;
	}
	// showTree(tree);
	ofstream ofs(outFileName, ios::binary);
	if (!getChunk(tree, v_byte, ifs, ofs))
	{
		deleteTree(tree);
		ifs.close();
		ofs.close();
		remove(outFileName);
		return false;
	}
	if (!(ofs.is_open()) || ofs.fail() || !ofs)
	{
		deleteTree(tree);
		ifs.close();
		ofs.close();
		remove(outFileName);
		return false;
	}
	return true;
}

bool compressFile ( const char * inFileName, const char * outFileName )
{
  // keep this dummy implementation (no bonus) or implement the compression (bonus)
  return false;
}
#ifndef __PROGTEST__
bool identicalFiles ( const char * fileName1, const char * fileName2 )
{
	ifstream file1(fileName1), file2(fileName2);
	if (file1.fail() || !(file1.is_open()) || file2.fail() || !(file2.is_open()))
	{
		file1.close();
		file2.close();
		return false;
	}

	while ((!file1.eof()) && (!file2.eof()))
	{
		string line1, line2;
		getline(file1, line1);
		getline(file2, line2);
		if (line1 != line2)
		{
			file1.close();
			file2.close();
			return false;
		}
		file1.close();
		file2.close();
	}

	return true;
}

int main ( void )
{
	assert(decompressFile("tests/test0.huf", "tempfile"));
	assert(identicalFiles("tests/test0.orig", "tempfile"));

	assert(decompressFile("tests/test1.huf", "tempfile"));
	assert(identicalFiles("tests/test1.orig", "tempfile"));

	assert(decompressFile("tests/test2.huf", "tempfile"));
	assert(identicalFiles("tests/test2.orig", "tempfile"));

	assert(decompressFile("tests/test3.huf", "tempfile"));
	assert(identicalFiles("tests/test3.orig", "tempfile"));

	assert(decompressFile("tests/test4.huf", "tempfile"));
	assert(identicalFiles("tests/test4.orig", "tempfile"));

	assert(decompressFile("tests/extra0.huf", "tempfile"));
	assert(identicalFiles("tests/extra0.orig", "tempfile"));

	assert(decompressFile("tests/extra1.huf", "tempfile"));
	assert(identicalFiles("tests/extra1.orig", "tempfile"));

	assert(decompressFile("tests/extra2.huf", "tempfile"));
	assert(identicalFiles("tests/extra2.orig", "tempfile"));

	assert(decompressFile("tests/extra3.huf", "tempfile"));
	assert(identicalFiles("tests/extra3.orig", "tempfile"));

	assert(decompressFile("tests/extra4.huf", "tempfile"));
	assert(identicalFiles("tests/extra4.orig", "tempfile"));

	assert(decompressFile("tests/extra5.huf", "tempfile"));
	assert(identicalFiles("tests/extra5.orig", "tempfile"));

	assert(decompressFile("tests/extra6.huf", "tempfile"));
	assert(identicalFiles("tests/extra6.orig", "tempfile"));

	assert(decompressFile("tests/extra7.huf", "tempfile"));
	assert(identicalFiles("tests/extra7.orig", "tempfile"));

	assert(decompressFile("tests/extra8.huf", "tempfile"));
	assert(identicalFiles("tests/extra8.orig", "tempfile"));

	assert(decompressFile("tests/extra9.huf", "tempfile"));
	assert(identicalFiles("tests/extra9.orig", "tempfile"));

	return 0;
}
#endif /* __PROGTEST__ */
