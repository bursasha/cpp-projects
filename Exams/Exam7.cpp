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
#include <cassert>

using namespace std;

class CTrain
{
public:
	void Add (istringstream& is)
	{
		string station_first, station_second;

		getline (is, station_first);

		if (!station_first.empty ()) m_Connections[station_first];

		while (getline (is, station_second))
		{
			m_Connections[station_first].insert (station_second); m_Connections[station_second].insert (station_first);
			station_first = station_second;
		}
	}

	int Count () const
	{
		set<string> visited_stations;
		int single_count = 0;

		for (const auto& station_pair : m_Connections)
		{
			if (visited_stations.count (station_pair.first)) continue;

			queue<string> stations_to_visit; stations_to_visit.push (station_pair.first);
			visited_stations.insert (station_pair.first);

			while (!stations_to_visit.empty ())
			{
				for (const auto& station : m_Connections.at (stations_to_visit.front ()))
				{ if (!visited_stations.count (station)) { stations_to_visit.push (station); visited_stations.insert (station); } }

				stations_to_visit.pop ();
			}

			single_count++;
		}

		return single_count;
	}

private:
	map<string, set<string>> m_Connections;

};


int main()
{
	CTrain t0;
	istringstream iss;
	assert(t0.Count() == 0);

	iss.clear();
	iss.str("Newton\n");
	t0.Add(iss);
	assert(t0.Count() == 1);

	iss.clear();
	iss.str("Newton\n");
	t0.Add(iss);
	assert(t0.Count() == 1);

	iss.clear();
	iss.str("\n");
	t0.Add(iss);
	assert(t0.Count() == 1);

	iss.clear();
	iss.str("\n\nNewton\n\nNewton\n\nNewton\n\n");
	t0.Add(iss);
	assert(t0.Count() == 1);

	iss.clear();
	iss.str("Black Hill\n");
	t0.Add(iss);
	assert(t0.Count() == 2);

	iss.clear();
	iss.str("Wood Side\n");
	t0.Add(iss);
	assert(t0.Count() == 3);

	iss.clear();
	iss.str("Green Hill\n");
	t0.Add(iss);
	assert(t0.Count() == 4);

	iss.clear();
	iss.str("Lakeside\n");
	t0.Add(iss);
	assert(t0.Count() == 5);

	iss.clear();
	iss.str("Newton\nBlack Hill\nWood Side\nGreen Hill\nLakeside\n");
	t0.Add(iss);
	assert(t0.Count() == 1);

	iss.clear();
	iss.str("Little Newton\nLittle Burnside\nCastle Hill\nNewton Crossroad\nLakeside Central\n");
	t0.Add(iss);
	assert(t0.Count() == 2);

	iss.clear();
	iss.str("Waterton West\nWaterton Central\nWaterton East\nWaterton Woods\nLittle Waterton\nWaterton West\n");
	t0.Add(iss);
	assert(t0.Count() == 3);

	iss.clear();
	iss.str("Little Waterton\nLakeside Central\n");
	t0.Add(iss);
	assert(t0.Count() == 2);

	iss.clear();
	iss.str("Great Newton\nLittle Burnside\nGreen Hill\nWood Side\n");
	t0.Add(iss);
	assert(t0.Count() == 1);

	iss.clear();
	iss.str("Sodor Ironworks\nSodor Steamworks\nKnapford\nMaron\n");
	t0.Add(iss);
	assert(t0.Count() == 2);

	iss.clear();
	iss.str("Tidmouth\nGordons Hill\nSuderry\nKnapford\nGreat Waterton\nBrendam Docks\n");
	t0.Add(iss);
	assert(t0.Count() == 2);

	iss.clear();
	iss.str("Newton\nNewton Crossroad\nTidmouth\nBrendam Docks\n");
	t0.Add(iss);
	assert(t0.Count() == 1);
	return 0;
}
