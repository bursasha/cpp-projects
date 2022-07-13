#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <cmath>
#include <cassert>
#include <iostream> 
#include <iomanip> 
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <memory>
using namespace std;
#endif /* __PROGTEST__ */

class CVATRegister
{
public:
	CVATRegister ( void );
	~CVATRegister ( void );
	bool newCompany ( const string &name, const string &addr, const string &taxID );
	bool cancelCompany ( const string &name, const string &addr );
	bool cancelCompany ( const string &taxID );
	bool invoice ( const string &taxID, unsigned int amount );
	bool invoice ( const string &name, const string &addr, unsigned int amount );
	bool audit ( const string &name, const string &addr, unsigned int &sumIncome ) const;
	bool audit ( const string &taxID, unsigned int &sumIncome ) const;
	bool firstCompany ( string &name, string &addr ) const;
	bool nextCompany ( string &name, string &addr ) const;
	unsigned int medianInvoice ( void ) const;

private:
	struct MCompany
	{
		string c_Name;
		string c_Addr;
		string c_TaxID;
		unsigned int c_Invoices;
	};
	vector<MCompany *> m_DBNameAddr;
	vector<MCompany *> m_DBTaxID;
	vector<unsigned int> m_DBInvoices;
	static void tolowerStr(string &str)
	{
		for (size_t i = 0; i < str.length(); i++)
			str.at(i) = tolower(str.at(i));
	}
	static bool compareNameAddr(const MCompany *inside, const MCompany *to_compare) // newCompany
	{
		string inside_name = inside->c_Name, inside_addr = inside->c_Addr;
		tolowerStr(inside_name); tolowerStr(inside_addr);
		string compare_name = to_compare->c_Name, compare_addr = to_compare->c_Addr;
		tolowerStr(compare_name); tolowerStr(compare_addr);
		return (inside_name < compare_name) || (inside_name == compare_name && inside_addr < compare_addr);
	}
	static bool compareTaxID(const MCompany *inside, const MCompany *to_compare) // newCompany
	{
		return (inside->c_TaxID < to_compare->c_TaxID);
	}
};

CVATRegister::CVATRegister(void)
: m_DBNameAddr(), m_DBTaxID(), m_DBInvoices()
{
}

CVATRegister::~CVATRegister(void)
{
	for (size_t i = 0; i < m_DBNameAddr.size(); i++)
		delete m_DBNameAddr.at(i);
	m_DBNameAddr.clear();
	m_DBNameAddr.clear();
}

bool CVATRegister::newCompany(const string &name, const string &addr, const string &taxID)
{
	string low_name1 = name, low_addr1 = addr;
	tolowerStr(low_name1); tolowerStr(low_addr1);
	for (size_t i = 0; i < m_DBNameAddr.size(); i++)
	{
		string low_name2 = m_DBNameAddr.at(i)->c_Name, low_addr2 = m_DBNameAddr.at(i)->c_Addr;
		tolowerStr(low_name2); tolowerStr(low_addr2);
		if ((low_name1 == low_name2 && low_addr1 == low_addr2) || taxID == m_DBNameAddr.at(i)->c_TaxID)
			return false;
	}
	MCompany *new_company = new MCompany();
	new_company->c_Name = name;
	new_company->c_Addr = addr;
	new_company->c_TaxID = taxID; 
	new_company->c_Invoices = 0;

	auto pos_name_addr = lower_bound(m_DBNameAddr.begin(), m_DBNameAddr.end(), new_company, compareNameAddr);
	auto pos_taxid = lower_bound(m_DBTaxID.begin(), m_DBTaxID.end(), new_company, compareTaxID);

	m_DBNameAddr.insert(pos_name_addr, new_company);
	m_DBTaxID.insert(pos_taxid, new_company);

	return true;
}

bool CVATRegister::cancelCompany(const string &name, const string &addr)
{
	MCompany *find_company = new MCompany();
	find_company->c_Name = name;
	find_company->c_Addr = addr;
	auto pos_name_addr = lower_bound(m_DBNameAddr.begin(), m_DBNameAddr.end(), find_company, compareNameAddr);
	if (pos_name_addr == m_DBNameAddr.end())
	{
		delete find_company;
		return false;
	}

	string low_name1 = name, low_addr1 = addr, low_name2 = (*pos_name_addr)->c_Name, low_addr2 = (*pos_name_addr)->c_Addr;
	tolowerStr(low_name1); tolowerStr(low_addr1); tolowerStr(low_name2); tolowerStr(low_addr2);
	if (low_name1 != low_name2 || low_addr1 != low_addr2)
	{
		delete find_company;
		return false;
	}

	find_company->c_TaxID = (*pos_name_addr)->c_TaxID;
	auto pos_taxid = lower_bound(m_DBTaxID.begin(), m_DBTaxID.end(), find_company, compareTaxID);
	if (pos_taxid == m_DBTaxID.end())
	{
		delete find_company;
		return false;
	}

	delete *pos_name_addr;
	m_DBNameAddr.erase(pos_name_addr);
	m_DBTaxID.erase(pos_taxid);

	delete find_company;

	return true;
}

bool CVATRegister::cancelCompany(const string &taxID)
{
	MCompany *find_company = new MCompany();
	find_company->c_TaxID = taxID;
	auto pos_taxid = lower_bound(m_DBTaxID.begin(), m_DBTaxID.end(), find_company, compareTaxID);
	if (pos_taxid == m_DBTaxID.end() || taxID != (*pos_taxid)->c_TaxID)
	{
		delete find_company;
		return false;
	}

	find_company->c_Name = (*pos_taxid)->c_Name;
	find_company->c_Addr = (*pos_taxid)->c_Addr;
	auto pos_name_addr = lower_bound(m_DBNameAddr.begin(), m_DBNameAddr.end(), find_company, compareNameAddr);
	if (pos_name_addr == m_DBNameAddr.end())
	{
		delete find_company;
		return false;
	}

	delete *pos_name_addr;
	m_DBNameAddr.erase(pos_name_addr);
	m_DBTaxID.erase(pos_taxid);

	delete find_company;

	return true;
}

bool CVATRegister::invoice(const string &taxID, unsigned int amount)
{
	MCompany *find_company = new MCompany();
	find_company->c_TaxID = taxID;
	auto pos_taxid = lower_bound(m_DBTaxID.begin(), m_DBTaxID.end(), find_company, compareTaxID);

	if (pos_taxid == m_DBTaxID.end() || taxID != (*pos_taxid)->c_TaxID)
	{
		delete find_company;
		return false;
	}

	(*pos_taxid)->c_Invoices += amount;
	auto pos_invoice = lower_bound(m_DBInvoices.begin(), m_DBInvoices.end(), amount);
	m_DBInvoices.insert(pos_invoice, amount);
	// m_DBInvoices.push_back(amount);

	delete find_company;

	return true;
}

bool CVATRegister::invoice(const string &name, const string &addr, unsigned int amount)
{
	MCompany *find_company = new MCompany();
	find_company->c_Name = name;
	find_company->c_Addr = addr;
	auto pos_name_addr = lower_bound(m_DBNameAddr.begin(), m_DBNameAddr.end(), find_company, compareNameAddr);
	if (pos_name_addr == m_DBNameAddr.end())
	{
		delete find_company;
		return false;
	}

	string low_name1 = name, low_addr1 = addr, low_name2 = (*pos_name_addr)->c_Name, low_addr2 = (*pos_name_addr)->c_Addr;
	tolowerStr(low_name1); tolowerStr(low_addr1); tolowerStr(low_name2); tolowerStr(low_addr2);
	if (low_name1 != low_name2 || low_addr1 != low_addr2)
	{
		delete find_company;
		return false;
	}

	(*pos_name_addr)->c_Invoices += amount;
	// m_DBInvoices.push_back(amount);
	auto pos_invoice = lower_bound(m_DBInvoices.begin(), m_DBInvoices.end(), amount);
	m_DBInvoices.insert(pos_invoice, amount);

	delete find_company;

	return true;
}

bool CVATRegister::audit(const string &name, const string &addr, unsigned int &sumIncome) const
{
	MCompany *find_company = new MCompany();
	find_company->c_Name = name;
	find_company->c_Addr = addr;
	auto pos_name_addr = lower_bound(m_DBNameAddr.begin(), m_DBNameAddr.end(), find_company, compareNameAddr);
	if (pos_name_addr == m_DBNameAddr.end())
	{
		delete find_company;
		return false;
	}

	string low_name1 = name, low_addr1 = addr, low_name2 = (*pos_name_addr)->c_Name, low_addr2 = (*pos_name_addr)->c_Addr;
	tolowerStr(low_name1); tolowerStr(low_addr1); tolowerStr(low_name2); tolowerStr(low_addr2);
	if (low_name1 != low_name2 || low_addr1 != low_addr2)
	{
		delete find_company;
		return false;
	}

	sumIncome = (*pos_name_addr)->c_Invoices;

	delete find_company;

	return true;
}

bool CVATRegister::audit(const string &taxID, unsigned int &sumIncome) const
{
	MCompany *find_company = new MCompany();
	find_company->c_TaxID = taxID;
	auto pos_taxid = lower_bound(m_DBTaxID.begin(), m_DBTaxID.end(), find_company, compareTaxID);

	if (pos_taxid == m_DBTaxID.end() || taxID != (*pos_taxid)->c_TaxID)
	{
		delete find_company;
		return false;
	}

	sumIncome = (*pos_taxid)->c_Invoices;

	delete find_company;

	return true;
}

bool CVATRegister::firstCompany(string &name, string &addr) const
{
	if (m_DBNameAddr.empty())
		return false;
	
	name = m_DBNameAddr.at(0)->c_Name;
	addr = m_DBNameAddr.at(0)->c_Addr;

	return true;
}

bool CVATRegister::nextCompany(string &name, string &addr) const
{
	MCompany *find_company = new MCompany();
	find_company->c_Name = name;
	find_company->c_Addr = addr;
	auto pos_name_addr = lower_bound(m_DBNameAddr.begin(), m_DBNameAddr.end(), find_company, compareNameAddr);
	if (pos_name_addr == m_DBNameAddr.end())
	{
		delete find_company;
		return false;
	}

	string low_name1 = name, low_addr1 = addr, low_name2 = (*pos_name_addr)->c_Name, low_addr2 = (*pos_name_addr)->c_Addr;
	tolowerStr(low_name1); tolowerStr(low_addr1); tolowerStr(low_name2); tolowerStr(low_addr2);
	if (low_name1 != low_name2 || low_addr1 != low_addr2)
	{
		delete find_company;
		return false;
	}

	++pos_name_addr;
	if (pos_name_addr == m_DBNameAddr.end())
	{
		delete find_company;
		return false;
	}

	name = (*pos_name_addr)->c_Name;
	addr = (*pos_name_addr)->c_Addr;

	delete find_company;

	return true;
}

unsigned int CVATRegister::medianInvoice(void) const
{
	if (m_DBInvoices.empty())
		return 0;

	return m_DBInvoices.at((int)m_DBInvoices.size() / 2);
}

#ifndef __PROGTEST__
int main(void)
{
  string name, addr;
  unsigned int sumIncome;

  CVATRegister b1;
  assert ( b1 . newCompany ( "ACME", "Thakurova", "666/666" ) );
  assert ( b1 . newCompany ( "ACME", "Kolejni", "666/666/666" ) );
  assert ( b1 . newCompany ( "Dummy", "Thakurova", "123456" ) );
  assert ( b1 . invoice ( "666/666", 2000 ) );
  assert ( b1 . medianInvoice () == 2000 );
  assert ( b1 . invoice ( "666/666/666", 3000 ) );
  assert ( b1 . medianInvoice () == 3000 );
  assert ( b1 . invoice ( "123456", 4000 ) );
  assert ( b1 . medianInvoice () == 3000 );
  assert ( b1 . invoice ( "aCmE", "Kolejni", 5000 ) );
  assert ( b1 . medianInvoice () == 4000 );
  assert ( b1 . audit ( "ACME", "Kolejni", sumIncome ) && sumIncome == 8000 );
  assert ( b1 . audit ( "123456", sumIncome ) && sumIncome == 4000 );
  assert ( b1 . firstCompany ( name, addr ) && name == "ACME" && addr == "Kolejni" );
  assert ( b1 . nextCompany ( name, addr ) && name == "ACME" && addr == "Thakurova" );
  assert ( b1 . nextCompany ( name, addr ) && name == "Dummy" && addr == "Thakurova" );
  assert ( ! b1 . nextCompany ( name, addr ) );
  assert ( b1 . cancelCompany ( "ACME", "KoLeJnI" ) );
  assert ( b1 . medianInvoice () == 4000 );
  assert ( b1 . cancelCompany ( "666/666" ) );
  assert ( b1 . medianInvoice () == 4000 );
  assert ( b1 . invoice ( "123456", 100 ) );
  assert ( b1 . medianInvoice () == 3000 );
  assert ( b1 . invoice ( "123456", 300 ) );
  assert ( b1 . medianInvoice () == 3000 );
  assert ( b1 . invoice ( "123456", 200 ) );
  assert ( b1 . medianInvoice () == 2000 );
  assert ( b1 . invoice ( "123456", 230 ) );
  assert ( b1 . medianInvoice () == 2000 );
  assert ( b1 . invoice ( "123456", 830 ) );
  assert ( b1 . medianInvoice () == 830 );
  assert ( b1 . invoice ( "123456", 1830 ) );
  assert ( b1 . medianInvoice () == 1830 );
  assert ( b1 . invoice ( "123456", 2830 ) );
  assert ( b1 . medianInvoice () == 1830 );
  assert ( b1 . invoice ( "123456", 2830 ) );
  assert ( b1 . medianInvoice () == 2000 );
  assert ( b1 . invoice ( "123456", 3200 ) );
  assert ( b1 . medianInvoice () == 2000 );
  assert ( b1 . firstCompany ( name, addr ) && name == "Dummy" && addr == "Thakurova" );
  assert ( ! b1 . nextCompany ( name, addr ) );
  assert ( b1 . cancelCompany ( "123456" ) );
  assert ( ! b1 . firstCompany ( name, addr ) );

  CVATRegister b2;
  assert ( b2 . newCompany ( "ACME", "Kolejni", "abcdef" ) );
  assert ( b2 . newCompany ( "Dummy", "Kolejni", "123456" ) );
  assert ( ! b2 . newCompany ( "AcMe", "kOlEjNi", "1234" ) );
  assert ( b2 . newCompany ( "Dummy", "Thakurova", "ABCDEF" ) );
  assert ( b2 . medianInvoice () == 0 );
  assert ( b2 . invoice ( "ABCDEF", 1000 ) );
  assert ( b2 . medianInvoice () == 1000 );
  assert ( b2 . invoice ( "abcdef", 2000 ) );
  assert ( b2 . medianInvoice () == 2000 );
  assert ( b2 . invoice ( "aCMe", "kOlEjNi", 3000 ) );
  assert ( b2 . medianInvoice () == 2000 );
  assert ( ! b2 . invoice ( "1234567", 100 ) );
  assert ( ! b2 . invoice ( "ACE", "Kolejni", 100 ) );
  assert ( ! b2 . invoice ( "ACME", "Thakurova", 100 ) );
  assert ( ! b2 . audit ( "1234567", sumIncome ) );
  assert ( ! b2 . audit ( "ACE", "Kolejni", sumIncome ) );
  assert ( ! b2 . audit ( "ACME", "Thakurova", sumIncome ) );
  assert ( ! b2 . cancelCompany ( "1234567" ) );
  assert ( ! b2 . cancelCompany ( "ACE", "Kolejni" ) );
  assert ( ! b2 . cancelCompany ( "ACME", "Thakurova" ) );
  assert ( b2 . cancelCompany ( "abcdef" ) );
  assert ( b2 . medianInvoice () == 2000 );
  assert ( ! b2 . cancelCompany ( "abcdef" ) );
  assert ( b2 . newCompany ( "ACME", "Kolejni", "abcdef" ) );
  assert ( b2 . cancelCompany ( "ACME", "Kolejni" ) );
  assert ( ! b2 . cancelCompany ( "ACME", "Kolejni" ) );

  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
