#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <set>
#include <queue>
#include <stack>
#include <deque>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <memory>
using namespace std;
#endif /* __PROGTEST__ */

/**
 * CDate - an auxiliary class whose objects store information about the expiry date of the items in the storehouse.
 * The expiration date has 3 params: year, month, and day - int c_Year, int c_Month, int c_Day.
 * A constructor, a default destructor, and 3 comparison operations for class objects are implemented inside the class.
 */
class CDate
{
public:
	/**
	 * CDate () - initializing constructor, accepts 3 parameters.
	 * @param[in] set_year year of the expiration date.
	 * @param[in] set_month month of the expiration date.
	 * @param[in] set_day day of the expiration date.
	 */
	CDate (const int set_year, const int set_month, const int set_day)
	: c_Year(set_year), c_Month(set_month), c_Day(set_day)
	{
	}
	/**
	 * ~CDate () - default destructor.
	 */
	~CDate ()
	{
	}

	/**
	 * operator < - a method comparing our CDate object with another existing CDate object (method gets it as a parameter).
	 * @param[in] other an object with a date that is accepted into the method.
	 * @returns true if our date is older than the date in the other object, false otherwise.
	 */
	bool operator < (const CDate &other) const
	{
		return (c_Year < other.c_Year || (c_Year == other.c_Year && c_Month < other.c_Month) 
							|| (c_Year == other.c_Year && c_Month == other.c_Month && c_Day < other.c_Day));
	}
	/**
	 * operator == - a method comparing our CDate object with another existing CDate object (method gets it as a parameter).
	 * @param[in] other an object with a date that is accepted into the method.
	 * @returns true if our date is equal to the date in the other object, false otherwise.
	 */
	bool operator == (const CDate &other) const
	{
		return (c_Day == other.c_Day && c_Month == other.c_Month && c_Year == other.c_Year);
	}
	/**
	 * operator != - a method comparing our CDate object with another existing CDate object (method gets it as a parameter).
	 * @param[in] other an object with a date that is accepted into the method.
	 * @returns true if our date is not equal to the date in the other object, false otherwise.
	 */
	bool operator != (const CDate &other) const
	{
		return !(operator == (other));
	}

private:
	/**
	 * 3 private class variables responsible for the day, month and year of the expiration date.
	 */
	int c_Year, c_Month, c_Day;

};

/**
 * CSupermarket - a class that implements the storage of items in a storehouse, as well as the operations of adding products,
 * selling products, and searching for all expired products.
 * The supermarket storehouse is implemented in the form of an unordered map, the keys of which are the names of each product,
 * and the values are sets ordered in ascending order with product structures (expiration date and count).
 * The class consists of a regular initializing constructor in which an empty unordered map is initialized,
 * of a destructor in which the unordered map is cleaned, of the main 3 methods - store, sell, expired.
 * The private field contains the layout of the item structure, another structure being a comparator for ordered sets inside the unordered map,
 * 2 auxiliary methods for the sell method, and the unordered map itself being a container for storing information about items, unordered_map c_Storehouse.
 * Unordered map allows to access each item in constant time due to its hashing, an ordered set allows to access a specific product in logarithmic time, 
 * these are the optimal containers for this task.
 */
class CSupermarket
{
public:
	/**
	 * CSupermarket () - initializing constructor. Initializes an empty c_Storehouse.
	 */
	CSupermarket ()
	: c_Storehouse()
	{
	}
	/**
	 * ~CSupermarket () - destructor. Clears the c_Storehouse using method clear.
	 */
	~CSupermarket ()
	{
		c_Storehouse.clear();
	}

	/**
	 * store - this method implements the addition of a product to the storehouse.
	 * The concept of the method:
	 * First there is a search for the added product in the storehouse.
	 * If there is no such product yet, a new key-value pair is created with this product.
	 * If there is such an item in stock, there is a search for a structure containing the same expiration date that we get as a parameter.
	 * If such a structure is not found within the set, a structure is added with a new date and quantity of this product batch.
	 * Otherwise, the amount of this product inside the found structure is increased.
	 *
	 * @param[in] set_name a name of the item to be added.
	 * @param[in] set_exp_date the expiration date of the item to be added.
	 * @param[in] set_count a count of the item to be added.
	 * @returns a modified storehouse with a product added to it, *this.
	 */
	CSupermarket &store (const string set_name, const CDate set_exp_date, const int set_count)
	{
		if (!c_Storehouse.count(set_name))														// Using the count method, method store checks whether there is such an item inside the unordered map c_Storehouse.
		{
			set <CItem, exp_date_cmp> to_store = {CItem {set_exp_date, set_count}}; // If no such item is found, a set consisting of a single structure of the added product is created.
			c_Storehouse.insert(make_pair(set_name, to_store));							// Adding a new key-value pair to the storehouse c_Storehouse, the name of an item is a key, set with the structure is a value.
		}
		else																								// The item is in stock.
		{
			auto to_store = c_Storehouse[set_name].find(set_exp_date);					// Search for a structure with the same expiration date as the added product. Here the overloaded find method is used for a set, the constructor for which is written inside the CItem structure, the operator for comparing 2 structures by expiration date is also written there.
			if ((*to_store).exp_date != set_exp_date)											// The find method returns an iterator to the structure inside the set. 
				c_Storehouse[set_name].insert(CItem {set_exp_date, set_count});		// If such a structure is not found, a new product structure is added with a new expiration date and quantity.
			else																							// The returned iterator has the same expiration date.
				(*to_store).count += set_count;													// An increasing of the item count inside its structure. It is possible due to the mutable keyword (find always returns const iterator to the needed item, mutable keyword allows to modify inner variables).
		}
		return *this;
	}
	/**
	 * sell - a method that implements the sale of products from the storehouse depending on the accepted shopping list.
	 * The concept of the method:
	 * The method goes through the entire shopping list and first searches the storehouse for products with the same name as in the list.
	 * After the search, the number of items found is checked, if more than one product with the same or similar name is found or nothing is found,
	 * then the transition to the next product in the list occurs.
	 * If only 1 item is found, then there is a passage through the set of this product, depending on the required quantity.
	 * At the same time, several vectors are filled in to delete the finished products in the storehouse and in the list.
	 * At the end, the method returns a shopping list after the manipulations performed.
	 *
	 * @param[in, out] shopping_list a shopping list implemented by a list container, which consists of pairs of names and the number of products we are looking for.
	 * @returns a modified shopping_list after the manipulations performed (that means with missing products in the list or with a reduced count).
	 */
	list <pair <string, int> > &sell (list <pair <string, int> > &shopping_list)
	{
		vector <list <pair <string, int> >::iterator> list_item_to_delete;									// A vector that, as we go through the shopping list, is being filled with iterators to its own products, the right amount of which was found in the storehouse and purchased. This is done to be able to move through the list to the next item and remove the item from the list.
		vector<string> store_item_to_delete;																			// A vector similar to the first vector, in which the names of fully redeemed items in the storehouse will be added (their set of structures have become empty).

		for (auto list_item = shopping_list.begin(); list_item != shopping_list.end(); list_item++)	// Going through the shopping list.
		{
			vector <string> sell_item;																						// A vector that will contain all found similar or identical item names in stock.
			(*this).find_item(list_item, sell_item);																	// The find_item method searches through the storehouse, inside c_Storehouse, and searches for the required product by name from the shopping_list.
			if (sell_item.empty() || sell_item.size() > 1)															// Checking the quantity of found products that are currently in the sell_item vector.
				continue;

			vector <set <CItem, exp_date_cmp>::iterator> empty_item_to_delete;								// A vector of iterators to structures within the set of a particular item that will be afterwards removed from the set (their count has become equal to 0).
			auto found_item = sell_item[0];																				// For the convenience of the following sale of the found product, it is put in a separate variable from the vector (string typename).
			(*this).sell_item(list_item, found_item, list_item_to_delete, empty_item_to_delete);		// This private method sells products from the storehouse, reduces the quantity in the shopping_list and in the c_Storehouse, and also fills in the vectors of products for deletion.

			for (auto &to_delete : empty_item_to_delete)																// Removing finished items from its set (count == 0).
				c_Storehouse[found_item].erase(to_delete);
			if (c_Storehouse[found_item].empty())																		// Checking whether the item set has become empty, if so, its name will be added to the vector to be removed from the unordered_map.
				store_item_to_delete.push_back(found_item);
		}

		for (auto &to_delete : store_item_to_delete)																	// Removing finished items from the c_Storehouse (empty set).
			c_Storehouse.erase(to_delete);
		for (auto &to_delete : list_item_to_delete)																	// Removing finished items from the shopping_list (count == 0).
			shopping_list.erase(to_delete);
			
		return shopping_list;
	}
	/**
	 * expired - a method implements the search for expired products in stock relative to the received date accepted within the method.
	 * The concept of the method:
	 * The method goes through all the items in the storehouse and checks in their sets of structures from the very beginning
	 * products that expiration date has already expired and adds them to the list (the date and quantity of such an item are added).
	 * After finding all such products, the list with products is sorted in descending order by quantity.
	 * The method returns this list with expired products.
	 *
	 * @param[in] find_exp_date the CDate object with an expiration date, relative to which the method searches for expired products.
	 * @returns a list of expired items sorted in descending order by the count of items.
	 */
	list <pair <string, int> > expired (const CDate find_exp_date) const
	{
		list <pair <string, int> > exp_items;																											// A doubly linked list which consists of pairs of names and the number of expired items.
		for (auto &i_name : c_Storehouse)																												// Going through the c_Storehouse.
		{
			int exp_count = 0;																																// A variable responsible for the number of expired instances of each product.
			for (auto item = i_name.second.begin(); item != i_name.second.end() && (*item).exp_date < find_exp_date; item++)	// Going through the set of each item until the cycle reaches the end or the iterating variable is not an expired product.
				exp_count += (*item).count;																												// Increasing the value of expired instances of an item.
			if (exp_count > 0)																																// Checking whether the set of the item contains expired structures.
				exp_items.push_back(make_pair(i_name.first, exp_count));																			// Adding the name of the expired product and its quantity.
		}
		exp_items.sort([](const pair <string, int> &a, const pair <string, int> &b){ return a.second > b.second; });			// Sorting the list exp_items using a lambda function inside the sort method. It compares 2 neighbours by the date.
		return exp_items;
	}

private:
	/**
	 * CItem - the structure implementing the instance of the product with its expiration date and quantity.
	 * Such structures are stored inside sets inside an unordered_map c_Storehouse.
	 * The structure contains 2 internal variables responsible for the expiration date in the form of a CDate object (exp_date)
	 * and the quantity of such an item (count (mutable keyword allows to change this variable inside the set container)).
	 * The structure also contains a copy constructor for the find set method and a comparison bool operator ==.
	 */
	struct CItem
	{
		CDate exp_date;
		mutable int count;

		CItem (const CDate d, const int c = 0)	// Copy constructor for find set method which accepts only CDate expiry date object and should find specific structure in the set, due to this find doesn't need to get count of the item and by default it is equal to 0.
		: exp_date(d), count(c) 
		{
		}
		/**
		 * operator == - a comparison operator for CItem structures for find set method.
		 *
		 * @param[in] other method accepts CItem structures in set to compare their exp_date expiration date.
		 * @returns true if dates are identic, false otherwise.
		 */
		bool operator == (const CItem &other) const
		{
			return exp_date == other.exp_date;
		}
	};
	/**
	 * exp_date_cmp - a structure that implements a comparator for multiple product structures in a storehouse.
	 * Contains an operator () for inserting a product into a set in ascending order of date (from older to newer date).
	 * Is used as an argument in CItem sets.
	 */
	struct exp_date_cmp
	{
		/**
		 * operator () - is used as CItem set comparator.
		 *
		 * @param[in] a CItem structure inside the set of CItem structures with items.
		 * @param[in] b method accepts CItem structure to the set to compare its exp_date with CItem date inside the set.
		 * @returns true if accepted CItem date is newer, false otherwise.
		 */
		bool operator () (const CItem &a, const CItem &b) const
		{
			return a.exp_date < b.exp_date;
		}
	};
	/**
	 * find_item - a method that implements the search for products in a storehouse with the same name as in the shopping list
	 * or with one mistake in the name.
	 * The concept of the method:
	 * First, there is a search for an item with an identical name in the storehouse, in c_Storehouse.
	 * If such a product is present, then its name is added to the received vector to store all the found products.
	 * If such a product was not found, method iterates through all the keys and checks for no more than 1 mistake in the product name
	 * (the product name must be of the same length and contain no more than 1 mistake in the name).
	 *
	 * @param[in] list_item the required product name to be found in the storehouse or to find a similar name.
	 * @param[in] sell_item vector for storing all found items.
	 */
	void find_item (const list <pair <string, int> >::iterator &list_item, vector <string> &sell_item)
	{
		auto same_item = c_Storehouse.find(list_item->first);					// An iterator to key in unordered_map c_Storehouse which is an item with same name or is the end of the c_Storehouse if nothing is found.
		if (same_item != c_Storehouse.end())
			sell_item.push_back((*same_item).first);								// Adding the name of the same name product.

		if (sell_item.empty())															// If an item with the identical name is not found.
		{
			for (auto &store_item : c_Storehouse)
			{
				if (list_item->first.length() == store_item.first.length()) // Check whether the length of an item inside the storehouse is equal to the length of accepted list item name.
				{
					int mistake = 0;														// A variable used for mistakes count.
					for (size_t m = 0; m < list_item->first.length(); m++)
					{
						if (list_item->first[m] != store_item.first[m])
							++mistake;
					}
					if (mistake <= 1)
						sell_item.push_back(store_item.first);						// Adding the name of the found item if its count of mistakes in the name is no more than 1.
				}
			}
		}
	}
	/**
	 * sell_item - a method implementing the sale of goods from a storehouse depending on the quantity of this product in the shopping list.
	 * The concept of the method:
	 * The method accepts an iterator to the product in the shopping list and repels from the quantity of the required product.
	 * The method appeals to the set of this product in stock and is iterated either until the end of the set or until the moment
	 * when the required quantity of this product is purchased.
	 * If there is enough of this product in stock, then later this product will be erased from the shopping list,
	 * if this product is not enough in stock, then it will eventually be sold out and removed from the storehouse.
	 *
	 * @param[in] list_item a required product name to be found in the storehouse from a shopping list.
	 * @param[in] found_item a found item name in the storehouse which should be sold.
	 * @param[in] list_item_to_delete a vector which contains all fully purchased items from the shopping list.
	 * @param[in] empty_item_to_delete a vector which contains all fully purchased CItem structures from the item set (iterators).
	 */
	void sell_item (const list <pair <string, int> >::iterator &list_item, const string &found_item, 
						 vector <list <pair <string, int> >::iterator> &list_item_to_delete, 
						 vector <set <CItem, exp_date_cmp>::iterator> &empty_item_to_delete)
	{
		for (auto to_sell = c_Storehouse[found_item].begin(); to_sell != c_Storehouse[found_item].end() && list_item->second > 0; to_sell++)
		{
			if (list_item->second > to_sell->count)			// If there is not enough of the found product in the structure.
			{
				list_item->second -= to_sell->count;
				to_sell->count = 0;
				empty_item_to_delete.push_back(to_sell);		// Adding an iterator to this fully purchased item.
			}
			else if (list_item->second == to_sell->count)	// If there is equal count of the required item to the item count in the structure.
			{
				to_sell->count = 0;
				list_item->second = 0;
				empty_item_to_delete.push_back(to_sell);		// Adding an iterator to this fully purchased item from the storehouse.
				list_item_to_delete.push_back(list_item);		// Adding an iterator to this fully purchased from a shopping list.
			}
			else															// If there is enough of the found product in the structure.
			{
				to_sell->count -= list_item->second;
				list_item->second = 0;
				list_item_to_delete.push_back(list_item);		// Adding an iterator to this fully purchased from a shopping list.
			}
		}
	}
	unordered_map <string, set <CItem, exp_date_cmp> > c_Storehouse;

};

#ifndef __PROGTEST__
int main ( void )
{
  CSupermarket s;
  s . store ( "bread", CDate ( 2016, 4, 30 ), 100 )
    . store ( "butter", CDate ( 2016, 5, 10 ), 10 )
    . store ( "beer", CDate ( 2016, 8, 10 ), 50 )
    . store ( "bread", CDate ( 2016, 4, 25 ), 100 )
    . store ( "okey", CDate ( 2016, 7, 18 ), 5 );

  list<pair<string,int> > l0 = s . expired ( CDate ( 2018, 4, 30 ) );
  assert ( l0 . size () == 4 );
  assert ( ( l0 == list<pair<string,int> > { { "bread", 200 }, { "beer", 50 }, { "butter", 10 }, { "okey", 5 } } ) );

  list<pair<string,int> > l1 { { "bread", 2 }, { "Coke", 5 }, { "butter", 20 } };
  s . sell ( l1 );
  assert ( l1 . size () == 2 );
  assert ( ( l1 == list<pair<string,int> > { { "Coke", 5 }, { "butter", 10 } } ) );

  list<pair<string,int> > l2 = s . expired ( CDate ( 2016, 4, 30 ) );
  assert ( l2 . size () == 1 );
  assert ( ( l2 == list<pair<string,int> > { { "bread", 98 } } ) );

  list<pair<string,int> > l3 = s . expired ( CDate ( 2016, 5, 20 ) );
  assert ( l3 . size () == 1 );
  assert ( ( l3 == list<pair<string,int> > { { "bread", 198 } } ) );

  list<pair<string,int> > l4 { { "bread", 105 } };
  s . sell ( l4 );
  assert ( l4 . size () == 0 );
  assert ( ( l4 == list<pair<string,int> > {  } ) );

  list<pair<string,int> > l5 = s . expired ( CDate ( 2017, 1, 1 ) );
  assert ( l5 . size () == 3 );
  assert ( ( l5 == list<pair<string,int> > { { "bread", 93 }, { "beer", 50 }, { "okey", 5 } } ) );

  s . store ( "Coke", CDate ( 2016, 12, 31 ), 10 );

  list<pair<string,int> > l6 { { "Cake", 1 }, { "Coke", 1 }, { "cake", 1 }, { "coke", 1 }, { "cuke", 1 }, { "Cokes", 1 } };
  s . sell ( l6 );
  assert ( l6 . size () == 3 );
  assert ( ( l6 == list<pair<string,int> > { { "cake", 1 }, { "cuke", 1 }, { "Cokes", 1 } } ) );

  list<pair<string,int> > l7 = s . expired ( CDate ( 2017, 1, 1 ) );
  assert ( l7 . size () == 4 );
  assert ( ( l7 == list<pair<string,int> > { { "bread", 93 }, { "beer", 50 }, { "Coke", 7 }, { "okey", 5 } } ) );

  s . store ( "cake", CDate ( 2016, 11, 1 ), 5 );

  list<pair<string,int> > l8 { { "Cake", 1 }, { "Coke", 1 }, { "cake", 1 }, { "coke", 1 }, { "cuke", 1 } };
  s . sell ( l8 );
  assert ( l8 . size () == 2 );
  assert ( ( l8 == list<pair<string,int> > { { "Cake", 1 }, { "coke", 1 } } ) );

  list<pair<string,int> > l9 = s . expired ( CDate ( 2017, 1, 1 ) );
  assert ( l9 . size () == 5 );
  assert ( ( l9 == list<pair<string,int> > { { "bread", 93 }, { "beer", 50 }, { "Coke", 6 }, { "okey", 5 }, { "cake", 3 } } ) );

  list<pair<string,int> > l10 { { "cake", 15 }, { "Cake", 2 } };
  s . sell ( l10 );
  assert ( l10 . size () == 2 );
  assert ( ( l10 == list<pair<string,int> > { { "cake", 12 }, { "Cake", 2 } } ) );

  list<pair<string,int> > l11 = s . expired ( CDate ( 2017, 1, 1 ) );
  assert ( l11 . size () == 4 );
  assert ( ( l11 == list<pair<string,int> > { { "bread", 93 }, { "beer", 50 }, { "Coke", 6 }, { "okey", 5 } } ) );

  list<pair<string,int> > l12 { { "Cake", 4 } };
  s . sell ( l12 );
  assert ( l12 . size () == 0 );
  assert ( ( l12 == list<pair<string,int> > {  } ) );

  list<pair<string,int> > l13 = s . expired ( CDate ( 2017, 1, 1 ) );
  assert ( l13 . size () == 4 );
  assert ( ( l13 == list<pair<string,int> > { { "bread", 93 }, { "beer", 50 }, { "okey", 5 }, { "Coke", 2 } } ) );

  list<pair<string,int> > l14 { { "Beer", 20 }, { "Coke", 1 }, { "bear", 25 }, { "beer", 10 } };
  s . sell ( l14 );
  assert ( l14 . size () == 1 );
  assert ( ( l14 == list<pair<string,int> > { { "beer", 5 } } ) );

  s . store ( "ccccb", CDate ( 2019, 3, 11 ), 100 )
    . store ( "ccccd", CDate ( 2019, 6, 9 ), 100 )
    . store ( "dcccc", CDate ( 2019, 2, 14 ), 100 );

  list<pair<string,int> > l15 { { "ccccc", 10 } };
  s . sell ( l15 );
  assert ( l15 . size () == 1 );
  assert ( ( l15 == list<pair<string,int> > { { "ccccc", 10 } } ) );

  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
