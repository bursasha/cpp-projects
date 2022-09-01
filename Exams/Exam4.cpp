#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <algorithm>
#include <map>
#include <set>
#include <vector>
#include <list>
#include <deque>
#include <queue>
#include <stack>
using namespace std;

class CTrain{
public:
	//Add
	void Add (istringstream& is)
	{
		string station_first, station_second;

		getline (is, station_first);

		while (getline (is, station_second))
		{
			m_Connections[station_first].insert (station_second);
			m_Connections[station_second].insert (station_first);
			station_first = station_second;
		}
	}

	//Dest
	set<string> Dest (const string& from, int max_cost)
	{
		set<string> visited_stations;
		queue<string> stations_to_visit;

		stations_to_visit.push (from); visited_stations.insert (from);

		int remaining_levels = max_cost, remaining_on_level = (int)stations_to_visit.size ();

		if (remaining_levels == 0) { stations_to_visit = queue<string> (); return visited_stations; }

		while (!stations_to_visit.empty ())
		{
			if (remaining_on_level == 0)
			{
				remaining_levels--; remaining_on_level = (int)stations_to_visit.size ();

				if (remaining_levels == 0) { stations_to_visit = queue<string> (); remaining_on_level = 0; break; }
			}

			for (const auto& station : m_Connections.at (stations_to_visit.front ()))
			{
				if (!visited_stations.count (station)) { stations_to_visit.push (station); visited_stations.insert (station); }
			}

			stations_to_visit.pop (); remaining_on_level--;
		}

		return visited_stations;
	}

private:
	map<string, set<string>> m_Connections;

};


int main () {
	CTrain t0;
	istringstream iss;
	iss . clear ();
	iss . str ("Forest Hill\nNational Museum\nWhite City\nChina Town\nWembley\nNew Cross\n");
	t0 . Add ( iss );
	iss . clear ();
	iss . str ("Lady Palace\nKings Cross\nOld Town\nNew Cross\nNew Alley\n");
	t0 . Add ( iss );
	iss . clear ();
	iss . str ("ZOO\nChina Town\nKings Cross\nFantom Bridge\n");
	t0 . Add ( iss );


	set<string> s = (t0.Dest("National Museum", 1));
	assert ((s == set<string> {"National Museum", "Forest Hill", "White City"}));
/*Forest Hill
  National Museum
  White City*/

	set<string> t = (t0.Dest("China Town", 0));
	assert ((t == set<string> {"China Town"}));
/*China Town*/

	set<string> u = (t0.Dest("China Town", 2));
	assert ((u == set<string> {"China Town", "Fantom Bridge", "Kings Cross", "Lady Palace", "National Museum",
	                           "New Cross", "Old Town", "Wembley", "White City", "ZOO"}));
/*China Town
  Fantom Bridge
  Kings Cross
  Lady Palace
  National Museum
  New Cross
  Old Town
  Wembley
  White City
  ZOO*/

	set<string> v = (t0.Dest("ZOO", 2));
	assert ((v == set<string> {"China Town", "Kings Cross", "Wembley", "White City", "ZOO"}));
/*China Town
  Kings Cross
  Wembley
  White City
  ZOO*/

	set<string> r = (t0.Dest("Old Town", 3));
	assert ((r == set<string> {"White City", "China Town", "Wembley", "New Cross", "Lady Palace", "Kings Cross", "Old Town", "New Alley", "ZOO", "Fantom Bridge"}));

	set<string> k = (t0.Dest("National Museum", 5));
	assert ((k == set<string> {"Forest Hill", "National Museum", "White City", "China Town", "Wembley", "New Cross", "Lady Palace", "Kings Cross", "Old Town", "ZOO", "Fantom Bridge", "New Alley"}));

	return 0;
}
