#include <cassert>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <map>
#include <set>
#include <list>
#include <vector>
#include <stack>
#include <queue>
#include <deque>
#include <algorithm>
#include <memory>
using namespace std;

class CTransport{
public:
	void AddLine (istream& is)
	{
		string station_first, station_second;

		getline (is, station_first);

		while (getline (is, station_second))
		{
			m_Connections[station_first].insert (station_second); m_Connections[station_second].insert (station_first);
			station_first = station_second;
		}
	}

	set<string> FindDest (const string& from, int cost_max) const
	{
		if (!m_Connections.count (from)) return set<string> { from };

		set<string> visited_stations = { from };
		queue<string> stations_to_visit; stations_to_visit.push (from);
		int remaining_levels = cost_max; size_t remaining_on_level = 0;

		while (!stations_to_visit.empty ())
		{
			if (remaining_on_level == 0)
			{
				remaining_levels--; remaining_on_level = stations_to_visit.size ();

				if (remaining_levels < 0) { remaining_on_level = 0; stations_to_visit = queue<string> (); break; }
			}

			for (const auto& station : m_Connections.at (stations_to_visit.front ()))
			{ if (!visited_stations.count (station)) { stations_to_visit.push (station); visited_stations.insert (station); } }

			stations_to_visit.pop (); remaining_on_level--;
		}

		return visited_stations;
	}

private:
	map<string, set<string>> m_Connections;

};

int main() {

	istringstream iss;
	CTransport t0;

	iss.clear();
	iss.str ("Newton\n" "Black Hill\n" "Wood Side\n" "Green Hill\n" "Lakeside\n");
	t0.AddLine(iss);

	iss.clear();
	iss.str("Great Newton\n" "Little Burnside\n" "Green Hill\n" "Wood Side\n");
	t0.AddLine(iss);

	iss.clear();
	iss.str("Little Newton\n" "Little Burnside\n" "Castle Hill\n" "Newton Crossroad\n"
	        "Lakeside Central\n");
	t0.AddLine(iss);

	iss.clear();
	iss.str("Tidmouth\n" "Gordon's Hill\n" "Suderry\n" "Knapford\n" "Great Waterton\n"
	        "Brendam Docks\n");
	t0.AddLine(iss);

	iss.clear();
	iss.str ("Sodor Ironworks\n" "Sodor Streamwork\n" "Knapford\n" "Maron\n");
	t0.AddLine(iss);

	iss.clear();
	iss.str("Lakeside Central\n" "Little Waterton\n");
	t0.AddLine(iss);

	iss.clear();
	iss.str("Waterton Central\n" "Waterton East\n" "Waterton Woods\n" "Little Waterton\n"
	        "Waterton West\n" "Waterton Central\n");
	t0.AddLine(iss);

	assert (t0.FindDest ("Maron", 0) == (set<string> {"Maron"}) );
	assert (t0.FindDest ("Maron", 1) == (set<string> {"Knapford", "Maron"}) );
	assert (t0.FindDest ("Maron", 2) == (set<string> {"Great Waterton", "Knapford", "Maron", "Sodor Streamwork", "Suderry"}) );
	assert (t0.FindDest ("Maron", 3) == (set<string> {"Brendam Docks", "Gordon's Hill", "Great Waterton", "Knapford", "Maron",
	                                                  "Sodor Ironworks", "Sodor Streamwork", "Suderry"}) );
	assert (t0.FindDest ("Maron", 4) == (set<string> {"Brendam Docks", "Gordon's Hill", "Great Waterton", "Knapford", "Maron",
	                                                  "Sodor Ironworks", "Sodor Streamwork", "Suderry", "Tidmouth"}) );
	assert (t0.FindDest ("Maron", 5) == (set<string> {"Brendam Docks", "Gordon's Hill", "Great Waterton", "Knapford", "Maron",
	                                                  "Sodor Ironworks", "Sodor Streamwork", "Suderry", "Tidmouth"}) );


	assert (t0.FindDest ("Waterton East", 0) == (set<string> {"Waterton East"}) );
	assert (t0.FindDest ("Waterton East", 1) == (set<string> {"Waterton Central", "Waterton East", "Waterton Woods"}) );
	assert (t0.FindDest ("Waterton East", 2) == (set<string> {"Little Waterton", "Waterton Central", "Waterton East",
	                                                          "Waterton West", "Waterton Woods"}) );
	assert (t0.FindDest ("Waterton East", 3) == (set<string> {"Lakeside Central", "Little Waterton", "Waterton Central",
	                                                          "Waterton East", "Waterton West", "Waterton Woods"}) );
	assert (t0.FindDest ("Waterton East", 4) == (set<string> {"Lakeside Central", "Little Waterton", "Newton Crossroad",
	                                                          "Waterton Central", "Waterton East", "Waterton West",
	                                                          "Waterton Woods"}) );
	assert (t0.FindDest ("Waterton East", 5) == (set<string> {"Castle Hill", "Lakeside Central", "Little Waterton", "Newton Crossroad",
	                                                          "Waterton Central", "Waterton East", "Waterton West", "Waterton Woods"}) );
	assert (t0.FindDest ("Waterton East", 6) == (set<string> {"Castle Hill", "Lakeside Central", "Little Burnside", "Little Waterton",
	                                                          "Newton Crossroad", "Waterton Central", "Waterton East", "Waterton West",
	                                                          "Waterton Woods"}) );
	assert (t0.FindDest ("Waterton East", 9) == (set<string> {"Black Hill", "Castle Hill", "Great Newton", "Green Hill", "Lakeside",
	                                                          "Lakeside Central", "Little Burnside", "Little Newton", "Little Waterton", "Newton Crossroad",
	                                                          "Waterton Central", "Waterton East", "Waterton West", "Waterton Woods", "Wood Side"}) );
	assert (t0.FindDest ("Waterton East", 10) == (set<string> {"Black Hill", "Castle Hill", "Great Newton", "Green Hill", "Lakeside",
	                                                           "Lakeside Central", "Little Burnside", "Little Newton", "Little Waterton",
	                                                           "Newton", "Newton Crossroad", "Waterton Central", "Waterton East",
	                                                           "Waterton West", "Waterton Woods", "Wood Side"}) );
	assert (t0.FindDest ("Waterton East", 11) == (set<string> {"Black Hill", "Castle Hill", "Great Newton", "Green Hill",
	                                                           "Lakeside", "Lakeside Central", "Little Burnside",
	                                                           "Little Newton", "Little Waterton", "Newton",
	                                                           "Newton Crossroad", "Waterton Central", "Waterton East",
	                                                           "Waterton West", "Waterton Woods", "Wood Side"}) );


	iss.clear();
	iss.str("Newton\n" "Newton Crossroad\n" "Tidmouth\n" "Brendam Docks\n");
	t0.AddLine(iss);

	assert (t0.FindDest ("Waterton East", 0) == (set<string> {"Waterton East"}) );
	assert (t0.FindDest ("Waterton East", 1) == (set<string> {"Waterton Central", "Waterton East", "Waterton Woods"}) );
	assert (t0.FindDest ("Waterton East", 2) == (set<string> {"Little Waterton", "Waterton Central", "Waterton East",
	                                                          "Waterton West", "Waterton Woods"}) );
	assert (t0.FindDest ("Waterton East", 3) == (set<string> {"Lakeside Central", "Little Waterton", "Waterton Central",
	                                                          "Waterton East", "Waterton West", "Waterton Woods"}) );
	assert (t0.FindDest ("Waterton East", 4) == (set<string> {"Lakeside Central", "Little Waterton", "Newton Crossroad",
	                                                          "Waterton Central", "Waterton East", "Waterton West",
	                                                          "Waterton Woods"}) );
	assert (t0.FindDest ("Waterton East", 5) == (set<string> {"Castle Hill", "Lakeside Central", "Little Waterton", "Newton", "Newton Crossroad", "Tidmouth",
	                                                          "Waterton Central", "Waterton East", "Waterton West", "Waterton Woods"}) );
	assert (t0.FindDest ("Waterton East", 6) == (set<string> {"Black Hill", "Brendam Docks", "Castle Hill", "Gordon's Hill", "Lakeside Central", "Little Burnside", "Little Waterton",
	                                                          "Newton", "Newton Crossroad", "Tidmouth", "Waterton Central", "Waterton East", "Waterton West",
	                                                          "Waterton Woods"}) );
	assert (t0.FindDest ("Waterton East", 9) == (set<string> {"Black Hill","Brendam Docks", "Castle Hill", "Gordon's Hill", "Great Newton", "Great Waterton", "Green Hill", "Knapford", "Lakeside",
	                                                          "Lakeside Central", "Little Burnside", "Little Newton", "Little Waterton", "Maron", "Newton", "Newton Crossroad",
	                                                          "Sodor Streamwork", "Suderry", "Tidmouth", "Waterton Central", "Waterton East", "Waterton West", "Waterton Woods", "Wood Side"}) );
	assert (t0.FindDest ("Waterton East", 10) == (set<string> {"Black Hill", "Brendam Docks", "Castle Hill", "Gordon's Hill", "Great Newton", "Great Waterton", "Green Hill", "Knapford", "Lakeside",
	                                                           "Lakeside Central", "Little Burnside", "Little Newton", "Little Waterton", "Maron",
	                                                           "Newton", "Newton Crossroad", "Sodor Ironworks", "Sodor Streamwork", "Suderry", "Tidmouth", "Waterton Central", "Waterton East",
	                                                           "Waterton West", "Waterton Woods", "Wood Side"}) );
	set<string> test = set<string> {"Black Hill", "Brendam Docks", "Castle Hill", "Gordon's Hill", "Great Newton", "Great Waterton", "Green Hill", "Knapford", "Lakeside",
	                                "Lakeside Central", "Little Burnside", "Little Newton", "Little Waterton", "Maron",
	                                "Newton", "Newton Crossroad", "Sodor Ironworks", "Sodor Streamwork", "Suderry", "Tidmouth", "Waterton Central", "Waterton East",
	                                "Waterton West", "Waterton Woods", "Wood Side"};
	set<string> test2 = t0.FindDest ("Waterton East", 11);
	assert (test2 == test );
	assert (t0.FindDest ("Waterton East", 1000000) == (set<string> {"Black Hill", "Brendam Docks", "Castle Hill", "Gordon's Hill", "Great Newton", "Great Waterton", "Green Hill", "Knapford", "Lakeside",
	                                                                "Lakeside Central", "Little Burnside", "Little Newton", "Little Waterton", "Maron",
	                                                                "Newton", "Newton Crossroad", "Sodor Ironworks", "Sodor Streamwork", "Suderry", "Tidmouth", "Waterton Central", "Waterton East",
	                                                                "Waterton West", "Waterton Woods", "Wood Side"}) );


	assert (t0.FindDest ("Suderry Woodside", 0) == (set<string> {"Suderry Woodside"}) );
	assert (t0.FindDest ("Suderry Woodside", 666) == (set<string> {"Suderry Woodside"}) );


	return 0;
}
