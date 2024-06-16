#ifndef __PROGTEST__
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <vector>
#include <array>
#include <cassert>
using namespace std;
#endif /* __PROGTEST__ */

class Contacts
{
private:
	vector <vector <string> > m_ContArr;
	vector <string> m_Cont;
	string m_Word;

public:
	Contacts();
	bool add_contact(const string buffer);
	void find_contact(const string buffer, ostream &out);
	void print();

};

Contacts::Contacts()
: m_ContArr(), m_Cont(), m_Word("")
{
}

bool Contacts::add_contact(const string buffer)
{
	stringstream sbuff;
	sbuff << buffer;
	while (sbuff >> m_Word)
		m_Cont.push_back(m_Word);
	m_ContArr.push_back(m_Cont);
	if (m_Cont.size() != 3 || m_Cont.at(2).length() != 9 || m_Cont.at(2).at(0) == '0')
		return false;
	for (size_t i = 0; i < m_Cont.at(2).length(); i++)
	{
		if (!isdigit(m_Cont.at(2).at(i)))
			return false;
	}
	m_Cont.clear();
	return true;
}

void Contacts::find_contact(const string data, ostream &out)
{
	int cont_cnt = 0;
	for (size_t i = 0; i < m_ContArr.size(); i++)
	{
		if (data == m_ContArr.at(i).at(0) || data == m_ContArr.at(i).at(1))
		{
			out << m_ContArr.at(i).at(0) << " " << m_ContArr.at(i).at(1) << " " << m_ContArr.at(i).at(2) << endl;
			++cont_cnt;
		}
	}
	out << "-> " << cont_cnt << endl;
}

void Contacts::print()
{
	for (size_t i = 0; i < m_ContArr.size(); i++)
	{
		for (size_t j = 0; j < m_ContArr.at(i).size(); j++)
			cout << m_ContArr.at(i).at(j) << " ";
		cout << endl;
	}
}

bool report ( const string & fileName, ostream & out )
{
	Contacts contacts;
	string buffer;
	ifstream file(fileName);

	if (!(file.is_open()))
		return false;

	while (getline(file, buffer) && !buffer.empty())
	{
		if (!contacts.add_contact(buffer))
			return false;
	}
	// contacts.print();
	while (getline(file, buffer))
		contacts.find_contact(buffer, out);

	file.close();

	return true;
}

#ifndef __PROGTEST__
int main ()
{
  ostringstream oss;
  oss . str ( "" );
  assert ( report( "tests/test0_in.txt", oss ) == true );
//   cout << oss.str();
  assert ( oss . str () ==
    "John Christescu 258452362\n"
    "John Harmson 861647702\n"
    "-> 2\n"
    "-> 0\n"
    "Josh Dakhov 264112084\n"
    "Dakhov Speechley 865216101\n"
    "-> 2\n"
    "John Harmson 861647702\n"
    "-> 1\n" );
  oss . str ( "" );
  assert ( report( "tests/test1_in.txt", oss ) == false );
  return 0;
}
#endif /* __PROGTEST__ */
