#include <sstream>
#include <string>
#include <list>
#include <vector>
#include <stack>
#include <queue>
#include <deque>
#include <iomanip>
#include <iostream>
#include <map>
#include <set>
#include <algorithm>
#include <memory>
#include <cassert>
using namespace std;

class CDelivery
{
public:
	CDelivery& addConn (const string& from, const string& to)
	{
		m_CitiesConnections [from].insert (to);

		return *this;
	}

	// returns a map containing the shortest path to each customer from any of the depots
	// map key is the customer name, value is a list of nodes on the shortest path (depot -> customer)
	// if there is no path, the value is an empty list
	map<string, list<string>> serveCustomers (const set<string>& customers, const set<string>& depots) const
	{
		map<string, list<string>> services;
		for (const auto& customer : customers)
		{
			map<string, string> all_paths; //to, from
			queue<string> queue_to_visit;

			for (const auto& depot : depots)
			{
				queue_to_visit.push (depot);
				all_paths.emplace (depot, "");
			}

			while (!queue_to_visit.empty ())
			{
				string city_to_visit = queue_to_visit.front ();
				queue_to_visit.pop ();

				if (city_to_visit == customer)
				{
					queue_to_visit = queue<string> ();
					break;
				}

				if (!m_CitiesConnections.count (city_to_visit)) continue;
				auto nbrs_to_visit = m_CitiesConnections.at (city_to_visit);
				for (const auto& nbr : nbrs_to_visit)
				{
					if (!all_paths.count (nbr))
					{
						all_paths.emplace (nbr, city_to_visit);
						queue_to_visit.push (nbr);
					}
				}
			}

			list<string> short_path;
			if (all_paths.count (customer))
			{
				for (auto city = customer; ; )
				{
					short_path.push_front (city);
					if (all_paths [city].empty ()) break;
					city = all_paths [city];
				}
			}

			services.emplace (customer, short_path);
		}

		return services;
	}

private:
	map<string, set<string>> m_CitiesConnections;

};


int main ()
{
	CDelivery t0;
	map<string, list<string>> r;

	t0.addConn("Austin", "Berlin");
	t0.addConn("Chicago", "Berlin");
	t0.addConn("Berlin", "Dallas");
	t0.addConn("Dallas", "Essen");
	t0.addConn("Essen", "Austin");
	t0.addConn("Frankfurt", "Gyor");
	t0.addConn("Gyor", "Helsinki");
	t0.addConn("Helsinki", "Frankfurt");

	r = t0.serveCustomers(set<string>{"Berlin", "Gyor"}, set<string>{"Essen", "Helsinki"});
	assert ( r == (map<string, list<string>>{ {"Berlin", {"Essen", "Austin", "Berlin"}}, {"Gyor", {"Helsinki", "Frankfurt", "Gyor"}} }) );

	r = t0.serveCustomers(set<string>{"Austin", "Gyor", "Chicago"}, set<string>{"Austin", "Dallas"});
	assert ( r == (map<string, list<string>>{ {"Austin", {"Austin"}}, {"Chicago", { }}, {"Gyor", { }} }) );
	t0.addConn("Chicago", "Helsinki");

	r = t0.serveCustomers(set<string>{"Austin", "Gyor", "Chicago"}, set<string>{"Austin", "Dallas"});
	assert ( r == (map<string, list<string>>{ {"Austin", {"Austin"}}, {"Chicago", { }}, {"Gyor", { }} }) );
	t0.addConn("Berlin", "Chicago");

	r = t0.serveCustomers(set<string>{"Austin", "Gyor", "Chicago"}, set<string>{"Austin", "Dallas"});
	assert ( r == (map<string, list<string>>{ {"Austin", {"Austin"}}, {"Chicago", {"Austin", "Berlin", "Chicago"}}, {"Gyor", {"Austin", "Berlin", "Chicago", "Helsinki", "Frankfurt", "Gyor"}} }) );
	t0.addConn("Essen", "Frankfurt");

	r = t0.serveCustomers(set<string>{"Austin", "Gyor", "Chicago"}, set<string>{"Austin", "Dallas"});
	assert ( r == (map<string, list<string>>{ {"Austin", {"Austin"}}, {"Chicago", {"Austin", "Berlin", "Chicago"}}, {"Gyor", {"Dallas", "Essen", "Frankfurt", "Gyor"}} }) );


	CDelivery t1;

	t1.addConn("A", "B");
	t1.addConn("B", "A");
	t1.addConn("B", "C");
	t1.addConn("B", "D");
	t1.addConn("B", "G");
	t1.addConn("C", "F");
	t1.addConn("C", "D");
	t1.addConn("D", "E");
	t1.addConn("D", "A");
	t1.addConn("G", "H");
	t1.addConn("H", "A");

	r = t1.serveCustomers(set<string>{"E", "F", "B"}, set<string>{"G", "D"});
	assert ( r == (map<string, list<string>>{ {"B", {"D", "A", "B"}}, {"E", {"D", "E"}}, {"F", {"D", "A", "B", "C", "F"}} }) );

	t1.addConn("F", "I");
	t1.addConn("I", "G");
	t1.addConn("I", "J");
	t1.addConn("E", "K");
	t1.addConn("K", "H");

	r = t1.serveCustomers(set<string>{"H", "J", "A", "E"}, set<string>{"C", "J", "D"});
	assert ( r == (map<string, list<string>>{ {"A", {"D", "A"}}, {"E", {"D", "E"}}, {"H", {"D", "E", "K", "H"}}, {"J", {"J"}} }) );


	CDelivery t2;

	t2.addConn("A", "B").addConn("A", "G");
	t2.addConn("B", "A").addConn("B", "C");
	t2.addConn("C", "B").addConn("C", "D").addConn("C", "I").addConn("C", "G");
	t2.addConn("D", "E").addConn("D", "I").addConn("D", "J");
	t2.addConn("E", "C");
	t2.addConn("F", "A").addConn("F", "G").addConn("F", "K");
	t2.addConn("G", "H");
	t2.addConn("I", "M");
	t2.addConn("J", "N");
	t2.addConn("K", "L").addConn("K", "P");
	t2.addConn("L", "F").addConn("L", "M");
	t2.addConn("M", "R");
	t2.addConn("N", "S");
	t2.addConn("O", "E");
	t2.addConn("P", "Q");
	t2.addConn("Q", "L");
	t2.addConn("R", "N").addConn("R", "S");
	t2.addConn("S", "T");
	t2.addConn("T", "O");

	r = t2.serveCustomers(set<string>{"G", "I"}, set<string>{"R", "P", "J"});
	assert ( r == (map<string, list<string>>{ {"G", {"P", "Q", "L", "F", "G"}}, {"I", {"R", "S", "T", "O", "E", "C", "I"}} }) );


	return EXIT_SUCCESS;
}
