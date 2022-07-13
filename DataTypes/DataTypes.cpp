#ifndef __PROGTEST__
#include <cstring>
#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cctype>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <deque>
#include <stdexcept>
#include <algorithm>
#include <typeinfo>
#include <unordered_map>
#include <unordered_set>
#include <memory>
using namespace std;
#endif /* __PROGTEST__ */

class CDataTypeInterface
{
public:
	virtual ~CDataTypeInterface () = default;
	virtual size_t getSize () const = 0;
	virtual void getType (ostream &os) const = 0;
	virtual CDataTypeInterface *clone () const = 0; 
	virtual bool operator == (const CDataTypeInterface &to_compare) const = 0;
	virtual bool operator != (const CDataTypeInterface &to_compare) const = 0;

};

class CDataTypeInt : public CDataTypeInterface
{
public:
	CDataTypeInt ()
	{
	}
	~CDataTypeInt ()
	{
	}
	size_t getSize () const override
	{
		return 4;
	}
	void getType (ostream &os) const override
	{
		os << "int";
	}
	CDataTypeInterface *clone () const override
	{
		return new CDataTypeInt (*this);
	}
	bool operator == (const CDataTypeInterface &to_compare) const override
	{
		return typeid(*this) == typeid(to_compare);
	}
	bool operator != (const CDataTypeInterface &to_compare) const override
	{
		return !(operator == (to_compare));
	}

};

class CDataTypeDouble : public CDataTypeInterface
{
public:
	CDataTypeDouble ()
	{
	}
	~CDataTypeDouble ()
	{
	}
	size_t getSize () const override
	{
		return 8;
	}
	void getType (ostream &os) const override
	{
		os << "double";
	}
	CDataTypeInterface *clone () const override
	{
		return new CDataTypeDouble(*this);
	}
	bool operator == (const CDataTypeInterface &to_compare) const override
	{
		return typeid(*this) == typeid(to_compare);
	}
	bool operator != (const CDataTypeInterface &to_compare) const override
	{
		return !(operator == (to_compare));
	}
};

class CDataTypeEnum : public CDataTypeInterface
{
public:
	CDataTypeEnum ()
	: c_Enum()
	{
	}
	CDataTypeEnum (const CDataTypeEnum &to_copy)
	: c_Enum(to_copy.c_Enum)
	{
	}
	~CDataTypeEnum ()
	{
		c_Enum.clear();
	}
	size_t getSize () const override
	{
		return 4;
	}
	void getType (ostream &os) const override
	{
		os << "enum" << endl << "{" << endl;
		for (auto to_print = c_Enum.begin(); to_print != c_Enum.end(); to_print++)
			to_print == --c_Enum.end() ? os << "  " << *to_print << endl : os << "  " << *to_print << "," << endl;
		os << "}";
	}
	CDataTypeInterface *clone () const override
	{
		return new CDataTypeEnum(*this);
	}
	CDataTypeEnum &add (const string to_add)
	{
		find(c_Enum.begin(), c_Enum.end(), to_add) != c_Enum.end()
		? throw invalid_argument("Duplicate enum value: " + to_add)
		: c_Enum.push_back(to_add);
		return *this;
	}
	bool operator == (const CDataTypeInterface &to_compare) const override
	{
		return typeid(*this) == typeid(to_compare) && c_Enum == dynamic_cast<const CDataTypeEnum &>(to_compare).c_Enum;
	}
	bool operator != (const CDataTypeInterface &to_compare) const override
	{
		return !(operator == (to_compare));
	}

protected:
	vector <string> c_Enum;

};

class CDataTypeStruct : public CDataTypeInterface
{
public:
	CDataTypeStruct ()
	: c_Struct(), c_Order()
	{
	}
	CDataTypeStruct (const CDataTypeStruct &to_copy)
	: c_Order(to_copy.c_Order)
	{
		for (const string &name_to_clone : to_copy.c_Order)
			c_Struct.insert(make_pair( name_to_clone, to_copy.c_Struct.at(name_to_clone)->clone() ));
	}
	~CDataTypeStruct ()
	{
		c_Struct.clear();
		c_Order.clear();
	}
	size_t getSize () const override
	{
		size_t size = 0;
		for (auto &value : c_Struct)
			size += value.second->getSize();
		return size;
	}
	void getType (ostream &os) const override
	{
		os << "struct" << endl << "{" << endl;
		for (auto &to_print : c_Order)
		{
			os << " ";
			c_Struct.at(to_print)->getType(os);
			os << " " << to_print << ";";
		}
		os << "}";
	}
	CDataTypeInterface *clone () const override
	{
		return new CDataTypeStruct(*this);
	}
	CDataTypeStruct &addField (const string name_to_add, const CDataTypeInterface &type_to_add)
	{
		c_Struct.count(name_to_add)
		? throw invalid_argument("Duplicate field: " + name_to_add)
		: c_Struct.insert(make_pair( name_to_add, unique_ptr <CDataTypeInterface> (type_to_add.clone()) )),
		c_Order.push_back(name_to_add);
		return *this;
	}
	CDataTypeInterface &field (const string to_find) const
	{
		if (!c_Struct.count(to_find))
			throw invalid_argument("Unknown field: " + to_find);
		return *(c_Struct.at(to_find));
	}
	bool operator == (const CDataTypeInterface &to_compare) const override
	{
		if (typeid(*this) == typeid(to_compare) && c_Order.size() == dynamic_cast<const CDataTypeStruct &>(to_compare).c_Order.size())
		{
			CDataTypeStruct dc_to_compare = dynamic_cast<const CDataTypeStruct &>(to_compare);
			for (size_t idx = 0; idx < c_Order.size(); idx++)
			{
				if ( typeid(c_Struct.at(c_Order[idx]).get()) != typeid(dc_to_compare.c_Struct.at(dc_to_compare.c_Order[idx]).get()) ||
					 *c_Struct.at(c_Order[idx]) != *dc_to_compare.c_Struct.at(dc_to_compare.c_Order[idx]) )
					return false;
			}
			return true;
		}
		return false;
	}
	bool operator != (const CDataTypeInterface &to_compare) const override
	{
		return !(operator == (to_compare));
	}

protected:
	unordered_map <string, unique_ptr <CDataTypeInterface> > c_Struct;
	vector <string> c_Order;

};

ostream &operator << (ostream &os, const CDataTypeInterface &to_print)
{
	to_print.getType(os);
	return os;
}

#ifndef __PROGTEST__
static bool whitespaceMatch (const string &a, const string &b)
{
	for (size_t i = 0, j = 0; i < a.size() && j < b.size(); i++, j++)
	{
		while (isspace(a[i]) && i < a.size()) i++;
		while (isspace(b[j]) && j < b.size()) j++;
		if (a[i]!=b[j]) return false;
	}
	return true;
}

template <typename T_>
static bool whitespaceMatch (const T_ &x, const string &ref)
{
  ostringstream oss;
  oss << x;
  return whitespaceMatch ( oss . str (), ref );
}

int main ( void )
{
  CDataTypeStruct  a = CDataTypeStruct () .
                        addField ( "m_Length", CDataTypeInt () ) .
                        addField ( "m_Status", CDataTypeEnum () . 
                          add ( "NEW" ) . 
                          add ( "FIXED" ) . 
                          add ( "BROKEN" ) . 
                          add ( "DEAD" ) ).
                        addField ( "m_Ratio", CDataTypeDouble () );
  
  CDataTypeStruct b = CDataTypeStruct () .
                        addField ( "m_Length", CDataTypeInt () ) .
                        addField ( "m_Status", CDataTypeEnum () . 
                          add ( "NEW" ) . 
                          add ( "FIXED" ) . 
                          add ( "BROKEN" ) . 
                          add ( "READY" ) ).
                        addField ( "m_Ratio", CDataTypeDouble () );
  
  CDataTypeStruct c =  CDataTypeStruct () .
                        addField ( "m_First", CDataTypeInt () ) .
                        addField ( "m_Second", CDataTypeEnum () . 
                          add ( "NEW" ) . 
                          add ( "FIXED" ) . 
                          add ( "BROKEN" ) . 
                          add ( "DEAD" ) ).
                        addField ( "m_Third", CDataTypeDouble () );

  CDataTypeStruct  d = CDataTypeStruct () .
                        addField ( "m_Length", CDataTypeInt () ) .
                        addField ( "m_Status", CDataTypeEnum () . 
                          add ( "NEW" ) . 
                          add ( "FIXED" ) . 
                          add ( "BROKEN" ) . 
                          add ( "DEAD" ) ).
                        addField ( "m_Ratio", CDataTypeInt () );
  assert ( whitespaceMatch ( a, "struct\n"
    "{\n"
    "  int m_Length;\n"
    "  enum\n"
    "  {\n"
    "    NEW,\n"
    "    FIXED,\n"
    "    BROKEN,\n"
    "    DEAD\n"
    "  } m_Status;\n"
    "  double m_Ratio;\n"
    "}") );

  assert ( whitespaceMatch ( b, "struct\n"
    "{\n"
    "  int m_Length;\n"
    "  enum\n"
    "  {\n"
    "    NEW,\n"
    "    FIXED,\n"
    "    BROKEN,\n"
    "    READY\n"
    "  } m_Status;\n"
    "  double m_Ratio;\n"
    "}") );

  assert ( whitespaceMatch ( c, "struct\n"
    "{\n"
    "  int m_First;\n"
    "  enum\n"
    "  {\n"
    "    NEW,\n"
    "    FIXED,\n"
    "    BROKEN,\n"
    "    DEAD\n"
    "  } m_Second;\n"
    "  double m_Third;\n"
    "}") );

  assert ( whitespaceMatch ( d, "struct\n"
    "{\n"
    "  int m_Length;\n"
    "  enum\n"
    "  {\n"
    "    NEW,\n"
    "    FIXED,\n"
    "    BROKEN,\n"
    "    DEAD\n"
    "  } m_Status;\n"
    "  int m_Ratio;\n"
    "}") );

  assert ( a != b );
  assert ( a == c );
  assert ( a != d );
  assert ( a . field ( "m_Status" ) == CDataTypeEnum () . add ( "NEW" ) . add ( "FIXED" ) . add ( "BROKEN" ) . add ( "DEAD" ) );
  assert ( a . field ( "m_Status" ) != CDataTypeEnum () . add ( "NEW" ) . add ( "BROKEN" ) . add ( "FIXED" ) . add ( "DEAD" ) );
  assert ( a != CDataTypeInt() );
  assert ( whitespaceMatch ( a . field ( "m_Status" ), "enum\n"
    "{\n"
    "  NEW,\n"
    "  FIXED,\n"
    "  BROKEN,\n"
    "  DEAD\n"
    "}") );

  CDataTypeStruct aOld = a;
  b . addField ( "m_Other", CDataTypeDouble ());

  a . addField ( "m_Sum", CDataTypeInt ());

  assert ( a != aOld );
  assert ( a != c );
  assert ( aOld == c );
  assert ( whitespaceMatch ( a, "struct\n"
    "{\n"
    "  int m_Length;\n"
    "  enum\n"
    "  {\n"
    "    NEW,\n"
    "    FIXED,\n"
    "    BROKEN,\n"
    "    DEAD\n"
    "  } m_Status;\n"
    "  double m_Ratio;\n"
    "  int m_Sum;\n"
    "}") );

  assert ( whitespaceMatch ( b, "struct\n"
    "{\n"
    "  int m_Length;\n"
    "  enum\n"
    "  {\n"
    "    NEW,\n"
    "    FIXED,\n"
    "    BROKEN,\n"
    "    READY\n"
    "  } m_Status;\n"
    "  double m_Ratio;\n"
    "  double m_Other;\n"
    "}") );

  c . addField ( "m_Another", a . field ( "m_Status" ));

  assert ( whitespaceMatch ( c, "struct\n"
    "{\n"
    "  int m_First;\n"
    "  enum\n"
    "  {\n"
    "    NEW,\n"
    "    FIXED,\n"
    "    BROKEN,\n"
    "    DEAD\n"
    "  } m_Second;\n"
    "  double m_Third;\n"
    "  enum\n"
    "  {\n"
    "    NEW,\n"
    "    FIXED,\n"
    "    BROKEN,\n"
    "    DEAD\n"
    "  } m_Another;\n"
    "}") );

  d . addField ( "m_Another", a . field ( "m_Ratio" ));

  assert ( whitespaceMatch ( d, "struct\n"
    "{\n"
    "  int m_Length;\n"
    "  enum\n"
    "  {\n"
    "    NEW,\n"
    "    FIXED,\n"
    "    BROKEN,\n"
    "    DEAD\n"
    "  } m_Status;\n"
    "  int m_Ratio;\n"
    "  double m_Another;\n"
    "}") );

  assert ( a . getSize () == 20 );
  assert ( b . getSize () == 24 );
  try
  {
    a . addField ( "m_Status", CDataTypeInt () );
    assert ( "addField: missing exception!" == nullptr );
  }
  catch ( const invalid_argument & e )
  {
    assert ( e . what () == "Duplicate field: m_Status"sv );
  }

  try
  {
    cout << a . field ( "m_Fail" ) << endl;
    assert ( "field: missing exception!" == nullptr );
  }
  catch ( const invalid_argument & e )
  {
    assert ( e . what () == "Unknown field: m_Fail"sv );
  }

  try
  {
    CDataTypeEnum en;
    en . add ( "FIRST" ) .
         add ( "SECOND" ) .
         add ( "FIRST" );
    assert ( "add: missing exception!" == nullptr );
  }
  catch ( const invalid_argument & e )
  {
    assert ( e . what () == "Duplicate enum value: FIRST"sv );
  }

  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
